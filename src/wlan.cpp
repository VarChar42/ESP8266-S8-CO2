#include <ESP8266WiFi.h>
#include <settings.h>

void setup_wlan()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("Connecting to WiFi ");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("Connected: ");
    Serial.println(WiFi.localIP().toString());
}