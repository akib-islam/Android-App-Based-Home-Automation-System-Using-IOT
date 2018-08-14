#include <SoftwareSerial.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

SoftwareSerial BT(8, 9); //TX, RX respetively
String voice;

void setup() 
{
 BT.begin(9600);
 Serial.begin(9600);

 lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T.V. OFF");
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);

}
//-----------------------------------------------------------------------//  
void loop() 
{
  while (BT.available())
  {  
  delay(10); 
  char c = BT.read(); 
  if (c == '#')
    {
      break;
    } 
  voice += c; 
  }  
    if (voice.length() > 0) 
    {
    Serial.println(voice); 

    if(voice == "*turn on the television") 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T.V turning on");
    } 
  
    if(voice == "*turn off the television") 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("T.V turning off");
    }

    if((voice == "*change to channel one")||(voice == "*change to channel 1")) 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel 1");
    }

    if((voice == "*change to channel two")||(voice == "*change to channel 2"))
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel 2");
    }

    if((voice == "*change to channel three")||(voice == "*change to channel 3"))
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel 3");
    }

    if((voice == "*change to channel four")||(voice == "*change to channel 4"))
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel 4");
    }

    if((voice == "*change to Channel V")||(voice == "*change to channel 5")) 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Channel 5");
    }

    if(voice == "*increase the volume") 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Increasing volumne");
    }

      if(voice == "*decrease the volume") 
    {
       lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Decreasing volumne");
    }

    
      if(voice == "*turn on coffee machine") 
    {
       digitalWrite(7, HIGH);
    }

    
      if(voice == "*turn off coffee machine") 
    {
      digitalWrite(7, LOW);
    }

     if(voice == "*turn on the bulb") 
    {
       digitalWrite(10, HIGH);
    }

    if(voice == "*turn off the bulb") 
    {
       digitalWrite(10, LOW);
    }

    voice="";
    }
    
  } 