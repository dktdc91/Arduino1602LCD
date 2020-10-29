#include <LiquidCrystal_I2C.h>
String a="Sanitizing Data";
int i = 0;
String b;
int ms = 0;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int time= 30;
void setup()
{
  lcd.init();
  
  lcd.setCursor (0,0);
  lcd.print(a);
  lcd.backlight();
  b = a;
  
}


void loop()
{
     //Move cursor to second line, first position

     if (i < a.length()){
        int x = b.length()<16?b.length():16;
        b.remove(0,x);
        i += 16;
     }else{
        b = a;
        i = 0;
     }
    lcd.setCursor(0,0);
    lcd.print(b);
    if(b.length()<15){
        for (int j=b.length(); j<15; j++){
          lcd.print(" ");
        }
    }
     
     delay(250);
     if(ms % 4 == 0){
       lcd.setCursor(0, 1);
       //Print Current Time
       lcd.print("Remaining: ");
       if(time < 10){
        lcd.print(" ");
       }
       lcd.print(time);
       lcd.print(" s");
       //Increment the time
       time--;
     }
     if(time < 0){
      time = 30;
     }
     ms++;
     
}
