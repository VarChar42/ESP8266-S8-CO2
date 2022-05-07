#include <ESP8266WiFi.h>
#include <settings.h>
#include "logging.h"

void setupWlan()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connecting to WiFi ");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    char message[128];

    log("Wi-Fi connected.");
    snprintf(message, 128, "SSID: %s", WiFi.SSID().c_str());
    log(message);
    snprintf(message, 128, "BSSID: %s", WiFi.BSSIDstr().c_str());
    log(message);
    snprintf(message, 128, "Hostname: %s", WiFi.hostname().c_str());
    log(message);
    snprintf(message, 128, "MAC address: %s", WiFi.macAddress().c_str());
    log(message);
    snprintf(message, 128, "IPv4 address: %s", WiFi.localIP().toString().c_str());
    log(message);
    snprintf(message, 128, "IPv4 subnet mask: %s", WiFi.subnetMask().toString().c_str());
    log(message);
    snprintf(message, 128, "IPv4 gateway: %s", WiFi.gatewayIP().toString().c_str());
    log(message);
    snprintf(message, 128, "Primary DNS server: %s", WiFi.dnsIP(0).toString().c_str());
    log(message);
    snprintf(message, 128, "Secondary DNS server: %s", WiFi.dnsIP(1).toString().c_str());
    log(message);
}