// include the library code:
#include <LiquidCrystal.h>

// https://github.com/rkoptev/ACS712-arduino
#include "ACS712.h"

/***********************************PINS***************************************/

#define PIN_POMP 2
#define PIN_ACS712 A8


// Initialize the library by associating any needed LCD interface pin
// With the arduino pin number it is connected to
const int rs = 8,
          en = 9,
          d4 = 4,
          d5 = 5,
          d6 = 6,
          d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/*
  This example shows how to measure the power consumption
  of devices in 230V electrical system
  or any other system with alternative current
*/

// We have 30 amps version sensor connected to A0 pin of arduino
// Replace with your version if necessary
ACS712 sensor(ACS712_30A, PIN_ACS712);

void setup() {
        Serial.begin(9600);
        pinMode(PIN_POMP, OUTPUT); // sets the operating mode - output
        
        // calibrate() method calibrates zero point of sensor,
        // It is not necessary, but may positively affect the accuracy
        // Ensure that no current flows through the sensor at this moment
        // If you are not sure that the current through the sensor will not leak during calibration - comment out this method
        Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
        sensor.calibrate();
        Serial.println("Done!");
        // set up the LCD's number of columns and rows:
        lcd.begin(16, 2);
        // Print a message to the LCD.
        //analogWrite(10,5);
}

void loop() {
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 0);
        lcd.print("T: ");
        lcd.print(millis() / 1000);

        // We use 230V because it is the common standard in European countries
        // Change to your local, if necessary
        float U = 230;

        // To measure current we need to know the frequency of current
        // By default 50Hz is used, but you can specify desired frequency
        // as first argument to getCurrentAC() method, if necessary
        float I = sensor.getCurrentAC();

        // To calculate the power we need voltage multiplied by current
        float P = U * I;

        Serial.println(String("I = ") + I + " A");
        Serial.println(String("P = ") + P + " Watts");

        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(String("P = ") + P + " Watts");
        if (P < 10) {
                digitalWrite(PIN_POMP, HIGH);
                delay(10 * 1000);
                digitalWrite(PIN_POMP, LOW);
        }
        
        //lcd.clear();
        delay(1000);

}
