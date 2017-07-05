#include <Arduino.h>

String getContentType(String filename){
	if(http_server.hasArg("download"))   return "application/octet-stream";
	else if(filename.endsWith(".htm"))   return "text/html";
	else if(filename.endsWith(".html"))  return "text/html";
	else if(filename.endsWith(".css"))   return "text/css";
	else if(filename.endsWith(".js"))    return "application/javascript";
	else if(filename.endsWith(".png"))   return "image/png";
	else if(filename.endsWith(".gif"))   return "image/gif";
	else if(filename.endsWith(".jpg"))   return "image/jpeg";
	else if(filename.endsWith(".jpeg"))  return "image/jpeg";
	else if(filename.endsWith(".ico"))   return "image/x-icon";
	else if(filename.endsWith(".xml"))   return "text/xml";
	else if(filename.endsWith(".ttf"))   return "application/x-font-truetype";
	else if(filename.endsWith(".eof"))   return "application/vnd.ms-fontobject";
	else if(filename.endsWith(".woff"))  return "application/font-woff";
	else if(filename.endsWith(".woff2")) return "application/font-woff2";
	else if(filename.endsWith(".pdf"))   return "application/x-pdf";
	else if(filename.endsWith(".zip"))   return "application/x-zip";
	else if(filename.endsWith(".gz"))    return "application/x-gzip";
	return "text/plain";
}

bool handle_file_read(String path){

	char filepath[50];
	path.toCharArray(filepath, 50);

	DEBUGGING("%s\n", filepath);
	if(path.endsWith("/")) path += "index.html";
	String contentType = getContentType(path);
	String pathWithGz = path + ".gz";
	if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
		DEBUGGING("%s found!\n", filepath);
		if(SPIFFS.exists(pathWithGz))
			path += ".gz"; //gzip preferred
		File file = SPIFFS.open(path, "r");
		size_t sent = http_server.streamFile(file, contentType);
		file.close();
		return true;
	}
	DEBUGGING("%s NOT found!\n", filepath);
	return false;
}


bool handle_root(void) {

	DEBUGGING("HTTP Server: handleRoot\n");
	return handle_file_read("/");

}

#define MAX_LEN_RESPONSE 250

void handle_command(void) {
	char response[MAX_LEN_RESPONSE];	
	unsigned int status_code = 0;

	status_code = phogo_controller(http_server.arg("plain"), response, MAX_LEN_RESPONSE);
	
	http_server.send(status_code, "text/json", response);
}

void handler_404(void) {

	if (handle_file_read(http_server.uri())) {
		return;
	}

	// digitalWrite ( led, 1 );
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += http_server.uri();
	message += "\nMethod: ";
	message += ( http_server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += http_server.args();
	message += "\n";

	for ( uint8_t i = 0; i < http_server.args(); i++ ) {
		message += " " + http_server.argName ( i ) + ": " + http_server.arg ( i ) + "\n";
	}

	http_server.send ( 404, "text/plain", message );
	// digitalWrite ( led, 0 );
}

void HTTPServerSetup ( void ) {
	
	// entrypoints
	http_server.on ( "/", handle_root );
	http_server.on ( "/index.html", handle_root );
	// api
	http_server.on ( "/json", HTTP_POST, handle_command);

	// handle generic files through the NotFound Handler
	http_server.onNotFound ( handler_404 );
	http_server.begin();

	DEBUGGING("HTTP server started\n");
}
