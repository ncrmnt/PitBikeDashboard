#include "config.h"
#include "log.h"

#include <Arduino.h>
#include <FS.h>
#include <TimeLib.h>
//#include <ESP8266WebServer.h>
#include <OneButton.h>
#include <U8g2lib.h>
#include <TinyGPS++.h>
#include <MAX31855.h>

#if LOG_OUTPUT == BT ||  LOG_OUTPUT == ALL || defined(BT_MONITORING)
    #include "BluetoothSerial.h"
    BluetoothSerial SerialBT;
#endif

#include "modules\Tachometer.h"
#include "modules\Display.h"
#include "modules\Blinker.h"

Blinker blinker(PIN_BLINKER);
Tachometer tachometer;

struct Location {
    double lat;
    double lng;
};

TinyGPSPlus gps;
File track;
//ESP8266WebServer server(80);

uint32_t distanceTotal = 0;
uint32_t distanceMarker = 0;
uint32_t motoMinutesTotal = 0;
uint32_t motoMinutesMarker = 0;
Location location = {0, 0};
Location locationMarker = {0, 0};
uint32_t trackPoints = 0;

bool serverMode = false;
bool timeSynchronized = false;

// TODO реализовать получение состояния. Логика: при запуске [engineStartMillis = millis()+1], при остановке [engineStartMillis=0]
uint32_t engineStartMillis = 1; 
uint8_t speedMax = 0;


void IRAM_ATTR isr_tach() {
    tachometer.pulse();
    LOG("pulse");
}

#include "ui.h"

void setup() {
    //pinMode(PIN_LED_INT, OUTPUT);
    //digitalWrite(PIN_LED_INT, HIGH);
  
    Serial.begin(115200);
    Serial2.begin(9600);  // GNSS transmitter

    //SPIFFS.begin();
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_TACHOMETER, INPUT_PULLUP);
    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    attachInterrupt(PIN_TACHOMETER, isr_tach, FALLING);
    
    //initMem();
    initUi();
    //initServer();
    blinker.start(BLINK_LOW);
    #ifdef BT_MONITORING
    SerialBT.begin("BikeDashboard");
    #endif
  
  /*
    while (!MAX31855.begin(PIN_THERMOCOUPLE_CS)) {
         Serial.println(F("Unable to start MAX31855. Waiting 3 seconds."));
        delay(3000);
    } 
*/
}


void loop() {
    button.tick();
    
    if (false /*serverMode*/) {
      //server.handleClient();
    } else {
        // Чтение потока данных от GPS-модуля
        while (Serial2.available() > 0) {
            gps.encode(Serial2.read());
        }
        
        // Синхронизация времени с GPS
        if (!timeSynchronized && gps.time.isValid() && gps.date.isUpdated() && gps.date.year() >= 2001) {
            setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
            adjustTime(TIME_ZONE);
            timeSynchronized = true;
            //blinker.stop();
            LOG("Сейчас %d.%02d.%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
        }

        // Обработка текущих координта
        if (timeSynchronized && gps.location.isValid() && gps.location.isUpdated() && gps.hdop.value() <= MAX_HDOP) {
            const Location l = { gps.location.lat(), gps.location.lng() };
            if (location.lat == 0 && location.lng == 0) {
                location = l;
                //recordLocation();
            } else {
                double d = TinyGPSPlus::distanceBetween(l.lat, l.lng, location.lat, location.lng);
                if (d >= TRACK_MIN_DISTANCE) {
                    distanceTotal += d;
                    location = l;
                    //recordLocation();
                    //commitTelemetry(distanceTotal, MEM_ADDR_DISTANCE_TOTAL);
                }
            }

            if (speedMax < gps.speed.kmph()) {
                speedMax = round(gps.speed.kmph());
            }
        }

        // Расчет моточасов
        if (engineStartMillis != 0) {
            motoMinutesTotal = round((millis() - engineStartMillis) / 1000.0 / 60.0);
        }

        drawScreen();
        
        // https://play.google.com/store/apps/details?id=com.frederikhauke.ArduTooth
        #ifdef BT_MONITORING
        if (millis() - t >= 500) {
            SerialBT.printf("%.2f,%d,%d,%g,%u,%g,%g,%u,%02d:%02d:%02d;"
                ,gps.speed.kmph()
                ,speedMax
                ,gps.satellites.value()
                ,gps.hdop.hdop()
                ,distanceTotal
                ,gps.location.lat()
                ,gps.location.lng()
                ,gps.charsProcessed()
                ,hour(), minute(), second());
            t = millis();
        }
        #endif
    }
}
