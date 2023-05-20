#ifndef ASYNC_OTA_H_
#define ASYNC_OTA_H_

#include <ESPAsyncWebServer.h>

#include "Arduino.h"
#include "Update.h"

class AsyncOtaClass {
 public:
  AsyncOtaClass();

  void onOTAStart(void callable(void));
  void onOTAProgress(void callable(void));
  void onOTAEnd(void callable(void));
  void begin(AsyncWebServer *server, const char *url = "/ota");

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
