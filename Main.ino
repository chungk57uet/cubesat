  // DHT11 lib
#include "DHT.h"            
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"

const int DHTPIN = 2;       // Read data from DHT11
const int DHTTYPE = DHT11;  // Sensor type

// Realtime module
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 RTC;
const byte DS1307 = 0x68;
const byte NumberOfFields = 7;
 
/* Time variables */
int second, minute, hour, day, wday, month, year;

void setup() {
  Serial.begin(9600);
  dht.begin();         // Starting DHT11 sensor
  Wire.begin();
  RTC.begin();
  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  // del values of EEPROM from 0-511
  for (int i = 0; i < 1024; i++) {
    EEPROM.write(i, 0);
    delay(5);
}
}

void loop() {
  int no = 1;
    for (int i=0; i < 1024; ){
    // Module Real time
    DateTime now = RTC.now();
    Serial.print("Bay gio la : ");
    Serial.print(now.hour(), DEC);
    printTime(now.minute());
    printTime(now.second());
    Serial.print(" Ngay "); 
    Serial.print(now.day());
    printDate(now.month());
    printDate(now.year());
    Serial.println(); 

    // Read, save and display tempt, humidity
    float h = dht.readHumidity(); 
    float t = dht.readTemperature();
    Serial.print("Nhiet do: ");
    Serial.println(t);           
    Serial.print("Do am: ");
    Serial.println(h);              
    Serial.print(" Ket thuc lan do thu ");
    Serial.println(no);
    no = no + 1;
    Serial.println();            
    
    // Write data to EEPROM
    EEPROM.write(i, t);
    delay(500);
    i = i + 1;
    EEPROM.write(i, h);
    delay(500);
    i = i + 1;
    if (i == 1024)
      i=0; 

    // Turn on/off LED
    pinMode(13,OUTPUT);
    digitalWrite(13, HIGH);
    delay(1000);
    pinMode(13,OUTPUT);
    digitalWrite(13, LOW);
  }
}

/* Convert format BCD (Binary-Coded Decimal) to Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Convert Decimal to BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}

// Display Time
void printTime(int digits){
    Serial.print(":");
    if(digits < 10)
        Serial.print('0');
        Serial.print(digits);
}

void printDate(int digits){
    Serial.print("/");
    if(digits < 10)
        Serial.print('0');
        Serial.print(digits);
}

/* Setup Time for DS1307 module */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00));
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}
