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

void handleHttpClient() {
    http_server.handleClient();
}

void setupHttpServer(int16_t (*func)()) {
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

void handle_http_root() {
    log_request();
    static char const *response = "Prometheus ESP8266 S8 CO2 sensor entpoint.";
    http_server.send(200, "text/plain; charset=utf-8", response);
}

void handle_http_metrics() {
    log_request();
    static size_t const BUFSIZE = 512;
    static char const *response_template =
        "# TYPE " PROM_NAMESPACE "_co2_ppm gauge\n"
        PROM_NAMESPACE "_co2_ppm %f\n";

    float metric = metricCallback();

    char response[BUFSIZE];    
    snprintf(response, BUFSIZE, response_template, metric);
    http_server.send(200, "text/plain; charset=utf-8", response);
}

void handle_http_not_found() {
    log_request();
    http_server.send(404, "text/plain; charset=utf-8", "Not found.");
}

void log_request() {
    char message[128];
    char method_name[16];
    get_http_method_name(method_name, 16, http_server.method());
    snprintf(message, 128, "Request: client=%s:%u method=%s path=%s",
            http_server.client().remoteIP().toString().c_str(), http_server.client().remotePort(), method_name, http_server.uri().c_str());
    log(message, LogLevel::INFO);
}

void get_http_method_name(char *name, size_t name_length, HTTPMethod method) {
    switch (method) {
    case HTTP_GET:
        snprintf(name, name_length, "GET");
        break;
    case HTTP_HEAD:
        snprintf(name, name_length, "HEAD");
        break;
    case HTTP_POST:
        snprintf(name, name_length, "POST");
        break;
    case HTTP_PUT:
        snprintf(name, name_length, "PUT");
        break;
    case HTTP_PATCH:
        snprintf(name, name_length, "PATCH");
        break;
    case HTTP_DELETE:
        snprintf(name, name_length, "DELETE");
        break;
    case HTTP_OPTIONS:
        snprintf(name, name_length, "OPTIONS");
        break;
    default:
        snprintf(name, name_length, "UNKNOWN");
        break;
    }
}