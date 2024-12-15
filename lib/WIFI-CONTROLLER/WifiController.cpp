#include <Arduino.h>
#include "WifiController.h"
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "SETUP-MASTER/Setup.h"


struct Credentials
{
    String ssid;
    String password;
    int error;
};

int check_config_exists(String config_file_name);
struct Credentials parse_wifi_credentials(String config_file_name);
int isSSIDVisible(String targetSSID);
int attempt_wifi_connection(String config_file_name);
int run_setup(String config_file_name);

const String config_file_name = "credentials.json";
int connection_timeout = 30000;




WifiController::WifiController(int timeout)
{
    connection_timeout = timeout;
}

int WifiController::begin()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("Failed to mount SPIFFS");
        return 1;
    }
    Serial.println("SPIFSS mounted successfully");

    int config_exists = check_config_exists(config_file_name);

    if (config_exists)
    {
        Serial.println("Config exists");
        int wifi_success = attempt_wifi_connection(config_file_name);
        if (!wifi_success){
            int setup_success = run_setup(config_file_name);
        }
    }
    else
    {
        Serial.println("Config file doesn't exist. Running setup.");
        int setup_success = run_setup(config_file_name);
    }
    return 0;
}

int check_config_exists(String config_file_name)
{
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        String this_file_name = String(file.name());
        if (this_file_name == config_file_name)
        {
            Serial.println("Found config file");
            return 1;
        }
        file = root.openNextFile();
    }
    return 0;
}

int attempt_wifi_connection(String config_file_name)
{
    Credentials wifi_credentials = parse_wifi_credentials(config_file_name);
    if (wifi_credentials.error > 0){
        Serial.print("There was an error accessing the wifi credentials: ");
        Serial.println(wifi_credentials.error);
        return 0;
    }


    if (!isSSIDVisible(wifi_credentials.ssid)){
        return 0;
    }

    WiFi.begin(wifi_credentials.ssid, wifi_credentials.password);
    int start_millis = millis();

    Serial.print("Trying to connect..");
    while(WiFi.status() != WL_CONNECTED){
        if ((millis() - start_millis) > connection_timeout ){
            Serial.println("");
            Serial.println("Faield to connect to network");
            return 0;
        }
        delay(100);
        Serial.print(".");
    }
    Serial.println("Successfully connected to wifi");
    return 1;
}

int isSSIDVisible(String targetSSID) {
    Serial.println("Scanning for Wi-Fi networks...");
    int networkCount = WiFi.scanNetworks();
    
    if (networkCount == -1) {
        Serial.println("Wi-Fi scan failed. Ensure Wi-Fi is initialized.");
        return 0;
    }
    
    for (int i = 0; i < networkCount; i++) {
        String ssid = WiFi.SSID(i);
        if (ssid == targetSSID) {
            Serial.println("SSID found: " + targetSSID);
            return 1;
        }
    }
    Serial.println("No network could be found with target SSID: " + targetSSID);
    return 0;
}

struct Credentials parse_wifi_credentials(String config_file_name)
{
    Credentials credentials = {"", "", 0};
    File config_file = SPIFFS.open("/" + config_file_name, "r");
    if (!config_file)
    {
        Serial.println("Failed to open config file when attempting connection");
        credentials.error = 1;
        return credentials;
    }
    String text = config_file.readString();
    config_file.close();

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, text);
    if (error)
    {
        Serial.print("Failed to deserialise json in config file: ");
        Serial.println(error.f_str());
        credentials.error = 2;
        return credentials;
    }
    if (!doc.containsKey("ssid") || !doc.containsKey("password"))
    {
        Serial.println("Missing required fields in JSON.");
        credentials.error = 3;
        return credentials;
    }

    credentials.ssid = doc["ssid"].as<String>();
    credentials.password = doc["password"].as<String>();
    return credentials;
}




int run_setup(String config_file_name){
    configure(config_file_name);
    return 1;
}