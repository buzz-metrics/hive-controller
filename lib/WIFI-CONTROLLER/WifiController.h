#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H



#include <Arduino.h>

class WifiController {
    public:
        WifiController(int _timeout);
        int begin();

    private:
        int timeout;


};


#endif