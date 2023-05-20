# AsyncOTA
ESP32 OTA update using a browser or PlatformIO

## Installation

### PlatformIO
If you are using PlatformIO, you can install the latest version of the library using the PlatformIO Library Manager or by adding `ESPAsyncOTA` to your `platformio.ini` file like:
```ini
; platformio.ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = https://github.com/mikaeltulldahl/AsyncOTA.git
```

### PlatformIO Manual Method
- Go to http://<your_upload_URL>/ota> or http://<my_hostname>.local/ota if you enabled mDNS
- navigate and select .pio\build\esp32\firmware.bin
- click Update

### PlatformIO Automatic Method

- Copy the file "platformio_upload.py" from this repository into the same folder as your `platformio.ini` file
- Set the upload method for your project in `platformio.ini`:

```
extra_scripts = platformio_upload.py
upload_protocol = custom
upload_url = <your upload URL, such as http://192.168.1.123/update>
;custom_ota_upload_url = http://<my_hostname>.local/update ; with mDNS
``` 

### Troubleshooting
If you don't know the URL, download an IP scanner like [Angry IP Scanner](https://angryip.org/).

## Credits

This project utilizes code from the following libraries:

- [ArduinoOTA](https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/OTAWebUpdater/OTAWebUpdater.ino)

  License: GNU Lesser General Public License v2.1

- [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA/blob/master/platformio_upload.py)

  License: MIT-License