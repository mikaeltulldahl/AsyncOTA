#ifndef ASYNC_OTA_H_
#define ASYNC_OTA_H_

#include "Arduino.h"
#include "stdlib_noniso.h"

#define HARDWARE "ESP32"
#include <ESPAsyncWebServer.h>

#include "Update.h"
#include "WiFi.h"
#include "WiFiClient.h"

class AsyncOtaClass {
 public:
  AsyncOtaClass();

  void onOTAStart(void callable(void));
  void onOTAProgress(void callable(void));
  void onOTAEnd(void callable(void));
  void begin(AsyncWebServer *server);

 private:
  AsyncWebServer *_server;

  bool _preUpdateRequired = false;
  bool _progressUpdateRequired = false;
  bool _postUpdateRequired = false;
  void (*preUpdateCallback)();
  void (*progressUpdateCallback)();
  void (*postUpdateCallback)();
};

extern AsyncOtaClass AsyncOTA;
#endif  // ASYNC_OTA_H_
