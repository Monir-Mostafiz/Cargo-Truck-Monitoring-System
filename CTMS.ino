#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//for gps
const int gpsTxPin = 10; 
const int gpsRxPin = 11;
SoftwareSerial gpsSerial(gpsTxPin, gpsRxPin);
TinyGPSPlus gps;
double latitude;
double longitude;
//for fuel
 int trigPin = 7;
 int echoPin = 6;
long duration;
int distance;
const float minDistance = 540.0; // Adjust this value based on your calibration
const float maxDistance = 80.0;

//for door
int trigPin2 = 5;
 int echoPin2 = 4;
 long duration2;

 MPU6050 mpu(Wire);
 unsigned long timer = 0;

LiquidCrystal_I2C lcd(0x27,16,2); 

int count=0;
int newcount;

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();
  Wire.begin();  
  mpu.begin();
  mpu.calcGyroOffsets();
  
  //for button
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  //for door
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT);     
  
  Serial.begin(9600);
  gpsSerial.begin(9600);
  
  lcd.setCursor(2,0);
  lcd.print("Cargo Truck");
  lcd.setCursor(0,1);   
  lcd.print("Monioring System");  
}

void loop() {
  if (digitalRead(2) == HIGH)
  {
    count=count+1;
    delay(300);
    Serial.println(count);
    lcd.clear(); 
  }
  while(count==1){

    //lcd.setCursor(0,0);
    //lcd.print("lat.  24.90000");
    //lcd.setCursor(0,1);
   // lcd.print("long. 90.17000");
      
    if (gpsSerial.available() > 0) 
   {
    if (gps.encode(gpsSerial.read())) 
    {
      if (gps.location.isUpdated()) 
      {
        //lcd.clear();
        // Read latitude and longitude
        latitude = gps.location.lat();
        longitude = gps.location.lng();

        // Print latitude and longitude to Serial Monitor
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);

        // Print latitude and longitude to LCD
        lcd.setCursor(10, 0);
        lcd.print("    ");
        lcd.setCursor(0,0);
        lcd.print("lat.");
        lcd.setCursor(6, 0);
        lcd.print(latitude, 6);

        lcd.setCursor(11, 1);
        lcd.print("    ");
        lcd.setCursor(0,1);
        lcd.print("long.");
        lcd.setCursor(6, 1);
        lcd.print(longitude, 6);
      }
    }
  }
  if (digitalRead(2) == HIGH)
   {
    count=count+1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
   }
   
  }
  
  while(count==2){
  
  lcd.setCursor(2,0);
  lcd.print("Fuel Level");
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //distance = duration * 0.034 / 2;
  lcd.setCursor(0,1);
  lcd.print("         ");
  lcd.setCursor(0,1);
  //Serial.println(distance);
  lcd.print(duration);
  //long percentage = ((duration-89)/450)*100;
   int waterLevelPercentage = map(duration, minDistance, maxDistance, 0, 100);
  lcd.setCursor(6,1);
  lcd.print(waterLevelPercentage);
  lcd.setCursor(8,1);
  lcd.print("%");
  lcd.setCursor(9,1);
  lcd.print("  ");

  delay(1000);
 
  if (digitalRead(3) == HIGH)
  {
    count=count-1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
  }
  else if (digitalRead(2) == HIGH)
   {
    count=count+1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
   }
  }

  while(count==3){
  
  lcd.setCursor(2,0);
  lcd.print("Door Status");
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);


  lcd.setCursor(0,1);
  lcd.print("         ");
  lcd.setCursor(0,1);
  //lcd.print(duration2);
  if(duration2<300)
  {
    //delay(300);
    lcd.setCursor(6,1);
    lcd.print("Close");
  }
  else
  {
    
    lcd.setCursor(6,1);
    lcd.print("Open ");
  }
  
  delay(1000);
 
  if (digitalRead(3) == HIGH)
  {
    count=count-1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
  }
  else if (digitalRead(2) == HIGH)
   {
    count=count+1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
   }
  }
  while(count==4){
  
  //lcd.setCursor(2,0);
  //lcd.print("Axle angle");
  mpu.update();
  if ((millis() - timer) > 100) { // print data every 100ms
    timer = millis();
    //lcd.clear();
    //lcd.setCursor(6,1);
    //lcd.print("Open ");
    lcd.setCursor(2,0);
    lcd.print(" Axel Angle: ");
    lcd.setCursor(0,1);
    lcd.print("            ");
    lcd.setCursor(5,1);
    lcd.print(int(mpu.getAngleZ()));     //Print Z angle value on LCD 
    delay(10);
  
 
  if (digitalRead(3) == HIGH)
  {
    count=count-1;
    delay(100);
    Serial.println(count);
    lcd.clear(); 
  }
  }
  }
}