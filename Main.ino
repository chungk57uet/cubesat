  // Gọi thư viện DHT11
#include "DHT.h"            
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"

const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
 
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 RTC;

/* Địa chỉ của DS1307 */
const byte DS1307 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;
 
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;

void setup() {
  Serial.begin(9600);
  dht.begin();         // Khởi động cảm biến
  Wire.begin();
  RTC.begin();
  
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  // xóa các giá trị tại các ô nhớ EEPROM từ 0-511 (có 512 ô nhớ)
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

    // Đọc, lưu và xuất KQ đo
    float h = dht.readHumidity();    //Đọc độ ẩm
    float t = dht.readTemperature(); //Đọc nhiệt độ
    Serial.print("Nhiet do: ");
    Serial.println(t);               //Xuất nhiệt độ
    Serial.print("Do am: ");
    Serial.println(h);               //Xuất độ ẩm
    Serial.print(" Ket thuc lan do thu ");
    Serial.println(no);
    no = no + 1;
    Serial.println();                //Xuống hàng
    
    // Ghi thong so do duoc len bo nho EEPROM
    EEPROM.write(i, t);
    delay(500);
    i = i + 1;
    EEPROM.write(i, h);
    delay(500);
    i = i + 1;
    if (i == 1024)
      i=0; 

    // Bật tắt led sau mỗi lần đo và hiện KQ
    pinMode(13,OUTPUT);
    digitalWrite(13, HIGH);
    delay(1000);
    pinMode(13,OUTPUT);
    digitalWrite(13, LOW);
  }
}

/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}

/* Auto đặt dấu thời gian */
void printTime(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu :
    Serial.print(":");
    if(digits < 10)
        Serial.print('0');
        Serial.print(digits);
}

/* Auto đặt dấu ngày */
void printDate(int digits){
    // các thành phần thời gian được ngăn chách bằng dấu /
    Serial.print("/");
    if(digits < 10)
        Serial.print('0');
        Serial.print(digits);
}

/* cài đặt thời gian cho DS1307 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(DS1307);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}
