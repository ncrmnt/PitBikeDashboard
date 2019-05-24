#define TACHOMETER_PULSES_PER_REVOLUTION    (2)
#define TACHOMETER_RPM_UPDATE_INTERVAL      (500)
#define TACHOMETER_RPM_MAX                  (10000)

#define TACHOMETER_PULSES_DEBOUNCE_INTERVAL (6 / TACHOMETER_PULSES_PER_REVOLUTION)

class Tachometer {
    private:
        unsigned long _lastCalc = 0;
        unsigned long _lastPulse = 0;
        uint16_t _RPM = 0;
        volatile uint32_t _pulses;

    public:  
        Tachometer() {
            //pinMode(pin, INPUT_PULLUP);
        };

        void IRAM_ATTR pulse() {
            /*
            об/мин	об/сек	мил.сек м-у об
            2 500   	 42   	 24,0   
            5 000   	 83   	 12,0   
            7 000   	 117   	 8,6   
            9 000   	 150   	 6,7   
            10 000   	 167   	 6,0   
            */

            //TODO: debounce?
            //if (millis() - _lastPulse >= 6) {
                _pulses++;
               _lastPulse = millis();
            //}
        };

        uint16_t getRPM() {
            
            /*float dt = (millis() - _lastPulse) / 1000.0;
            if (dt == 0 || dt >= 1) {
                return 0;
            } else {
                float rpm = (1/dt) * 60;
                return round(rpm);
            }*/
         
            unsigned long dt = millis() - _lastCalc;
            if (dt >= TACHOMETER_RPM_UPDATE_INTERVAL) {
                noInterrupts();
                _RPM = (60 * 1000 / TACHOMETER_PULSES_PER_REVOLUTION) / dt * _pulses;
                _lastCalc = millis();
                _pulses = 0;
                interrupts();
            } 
         
            return _RPM;
        };
};
