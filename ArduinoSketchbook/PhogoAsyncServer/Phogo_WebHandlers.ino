#define MAX_LEN_RESPONSE 250

static String current_command = String();

void get_command(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  if (!index) {
    DEBUGGING("Body Start: %u/%u\n", len, total);
  }
  DEBUGGING("Body in progress: |%s| %uB\n", (const char*)data, index + len);
  current_command += String((const char*)data);
  if (index + len == total) {
    DEBUGGING("Body Success: %uB\n", index + len);
  }
}

void handle_command(AsyncWebServerRequest * request) {
  char response[MAX_LEN_RESPONSE] = "{\"result\": \"OK\"}";
  unsigned int status_code = 0;

  DEBUGGINGC("Handling command: ");
  DEBUGGINGL(current_command.c_str());

  status_code = phogo_controller(current_command.c_str(), response, MAX_LEN_RESPONSE);

  current_command = String();

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
  server.on("/json", HTTP_POST, handle_command, NULL, get_command);

  server.onNotFound( handler_404 );

  server.begin();

  DEBUGGING("HTTP AsyncServer started\n");
}
