/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "YourSSIDHere";
const char *password = "YourPSKHere";

ESP8266WebServer server ( 80 );*/

// #include <ArduinoJson.h>

// const int led = LED_BUILTIN;

/*void handleRoot1() {
	digitalWrite ( led, 1 );
	char temp[400];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf ( temp, 400,

	          "<html>\
	          <head>\
	          <meta http-equiv='refresh' content='5'/>\
	          <title>ESP8266 Demo</title>\
	          <style>\
	          body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
	          </style>\
	          </head>\
	          <body>\
	          <h1>Hello from ESP8266!</h1>\
	          <p>Uptime: %02d:%02d:%02d</p>\
	          <img src=\"/test.svg\" />\
	          </body>\
	          </html>",

	          hr, min % 60, sec % 60
	          );
	http_server.send ( 200, "text/html", temp );
	digitalWrite ( led, 0 );
}*/


void handler_root(void) {

	DEBUGGING("HTTP Server: handleRoot\n");
	http_server.send( 200, "text/plain", "This is the root." );

}

#define MAX_LEN_RESPONSE 200

void handler_api_do(void) {
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
	
	http_server.on ( "/", handler_root );
	http_server.on ( "/do", HTTP_POST, handler_api_do);

	// http_server.on("/post", HTTP_POST, [](){
		// http_server.send ( 200, "text/json", "{\"http_server\":\"plain\"}" );
		// http_server.send(200, "text/json", http_server.arg("plain"));
	/*	StaticJsonBuffer<200> buff;
		JsonObject& req = buff.parseObject(http_server.arg("plain"));

		req.prettyPrintTo(Serial);

		req.prettyPrintTo(tmp, sizeof(tmp));

		DEBUGGING("%s\n", tmp);

		http_server.send(200, "text/json", tmp);
	});*/
	
	http_server.onNotFound ( handler_404 );
	http_server.begin();

	DEBUGGING("HTTP server started");
}
