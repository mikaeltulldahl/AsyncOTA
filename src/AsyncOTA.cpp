#include "AsyncOTA.h"

/*
 * Server OTA Page
 */
const char* OTA_HTML =
    "<script "
    "src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></"
    "script>"
    "<form method='POST' action='#' enctype='multipart/form-data' "
    "id='upload_form'>"
    "<input type='file' name='update'>"
    "<input type='submit' value='Update'>"
    "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
    "$('form').submit(function(e){"
    "e.preventDefault();"
    "var form = $('#upload_form')[0];"
    "var data = new FormData(form);"
    " $.ajax({"
    "url: '/update',"
    "type: 'POST',"
    "data: data,"
    "contentType: false,"
    "processData:false,"
    "xhr: function() {"
    "var xhr = new window.XMLHttpRequest();"
    "xhr.upload.addEventListener('progress', function(evt) {"
    "if (evt.lengthComputable) {"
    "var per = evt.loaded / evt.total;"
    "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
    "}"
    "}, false);"
    "return xhr;"
    "},"
    "success:function(d, s) {"
    "console.log('success!')"
    "},"
    "error: function (a, b, c) {"
    "}"
    "});"
    "});"
    "</script>";

AsyncOtaClass::AsyncOtaClass() {}

void AsyncOtaClass::begin(AsyncWebServer* server) {
  _server = server;

  _server->on("/ota", HTTP_GET, [&](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response =
        request->beginResponse_P(200, "text/html", OTA_HTML);
    response->addHeader("Connection", "close");
    request->send(response);
  });

  _server->on(
      "/update", HTTP_POST,
      [&](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse_P(
            200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        response->addHeader("Connection", "close");
        request->send(response);

        // Needs some time for Core 0 to send response
        delay(100);
        yield();
        delay(100);
        ESP.restart();
      },
      [&](AsyncWebServerRequest* request, String filename, size_t index,
          uint8_t* data, size_t len, bool final) {
        //  Actual OTA Download
        if (index == 0) {
          Serial.printf("Update Received: %s\n", filename.c_str());
          if (!request->hasParam("MD5", true)) {
            return request->send(400, "text/plain", "MD5 parameter missing");
          }

          if (!Update.setMD5(request->getParam("MD5", true)->value().c_str())) {
            return request->send(400, "text/plain", "MD5 parameter invalid");
          }
          if (_preUpdateRequired) preUpdateCallback();
          if (filename == "filesystem") {
            // start with max available size
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS)) {
              Update.printError(Serial);
            }
          } else {
            // start with max available size
            if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
              Update.printError(Serial);
            }
          }
        }

        if (Update.write(data, len) != len) {
          Update.printError(Serial);
        }
        if (_progressUpdateRequired) progressUpdateCallback();
        Serial.printf("\rProgress: %zu%%",
                      Update.progress() * 100 / Update.size());
        if (final) {
          size_t totalBytes = Update.progress();
          if (Update.end(true)) {
            Serial.printf(
                "\nUpdate Success!\nTotal size: %u bytes.\nRebooting...\n",
                totalBytes);
          } else {
            Update.printError(Serial);
          }
          if (_postUpdateRequired) postUpdateCallback();
        }
      });
}

void AsyncOtaClass::onOTAStart(void callable(void)) {
  preUpdateCallback = callable;
  _preUpdateRequired = true;
}

void AsyncOtaClass::onOTAProgress(void callable(void)) {
  progressUpdateCallback = callable;
  _progressUpdateRequired = true;
}

void AsyncOtaClass::onOTAEnd(void callable(void)) {
  postUpdateCallback = callable;
  _postUpdateRequired = true;
}

AsyncOtaClass AsyncOTA;
