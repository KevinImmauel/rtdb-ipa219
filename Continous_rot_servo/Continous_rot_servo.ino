#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>

Servo servo;  //Servo name is myservo
RTC_DS3231 rtc;
const int buzzer = 6;
char t[32];
void setup()
{
  Serial.begin(9600);
  servo.attach(6);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  pinMode(buzzer, OUTPUT); 
}
void loop()
{
  DateTime now = rtc.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  Serial.print(F("Date/Time: "));
  Serial.println(t);
  delay(5000);
  if (now.hour() == 2 && now.minute() == 10){
    servo.write(0); //Spin in one direction
  delay(3000);
  servo.write(90); // Stop
  delay(500);
  servo.write(180);  // Spin in opposite direction
  delay(3000);
  servo.write(90);  // Stop
  delay(500);
  delay(1000);
  digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    }
  
}