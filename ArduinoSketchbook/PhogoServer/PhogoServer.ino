#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <WebSocketsServer.h>
#include <ESP8266HTTPUpdateServer.h>
// #include <ESP8266mDNS.h>
#include <FS.h>
#include <string.h>

#include <Arduino.h>

#define DEBUG true
// #define DBG_PORT Serial
#include "debug.h"

const char* ssid = "JAZZTEL_FkyP";
const char* password = "ua37pfce6kg5";
// const char* ssid = "Chache Hotspot";
// const char* password = "wificarlos";
const char* host = "phogo";

//Globals
ESP8266WebServer http_server(80);
//WebSocketsServer webSocket = WebSocketsServer(81);

/* Support for OTAs (Over-The-Air Updates) */
ESP8266HTTPUpdateServer http_updater(DEBUG);

const char* OTAUser     = "";
const char* OTAPassword = "";
void OTAService(ESP8266WebServer* server, const char* path) {
    if (strcmp(OTAUser, "") && strcmp(OTAPassword, "")) {
        http_updater.setup(server, path);
    } else {
        http_updater.setup(server, path, OTAUser, OTAPassword);
    }
}

/*void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

    char buffer[30];
    char* resp = NULL;

    switch (type) {
        case WStype_DISCONNECTED: {
            DEBUGGING("[%u] Disconnected!\n", num);
        }
        break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            DEBUGGING("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
        break;

        case WStype_TEXT: {
            // CODE
            DEBUGGING("[%u] got Text: %s\n", num, payload);

            // send message to client
            resp = doStuff(buffer, payload);

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");

            if (resp != NULL) {
                webSocket.sendTXT(num, resp);
            }
            resp = NULL;
        }
        break;

        case WStype_BIN: {
            DEBUGGING("[%u] got binary lenght: %u\n", num, length);

            hexdump(payload, length);
            
            // echo data back to browser
            webSocket.sendBIN(num, payload, length);
        }
        break;
    }
}*/

// Wifi Connection
void WifiConnect() {

    // WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    DEBUGGING("WiFi Connected. Local IP: %u.%u.%u.%u\n", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
}

// WebSocket Connection
/*void WebSocketConnect() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}*/

// MDNS
/*void MDNSConnect() {
    if (!MDNS.begin(host)) {
        DEBUGGING("Error setting up MDNS responder!\n");
        while (1) {
            delay(1000);
        }
    }
    DEBUGGING("mDNS responder started\n");
    MDNS.addService("ws", "tcp", 81);
    MDNS.addService("http", "tcp", 80);
}*/

// HTTP updater connection
/*void HTTPUpdateConnect() {
    http_updater.setup(&http_server);
    // http_server.begin();
    // DEBUGGING("HTTPUpdateServer ready!\n");
    // DEBUGGING("Type http://%s.local/update in your browser\n", host);
}*/


void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) {
        // wait for initialization
    }
    // Serial.println();
#endif

    WifiConnect();
    //WebSocketConnect();
    //MDNSConnect();
    //HTTPUpdateConnect();
    HTTPServerSetup();

}

int lastTimeHost;
int lastTimeRefresh;
// the loop function runs over and over again forever
void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        WifiConnect();
        //WebSocketConnect();
        //MDNSConnect();
    } else {

        // webSocket.loop();
        if (millis() - lastTimeHost > 10) {
            http_server.handleClient();
            lastTimeHost = millis();
        }
    }
}
