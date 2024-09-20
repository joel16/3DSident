# 3DSident [![Github latest downloads](https://img.shields.io/github/downloads/joel16/3DSident/total.svg)](https://github.com/joel16/3DSident/releases/latest)

![3DSident Banner](http://i.imgur.com/HPWNgmz.png)

ctr_vercheck was originally created by wolfvak - It was initially intended to be a small application for the Nintendo 3DS to check your current FIRM and OS version only. I decided to fork this project, and added additional device info similar to PSPident, hence the name 3DSident.

# Features:
- Current kernel, FIRM and system version detection. 
- Display initial system version.
- Model detection with code name and hardware info (Retail/Devboard/Debugger/Capture unit)
- Displays screen type (TN/IPS).
- Displays region.
- Displays language.
- Displays MAC address.
- Displays serial.
- Displays SDMC and NAND CID
- Displays NNID username, principal ID, persistent ID, transferable base ID, country and timezone.
- Displays device ID.
- Displays soap ID.
- Battery charging status.
- AC Adapter connection status.
- Battery percentage (actual battery percentage using mcu::HWC).
- Displays battery voltage (estimated).
- Displays battery temperature.
- Displays MCU firmware.
- SD detection.
- Displays SD free and total storage capacity.
- Displays CTR free and total storage capacity.
- Displays TWL free and total storage capacity. (GUI exclusive)
- Displays TWL photo free and total storage capacity. (GUI exclusive)
- Displays number of titles installed on SD and NAND.
- Displays number of tickets installed. (GUI exclusive)
- Displays volume slider state and percentage.
- Displays 3D slider state and percentage.
- Displays Wifi signal strength.
- Displays IP address.
- Displays current brightness.
- Display auto-brightness status. (GUI exclusive)
- Display power saving mode. (GUI exclusive)
- Display sound output mode. (GUI exclusive)
- Displays if the console is a debug unit. (GUI exclusive)
- Displays headphone/audio jack status. (GUI exclusive)
- Card slot status and inserted card type (CTR/NAND).
- Displays parental control pin,  email address and secret answer. (GUI exclusive)
- Displays home menu ID.
- Displays Wifi slot info (SSID, password and MAC address).
- Displays original/NAND local friend code seed.

# Credits:
- **Preetisketch** for the logo/banner.
- **devkitPro maintainers and contributors** for libctru, citro2d, devkitARM, and other packages used by this project.
