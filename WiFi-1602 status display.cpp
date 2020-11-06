#include<SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial client(2,3); //RX, TX
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String webpage="";
int i=0,k=0;
String readString;
int x=0;

boolean No_IP=false;
String IP="";
char temp1='0';

String name="<p>This data is sent by the microchip</p>";
String dat="<p>by Kevin dela Cruz</p>";
     
void check4IP(int t1)
{
  int t2=millis();
  while(t2+t1>millis())
  {
    while(client.available()>0)
    {
      if(client.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
    }
  }
}

void get_ip()
{
  IP="";
  char ch=0;
  while(1)
  {
    client.println("AT+CIFSR");
    while(client.available()>0)
    {
      if(client.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(client.available()>0)
        {
          ch=client.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}

void connect_wifi(String cmd, int t)
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);
    client.println(cmd); 
    while(client.available())
    {
      if(client.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8){
    Serial.println("OK");
  }
  else{
    Serial.println("Error");
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print("Failed");
    lcd.setCursor (0,1);
    lcd.print("Retrying ...");
  }
}

void wifi_init()
{
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.print("Connecting...");
      connect_wifi("AT",100);
      connect_wifi("AT+CWMODE=3",100);
      connect_wifi("AT+CWQAP",100);  
      connect_wifi("AT+RST",5000);
      check4IP(5000);
      if(!No_IP)
      {
        Serial.println("Connecting Wifi....");
        connect_wifi("AT+CWJAP=\"PTCSPC0247 0557\",\"0R02p|45\"",7000); 
      }
      else{
      }
      Serial.println("Wifi Connected"); 
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.print("WiFi Connected");
      get_ip();
      connect_wifi("AT+CIPMUX=1",100);
      connect_wifi("AT+CIPSERVER=1,80",100);
}

void sendwebdata(String webPage)
{
    int ii=0;
     while(1)
     {
        unsigned int l=webPage.length();
        Serial.print("AT+CIPSEND=0,");
        client.print("AT+CIPSEND=0,");
        Serial.println(l+2);
        client.println(l+2);
        delay(100);
        Serial.println(webPage);
        client.println(webPage);
        while(client.available())
        {
          //Serial.print(Serial.read());
          if(client.find("OK"))
          {
            ii=11;
            break;
          }
        }
        if(ii==11)
        break;
        delay(100);
      }
}

void setup() 
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  client.begin(9600);
  wifi_init();
  Serial.println("System Ready..");
  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("System Ready");
  lcd.setCursor (0,1);
  lcd.print(IP);

}

void loop() 
{
  k=0;
  while(k<1000)
  {
    k++;
   while(client.available())
   {
    if(client.find("0,CONNECT"))
    {
      Serial.println("Start Printing");
      Send();
      Serial.println("Done Printing");
      delay(1000);
    }
  }
  delay(1);
 }
}

void Send()
{
  webpage = "<h1>Welcome to my server</h1><body bgcolor=f0f0f0>";
  sendwebdata(webpage);
  delay(3000);
  webpage=name;
  webpage+=dat;
  sendwebdata(webpage);

  client.println("AT+CIPCLOSE=0"); 
}
