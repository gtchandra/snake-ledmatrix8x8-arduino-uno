#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <Snake.h>

long countr=0;
long lastmove=0;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
Snake mysnake = Snake(8,8);
float th_low = 200;
float th_high = 600;
int analog0;
int analog1;

void setup() {
  //control using analog joystick on A0 / A1 pins (hi-low thresholds)
  pinMode (A0,INPUT);
  pinMode (A1,INPUT);
  pinMode (PD3,OUTPUT); //pin used to shutdown power
  digitalWrite(PD3,HIGH);
  matrix.setRotation(1); //ROTATION needed in new design board
  matrix.setBrightness(1);
  Serial.begin(9600);
  Serial.println("Snake Started");
  matrix.begin(0x70);  // pass in the address
  matrix.clear();
  for (int i=3;i>=0;i--) {
    matrix.drawRect(0+i,0+i, 8-i*2,8-i*2, LED_ON);
    matrix.writeDisplay();  // write the changes we just made to the display
    delay(100);
    matrix.drawRect(0+i,0+i, 8-i*2,8-i*2, LED_OFF);
    matrix.writeDisplay();  // write the changes we just made to the display
  }
  for (int i=0;i<5;i++){
    matrix.drawRect(0,0, 8,8, LED_ON);
    matrix.writeDisplay();
    delay(80);
    matrix.drawRect(0,0, 8,8, LED_OFF);
    matrix.writeDisplay();
    delay(100);
  }

  matrix.clear();
  matrix.writeDisplay();  // write the changes we just made to the display
  countr=millis();
}

void loop() {
  analog0=analogRead(A0);
  analog1=analogRead(A1);
  if (analog0>th_high) {
    delay(100);
    mysnake.setDirection('u');
    countr=millis();
  }
  if (analog0<th_low) {
    delay(100);
    mysnake.setDirection('d');
    countr=millis();
  }
  if (analog1>th_high) {
    delay(100);
    mysnake.setDirection('r');
    countr=millis();
  }
  if (analog1<th_low) {
    delay(100);
    mysnake.setDirection('l');
    countr=millis();
  }
  if ((millis()-countr)<20000) {
        if ((millis()-lastmove)>100) {
              matrix.drawPixel(mysnake.applex, mysnake.appley, LED_ON);
              matrix.drawPixel(mysnake.posx, mysnake.posy, LED_ON);
              matrix.drawPixel(mysnake.todelx, mysnake.todely, LED_OFF);
              mysnake.nextmove();
              lastmove=millis();
              matrix.writeDisplay();
              }
       if (mysnake.gameover) {
            //game over sequence
            for(int zz=0;zz<10;zz++) {
              matrix.drawPixel(mysnake.posx, mysnake.posy, LED_OFF);
              delay(150);
              matrix.writeDisplay();
              matrix.drawPixel(mysnake.posx, mysnake.posy, LED_ON);
              delay(50);
              matrix.writeDisplay();
            }
             delay (1000);
             matrix.setTextWrap(false);
             for (int8_t x=7; x>=-41; x--) {
                 matrix.clear();
                 matrix.setCursor(x,0);
                 matrix.print("Score:"+(String)mysnake.length);
                 matrix.writeDisplay();
                 delay(100);
             }
             delay(1000);
             countr=0;
           }

    }
    else
      {
          matrix.clear();
          for (int i=0;i<5;i++){
            matrix.drawRect(0,0, 8,8, LED_ON);
            matrix.writeDisplay();
            delay(80);
            matrix.drawRect(0,0, 8,8, LED_OFF);
            matrix.writeDisplay();
            delay(100);
          }
          for (int i=0;i<4;i++) {
            matrix.drawRect(0+i,0+i, 8-i*2,8-i*2, LED_ON);
            matrix.writeDisplay();  // write the changes we just made to the display
            delay(50);
            matrix.drawRect(0+i,0+i, 8-i*2,8-i*2, LED_OFF);
            matrix.writeDisplay();  // write the changes we just made to the display
          }
          matrix.writeDisplay();
          delay(1000);
          digitalWrite(PD3,LOW); //shutdown
          delay(1000);
          digitalWrite(PD3,HIGH);
  }
}
