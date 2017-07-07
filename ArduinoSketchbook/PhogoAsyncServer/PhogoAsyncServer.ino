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
#define RETRIES 3
uint8_t consumed_retries = 0;
// Wifi Connection
bool isAP = false;
void WifiConnect() {
    int connection_time = 0;
    int connection_start = millis();
    bool connection_success = true;
    WiFi.hostname(mdns_hostname);
    WiFi.mode(WIFI_STA);

    DEBUGGING("[WIFI]\tconnecting to '%s'\n", ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && connection_success) {
        connection_time = millis() - connection_start;
        if (connection_time > WIFI_CONNECTION_TIMEOUT) {
            // we leave the loop after the timeout
            connection_success = false;
        }
        delay(100);
    }
    if (connection_success) {
        DEBUGGING("[WIFI]\tConnected to '%s'. Local IP: ", ssid);
        DEBUGGINGL(WiFi.localIP());
        DEBUGGINGC("\n");
        return;
    } else {
        DEBUGGING("[WIFI]\tConnection to '%s' timed out after ", ssid);
        DEBUGGINGL(connection_time / 1000.0);
        DEBUGGINGL(" s\n");
    }

    // TODO: maybe retry connection?

    char APName[20]\t= {0};
    sprintf(APName, "Phogo%08X", ESP.getFlashChipId());
    DEBUGGING("[WIFI]\tStarting AP mode as '%s'\n", APName);
    WiFi.mode(WIFI_AP);
    // ap config
    IPAddress ip(10, 0, 0, 2);
    IPAddress gateway(10, 0, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.softAPConfig(ip, gateway, subnet);
    
    connection_success = WiFi.softAP(APName, APName);
    delay(500);

    if (connection_success) {
        DEBUGGING("[WIFI]\tStarted AP mode as '%s' [password=%s] [ip=", APName, APName);
        DEBUGGINGL(WiFi.softAPIP());
        DEBUGGINGC("]\n");
        isAP = true;
    } else {
        DEBUGGING("[WIFI]\tAP mode failed\n");
    }
    
    if (consumed_retries > RETRIES) {
        DEBUGGING("[PHOGO]\tConnection failed. Stopping forever.");
        stop_forever();
    } else {
        ++consumed_retries;
        WifiConnect();
    }
}

void stop_forever() {
    init_led();
    while (1) {
        led(ON);
        delay(50);
        led(OFF);
        delay(50);
    }
}

// mDNS
void mDNSConnect() {
    if (!MDNS.begin(mdns_hostname)) {
        DEBUGGING("[MDNS]\tSetup error\n");
    }
    DEBUGGING("[MDNS]\tStarted: 'http://%s.local/'\n", mdns_hostname);
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
}

int lastTimeHost;
int lastTimeRefresh;

void loop() {
    if (WiFi.status() != WL_CONNECTED && !isAP) {
        // TODO: ensure_connection()
        WifiConnect();
        mDNSConnect();
    }

    delay(1000);
}
