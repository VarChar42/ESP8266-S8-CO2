#include <ESP8266WebServer.h>
#include "logging.h"

#define HTTP_METRICS_ENDPOINT "/metrics"
#define PROM_NAMESPACE "IOT_CO2"

ESP8266WebServer http_server(8080);
int16_t (*metricCallback)();

void handle_http_root();
void log_request();
void handle_http_not_found();
void handle_http_metrics();
void get_http_method_name(char *name, size_t name_length, HTTPMethod method);

void handle_http_client()
{
    http_server.handleClient();
}

void setup_http_server(int16_t (*func)())
{
    metricCallback = func;
    char message[128];
    log("Setting up HTTP server");
    http_server.on("/", HTTPMethod::HTTP_GET, handle_http_root);
    http_server.on(HTTP_METRICS_ENDPOINT, HTTPMethod::HTTP_GET, handle_http_metrics);
    http_server.onNotFound(handle_http_not_found);
    http_server.begin();
    log("HTTP server started", LogLevel::DEBUG);
    snprintf(message, 128, "Metrics endpoint: %s", HTTP_METRICS_ENDPOINT);
    log(message);
}

void handle_http_root()
{
    log_request();
    static char const *response = "Prometheus ESP8266 S8 CO2 sensor entpoint.";
    http_server.send(200, "text/plain; charset=utf-8", response);
}

void handle_http_metrics()
{
    log("begin metric request");

    float metric = metricCallback();

    if (metric == 0)
    {
        http_server.send(200, "text/plain; charset=utf-8", "# Could not get value");
        return;
    }

    static size_t const BUFSIZE = 512;
    static char const *response_template =
        "# TYPE " PROM_NAMESPACE "_co2_ppm gauge\n" PROM_NAMESPACE "_co2_ppm %f\n";

    char response[BUFSIZE];
    snprintf(response, BUFSIZE, response_template, metric);
    http_server.send(200, "text/plain; charset=utf-8", response);

    log("end metric request");
}

void handle_http_not_found()
{
    log_request();
    http_server.send(404, "text/plain; charset=utf-8", "Not found.");
}

void log_request()
{
    char message[128];

    snprintf(message, 128, "Request: client=%s:%u path=%s",
             http_server.client().remoteIP().toString().c_str(), http_server.client().remotePort(), http_server.uri().c_str());
    log(message, LogLevel::INFO);
}
