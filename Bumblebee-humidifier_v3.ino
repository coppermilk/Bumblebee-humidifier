/* This code works with ACS712 current sensor, it permits the calculation of the signal TRMS
 * Visit www.surtrtech.com for more details
 */
// https://drive.google.com/file/d/1Zb3yGRoztrP29oK-2G0f1e55asYaJVlV/view?usp=sharing


//This library does a massive work check it's .cpp file
// include the library code:
#include <LiquidCrystal.h>

#include <Filters.h>

//Here we keep the raw data valuess
float ACS_Value;
// Test signal frequency (Hz)
float testFrequency = 50;
// How long to average the signal, for statistist
float windowLength = 40.0 / testFrequency;

// To be adjusted based on calibration testing
float intercept = 0;
// To be adjusted based on calibration testing
// Please check the ACS712 Tutorial video by SurtrTech to see how to get them because it depends on your sensor, or look below
float slope = 0.1017;
// Estimated actual current in amps
float Amps_TRMS;

// In milliseconds
unsigned long printPeriod = 1000;
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;

boolean ledState = 0; // переменная состояния светодиода

/***********************************PINS***************************************/

#define PIN_POMP 2

//Sensor data pin on A8 analog input
#define ACS_Pin A8

// Initialize the library by associating any needed LCD interface pin
// With the arduino pin number it is connected to
const int rs = 8,
        en = 9,
        d4 = 4,
        d5 = 5,
        d6 = 6,
        d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
        Serial.begin(9600);
        // Sets the operating mode - output
        pinMode(PIN_POMP, OUTPUT); // sets the operating mode - output
        //Define the pin mode
        pinMode(ACS_Pin, INPUT);

        // Set up the LCD's number of columns and rows:
        lcd.begin(16, 2);

}

void loop() {
        // Create statistics to look at the raw test signal      
        RunningStatistics inputStats;
        //Set the window length                
        inputStats.setWindowSecs(windowLength);

        while (true) {
                // Read the analog in value:
                ACS_Value = analogRead(ACS_Pin);
                // Log to Stats function
                inputStats.input(ACS_Value);

                //Every second we do the calculation
                if ((unsigned long)(millis() - previousMillis) >= printPeriod) {
                        // Update time
                        previousMillis = millis();

                        Amps_TRMS = intercept + slope * inputStats.sigma();
                        Serial.print("Without cal: ");
                        Serial.print(inputStats.sigma());

                        Serial.print("\t Amps: ");
                        Serial.print(Amps_TRMS);

                        Serial.print("\t To test: ");
                        Serial.println(inputStats.sigma() + 0);
                        // Set the cursor to column 0, line 1
                        // (Note: line 1 is the second row, since counting begins with 0):
                        lcd.setCursor(0, 0);
                        lcd.print("Time: ");
                        lcd.print(millis() / 1000);

                        lcd.setCursor(0, 1);
                        lcd.print("Amps: ");
                        lcd.print(Amps_TRMS);

                        if (Amps_TRMS < 0.15) {
                                digitalWrite(PIN_POMP, HIGH);
                        }else{
                           digitalWrite(PIN_POMP, LOW);
                          }

                }
        }

}
