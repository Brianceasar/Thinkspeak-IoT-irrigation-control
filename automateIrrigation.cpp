
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "ThingSpeak.h"         // Include the OneWire library used with DS18B20 sensor



#define SOUND_SPEED 0.034

const char* ssid="#";//replace subscribe with your WiFi SSID(Name)
const char* password ="#";//replace with Your Wifi Password name
const char* host = "api.thingspeak.com";
const char* myWriteAPIKey  = "#"; //replace with your ThingSpeak channel API Key.
volatile byte pulseCount;
long currentMillis = 0;
long previousMillis2 = 0;
long previousMillis = 0;
int interval = 20000;
int interval2 = 5000;
long duration;
unsigned long timer1=0;  
unsigned long timer2=0;
unsigned long tofauti=0; 
unsigned long tofauti2=0; 
volatile int i=0;
int statusCode = 0,PumpState,ValveState,ControlState;
int field[6] = {1,2,3,4,5,6};
const int h=30,TRIG_PIN=18,ECHO_PIN=19;
// channel details
unsigned long myChannelNumber = 2491795;
unsigned int counterFieldNumber = 1; 
LiquidCrystal_I2C lcd(0x27, 20, 4);

struct dataStruct{                               // Group list of variables under one name in a block of memory
  float WaterReserve;                                  // Temperature in deg C to be transmitted 
  float SoilMoisture;                                  // Temperature in deg F to be transmitted
  float TankLevel; 
  bool PumpState; 
  bool ValveState;
  bool ControlState;
}myData;
 WiFiClient client;
 int WaterReserveP=35,SoilMoistureP=32,Pump=26,Valve=27;// DS18B20 Data wire is connected to pin 2 of the Arduino;
//22,23
//float WaterReserve,SoilMoisture,sulphide,oxygen,temp; 


String messege;
const int max_SoilMoisture=40,max2_SoilMoisture=150,min_SoilMoisture=10, min_WaterReserve=20,max_TankLevel=70,max2_TankLevel=100,min_TankLevel=10,sending=0; 


void setup() {
  // initialize serial communication at 9600 bits per second:
 
  Serial.begin(115200);
  lcd.init();
   Serial.println("Connecting to ");
   Serial.println(ssid);
 // Connect to WiFi network
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   lcd.setCursor(0,0);
   lcd.print("   connecting   ");
  lcd.setCursor(i,1);
  lcd.print(".");
  i++;
  }
    i=0; 
    lcd.clear();
   Serial.println("");
   Serial.println("WiFi connected");
   lcd.setCursor(0,0);
   lcd.print("  connected to ");
  lcd.setCursor(0,1);
  lcd.print("   ");
  lcd.print(ssid);
 //----------------------------------------ThingSpeak.begin(client)
 ThingSpeak.begin(client);
   delay(3000); 


 lcd.setCursor(0,0);
 lcd.print("     SMART      ");
 lcd.setCursor(0,1);
 lcd.print("     IRRIGATION    ");
 lcd.setCursor(0,2);
 lcd.print("    SYSTEM     ");


  pinMode(WaterReserveP,INPUT);
  pinMode(SoilMoistureP, INPUT);
   pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO_PIN, INPUT);
    pinMode(Pump,OUTPUT);
    pinMode(Valve,OUTPUT);
//pinMode(BUZZER, INPUT);
   delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tank1=");
 // lcd.setCursor(11,0);
 // lcd.print("motor");
  lcd.setCursor(0,1);
  lcd.print("Tank2=");
  lcd.setCursor(0,2);
  lcd.print("Moisture="); 
  lcd.setCursor(1,3);
  lcd.print("Pump=");
  lcd.setCursor(12,3);
  lcd.print("Valve=");
  
}

  void WaterReserves(){
  // read the value from the sensor:
   myData.WaterReserve = analogRead(WaterReserveP);
 
   myData.WaterReserve =4095-myData.WaterReserve;
   myData.WaterReserve =myData.WaterReserve /4095;
   myData.WaterReserve = myData.WaterReserve*100;
 
   }

  void TankLevel() {
   
 digitalWrite(TRIG_PIN,LOW); 
 delayMicroseconds(2); 
 
 digitalWrite(TRIG_PIN,HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(TRIG_PIN,LOW);
 duration = pulseIn(ECHO_PIN,HIGH);
 
//Calculate the distance (in cm) based on the speed of sound.
myData.TankLevel = duration * SOUND_SPEED/2;
myData.TankLevel =(duration/58.2);
myData.TankLevel=(h-myData.TankLevel );
myData.TankLevel=(myData.TankLevel/h)*100;
 
 }

    void SoilMoistures() {
  // read the value from the sensor:
   myData.SoilMoisture = analogRead(SoilMoistureP);

   myData.SoilMoisture = myData.SoilMoisture/4095;
   myData.SoilMoisture = myData.SoilMoisture*100;
   myData.SoilMoisture = 100-myData.SoilMoisture;  
   }

   void PumpOperation() {
  // read the value from the sensor:
   myData.PumpState ;
   
   }

      void ValveOperation() {
  // read the value from the sensor:
   myData.ValveState ;
     
   }

  void datas(){
  
         Serial.print("WaterReserve=");
         Serial.println(myData.WaterReserve);
         Serial.print("Moisture=");
         Serial.println(myData.SoilMoisture);
          
            Serial.print("TankLevel=");
            Serial.println(myData.TankLevel);
           
           Serial.print("irrigationPump=");
           Serial.println(myData.PumpState);
            Serial.print("irrigationValve=");
            Serial.println(myData.ValveState);
            Serial.print("ControlState=");
            Serial.println(myData.ControlState);
              
           Serial.println("------------------------------------");     
   }


void Display(){
     lcd.setCursor(6,3);  
    lcd.print(myData.PumpState);
     lcd.setCursor(18,3);  
    lcd.print(myData.ValveState);
   
if((myData.SoilMoisture<max2_SoilMoisture)&&(myData.SoilMoisture>min_SoilMoisture)){  
   
  lcd.setCursor(0,2);
  
  lcd.print("Moisture=");   
    lcd.print(myData.SoilMoisture,0);
    lcd.print("% ");
    lcd.print("MIN=");
  lcd.print(min_SoilMoisture);
  lcd.print("% ");   
  }
 else if(myData.SoilMoisture<max_SoilMoisture){
      //messege="";
      
       lcd.setCursor(0,2);
 
       lcd.print("IrrigateNow ");
        lcd.print(min_SoilMoisture);
          lcd.print("%");
        lcd.print("(");
         lcd.print(myData.SoilMoisture,0); 
         lcd.print("%)");

  }

 
  if(myData.WaterReserve  >  min_WaterReserve){  
  lcd.setCursor(0,0);
  lcd.print("Tank1=");
    //lcd.setCursor(6,0); 
    lcd.print( myData.WaterReserve,0);
    lcd.print("% ");
    lcd.print("MIN=");
  lcd.print( min_WaterReserve);
  lcd.print("%   ");
   
    }
 else if(myData.WaterReserve < min_WaterReserve){
      //messege="";

        lcd.setCursor(0,0); 
       lcd.print("RefillTank1 ");
        //lcd.setCursor(12,0);
        lcd.print( min_WaterReserve);
          lcd.print("%");
        lcd.print("(");
        lcd.setCursor(16,0);
         lcd.print(myData.WaterReserve,0); 
         lcd.print("%)");
  }



if((myData.TankLevel<max2_TankLevel)&&(myData.TankLevel>min_TankLevel)){  
   
    lcd.setCursor(0,1);
    lcd.print("Tank2="); 
    lcd.print(myData.TankLevel,0);
    lcd.print("%  ");
    lcd.print("MIN=");
  lcd.print(min_TankLevel);
  lcd.print("%   ");
    
    }
 else if(myData.TankLevel<min_TankLevel){
      //messege="";
      
       lcd.setCursor(0,1);
       lcd.print("RefillTank2 ");;
        lcd.print(min_TankLevel);
        lcd.print("%");
       
       // lcd.setCursor(15,0);
         lcd.print("(");
         lcd.print(myData.TankLevel,0); 
         lcd.print("%)");
  }

   }

  void loop() {
  //normal();

    WaterReserves();
    SoilMoistures();
    TankLevel();
    PumpOperation();
    ValveOperation();
    datas();
    Display();
    
    
    ControlMode();
    thingSpeak();
   // delay(2000);
  //lcd.clear();
   
  }
 void ControlMode(){
  // myData.ControlState=1;
   ReadfieldValue();
  if (myData.ControlState==1){
         if(PumpState==1){
        digitalWrite(Pump,HIGH);
        myData.PumpState=1;
      }
      else if(PumpState==0){
        digitalWrite(Pump,LOW);
         myData.PumpState=0;
      }
    if(ValveState==1){
        digitalWrite(Valve,HIGH);
         myData.ValveState=1;
      }
    else if(ValveState==0){
        digitalWrite(Valve,LOW);
        myData.ValveState=0;  
      }
 }

  else if(myData.ControlState==0){
  Automations();
 }
 }

  void Automations(){
    if(myData.TankLevel < min_TankLevel){
       if(myData.WaterReserve > min_WaterReserve){
        digitalWrite(Pump,HIGH);
        digitalWrite(Valve,LOW);
        myData.PumpState=1;
        myData.ValveState=0;
       }   
      }
    else if((myData.WaterReserve < min_WaterReserve)||(myData.TankLevel > max_TankLevel)){
                 digitalWrite(Pump,LOW);
                  myData.PumpState=0; 
                  myData.ValveState=0; 
          }
       
       
     
     
   if((myData.SoilMoisture < min_SoilMoisture)&&(myData.PumpState==0)){
        //digitalWrite(Pump,LOW);
        digitalWrite(Valve,HIGH);
        myData.ValveState=1;
       // myData.PumpState=0;
        };
     
  
    if(myData.SoilMoisture > max_SoilMoisture){
        //digitalWrite(Pump,LOW);
        digitalWrite(Valve,LOW);
        myData.ValveState=0;
        //myData.PumpState=0;}
  
  }
   }

void thingSpeak(){
    
    tofauti=millis()-timer1;
    if(tofauti>interval) {   
   // Read in field 1 of the public channel recording the level
//  float Temp =ThingSpeak.readFloatField(weatherStationChannelNumber, counterFieldNumber);  
//  Serial.println(String(statusCode));
  // Check the status of the read operation to see if it was successful
//  statusCode = ThingSpeak.getLastReadStatus();
//  if(statusCode == 200){
  ThingSpeak.setField(1, myData.WaterReserve);
  ThingSpeak.setField(2, myData.TankLevel);
  ThingSpeak.setField(3, myData.SoilMoisture);
  ThingSpeak.setField(4, myData.PumpState);
  ThingSpeak.setField(5, myData.ValveState);
  ThingSpeak.setField(6, myData.ControlState);
  
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }


     timer1=millis();
    
    }
   }
void ReadfieldValue(){
  tofauti2=millis()-timer2;
  if(tofauti2>interval2) {
  statusCode = ThingSpeak.readMultipleFields(myChannelNumber);
    if(statusCode == 200)
    {   
      PumpState= ThingSpeak.getFieldAsInt(field[3]); // Field 4
      ValveState= ThingSpeak.getFieldAsInt(field[4]);  // Field 5   
       ControlState = ThingSpeak.getFieldAsInt(field[5]); // Field 6      
        Serial.print("ControlState=================");
        Serial.println(myData.ControlState); // Field 5
        Serial.print("Reading status code===============");
        Serial.println(statusCode);
        Serial.print("manual Valve state===============");
           Serial.println(ValveState);
            Serial.print("manual pump state==============");
            Serial.println(PumpState);
//delay(2000);
timer2=millis();
    }
    }
      }
        


  
 