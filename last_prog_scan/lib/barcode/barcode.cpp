#include "barcode.h"
#include "SoftwareSerial.h"

#define DEBUG_SERIAL Serial
#define SCAN_LEFT Serial1
#define SCAN_RIGHT Serial2

const int txPin = 13;
const int rxPin = 12;
SoftwareSerial rackSerial(txPin, rxPin);

Barcode::Barcode()
{
    DEBUG_SERIAL.begin(9600);
    SCAN_LEFT.begin(9600);
    SCAN_RIGHT.begin(9600);
    Serial3.begin(9600);

    while(!Serial);
    pinMode(txPin, OUTPUT);
    pinMode(rxPin, INPUT);
    rackSerial.begin(9600);

    SCAN_LEFT.setTimeout(10);
    SCAN_RIGHT.setTimeout(10);
    clear();
}

void Barcode::clear()
{
    counter_left = 0;
    counter_right = 0;
    for (int i = 0; i < 24; ++i)
    {
        arr[0][i] = 0;
        arr[1][i] = 0;
    }
}

void Barcode::debugData()
{
    if (SCAN_LEFT.available() > 1)
    {
        arr[0][counter_left++] = SCAN_LEFT.parseInt();
    }
    if (SCAN_RIGHT.available() > 1)
    {
        arr[1][counter_right++] = SCAN_RIGHT.parseInt();
    }
    if (counter_left >= 23 && counter_right >= 23)
    {
        for (int a = 0; a < 24; a++)
        {
            DEBUG_SERIAL.println(arr[0][a]);
            DEBUG_SERIAL.println('   ');
            DEBUG_SERIAL.println(arr[1][a]);
            DEBUG_SERIAL.println('\n');
        }
        clear();
    }
}