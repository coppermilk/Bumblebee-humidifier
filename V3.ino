/* This code works with ACS712 current sensor, it permits the calculation of the signal TRMS
 * Visit www.surtrtech.com for more details
 */
// https://drive.google.com/file/d/1Zb3yGRoztrP29oK-2G0f1e55asYaJVlV/view?usp=sharing
#include <Filters.h>                      //This library does a massive work check it's .cpp file


float ACS_Value;                              //Here we keep the raw data valuess
float testFrequency = 50;                    // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist



float intercept = 0; // to be adjusted based on calibration testing
float slope = 0.1; // to be adjusted based on calibration testing
                      //Please check the ACS712 Tutorial video by SurtrTech to see how to get them because it depends on your sensor, or look below


float Amps_TRMS; // estimated actual current in amps

unsigned long printPeriod = 1000; // in milliseconds
// Track time in milliseconds since last reading 
unsigned long previousMillis = 0;


// include the library code:
#include <LiquidCrystal.h>


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
        pinMode(PIN_POMP, OUTPUT); // sets the operating mode - output
        pinMode(ACS_Pin,INPUT);  //Define the pin mode
      
        // set up the LCD's number of columns and rows:
        lcd.begin(16, 2);
        // Print a message to the LCD.
        //analogWrite(10,5);
        
}

void loop() {
       
RunningStatistics inputStats;                 // create statistics to look at the raw test signal
  inputStats.setWindowSecs( windowLength );     //Set the window length
   
  while( true ) {   
    ACS_Value = analogRead(ACS_Pin);  // read the analog in value:
    inputStats.input(ACS_Value);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) { //every second we do the calculation
      previousMillis = millis();   // update time
      
      Amps_TRMS = intercept + slope * inputStats.sigma();
      Serial.print("Without cal: ");
      Serial.print(inputStats.sigma());
      
      Serial.print( "\t Amps: " ); 
      Serial.print( Amps_TRMS );

      Serial.print("\t To test: ");
      Serial.print(inputStats.sigma());
       // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 0);
      lcd.print("UPTIME: ");
      lcd.print(millis() / 1000);
      
      lcd.setCursor(0, 1);
      lcd.print( "Amps: " ); 
      lcd.print( Amps_TRMS );

    }
  }

}
