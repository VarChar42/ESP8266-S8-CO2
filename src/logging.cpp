#include <Arduino.h>
#include "logging.h"

void log(char const *message, LogLevel level)
{
    char str_level[10];
    switch (level)
    {
    case DEBUG:
        strcpy(str_level, "DEBUG");
        break;
    case INFO:
        strcpy(str_level, "INFO");
        break;
    case ERROR:
        strcpy(str_level, "ERROR");
        break;
    default:
        break;
    }
    char record[150];
    snprintf(record, 150, "[%-5s] %s", str_level, message);
    Serial.println(record);
}