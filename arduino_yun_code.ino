#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <SimpleTimer.h>
#include <dht.h>
#include <SPI.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Temboo.h>
#include <Process.h>
#include <Wire.h>
#include "TembooAccount.h" 

int onlyhours, onlyminutes;
int starthour, startminute, stophour, stopminute;
dht DHT;
float b,d;
int a,c;
int x=0;
int z;

Process picture;

int calls = 1;        // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed 

int front = 0; //front door monitoring
int count = 0;

// Filename
String filename;


// Path
String path = "/mnt/sda1/";

//DHT PIN DEFINING
//sda to pin 2
//scl to pin 3

#define DHT11_PIN 4
#define DHTNEW_PIN 5

SimpleTimer timer;

WidgetRTC rtc;

char auth[] = "your_authentication_token";

WidgetLCD lcd(V1);
WidgetLCD lcdmonitor(V4);

BLYNK_WRITE(V29)
{
  int gpsstate= param.asInt();

  if(gpsstate== HIGH)
     {
        digitalWrite(11, HIGH);
        Serial.println("inside");
     }

   else if(gpsstate== LOW)
     {
      digitalWrite(11, LOW);
      Serial.println("outside");
     }
}

  
BLYNK_WRITE(V31) 
{
  TimeInputParam t(param);  // Process start time

   

   if(t.hasStartTime())
     {
        starthour = t.getStartHour();
        startminute = t.getStartMinute();
      

     }

  if (t.hasStopTime())
  {
     stophour = t.getStopHour();
     stopminute = t.getStopMinute();
  }

  
 
  Serial.println(String("Time zone: ") + t.getTZ());

  Serial.println(String("Time zone offset: ") + t.getTZ_Offset());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

  for (int i = 1; i <= 7; i++)
  {
    if (t.isWeekdaySelected(i))
    {
      Serial.println(String("Day ") + i + " is selected");
    }
  }
  
     Serial.print("starthour ");
       Serial.println(starthour);
         Serial.print("startminute ");
       Serial.println(startminute);
         Serial.print("stophour");
       Serial.println(stophour);
         Serial.print("stopminute ");
       Serial.println(stopminute);
}

void myTimerEvent()
{
    if (digitalRead(10) == true)
  {

    // Generate filename with timestamp
    filename = "";
    picture.runShellCommand("date +%s");
    while(picture.running());

    while (picture.available()>0) {
      char c = picture.read();
      filename += c;
    } 
    filename.trim();
    filename += ".png";
 
    // Take picture
    picture.runShellCommand("fswebcam " + path + filename + " -r 1280x720");
    while(picture.running());
    
    // Upload to Dropbox
    picture.runShellCommand("python " + path + "upload_picture.py " + path + filename);
    while(picture.running());
  }
   int chk = DHT.read11(DHT11_PIN);
   
   a=DHT.temperature;
   b=DHT.humidity;

  Blynk.virtualWrite(V2,a);  
  Blynk.virtualWrite(V10,b);
  Blynk.virtualWrite(V9,a);
  Blynk.virtualWrite(V5,b);
 

  int cok = DHT.read11(DHTNEW_PIN);   
  
  c=DHT.temperature;
  d=DHT.humidity;

  Blynk.virtualWrite(V6,c);  
  Blynk.virtualWrite(V7,d);
  Blynk.virtualWrite(V8,c);
  Blynk.virtualWrite(V9,d);

   if((starthour==onlyhours)&&(startminute==onlyminutes))
     {
      digitalWrite(7, HIGH); 
      
     
     }

    if((stophour==onlyhours)&&(stopminute==onlyminutes))
      {
        digitalWrite(7, LOW);
    
     
      }


  
 int fire= analogRead(A0);
 int ma = map(fire,0,1023,1023,0);
 Blynk.virtualWrite(V11, ma);
 Blynk.virtualWrite(V12, ma);
 

 if(fire<500)
  {
    Blynk.notify("Fire Alert in the main room");
  
  }

  if(fire>=500)
  {
  
  } 

  if ( (front==1) && (count==1) ) 
  {
   
    
    TembooChoreo SendSMSChoreo;

     SendSMSChoreo.begin();    // Invoke the Temboo client

    // Set Temboo account credentials
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    
    SendSMSChoreo.setProfile("your_profile_name"); //use the profile name you used while saving the details in temboo Sendsms choreo
      
    
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");   // Identify the Choreo to run
    
    SendSMSChoreo.run();  // Run the Choreo; when results are available, print them to serial
    
    while(SendSMSChoreo.available()) 
    {
      char c = SendSMSChoreo.read();
    
    }
    SendSMSChoreo.close();
    count=0;

     // Generate filename with timestamp
    filename = "";
    picture.runShellCommand("date +%s");
    while(picture.running());

    while (picture.available()>0) {
      char c = picture.read();
      filename += c;
    } 
    filename.trim();
    filename += ".png";
 
    // Take picture
    picture.runShellCommand("fswebcam " + path + filename + " -r 1280x720");
    while(picture.running());
    
    // Upload to Dropbox
    picture.runShellCommand("python " + path + "upload_picture.py " + path + filename);
    while(picture.running());
  }

  


}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);
  Bridge.begin();
    rtc.begin();
  pinMode(7, OUTPUT);
  pinMode(10, INPUT);
  Wire.begin(9);                  // Start the I2C Bus as Slave on address 9
  Wire.onReceive(receiveEvent);     // Attach a function to trigger when something is received.
  
  while (Blynk.connect() == false)
  {}

  lcd.clear();
  lcd.print(4, 0, "Welcome"); 
  lcd.print(2, 1, "Akib Islam");
  
  timer.setInterval(5000, myTimerEvent);
  timer.setInterval(10000L, clockDisplay);
  
}

void receiveEvent(int bytes)
{
front = Wire.read();                         // read one character from the I2C
count=1;
}

void clockDisplay()
{
  
  String currentTime = String(hour()) + ":" + minute();
  String currentDate = String(day()) + " " + month() + " " + year();
  


   onlyhours = hour();
   onlyminutes = minute();
   

  Blynk.virtualWrite(V19, onlyhours);
  Blynk.virtualWrite(V20, onlyminutes);

    lcdmonitor.clear();
  lcdmonitor.print(0, 1,  "Time"); 
  lcdmonitor.print(5, 1, currentTime); 
   lcdmonitor.print(11, 1,  "Hours"); 
 
}

void loop()
{

  Blynk.run();
  timer.run(); 

  
}


