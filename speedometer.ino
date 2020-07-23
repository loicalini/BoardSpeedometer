#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long t1 = micros();
double deltaT;
double topSpeed;
float recordSpeed;
int loopCount;
void setup() {
  
  // put your setup code here, to run once:     
  //pinMode(hallSensorPin, INPUT);   
  Serial.begin(9600);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Welcome Onboard,");
  lcd.setCursor(1,1);
  lcd.print("Ride Safe! :P");
  delay(3000);
  lcd.clear();  
  attachInterrupt(0, magnet_detect, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
  //EEPROM.get(0,recordSpeed); disabled for now, reenable when code is finalized
  recordSpeed = 18.6;
  
}

void loop() {
  lcd.setCursor(4,0);
  lcd.print(deltaT);
  lcd.setCursor(8,0);
  lcd.print(" MPH");
  
  loopCount = loopCount + 1;
  
  if (loopCount >= 130){
    lcd.setCursor(1,1);
    lcd.print("REC = ");
    lcd.setCursor(7,1);
    lcd.print(recordSpeed);
    lcd.setCursor(11,1);
    lcd.print(" MPH");
    if (loopCount == 150){
      loopCount = 0;
    }
  }
  else {
    lcd.setCursor(1,1);
    lcd.print("TOP = ");
    lcd.setCursor(7,1);
    lcd.print(topSpeed);
    lcd.setCursor(11,1);
    lcd.print(" MPH");
  }

  if (((double) (micros()-t1) / 1000000) > 3.0){
    deltaT = 0.00;
  }
  delay(100); //delay function is in milliseconds, 1000 milliseconds in one second

  
}

 void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   deltaT = (double) (micros() - t1) / 1000000;
   t1 = micros();

   deltaT = .08*3.14159*2.23694/deltaT;

   if (topSpeed < deltaT){
    topSpeed = deltaT;
   }

   if (recordSpeed < topSpeed){
    recordSpeed = (float) topSpeed;
    EEPROM.put(0,recordSpeed);
   }
    
   }
   
   
 
