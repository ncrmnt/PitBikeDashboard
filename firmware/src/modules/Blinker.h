#define BLINK_LOW    (1)
#define BLINK_NORMAL (128)
#define BLINK_HIGH   (255)

class Blinker {
    private:
        static const uint8_t _channel = 0;
    public:  
        Blinker(byte pin) {
            ledcSetup(_channel, 1, 8);
            ledcAttachPin(pin, _channel);
        };

        void start(uint8_t intensity) {
            ledcWrite(_channel, intensity);
        };

        void stop() {
            ledcWrite(_channel, 0);
        };
};