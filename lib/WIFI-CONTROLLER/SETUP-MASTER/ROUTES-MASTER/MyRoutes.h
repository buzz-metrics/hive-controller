#ifndef MYROUTES_H
#define MYROUTES_H
#include <Arduino.h>

bool set_wifi_credentials(String ssid, String password);
String get_wifi_credentials();
bool verify_wifi_credentials(String ssid, String password);
String get_available_wifi();


bool restore_config_file();
#endif