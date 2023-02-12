#include "RLcd_TIW.h"
#include "RTimer.h"
#include "portal.h"
#include "SoftwareSerial.h"
#include "button.h"
#include "menu.h"
#include <EEPROM.h>

const int txPin = 13;
const int rxPin = 12;
SoftwareSerial rackSerial(txPin, rxPin);

#define SCAN_LEFT Serial1
#define SCAN_RIGHT Serial2

// Параметры пользователя
long BLIND_SPOT_HOME = 270; // слепая зона от "дома" до первой пробирки
long BLIND_SPOT = 125;      // слепая зона между пробирками
long SCANNING_AREA = 5;     // зона сканирования
long SPEED_SCAN = 18000;    // скорость движения портала при сканировании

// Переменные для EEPROM
bool startKey = false;
int key = 0;
//*******************************************************************
//*******************************************************************
class Rack
{
public:
    Rack()
    {
        pinMode(txPin, OUTPUT);
        pinMode(rxPin, INPUT);
        rackSerial.begin(9600);
    }
    long read()
    {
        if (rackSerial.available() > 0)
        {
            return rackSerial.read();
        }
    }

protected:
};

//*******************************************************************
//*******************************************************************
Portal Tray;
Rack rack;

void moveStepper()
{
    Tray.run_HomePosition(startKey);
}

void setup()
{

   // EEPROM.get(20, key);
    //if (key == 2571)
   // {
        //  EEPROM.get(0, BLIND_SPOT_HOME);
        // EEPROM.get(4, BLIND_SPOT);
        // EEPROM.get(8, SCANNING_AREA);
        // EEPROM.get(12, SPEED_SCAN);
   // }
    init_menu();
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial3.begin(9600);

    Timer1.initialize(100); // прерывание 10кГц
    Timer1.attachInterrupt(moveStepper);
}

void loop()
{
    call_menu();

}
