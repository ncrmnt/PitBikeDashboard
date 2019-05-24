#define DISPLAY_TYPE U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI

class Display : public DISPLAY_TYPE {
    private:
        u8g2_uint_t _width;
        u8g2_uint_t _height;
    public:  
        Display(): DISPLAY_TYPE(U8G2_R0, PIN_DISPLAY_CS, PIN_DISPLAY_DC, PIN_DISPLAY_RST) { 
            _width = getDisplayWidth();
            _height = getDisplayHeight();
        };  
        
        void drawStrCenterTop(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
            drawStr((_width - getStrWidth(s))/2 + x, y, s);
        };
        
        void drawStrRightTop(u8g2_uint_t x, u8g2_uint_t y, const char *s) {
            drawStr(_width - getStrWidth(s) - x, y, s);
        };
        
        void drawStrCenterMiddle(const char *s) {
            drawStr((_width - getStrWidth(s))/2, (_height + getAscent())/2, s);
        };
        
        //void drawVerticleGauge(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t width, u8g2_uint_t height, float value);
        void drawVerticleGauge(u8g2_uint_t x, u8g2_uint_t y, uint8_t width, uint8_t height, float v) {
            drawFrame(x, y, width, height);
            uint8_t h = round(height * (v > 1 ? 1 : v));
            drawBox(x, y + height - h, width, h);
        };
};