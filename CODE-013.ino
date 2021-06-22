//#VTM https://www.youtube.com/c/VTMVlogVoThongMinh
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>       
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define stepPin1 11 //clk+ 
#define dirPin1 12 //cw+ 
#define stepPin2 6 //clk+ 
#define dirPin2 7 //cw+ 
#define start 5
#define stopp 4
#define outputA A2
#define outputB A1
#define sw A0
int counter = 0;
int aState;
int aLastState;
double b = EEPROM.read(0); //speed
double a = EEPROM.read(1); //step1
double c = EEPROM.read(2); //step2
int d = EEPROM.read(3); //counter
int demSW = 0;
//...........................................................................
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(start, INPUT_PULLUP);
  pinMode(stopp, INPUT_PULLUP);
  pinMode(sw , INPUT_PULLUP);
  aLastState = digitalRead(outputA);
  lcd.setCursor(0, 0);
  lcd.print("Speed   :");
  lcd.setCursor(15, 0);
  lcd.print("(rps)");
  lcd.setCursor(0, 1);
  lcd.print("Step 1  :");
  lcd.setCursor(15, 1);
  lcd.print("(r)");
  lcd.setCursor(0, 2);
  lcd.print("Step 2  :");
  lcd.setCursor(15, 2);
  lcd.print("(r)");
  lcd.setCursor(0, 3);
  lcd.print("Counter :");
  lcd.setCursor(10, 0);
  lcd.print(b / 20);
  lcd.setCursor(10, 1);
  lcd.print(a / 20);
  lcd.setCursor(10, 2);
  lcd.print(c / 20);
  lcd.setCursor(10, 3);
  lcd.print(d / 2);
}
//...............................................
void loop() {
  if (digitalRead(sw) == 0) {
    readVR();
  }
  if (digitalRead(start) == 0) {
    runn();
  }
}
//...............................................
void readVR() {
  while (1) {
    lcd.setCursor(15, 3);
    lcd.print("  ");
    lcd.setCursor(8, 0);
    lcd.print(">");
    counter = b; //setting speed
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 198 ) {
          counter = 198;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        b = counter;
        lcd.setCursor(10, 0);
        lcd.print(b / 20);
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto step1;
      }
    }
  }
step1:
  while (1) {
    lcd.setCursor(8, 0);
    lcd.print(":");
    lcd.setCursor(8, 1);
    lcd.print(">");
    counter = a; //setting step1
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 250) {
          counter = 250;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        a = counter;
        lcd.setCursor(10, 1);
        lcd.print(a / 20);
        if (a < 200) {
          lcd.setCursor(14, 1);
          lcd.print(' ');
        }
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto step2;
      }
    }
  }
step2:
  while (1) {
    lcd.setCursor(8, 1);
    lcd.print(":");
    lcd.setCursor(8, 2);
    lcd.print(">");
    counter = c; //setting step2
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 250) {
          counter = 250;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        c = counter;
        lcd.setCursor(10, 2);
        lcd.print(c / 20);
        if (c < 200) {
          lcd.setCursor(14, 2);
          lcd.print(' ');
        }
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto step3;
      }
    }
  }
step3:
  while (1) {
    lcd.setCursor(8, 2);
    lcd.print(":");
    lcd.setCursor(8, 3);
    lcd.print(">");
    counter = d; //setting counter
    delay(500);
    while (1) {
      aState = digitalRead(outputA);
      if (aState != aLastState)
      {
        if (digitalRead(outputB) != aState) {
          counter++;
        }
        else {
          counter--;
        }
        //Serial.println(counter);
        if (counter > 250) {
          counter = 250;
        }
        if (counter < 2 ) {
          counter = 2;
        }
        d = counter;
        lcd.setCursor(10, 3);
        lcd.print(d / 2);
        if (d < 20) {
          lcd.setCursor(11, 3);
          lcd.print(' ');
        }
        else if (d < 200) {
          lcd.setCursor(12, 3);
          lcd.print(' ');
        }
      }
      aLastState = aState;
      if (digitalRead(sw) == 0) {
        goto step4;
      }
    }
  }
step4:
  EEPROM.write(0, b);
  EEPROM.write(1, a);
  EEPROM.write(2, c);
  EEPROM.write(3, d);
  lcd.setCursor(8, 3);
  lcd.print(":");
  lcd.setCursor(15, 3);
  lcd.print("OK");
  delay(500);
}
//...............................................
void runn() {
  int count = d / 2;
  lcd.setCursor(15, 3);
  lcd.print("START");
  while (count > 0) {
    digitalWrite(dirPin1, LOW);
    for (int x = 0; x < 80 * a; x++) {
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(2500 / b);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(2500 / b);
      if (digitalRead(stopp) == 0) {
        goto endd;
      }
    }
    digitalWrite(dirPin2, LOW);
    for (int x = 0; x < 80 * c; x++) {
      digitalWrite(stepPin2, HIGH);
      delayMicroseconds(2500 / b);
      digitalWrite(stepPin2, LOW);
      delayMicroseconds(2500 / b);
      if (digitalRead(stopp) == 0) {
        goto endd;
      }
    }
    count--;
    lcd.setCursor(10, 3);
    lcd.print(count);
    if (count < 10) {
      lcd.setCursor(11, 3);
      lcd.print(' ');
    }
    else if (count < 100) {
      lcd.setCursor(12, 3);
      lcd.print(' ');
    }
  }
endd:
  lcd.setCursor(10, 3);
  lcd.print(d / 2);
  lcd.setCursor(15, 3);
  lcd.print("     ");
}
