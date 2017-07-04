#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <string.h>

#include <Arduino.h>

#define DEBUG true

#include "debug.h"

// const char* ssid = "JAZZTEL_FkyP";
// const char* password = "ua37pfce6kg5";
const char* ssid = "Chache Hotspot";
const char* password = "wificarlos";
const char* host = "phogo";

//Globals
ESP8266WebServer http_server(80);

/* Support for OTAs (Over-The-Air Updates) */
ESP8266HTTPUpdateServer http_updater(DEBUG);

const char* OTAUser     = "";
const char* OTAPassword = "";
void OTAService(ESP8266WebServer* server, const char* path) {
    if (!strcmp(OTAUser, "") && !strcmp(OTAPassword, "")) {
        http_updater.setup(server, path);
    } else {
        http_updater.setup(server, path, OTAUser, OTAPassword);
    }
}

#define WIFI_CONNECTION_TIMEOUT 1000 * 10 // 10 s
// Wifi Connection
void WifiConnect() {
    int connection_time = millis();
    bool connection_success = true;
    // WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && connection_success) {
        if (millis() - connection_time > WIFI_CONNECTION_TIMEOUT) {
            // we leave the loop after the timeout
            connection_success = false;
        }
        delay(100);
    }
    if (connection_success) {
        DEBUGGING("WiFi Connected. Local IP: %u.%u.%u.%u\n", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    } else {
        DEBUGGING("WiFi Connection to '%s' timed out after %.2f s\n", ssid, (millis() - connection_time) / 1000.0);
    }

    // TODO: maybe retry connection?
    
    // TODO: start a WiFi AP

}

// mDNS
void mDNSConnect() {
    if (!MDNS.begin(host)) {
        DEBUGGING("Error setting up mDNS!\n");
        while (1) {
            delay(1000);
        }
    }
    DEBUGGING("mDNS started\n");
    // MDNS.addService("ws", "tcp", 81);
    MDNS.addService("http", "tcp", 80);
}

// HTTP updater connection
void HTTPUpdateConnect() {
    http_updater.setup(&http_server);
    http_server.begin();
    DEBUGGING("HTTPUpdateServer ready!\n");
    DEBUGGING("Updater available at: http://%s.local/update\n", host);
}


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
    mDNSConnect();
    HTTPUpdateConnect();
    HTTPServerSetup();

}

int lastTimeHost;
int lastTimeRefresh;
// the loop function runs over and over again forever
void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        WifiConnect();
        //WebSocketConnect();
        //mDNSConnect();
    } else {

        // webSocket.loop();
        // give the ESP a change to du its own stuff
        // we serve the client once every 10 ms
        if (millis() - lastTimeHost > 10) {
            http_server.handleClient();
            lastTimeHost = millis();
        }
    }
}
