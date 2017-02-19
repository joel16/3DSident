#include "fs.h"
#include "screenshot.h"

static inline void update_gfx(void)
{
	gfxFlushBuffers();
	gspWaitForVBlank();
	gfxSwapBuffers();
}

#define USE_CALLBACK

static void png_file_write(png_structp png_ptr, png_bytep data, png_size_t length)
{
	ssize_t rc;
	FILE    *fp = (FILE*)png_get_io_ptr(png_ptr);
	
	if(length <= 0)
	{
		fprintf(stderr, "%s: length <= 0\n", __func__);
		longjmp(png_jmpbuf(png_ptr), 1);
	}

	rc = fwrite(data, 1, length, fp);
	if(rc <= 0)
	{
		fprintf(stderr, "fwrite failed\n");
		longjmp(png_jmpbuf(png_ptr), 1);
	}
	else if(rc < length)
	{
		fprintf(stderr, "fwrite: wrote %zu/%zu bytes\n", rc, length);
		longjmp(png_jmpbuf(png_ptr), 1);
	}
}

static void
png_file_flush(png_structp png_ptr)
{
	/* no-op */
}

static void
png_file_error(png_structp png_ptr, png_const_charp error_msg)
{
	fprintf(stderr, "%s\n", error_msg);
}

static void png_file_warning(png_structp png_ptr, png_const_charp warning_msg)
{
	fprintf(stderr, "%s\n", warning_msg);
}

#ifdef USE_CALLBACK

static void png_row_callback(png_structp png_ptr, png_uint_32 row, int pass)
{
	fprintf(stderr, "\x1b[2;0H%3d%%\n", row*100 / 480);
}

#endif

static u8 png_buffer[400*480*4];
static u8 *png_lines[480];

static u32 bytes_per_pixel(GSPGPU_FramebufferFormats format)
{
	switch(format)
	{
		case GSP_RGBA8_OES:
			return 4;
		case GSP_BGR8_OES:
			return 3;
		case GSP_RGB565_OES:
		case GSP_RGB5_A1_OES:
		case GSP_RGBA4_OES:
			return 2;
	}		

	return 3;
}

static void pixel_to_rgba(u8 *dst, const u8 *src, GSPGPU_FramebufferFormats format)
{
	u16 half;

	switch(format)
	{
		case GSP_RGBA8_OES:
			dst[0] = src[3];
			dst[1] = src[2];
			dst[2] = src[1];
			dst[3] = src[0];
			break;

		case GSP_BGR8_OES:
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = 0xFF;
			break;

		case GSP_RGB565_OES:
			memcpy(&half, src, sizeof(half));
			dst[0] = (half >> 8) & 0xF8;
			dst[1] = (half >> 3) & 0xFC;
			dst[2] = (half << 3) & 0xF8;
			dst[3] = 0xFF;
			break;

		case GSP_RGB5_A1_OES:
			memcpy(&half, src, sizeof(half));
			dst[0]  = (half >> 8) & 0xF8;
			dst[0] |= dst[0] >> 5;
			dst[1]  = (half >> 3) & 0xF8;
			dst[0] |= dst[1] >> 5;
			dst[2]  = (half << 2) & 0xF8;
			dst[0] |= dst[2] >> 5;
			dst[3]  = half & 1 ? 0xFF : 0x00;
			break;

		case GSP_RGBA4_OES:
			memcpy(&half, src, sizeof(half));
			dst[0]  = (half >> 8) & 0xF0;
			dst[0] |= dst[0] >> 4;
			dst[1]  = (half >> 4) & 0xF0;
			dst[1] |= dst[1] >> 4;
			dst[2]  = (half >> 0) & 0xF0;
			dst[2] |= dst[2] >> 4;
			dst[3]  = (half << 4) & 0xF0;
			dst[3] |= dst[3] >> 4;
			break;
	}
}

static inline u8* get_pixel(u8 *fb, u16 x, u16 y, u16 w, u16 h, u32 bpp)
{
	return &fb[(x*w + (w-y-1))*bpp];
}

static void fill_png_buffer(void)
{
	size_t x, y;
	u8     *p = png_buffer;
	u16    fbWidth, fbHeight;
	u8     *fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &fbWidth, &fbHeight);
	GSPGPU_FramebufferFormats topFmt = gfxGetScreenFormat(GFX_TOP);
	GSPGPU_FramebufferFormats botFmt = gfxGetScreenFormat(GFX_BOTTOM);
	u32    topBPP = bytes_per_pixel(topFmt);
	u32    botBPP = bytes_per_pixel(botFmt);

	/* top screen */
	for(y = 0; y < 240; ++y)
	{
		for(x = 0; x < 400; ++x)
		{
			u8 *pixel = get_pixel(fb, x, y, fbWidth, fbHeight, topBPP);
			pixel_to_rgba(p, pixel, topFmt);
			p += 4;
		}
	}

	/* bottom screen */
	fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &fbWidth, &fbHeight);
	for(y = 0; y < 240; ++y)
	{
		for(x = 0; x < 400; ++x)
		{
			if(x < 40 || x > 360)
			{
				*p++ = 0x00;
				*p++ = 0x00;
				*p++ = 0x00;
				*p++ = 0x00;
			}
			else
			{
				u8 *pixel = get_pixel(fb, x-40, y, fbWidth, fbHeight, botBPP);
				pixel_to_rgba(p, pixel, botFmt);
				p += 4;
			}
		}
	}
}

int screenshot_png(const char *path, int level)
{
	size_t i;
	png_structp png_ptr;
	png_infop info_ptr;
	FILE *fp;

	if(level < Z_NO_COMPRESSION || level > Z_BEST_COMPRESSION)
	{
		fprintf(stderr, "invalid compression level %d\n", level);
		return -1;
	}

	for(i = 0; i < 480; ++i)
		png_lines[i] = &png_buffer[400*4*i];

	fp = fopen(path, "wb");
	if(fp == NULL)
	{
		fprintf(stderr, "failed to open '%s'\n", path);
		return -1;
	}

	setvbuf(fp, NULL, _IOFBF, 1024*8);

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_file_error, png_file_warning);
	if(png_ptr == NULL)
	{
		fprintf(stderr, "png_create_write_struct failed\n");
		fclose(fp);
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		fprintf(stderr, "png_create_info_struct failed\n");
		png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		return -1;
	}

	png_set_write_fn(png_ptr, fp, png_file_write, png_file_flush);
	
#ifdef USE_CALLBACK

	png_set_write_status_fn(png_ptr, png_row_callback);

#endif

	if(setjmp(png_jmpbuf(png_ptr)))
	{
		fprintf(stderr, "png failure\n");
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return -1;
	}

	png_set_compression_level(png_ptr, level);
	png_set_IHDR(png_ptr, info_ptr, 400, 480, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	fill_png_buffer();
	png_write_image(png_ptr, png_lines);

	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	fprintf(stderr, "\x1b[2;0H    \n");

	return 0;
}

int lastNumber = -1;

void genScreenshotFileName(int lastNumber, char *fileName, const char *ext)
{

	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
	int num = lastNumber;
	int day = timeStruct->tm_mday;
	int month = timeStruct->tm_mon + 1;
	int year = timeStruct->tm_year;
	
	if (!(dirExists("/screenshots/")))
		makeDir("/screenshots");

	sprintf(fileName, "/screenshots/screenshot-%d-%d-%d-%i-%s", day, month, year, num, ext);
}

void captureScreenshot()
{	
	sprintf(fileName, "%s", "screenshot"); 

	if(lastNumber == -1)
	{
		lastNumber = 0;
	}

	genScreenshotFileName(lastNumber, fileName, ".png"); 
	
	while (fileExists(fileName))
	{
		lastNumber++;
		genScreenshotFileName(lastNumber, fileName, ".png");
	}
	
	screenshot_png(fileName, Z_NO_COMPRESSION);
	lastNumber++;
}