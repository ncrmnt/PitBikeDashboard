#include <Arduino.h>
#define LOG_OUTPUT USB      // USB, BT, ALL
//#define BT_MONITORING

// HDOP
// ≤1    Идеальная     Рекомендуется к использованию в системах, требующих максимально возможную точность во всё время их работы
// 2-3   Отличная	   Достаточная точность для использования результатов измерений в достаточно чувствительной аппаратуре и программах
// 4-6   Хорошая       Рекомендуемый минимум для принятия решений по полученным результатам. Результаты могут быть использованы для достаточно точных навигационных указаний.
// 7-8   Средняя       Результаты можно использовать в вычислениях, однако рекомендуется озаботиться повышением точности, например, выйти на более открытое место.
// 9-20  Ниже среднего Результаты могут использоваться только для грубого приближения местоположения
// 21-50 Плохая        Выходная точность ниже половины футбольного поля. Обычно такие результаты должны быть отброшены.
// TinyGPS++ выдает HDOP * 100 (https://github.com/mikalhart/TinyGPSPlus/issues/8#issuecomment-66882940)
const int MAX_HDOP = 600;

const int TIME_ZONE = 4 * 60 * 60;                                          // Часовой пояс (сек)
const char TRACK_DIR[] = "/t";                                              // Директория для хранения трэков
const char TRACK_PATH_FORMAT[] = "%s/%d.%02d.%02d_%02d-%02d-%02d.txt";      // Формат полного имени фйла трэка
//const char TRACK_HEADER[] = "date;time;latitude;longitude;speed\n";         // Заголовок в файле с трэком
const char TRACK_HEADER[] = "date;time;latitude;longitude;speed;sat;hdop"; 
//const char TRACK_ROW_FORMAT[] = "%02d.%02d.%04d;%02d:%02d:%02d;%g;%g;%d\n"; // Формать строки файла с трэком (DD-MM-YYYY;MM:HH:SS;latitude;longitude;speed)
const char TRACK_ROW_FORMAT[] = "%02d.%02d.%04d;%02d:%02d:%02d;%g;%g;%d;%d;%g\n";

const uint8_t TRACK_MIN_DISTANCE = 10;                                // Минимальное расстояние до следующей точки записываемой в трэк
const size_t TRACK_MAX_SIZE = 200 * 1024;                             // Максимальный размер файла с трэком (байт)
const size_t MIN_FREE_SPACE = 200 * 1024;                             // Минимальный объем свободного места во внутреннем хранилище сохраняемый при записи трэков

const char AP_SSID[] = "PitBikeDashboard";                                  // Имя точки доступа
const char AP_PASSWORD[] = "012345678";                                     // Пароль точки доступа

#define PIN_BUTTON          (22)
#define PIN_DISPLAY_CS      (15)
#define PIN_DISPLAY_DC      (2)
#define PIN_DISPLAY_RST     (4)
#define PIN_THERMOCOUPLE_CS (5)
#define PIN_TACHOMETER      (21)
#define PIN_BLINKER         (19)
