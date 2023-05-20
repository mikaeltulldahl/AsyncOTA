# AsyncOTA
Minimalist ESP32 Over-The-Air(OTA) firmware update using a browser or PlatformIO, using TCP and AsyncWebServer.

## Usage

### platformio.ini
If you are using PlatformIO, you can install the latest version of the library using the PlatformIO Library Manager or by adding `AsyncOTA` to your `platformio.ini` file like:
```ini
; platformio.ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = mikaeltulldahl/AsyncOta@^1.0.1
```

### Setup

```c++
#include "AsyncOTA.h"
#include "WIFI.h"

AsyncWebServer server(80);

void setup(){
  // Start wifi first
  WiFi.mode(WIFI_STA);
  WiFi.begin("my wifi name", "my wifi password");
  while (WiFi.status() != WL_CONNECTED) { delay(500);}
  
  // Start OTA server
  AsyncOTA.begin(&server);
  server.begin();
}
```
### Loop
Nothing needed here.
```c++
void loop(){
}
```

### Browser Upload
- Go to `http://<your_URL>/ota>` or `http://<my_hostname>.local/ota` if you enabled mDNS
- Navigate to and select `.pio\build\<my_env_name>\firmware.bin`.
- Click Update

### PlatformIO Upload

- Copy the file `platformio_upload.py` from this repository into the same folder as your `platformio.ini` file
- Set the upload method for your project in `platformio.ini`:

```ini
extra_scripts = platformio_upload.py
upload_protocol = custom
custom_ota_upload_url = <your upload URL, such as http://192.168.1.123/update>
;custom_ota_upload_url = http://<my_hostname>.local/update ; with mDNS
``` 
- Click the Upload button in PlatformIO

### Tips'n'Tricks
It's useful to have multiple envs in `platformio.ini` to specify different upload modes. Here's a complete example:
```ini
; platformio.ini
[esp32_base]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = mikaeltulldahl/AsyncOta@^1.0.1

; Upload over USB
[env:esp32_USB]
extends = esp32_base

; Upload over WIFI
[env:esp32_OTA]
extends = esp32_base
extra_scripts = platformio_upload.py
upload_protocol = custom
custom_ota_upload_url = <your upload URL, such as http://192.168.1.123/update>
;custom_ota_upload_url = http://<my_hostname>.local/update ; with mDNS
```

### Troubleshooting
If you don't know the URL your esp32 is hosting at, find it using an IP scanner like [Angry IP Scanner](https://angryip.org/).

## Credits

This project utilizes code from the following libraries:

- [ArduinoOTA](https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/OTAWebUpdater/OTAWebUpdater.ino)

  License: GNU Lesser General Public License v2.1

- [AsyncElegantOTA](https://github.com/ayushsharma82/AsyncElegantOTA/blob/master/platformio_upload.py)

  License: MIT-License
