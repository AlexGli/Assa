#include "portal.h"
#include "RStepper.h"

RStepper Tstepper(1, 11, 9);

const int pinSensorHome = 30;   // датчик положения "дома"
const int pinSensorStart = 29;  // датчик положения "старта"
const int pinleftBarcode = 33;  // пин включения левого сканера
const int pinrightBarcode = 34; // пин включения правого сканера

extern long BLIND_SPOT_HOME; // слепая зона от "дома" до первой пробирки
extern long BLIND_SPOT;      // слепая зона между пробирками
extern long SCANNING_AREA;     // зона сканирования
extern long SPEED_SCAN;    // скорость движения портала при сканировании

enum TrayState
{
    homePositon = 1,
    startPosition = 2,
    nonePositon = 3
};
enum zone
{
    BLIND_SPOT_CASEHOME = 0, // слепая зона от позиции дома до первой пробирки
    BLIND_SPOT_CASETUBE,     // слепая зона между пробирками
    SCAN_REGION,             // зона сканирования
};

Portal::Portal()
{
    pinMode(pinleftBarcode, OUTPUT);
    pinMode(pinrightBarcode, OUTPUT);
    pinMode(pinSensorHome, INPUT);
    pinMode(pinSensorStart, INPUT);

    Tstepper.setMaxSpeed(4000);     // SPEED = Steps / second
    Tstepper.setAcceleration(1000); // ACCELERATION = Steps /(second)^2
    Tstepper.enableOutputs();       // enable pins
    Tstepper.moveTo(150000);
    Tstepper.run();
}

void Portal::run_HomePosition(bool state) // портал в поизиции дома
{
    static bool start = false;
    if (state && statePortal() == homePositon) // нажали старт
    {
        start = true;
    }
    if (positionHome)
    {
        Tstepper.moveTo(0);
        Tstepper.run();
        if (Tstepper.currentPosition() == 0)
        {
            positionHome = 0;
        }
    }
    if (start)
    {
        if (statePortal() == startPosition) // дошли до датчика положения старта
        {
            start = false;
            cnt = 0;
            positionHome = true;
            digitalWrite(pinleftBarcode, LOW);
            digitalWrite(pinrightBarcode, LOW);
        }
        switch (cnt)
        {
        case BLIND_SPOT_CASEHOME:
            Tstepper.moveTo(BLIND_SPOT_HOME); // едем до первой пробирки от дома
            Tstepper.run();
            if (Tstepper.currentPosition() == BLIND_SPOT_HOME)
            {
                cnt = SCAN_REGION;
                Capture = Tstepper.currentPosition();
            }
            break;

        case SCAN_REGION:
            sum = Capture + SCANNING_AREA;
            Tstepper.moveTo(sum);
            Tstepper.run();
            if (Tstepper.currentPosition() == sum)
            {
                digitalWrite(pinleftBarcode, HIGH);
                digitalWrite(pinrightBarcode, HIGH);
                cnt = BLIND_SPOT_CASETUBE;
                Capture = Tstepper.currentPosition();
            }
            break;

        case BLIND_SPOT_CASETUBE:
            sum = Capture + BLIND_SPOT;
            Tstepper.moveTo(sum);
            Tstepper.run();
            if (Tstepper.currentPosition() == sum)
            {
                digitalWrite(pinleftBarcode, LOW);
                digitalWrite(pinrightBarcode, LOW);
                cnt = SCAN_REGION;
                Capture = Tstepper.currentPosition();
            }
            break;
        }
    }
}

int Portal::statePortal()
{
    if (digitalRead(pinSensorHome))
    {
        return homePositon;
    }
    else if (digitalRead(pinSensorStart))
    {
        return startPosition;
    }
}