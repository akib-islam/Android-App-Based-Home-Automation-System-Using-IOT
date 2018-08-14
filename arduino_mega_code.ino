#include <LiquidCrystal.h>
#include <Servo.h>
#include <Wire.h>
#include <RFID.h>
#include <SPI.h>

Servo doorservo;    // create servo object to control a servo 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int inputPin = 6;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

#define SS_PIN 9
#define RST_PIN 8


RFID rfid(SS_PIN,RST_PIN);

int serNum[5];
int cards[5] = {128,169,132,122,215};
int checkcard[5];
int loopcounter=0;
int chicagofirecheck;
int chicagofire;
 int rfidcounter;

int r1 = 22;
int r2 = 24;
int r3 = 26;
int r4 = 28;
int c1 = 30;
int c2 = 32;
int c3 = 34;
int c4 = 36;
int colm1;
int colm2;
int colm3;
int colm4;
int a, b, c, d, e, f;
int buzzer=38;

int pos=0;         // variable to store the servo position 
static int x[4];
static int y[4];
static int i, j, p, s, k;
int initial = 0, attempts = 0;
int count = 0;
int error;

void setup()
{
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);
  pinMode(c4, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(buzzer, LOW);
  Wire.begin();
  

   // pinMode(inputPin, INPUT);     // declare sensor as input
    
 
  Serial.begin(9600);
  doorservo.attach(10);  // attaches the servo on pin 38 to the servo object 

  SPI.begin();
  rfid.init();

  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
  digitalWrite(c3, HIGH);
  digitalWrite(c4, HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop()
{
  if (initial == 0)
    newpassword();

  if (attempts < 3)
    enterpassword();

  if (attempts >= 3)
    lockdoor();

  if (count == 4)
  {
    Serial.println("password matches");
    rfidcounter=0;
    loopcounter=0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("password matches");
    delay(1000);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Please verify");
    lcd.setCursor(0,1);
    lcd.print("RFID tag");

    while(rfidcounter==0)
       {
             if(rfid.isCard())
                {

                    if(rfid.readCardSerial())
                       {
                               checkcard[0]= rfid.serNum[0];
                               checkcard[1]= rfid.serNum[1];
                               checkcard[2]= rfid.serNum[2];
                               checkcard[3]= rfid.serNum[3];
                               checkcard[4]= rfid.serNum[4];
                                                    
                                                    for(chicagofire=0; chicagofire<5; chicagofire++)
                                                       {

                                                                 if(checkcard[chicagofire]==cards[chicagofire])
                                                                     {
                                                                          chicagofirecheck++;
                                                                     }

                                                        }
                        
                        }
                        
                   
                               
                }

                 delay(1000); 
                 loopcounter++;

                 if(loopcounter==5)
                 {
                  rfidcounter=1;
                 }
          }

    if((chicagofirecheck==5)||(chicagofirecheck==10)||(chicagofirecheck==15)||(chicagofirecheck==20)||(chicagofirecheck==25))
    {
      lcd.clear();
      lcd.setCursor(0,0);
          lcd.print("Welcome");
       digitalWrite(13, HIGH);
    delay(2000);



delay(500);
      for(pos=90; pos>=0; pos--)
  {
    doorservo.write(pos);
    delay(50);
  }
    lcd.clear();
    
    attempts = 0;
    count = 0;
    error = 0;
    initial = 1;
    delay(5000);
    Serial.println(" Door will close in 10 seconds ");
    lcd.setCursor(0,0);
    


    lcd.print("Close in 10 sec");
    for (s = 10; s >= 0; s--)
    {
         lcd.clear();
    lcd.print("Closing in");
    lcd.setCursor(11,0);
    lcd.print(s);
    lcd.setCursor(13,0);
    lcd.print("sec");
    delay(1000);
    }
       for(pos=0; pos<90; pos++)
  {
    doorservo.write(pos);
    delay(50);
  }
    lcd.clear();
    lcd.print("Door closed");
    Serial.println("Door closed");
    digitalWrite(13, LOW);
    delay(1000);
    chicagofirecheck=0;
    }
    
   else  
   {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sorry RFID");
    lcd.setCursor(0,1);
    lcd.print("doesn't match");
    digitalWrite(7, HIGH);
    delay(5000);

     attempts = 0;
    count = 0;
    error = 0;
    initial = 1;

    chicagofirecheck=0;
    digitalWrite(7 , LOW);
   }
    
}

  if (error > 0)
  {
    Serial.println(" Incorrect password");
      lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print("Wrong password");
  digitalWrite(7, HIGH);
  delay(2000);
 
    initial = 1;
    attempts++;
    error = 0;
    count = 0;
digitalWrite(7, LOW);
  }

}


void newpassword()              //to create new password
{
  lcd.clear();
  Serial.println(" Enter new password ");
    lcd.setCursor(0,0);
  lcd.print("Enter new pass");
   

  while (1)
  {
    
    digitalWrite(r1, LOW);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      x[i] = 1;
      Serial.println(x[i]);
        lcd.setCursor(i,1);
        lcd.print(x[i]);
      delay(400);
      i++;
    }
    else
    {
      if (colm2 == LOW)
      {
        x[i] = 2;
        Serial.println(x[i]);
        lcd.setCursor(i,1);
        lcd.print(x[i]);
        delay(400);
        i++;
      }
      else
      {
        if (colm3 == LOW)
        {
          x[i] = 3;
          Serial.println(x[i]);
           lcd.setCursor(i,1);
        lcd.print(x[i]);
          delay(400);
          i++;
        }
        else
        {
          if (colm4 == LOW)
          {
            x[i] = 10;
            Serial.println(x[i]);
             lcd.setCursor(i,1);
        lcd.print(x[i]);
            delay(400);
            i++;
          }
        }
      }
    }

    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      x[i] = 4;
      Serial.println(x[i]);
       lcd.setCursor(i,1);
        lcd.print(x[i]);
      delay(400);
      i++;
    }
    else
    {
      if (colm2 == LOW)
      {
        x[i] = 5;
        Serial.println(x[i]);
         lcd.setCursor(i,1);
        lcd.print(x[i]);
        delay(400);
        i++;
      }
      else
      {
        if (colm3 == LOW)
        {
          x[i] = 6;
          Serial.println(x[i]);
           lcd.setCursor(i,1);
        lcd.print(x[i]);
          delay(400);
          i++;
        }
        else
        {
          if (colm4 == LOW)
          {
            x[i] = 11;
            Serial.println(x[i]);
             lcd.setCursor(i,1);
        lcd.print(x[i]);
            delay(400);
            i++;
          }
        }
      }
    }

    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      x[i] = 7;
      Serial.println(x[i]);
       lcd.setCursor(i,1);
        lcd.print(x[i]);
      delay(400);
      i++;
    }
    else
    {
      if (colm2 == LOW)
      {
        x[i] = 8;
        Serial.println(x[i]);
         lcd.setCursor(i,1);
        lcd.print(x[i]);
        delay(400);
        i++;
      }
      else
      {
        if (colm3 == LOW)
        {
          x[i] = 9;
          Serial.println(x[i]);
           lcd.setCursor(i,1);
        lcd.print(x[i]);
          delay(400);
          i++;
        }
        else
        {
          if (colm4 == LOW)
          {
            x[i] = 12;
            Serial.println(x[i]);
             lcd.setCursor(i,1);
        lcd.print(x[i]);
            delay(400);
            i++;
          }
        }
      }
    }
    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, LOW);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      x[i] = 15;
      Serial.println(x[i]);
       lcd.setCursor(i,1);
        lcd.print(x[i]);
      delay(400);
      i++;
    }
    else
    {
      if (colm2 == LOW)
      {
        x[i] = 0;
        Serial.println(x[i]);
         lcd.setCursor(i,1);
        lcd.print(x[i]);
        delay(400);
        i++;
      }
      else
      {
        if (colm3 == LOW)
        {
          x[i] = 14;
          Serial.println(x[i]);
           lcd.setCursor(i,1);
        lcd.print(x[i]);
          delay(400);
          i++;
        }
        else
        {
          if (colm4 == LOW)
          {
            x[i] = 13;
            Serial.println(x[i]);
             lcd.setCursor(i,1);
        lcd.print(x[i]);
            delay(400);
            i++;
          }

        }
      }
    }
    if (i == 4)
    {
      break;
    }
  }
  lcd.clear();
}


void enterpassword()        //to check the password
{
lcd.clear();
  Serial.println("Please enter the password");
   lcd.setCursor(0, 0);
        lcd.print("Enter password");

     
  while (1)
  {
  val = digitalRead(inputPin);  // read input value   
       if (val == HIGH) 
       {           

    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      opendoor();
    }
    }

     if(val==LOW) {
 
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
     pirState = LOW;
     closedoor();
    }
  }

    
    digitalWrite(r1, LOW);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      y[j] = 1;
      Serial.println(y[j]);
       lcd.setCursor(j,1);
        lcd.print("*");
      delay(400);
      j++;
    }
    else
    {
      if (colm2 == LOW)
      {
        y[j] = 2;
        Serial.println(y[j]);
         lcd.setCursor(j,1);
        lcd.print("*");
        delay(400);
        j++;
      }
      else
      {
        if (colm3 == LOW)
        {
          y[j] = 3;
          Serial.println(y[j]);
           lcd.setCursor(j,1);
        lcd.print("*");
          delay(400);
          j++;
        }
        else
        {
          if (colm4 == LOW)
          {
            y[j] = 10;
            Serial.println(y[j]);
             lcd.setCursor(j,1);
        lcd.print("*");
            delay(400);
            j++;
          }
        }
      }
    }

    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      y[j] = 4;
      Serial.println(y[j]);
       lcd.setCursor(j,1);
        lcd.print("*");
      delay(400);
      j++;
    }
    else
    {
      if (colm2 == LOW)
      {
        y[j] = 5;
        Serial.println(y[j]);
         lcd.setCursor(j,1);
        lcd.print("*");
        delay(400);
        j++;
      }
      else
      {
        if (colm3 == LOW)
        {
          y[j] = 6;
          Serial.println(y[j]);
           lcd.setCursor(j,1);
        lcd.print("*");
          delay(400);
          j++;
        }
        else
        {
          if (colm4 == LOW)
          {
            y[j] = 11;
            Serial.println(y[j]);
             lcd.setCursor(j,1);
        lcd.print("*");
            delay(400);
            j++;
          }
        }
      }
    }

    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, LOW);
    digitalWrite(r4, HIGH);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      y[j] = 7;
      Serial.println(y[j]);
       lcd.setCursor(j,1);
        lcd.print("*");
      delay(400);
      j++;
    }
    else
    {
      if (colm2 == LOW)
      {
        y[j] = 8;
        Serial.println(y[j]);
         lcd.setCursor(j,1);
        lcd.print("*");
        delay(400);
        j++;
      }
      else
      {
        if (colm3 == LOW)
        {
          y[j] = 9;
          Serial.println(y[j]);
           lcd.setCursor(j,1);
        lcd.print("*");
          delay(400);
          j++;
        }
        else
        {
          if (colm4 == LOW)
          {
            y[j] = 12;
            Serial.println(y[j]);
             lcd.setCursor(j,1);
        lcd.print("*");
            delay(400);
            j++;
          }
        }
      }
    }
    digitalWrite(r1, HIGH);
    digitalWrite(r2, HIGH);
    digitalWrite(r3, HIGH);
    digitalWrite(r4, LOW);
    colm1 = digitalRead(c1);
    colm2 = digitalRead(c2);
    colm3 = digitalRead(c3);
    colm4 = digitalRead(c4);
    if (colm1 == LOW)
    {
      y[j] = 15;
      Serial.println(y[j]);
       lcd.setCursor(j,1);
        lcd.print("*");
      delay(400);
      j++;
    }
    else
    {
      if (colm2 == LOW)
      {
        y[j] = 0;
        Serial.println(y[j]);
         lcd.setCursor(j,1);
        lcd.print("*");
        delay(400);
        j++;
      }
      else
      {
        if (colm3 == LOW)
        {
          y[j] = 14;
          Serial.println(y[j]);
           lcd.setCursor(j,1);
        lcd.print("*");
          delay(400);
          j++;
        }
        else
        {
          if (colm4 == LOW)
          {
            y[j] = 13;
            Serial.println(y[j]);
             lcd.setCursor(j,1);
        lcd.print("*");
            delay(400);
            j++;
          }

        }
      }
    }
    if (j == 4)
      break;
  }
  lcd.clear();

  check();




}




void check()
{
  lcd.clear();
  Serial.println("Checking");
   lcd.setCursor(0, 0);
        lcd.print("Checking");
        delay(1000);
  for (k = 0; k < 4; k++)
  {
    if (x[k] == y[k])
    {
      count++;
      i = 0;
      j = 0;
    }
    else
    {
      error++;
      i = 0;
      j = 0;
    }

  }
}

void lockdoor()
{
  Serial.println(" Door is permanently locked");
  lcd.clear();
   lcd.setCursor(0, 0);
        lcd.print("permanently lock");
       digitalWrite(7, HIGH);
        delay(1000);
        lcd.clear();

      int op=1;
      Wire.beginTransmission(9);           // transmit to device #9
      Wire.write(op);                      // sends x
      Wire.endTransmission();

      delay(500);
  for (p = 30; p >= 0; p--)
  {
            if(p<10)
        {
          digitalWrite(buzzer, HIGH);
          delay(100);
            lcd.clear();
          lcd.print("try after");
    lcd.setCursor(10,0);
    lcd.print(p);
    lcd.setCursor(13,0);
    lcd.print("sec");
    delay(1000);
        }
      
   else
   {
    digitalWrite(buzzer, HIGH);
          delay(100);
     lcd.clear();
    lcd.print("try after");
    lcd.setCursor(10,0);
    lcd.print(p);
    lcd.setCursor(13,0);
    lcd.print("sec");
    delay(1000);
   }
digitalWrite(38, LOW);
digitalWrite(7, LOW);
delay(100);

  }

  attempts = 0;



}
void opendoor()
{


 digitalWrite(13, HIGH);
   for(pos=100; pos>=0; pos--)
  {
    doorservo.write(pos);
    delay(50);
  }
  delay(20000);
  attempts=0;
  
}

void closedoor()
{

  for(pos=0; pos<100; pos++)
  {
    doorservo.write(pos);
    delay(50);
  }
digitalWrite(13, LOW);
}
