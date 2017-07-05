#define MAX_LEN_RESPONSE 250

void handle_command(AsyncWebServerRequest * request) {
  char response[MAX_LEN_RESPONSE] = "{\"result\": \"OK\"}";
  unsigned int status_code = 200;

  String payload = "";

  DEBUGGINGC("Handling command\n");

  int args = request->args();
  DEBUGGINGC("%d\n", args);
  for (int i = 0; i < args; i++) {
    DEBUGGINGC("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
  }

  if (request->hasParam("download", true)) {
    AsyncWebParameter* p = request->getParam("download", true);
    DEBUGGINGC("PARAM[%s]: %s\n", p->name().c_str(), p->value().c_str());
  }
  //status_code = phogo_controller(request->arg("plain"), response, MAX_LEN_RESPONSE);

  request->send(status_code, "text/json", response);
}

void handler_404(AsyncWebServerRequest * request) {

  DEBUGGING("%s\n", request->url().c_str());
#ifdef DEBUG
  String message = "404 Not Found\n\n";

  message += "\nMethod: ";
  if (request->method() == HTTP_GET)
    message += "GET";
  else if (request->method() == HTTP_POST)
    message += "POST";
  else if (request->method() == HTTP_DELETE)
    message += "DELETE";
  else if (request->method() == HTTP_PUT)
    message += "PUT";
  else if (request->method() == HTTP_PATCH)
    message += "PATCH";
  else if (request->method() == HTTP_HEAD)
    message += "HEAD";
  else if (request->method() == HTTP_OPTIONS)
    message += "OPTIONS";
  else
    message += "UNKNOWN";

  message += "\nURL: ";
  message += "http://" + request->host() + request->url() + "\n";

  if (request->contentLength()) {
    message += "Content-Type: " + request->contentType() + "\n";
    message += "Content-Length: " + request->contentLength();
    message += "\n";
  }

  int headers = request->headers();
  if (headers) {
    message += "\n\nHeaders:\n";
  }
  int i;
  for (i = 0; i < headers; i++) {
    AsyncWebHeader* h = request->getHeader(i);
    message += "\t" + h->name() + ": " +  h->value() + "\n";
  }

  int params = request->params();
  if (params) {
    message += "\nParameters:\n";
  }
  for (i = 0; i < params; i++) {
    AsyncWebParameter* p = request->getParam(i);
    if (p->isFile()) {
      message += "\tFile [" + p->name() + "]: " + p->value() + " size: " + p->size() + "\n";
    } else if (p->isPost()) {
      message += "\tPOST [" + p->name() + "]: " + p->value() + "\n";
    } else {
      message += "\tGET [" + p->name() + "]: " + p->value() + "\n";
    }
  }

  request->send(404, "text/plain", message);
#else
  request->send(404);
#endif //DEBUG?
}

void HTTPServerSetup ( void ) {

  // entrypoints
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // api
  server.on ( "/json", HTTP_POST, handle_command);

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (!index)
      Serial.printf("BodyStart: %u/%u\n", len, total);
    Serial.printf("%s", (const char*)data);
    if (index + len == total)
      Serial.printf("BodyEnd: %u\n", total);
  });

  server.onFileUpload([](AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      Serial.printf("UploadStart: %u/%s\n", len, filename.c_str());
    }
    for (size_t i = 0; i < len; i++) {
      Serial.write(data[i]);
    }
    if (final) {
      Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index + len);
    }
  });

  server.onNotFound( handler_404 );

  server.begin();

  DEBUGGING("HTTP AsyncServer started\n");
}
