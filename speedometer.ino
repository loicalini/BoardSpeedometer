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
  
     
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), magnet_detect, FALLING);

  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Welcome Onboard,");
  lcd.setCursor(1,1);
  lcd.print("Ride Safe! :P");
  delay(3000);
  lcd.clear();  

  lcd.setCursor(8,0);
  lcd.print(" MPH");
  lcd.setCursor(11,1);
  lcd.print(" MPH");
  EEPROM.get(0,recordSpeed); //disabled for now, reenable when code is finalized
  
}

void loop() {

  //Update whether top speed or record speed is showing at the moment
  if (loopCount == 0) {
    lcd.setCursor(1,1);
    lcd.print("TOP = ");
  }

  if (loopCount == 130){
    lcd.setCursor(1,1);
    lcd.print("REC = ");
  }



  
  lcd.setCursor(4,0);
  lcd.print(deltaT);
  
  
  loopCount = loopCount + 1; 
  if (loopCount >= 130){
    lcd.setCursor(7,1);
    lcd.print(recordSpeed);
    lcd.setCursor(11,1);
  lcd.print(" ");
    loopCount = loopCount*(loopCount != 150);
  }
  
  else {
    lcd.setCursor(7,1);
    lcd.print(topSpeed);
    lcd.setCursor(11,1);
    lcd.print(" ");
    }

  if (((double) (micros()-t1) / 1000000) > 3.0){ // if wheel doesnt spin for 3 seconds, speed set to 0.
    deltaT = 0.00;
  }
  delay(100); //delay function is in milliseconds

  
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
   
   
 
