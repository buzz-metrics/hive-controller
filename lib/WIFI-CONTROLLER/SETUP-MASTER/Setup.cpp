#include "Setup.h"
#include <ArduinoJson.h>
#include "SPIFFS.h"
#include "ROUTES-MASTER/MyRoutes.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <DNSServer.h>

AsyncWebServer server(80);

const char *ssid = "ACCESS-POINT";
const char *password = "Password123!";

IPAddress local_IP(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);


bool configure(String config_file_name)
{
  if (!WiFi.softAPConfig(local_IP, gateway, subnet))
  {
    Serial.println("Failed to configure AP");
  }

  if (!WiFi.softAP(ssid, password))
  {
    Serial.println("Failed to start AP");
  }


    // DNS server for captive portal
    // Serial.println("Starting dns");
    // if (dnsServer.start(53, "*", WiFi.softAPIP()))
    // {
    //   Serial.println("DNS server started successfully");
    // }
    // else
    // {
    //   Serial.println("Failed to start DNS server");
    // }



  // Define routes -Static
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Routing on /");
    request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Routing on /script.js");
    request->send(SPIFFS, "/script.js", "text/javascript"); });



  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Routing on /style.css");
    request->send(SPIFFS, "/style.css", "text/css"); });
    
  server.onNotFound([](AsyncWebServerRequest *request){
    // Redirect to captive portal page
    request->redirect("/");
  });

  //
  //
  //
  //
  //
  //
  //
  //

server.on("/set_wifi_credentials", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Routing on set_wifi_credentials");
    if (!(request->hasParam("ssid") && request->hasParam("password"))) {
        request->send(400, "text/plain", "Failure: Missing parameters");
        return;
    }
    String input_ssid = request->getParam("ssid")->value();
    String password = request->getParam("password")->value();
    bool success = set_wifi_credentials(input_ssid, password);
    String response = success ? "Success" : "Failure";
    request->send(200, "text/plain", response);
});

server.on("/get_wifi_credentials", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Routing on get_wifi_credentials");
    String credentials = get_wifi_credentials();
    request->send(200, "text/plain", credentials);
});

server.on("/verify_wifi_credentials", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!(request->hasParam("ssid") && request->hasParam("password"))) {
        request->send(400, "text/plain", "Failure: Missing parameters");
        return;
    }
    String input_ssid = request->getParam("ssid")->value();
    String password = request->getParam("password")->value();
    bool success = verify_wifi_credentials(input_ssid, password);
    String response = success ? "Success" : "Failure";
    request->send(200, "text/plain", response);
});

server.on("/get_available_wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
    String available_ssid = get_available_wifi();
    request->send(200, "text/plain", available_ssid);
});


  //
  //
  //
  //
  //
  // General
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Routing on reboot");

    request->send(200, "text/plain", "Rebooting");
    ESP.restart(); });

  server.on("/restore_config_file", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Routing on restore_config_file");
    bool success = restore_config_file();

    String response = success ? "Success": "Failure";
    
    request->send(200, "text/plain", response); });


  server.begin();
  Serial.println("Setup finished");

  return true;
}