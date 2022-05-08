#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "s8_uart.h"
#include "wlan.h"
#include "prometheus.h"


SoftwareSerial S8_serial(5, 4);

S8_UART *sensor_S8;
S8_sensor sensor;


int16_t get_metric() {
  return sensor_S8->get_co2();;
}

void setup() {
    Serial.begin(115200);
  Serial.println("Starting...");

  S8_serial.begin(S8_BAUDRATE);
  sensor_S8 = new S8_UART(S8_serial);

  sensor_S8->get_firmware_version(sensor.firm_version);
  int len = strlen(sensor.firm_version);
  if (len == 0) {
      Serial.println("SenseAir S8 CO2 sensor not found!");
  } else {
    Serial.println(">>> SenseAir S8 NDIR CO2 sensor <<<");
    printf("Firmware version: %s\n", sensor.firm_version);
    sensor.sensor_id = sensor_S8->get_sensor_ID();
    Serial.print("Sensor ID: 0x"); printIntToHex(sensor.sensor_id, 4); Serial.println("");
  }
  Serial.println("Setup done!");


  setup_wlan();
  setup_http_server(&get_metric);

  Serial.flush();
}

void loop() {
  handle_http_client();
}

