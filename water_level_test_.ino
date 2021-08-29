#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x38,16,2);

// Water level indicator
int sump_indicator=A0;
int quarter_indicator=A1;
int half_indicator=A2;
int three_quarter_indicator=A3;
int full_indicator=3;

// Device output indicator
int motor_output=8;
int buzzer_output=7;

// Sump tank status
int sump_is_full;

// Water tank status
int is_quarter;
int is_half;
int is_three_quarter;
int is_full;

// Status flag indicator
int motor_current_status;
int buzzer_status=0;
int motor_update_status=0;
int sump_not_full=0;      // this variable could be deleted



void setup()
{
  //lcd.init(); 
  lcd.backlight();
  
  pinMode(sump_indicator,INPUT);
  pinMode(quarter_indicator,INPUT);
  pinMode(half_indicator,INPUT);
  pinMode(three_quarter_indicator,INPUT);
  pinMode(full_indicator,INPUT);

  pinMode(motor_output,OUTPUT);
  pinMode(buzzer_output,OUTPUT);
  digitalWrite(buzzer_output,LOW);
}

void loop()
{

  motor_current_status=digitalRead(motor_output);

  sump_is_full=digitalRead(sump_indicator);
  is_quarter=digitalRead(quarter_indicator);
  is_half=digitalRead(half_indicator);
  is_three_quarter=digitalRead(three_quarter_indicator);
  is_full=digitalRead(full_indicator);

  lcd.clear();

  if(is_full== 1 && is_three_quarter == 1 && is_half == 1 && is_quarter == 1) {
    lcd.setCursor(0,0);
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.setCursor(5,0);
    lcd.print("FULL");

    motor_update_status=0;
    buzzer_status=0;

  } else if(is_full == 0 && is_three_quarter == 1 && is_half == 1 && is_quarter == 1) {
    lcd.setCursor(0,0);
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.print("_");
    lcd.setCursor(5,0);
    lcd.print("3/4th");

    buzzer_status=0;

  } else if(is_full == 0 && is_three_quarter == 0 && is_half == 1 && is_quarter == 1) {
    lcd.setCursor(0,0);
    lcd.print(char(219));
    lcd.print(char(219));
    lcd.print("_");
    lcd.print("_");
    lcd.setCursor(5,0);
    lcd.print("HALF");

    motor_update_status=1;
    buzzer_status=0;

  } else if(is_full == 0 && is_three_quarter == 0 && is_half == 0 && is_quarter == 1) {
    lcd.setCursor(0,0);
    lcd.print(char(219));
    lcd.print("_");
    lcd.print("_");
    lcd.print("_");
    lcd.setCursor(5,0);
    lcd.print("1/4th");

    buzzer_status=0;

  } else if(is_full == 0 && is_three_quarter == 0 && is_half == 0 && is_quarter == 0) {
    lcd.setCursor(0,0);
    lcd.print("_");
    lcd.print("_");
    lcd.print("_");
    lcd.print("_");
    lcd.setCursor(5,0);
    lcd.print("LOW");

    buzzer_status=0;

  } else {
    digitalWrite(motor_output,LOW);
    lcd.setCursor(0,0);
    lcd.print("ERROR!");
    buzzer_status=1;
  }
  
  
  
  // Display motor status
  if(motor_current_status==HIGH) {
    lcd.setCursor(0,1);
    lcd.print("Pump ON");
  } else {
    lcd.setCursor(0,1);
    lcd.print("Pump OFF");
  }



  if(sump_is_full==1 && motor_update_status==1) {
    digitalWrite(motor_output,HIGH);
  }
  if(sump_is_full==0) {
    digitalWrite(motor_output,LOW);
    lcd.setCursor(11,0);
    lcd.print("Low");
    lcd.setCursor(11,1);
    lcd.print("Sump");
    sump_not_full=1;
  }
  if(sump_is_full == 1) {
    sump_not_full=0;
  }

  if(motor_update_status == 0) {
    digitalWrite(motor_output,LOW);
  }

  if(buzzer_status == 1 || sump_not_full == 1) {
    digitalWrite(buzzer_output,HIGH);
    delay(500);
    digitalWrite(buzzer_output,LOW);
  } else {
    digitalWrite(buzzer_output,LOW);
  }
  delay(100);
  lcd.clear();
}
