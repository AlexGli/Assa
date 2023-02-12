#include "RLcd_TIW.h"
#include "RTimer.h"
#include "portal.h"
#include "barcode.h"
#include "button.h"
#include "menu.h"
#include <EEPROM.h>

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
Portal Tray;
Barcode barcode;

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
    Timer1.initialize(100); // прерывание 10кГц
    Timer1.attachInterrupt(moveStepper);
}

void loop()
{
    call_menu();
    barcode.debugData();
}
