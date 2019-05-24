
//#include <Ticker.h>
//Ticker tickerSetLow;

//https://xbm.jazzychad.net/

struct screen {
    void (*draw)();
    void (*action)();
};

Display display;
OneButton button(PIN_BUTTON, true);



void drawSpeedometerScreen() {
    // https://github.com/olikraus/u8g2/wiki/fntgrpx11
    // https://github.com/olikraus/u8g2/wiki/fntgrpu8g
    
    char buf[10];

    display.setFont(u8g2_font_logisoso32_tn);
    sprintf(buf, "%.0f", gps.speed.kmph());
    display.drawStrCenterMiddle(buf);


    display.setFont(u8g2_font_blipfest_07_tr);
    sprintf(buf, "MAX %d", speedMax);
    display.drawStr(0, 63, buf);
    sprintf(buf, "ODO %.2f", distanceTotal / 1000.0);
    display.drawStrRightTop(0, 63, buf);

      /*
    display.setFont(u8g2_font_u8glib_4_tf);
    sprintf(buf, "%02d:%02d:%02d", hour(), minute(), second());
    display.drawStr(0, 6, buf);
    */
    //display.setFont(u8g2_font_lastapprenticebold_tr);
}

void drawOdometerScreen() {
    display.setFont(u8g2_font_freedoomr25_tn);  

    char buf[30];
    sprintf(buf, "%d", distanceTotal - distanceMarker );
    display.drawStr(0, 25, buf);

    sprintf(buf, "%d", distanceTotal);
    display.drawStr(0, 55, buf);
}

void drawServerScreen() {
    display.clearBuffer();
    display.setFont(u8g2_font_profont12_tr);  
    display.drawStr(0,15,"WiFi");
    display.drawStr(5,30,AP_SSID);
    display.drawStr(5,45,AP_PASSWORD);
    //display.drawStr(5,60,WiFi.softAPIP().toString().c_str());
    display.sendBuffer();
}

/*
void drawLocationMarkerScreen() {
    static const uint8_t
        r0 = SCREEN_MAX_Y / 2,
        r1 = r0 - 5;
    static const float ad = DEG_TO_RAD * 160;
    float a0 = 0, distance = 0;

    if (gps.hdop.value() <= MAX_HDOP) {
   
        //angle = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(),
        //    locationMarker.lat, locationMarker.lng);
        
        a0 = DEG_TO_RAD * (TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(),
            locationMarker.lat, locationMarker.lng) - gps.course.deg() - 90);
        distance = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(),
            locationMarker.lat, locationMarker.lng) / 1000.0;
    }

    float
        a1 = a0 - ad,
        a2 = a0 + ad;

    uint8_t 
        x0 = round(r1 * cos(a0) + r0),
        y0 = round(r1 * sin(a0) + r0),
        x1 = round(r1 * cos(a1) + r0),
        y1 = round(r1 * sin(a1) + r0),
        x2 = round(r1 * cos(a2) + r0),
        y2 = round(r1 * sin(a2) + r0);
    
    display.drawCircle(r0, r0, r0, U8G2_DRAW_ALL);
    display.drawTriangle(x0, y0, x1, y1, x2, y2);

    display.setFont(u8g2_font_6x13_tr);
    char buf[30];

    sprintf(buf, "DST  %.2f", distance);
    display.drawStr(r0*2+7, 13, buf);

    sprintf(buf, "CRS  %.0f", gps.course.deg());
    display.drawStr(r0*2+7, 13*2, buf);

    sprintf(buf, "CRSt %.0f", TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), locationMarker.lat, locationMarker.lng));
    display.drawStr(r0*2+7, 13*3, buf);

    sprintf(buf, "DIR  %.0f", (float)(a0 * RAD_TO_DEG + 90));
    display.drawStr(r0*2+7, 13*4, buf);
}
*/

void drawHourMeterScreen() {
    display.setFont(u8g2_font_freedoomr25_tn);  
    char buf[30];
    sprintf(buf, "%02d:%02d",  motoMinutesTotal / 60, motoMinutesTotal % 60);
    display.drawStr(0, 25, buf);

    uint32_t d = motoMinutesTotal - motoMinutesMarker;
    sprintf(buf, "%02d:%02d",  d / 60, d % 60);
    display.drawStr(0, 55, buf);
}

screen screens[] = {
    { drawSpeedometerScreen,    []() { speedMax = 0; }},
    //{ drawLocationMarkerScreen, []() { locationMarker = {.lat = gps.location.lat(), .lng = gps.location.lng()}; }},
    //{ drawHourMeterScreen,      []() { motoMinutesMarker = motoMinutesTotal; }},
    //{ drawOdometerScreen,       []() { distanceMarker = distanceTotal; }},
    {
        []() {
            char buf[40];
            const byte w = 10;
            display.setFont(u8g2_font_profont10_tr);

            sprintf(buf, "%02d.%02d.%02d %02d:%02d:%02d", day(), month(), year(), hour(), minute(), second());
            display.drawStr(0, w*1, buf);
            
            sprintf(buf, "Sat: %d   HDOP:%g", gps.satellites.value(), gps.hdop.hdop());
            display.drawStr(0, w*2, buf);

            sprintf(buf, "NMEA chrs: %d", gps.charsProcessed());
            display.drawStr(0, w*3, buf);
            
            sprintf(buf, "Loc is valid %s", gps.location.isValid() ? "YES" : "NO" );
            display.drawStr(0, w*4, buf);

            sprintf(buf, "Loc: %g %g", gps.location.lat(), gps.location.lng());
            display.drawStr(0, w*5, buf);
        }, 
        []() { }
    },

    {
        []() {
            char buf[40];
            const byte w = 10;
            display.setFont(u8g2_font_profont10_tr);

            sprintf(buf, "Speed: %.2f", gps.speed.kmph());
            display.drawStr(0, w*1, buf);

            sprintf(buf, "Max speed: %d", speedMax);
            display.drawStr(0, w*2, buf);

            sprintf(buf, "Speed is valid %s", gps.speed.isValid() ? "YES" : "NO" );
            display.drawStr(0, w*3, buf);

            sprintf(buf, "Distance: %.2f", distanceTotal / 1000.0 );
            display.drawStr(0, w*4, buf);

            display.drawVerticleGauge(114,0, 14, 57, tachometer.getRPM() / 8000.0);
            sprintf(buf, "%d", tachometer.getRPM());
            display.drawStrRightTop(0, 64, buf);
        }, 
        []() { }
    },
};

const uint8_t SCREEN_COUNT = sizeof(screens) / sizeof(screens[0]);
uint8_t activeScreenNum = 0;


inline void drawStatusBar() {
    char buf[8];

    display.setFont(u8g2_font_blipfest_07_tr);

    sprintf(buf, "%d", gps.satellites.value());
    display.drawStr(0, 5, buf);

    sprintf(buf, "%02d:%02d", hour(), minute());
    display.drawStrCenterTop(0, 5, buf);
    
    sprintf(buf,  gps.location.isValid() ? "%02.2f" : "-", gps.hdop.hdop());
    display.drawStrRightTop(0, 5, buf);
    
    display.drawHLine(0, 7, 127);
}


struct menuItem {
    char (*print)();
    char (*action)();
};

menuItem menu [] = {};

void drawMenu() {

}

/*
 * Отрисовка активного экрана
 */
void drawScreen() {
    display.clearBuffer();
    //drawStatusBar();
    (*screens[activeScreenNum].draw)();
    //display.setFont(u8g2_font_6x10_tf);
    //display.userInterfaceSelectionList("Title", 2, "abcdef\nghijkl\nmnopqr\nasdfsdf\nasdfsdf\nasdfsdf\nasdfsdf\nasdfsdf\nasdfsdf");
    display.sendBuffer();
}

void onButtonClick() {
    LOG("Click");
    activeScreenNum++;
    if (activeScreenNum >= SCREEN_COUNT) {
        activeScreenNum = 0;
    }
}

void onButtonLongPress() {
    LOG("LongPress");
    (*screens[activeScreenNum].action)();
}

void initUi() {
    display.begin();
    button.attachLongPressStart(onButtonLongPress);
    button.attachClick(onButtonClick);
}