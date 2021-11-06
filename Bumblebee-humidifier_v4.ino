//More information at: https://www.aeq-web.com/
// include the library code:
#include <LiquidCrystal.h>

int voltage = 220; //Voltage of your Power Grid
int sensorpin = A1; //Pin of Sensor
#define PIN_POMP 2

float val = 0;
float current = 0;
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
  lcd.begin(16, 2);
}

void loop() {
  int i = 0;
  val = 0;
  
  while(i < 2000){//Meassure current 2000 times to get a clean value
  int sensorValue = analogRead(sensorpin);
  val = val + sensorValue;
  i++;
  }
  
  current = ((val/2000)*7)/100; //Get current in A
  
  Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" A Watts: ");
  Serial.print(current*voltage); //Convert to Watts (P = A*V) 
  Serial.print(" RAW: ");
  Serial.print(val/1000);
  Serial.println(" mV");
          lcd.setCursor(0, 0);
        lcd.print("T: ");
        lcd.print(millis() / 1000);
        
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(String("Watts: ") + current*voltage);
                if (current*voltage < 1) {
                digitalWrite(PIN_POMP, HIGH);
                delay(10 * 1000);
                digitalWrite(PIN_POMP, LOW);
        }
  delay(500);
  
}
