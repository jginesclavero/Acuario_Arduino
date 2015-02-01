#include <Wire.h>
#include "RTClib.h" 
#include "stdio.h"
#include <SD.h>
#include <string.h>
#include <OneWire.h>
#include <DallasTemperature.h>

RTC_DS1307 RTC; 
#include <LiquidCrystal.h>
#include <Ultrasonic.h>

#define Pin 45

Ultrasonic ultrasonic(10,48); // (Trig PIN,Echo PIN)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature
int pantalla1 = 26; 
int pantalla2 = 24;
int luzluna = 28;
int luzuv = 39;
int emergency = 46;
int pushButtonUp = 49;
int pushButtonDown = 47;
int buzzer = 44;
int ventilador = 31;
int calentador = 32;
int bomb1 = 51;
int bomb2 = 52;
int bombper1 = 38;
int bombper2 = 36;
int bombper3 = 41;
int c=1,d,ms,porc,sumporc,warning;
int maxdelay = 2400;
float arrayTemp[20],sum,media;
int numTemp;
boolean readed = false;

int starthp1 = 11;
int startmp1 = 0;
int endhp1 = 20;
int endmp1 = 0;


int timeBombPer1 = 1;
int timeBombPer2 = 26;
int timeBombPer3 = 26;


int starthp2 = 12;
int startmp2 = 0;
int endhp2 = 19;
int endmp2 = 0;


int starthll = 20;
int startmll = 0;
int endhll = 01;
int endmll = 0;

int starthuv = 12;
int startmuv = 0;
int endhuv = 17;
int endmuv = 0;

int nowHour;
int nowMin;
int nowSec;
int nowDay;
int nowWeek;

void setup() {
  lcd.begin(16, 2);
  Wire.begin();
  sensors.begin();
  RTC.begin();
  Serial.begin(9600); 
  lcd.setCursor(0, 1);
  pinMode(pushButtonUp,INPUT);
  pinMode(pushButtonDown,INPUT);
  pinMode(emergency,INPUT);
  pinMode(pantalla1,OUTPUT);
  pinMode(pantalla2,OUTPUT);
  pinMode(luzluna,OUTPUT);
  pinMode(luzuv,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(ventilador,OUTPUT);
  pinMode(calentador,OUTPUT);
  pinMode(bombper1,OUTPUT);
  pinMode(bombper2,OUTPUT);
  pinMode(bombper3,OUTPUT);
  pinMode(bomb1, OUTPUT);
  pinMode(bomb2, OUTPUT);
  
  //RTC.adjust(DateTime(2015,2,1,13,30,00)); // Establece la fecha (año,mes,dia) y ho
}

void loop()
{  
  noTone(buzzer);
  ms = ultrasonic.Timing();
  sensors.requestTemperatures();
  DateTime now = RTC.now();
  arrayTemp[numTemp] = sensors.getTempCByIndex(0);
  nowHour = now.hour();
  nowMin = now.minute();
  nowSec = now.second();
  nowDay = now.day();
  nowWeek = now.dayOfWeek();
  
  if (digitalRead(pushButtonUp) == 1){
    tone(buzzer,5000,100);
    c++;
  }
  if (digitalRead(pushButtonDown)==1){
    tone(buzzer,5000,100);
    c--;
  }
    
   if(ms > maxdelay){
    warning++;
    if(warning>5){
       lcd.clear();
       lcd.print("ALERTA: TANQUE");
       tone(buzzer,500,200);
       delay(1000);
       lcd.noDisplay();
       delay(500);
       lcd.display();
       warning = 0;
    }
  }else if(c == 1){
    sumporc = sumporc + (maxdelay-ms)/24;
    if (d == 10){
      porc = sumporc / 10;
      d = 0;
      sumporc = 0;
    }
    lcd.clear();
    lcd.print("NIVEL AGUA ");
    lcd.print(porc);
    lcd.print("%");
    d++;
  }else if(c == 2){
    sum = sum + arrayTemp[numTemp];
    numTemp++;

    if (numTemp >= 5){
      numTemp = 0;
      media = sum/5;
      lcd.clear();
      lcd.print(media); //Se lee e imprime la temperatura en grados Celsius
      lcd.print(" C");
      media = 0.0;
      sum = 0.0;
    }
  }else if(c==3){
    lcd.clear();
    lcd.print("    ");
    lcd.print(nowHour,DEC);
    lcd.print(":");
    lcd.print(nowMin,DEC);
    lcd.print(":");
    lcd.print(nowSec,DEC);
  }
  
  if (c>3){
    c = 1;
  }else if(c<1){
    c = 3;
  }
  
  digitalWrite(bomb1,LOW);
  
  if((nowHour >= starthp1)&& (nowMin >= startmp1)&&(nowHour < endhp1)){
    digitalWrite(pantalla1,LOW); 
    digitalWrite(bomb2,HIGH);
  }else{
    digitalWrite(pantalla1,HIGH);
    digitalWrite(bomb2,LOW);
  }
    
  if((nowHour >= starthp2)&& (nowMin>= startmp2)&&(nowHour < endhp2))
    digitalWrite(pantalla2,LOW); 
  else
    digitalWrite(pantalla2,HIGH);
    
  if((nowHour >= starthll)&& (nowMin >= startmll) || (nowHour <= endhll))
      digitalWrite(luzluna,LOW);
  else
      digitalWrite(luzluna,HIGH);
      
     
   if (arrayTemp[numTemp] < 25.00){
     digitalWrite(calentador,LOW);
     digitalWrite(ventilador,HIGH);
   }else if (arrayTemp[numTemp]> 26.50 && arrayTemp[numTemp]< 28.00){
     digitalWrite(ventilador,HIGH);
     digitalWrite(calentador,HIGH);
   }else if (arrayTemp[numTemp]> 28.00) {
     digitalWrite(ventilador,LOW);
     digitalWrite(calentador,HIGH);
   }
   
   if((nowHour > starthuv) && (nowHour < endhuv) && (nowDay != 2) && (nowDay != 3) && (nowDay != 16) && (nowDay != 17))
      digitalWrite(luzuv,LOW); 
   else
      digitalWrite(luzuv,HIGH);
  
  if((nowHour == starthp1) && (nowMin == startmp1) && (nowSec < timeBombPer1))
    digitalWrite(bombper1,HIGH);
  else
    digitalWrite(bombper1,LOW);
    
  if((nowWeek == 0 ) && (nowHour == endhp2) && (nowMin == endmp2) && (nowSec < timeBombPer2))
    digitalWrite(bombper2,HIGH);
  else
    digitalWrite(bombper2,LOW);
  
  if(((nowDay == 1) ||(nowDay == 15)) && (nowHour == starthll) && (nowMin == startmll) && (nowSec < timeBombPer3))
     digitalWrite(bombper3,HIGH);
  else
    digitalWrite(bombper3,LOW);
    
  delay(100);
  
}
