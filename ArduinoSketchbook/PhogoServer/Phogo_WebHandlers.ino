void handle_root(void) {

	DEBUGGING("HTTP Server: handleRoot\n");
	http_server.send( 200, "text/plain", "This is the root." );

}

#define MAX_LEN_RESPONSE 250

void handle_command(void) {
	char response[MAX_LEN_RESPONSE];	
	unsigned int status_code = 0;

	status_code = phogo_controller(http_server.arg("plain"), response, MAX_LEN_RESPONSE);
	
	http_server.send(status_code, "text/json", response);
}

void handler_404() {
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
	
	http_server.on ( "/", handle_root );
	http_server.on ( "/command", HTTP_POST, handle_command);

	http_server.onNotFound ( handler_404 );
	http_server.begin();

	DEBUGGING("HTTP server started");
}
