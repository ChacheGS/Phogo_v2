#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <string.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

#include "debug.h"

const char* ssid = "Low Signal";
const char* password = "sierragador15";
// const char* ssid = "Chache Hotspot";
// const char* password = "wificarlos";
const char* mdns_hostname = "phogo";

//Globals
AsyncWebServer server(80);

#define WIFI_CONNECTION_TIMEOUT 1000 * 10 // 10 s total
// Wifi Connection
bool isAP = false;
void WifiConnect() {
    int connection_time = millis();
    bool connection_success = true;
    WiFi.hostname(mdns_hostname);
    WiFi.mode(WIFI_STA);

    DEBUGGING("[WIFI] connecting to '%s'\n", ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && connection_success) {
        if (millis() - connection_time > WIFI_CONNECTION_TIMEOUT) {
            // we leave the loop after the timeout
            connection_success = false;
        }
        delay(100);
    }
    if (connection_success) {
        DEBUGGING("WiFi Connected. Local IP: ");
        DEBUGGINGL(WiFi.localIP());
        DEBUGGINGC("\n");
        return;
    } else {
        DEBUGGING("WiFi Connection to '%s' timed out after ", ssid);
        DEBUGGINGL((millis() - connection_time) / 1000.0);
        DEBUGGINGL(" s\n");
    }

    // TODO: maybe retry connection?

    char APName[20] = {0};
    sprintf(APName, "Phogo%08X", ESP.getFlashChipId());
    DEBUGGING("Starting AP mode as '%s'\n", APName);
    WiFi.mode(WIFI_AP);
    // ap config
    IPAddress ip(10, 0, 0, 2);
    IPAddress gateway(10, 0, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(APName);

    delay(500);
    DEBUGGING("Started AP mode as '%s' [ip=", APName);
    DEBUGGINGL(WiFi.softAPIP());
    DEBUGGINGC("]\n");
    isAP = true;
}

void stop_forever() {
    while (1) {
        delay(1000);
    }
}

// mDNS
void mDNSConnect() {
    if (!MDNS.begin(mdns_hostname)) {
        DEBUGGING("Error setting up mDNS!\n");
        stop_forever();
    }
    DEBUGGING("mDNS started: 'http://%s.local/'\n", mdns_hostname);
    MDNS.addService("http", "tcp", 80);
}

void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) {
        // wait for initialization
    }
    // Serial.println();
#endif

    SPIFFS.begin();
    server.begin();

    WifiConnect();
    mDNSConnect();
    HTTPServerSetup();
    phogo_setup();
}

int lastTimeHost;
int lastTimeRefresh;

void loop() {
    if ((WiFi.status() != WL_CONNECTED) && !isAP) {
        // TODO: ensure_connection()
        WifiConnect();
        mDNSConnect();
    }

    delay(1000);
}
