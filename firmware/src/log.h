#include <Arduino.h>

#define __FILENAMEW__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILENAMEU__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifndef DEBUGUTILS_H
    #define DEBUGUTILS_H
    #if !defined LOG_OUTPUT
        #define LOG(...)
    #else
        #if LOG_OUTPUT == USB
            #define _PRINT(...) Serial.printf(__VA_ARGS__);
        #elif LOG_OUTPUT == BT
            #define _PRINT(...) SerialBT.printf(__VA_ARGS__);
        #else   
            #define _PRINT(...) Serial.printf(__VA_ARGS__); SerialBT.printf(__VA_ARGS__);
        #endif
        #define LOG(...) \ 
            _PRINT("[%s, %03d, %s] ", __FILENAMEW__, __LINE__, __PRETTY_FUNCTION__);\
            _PRINT(__VA_ARGS__);\
            _PRINT("\n");
    #endif 
#endif
