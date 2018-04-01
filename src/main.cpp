#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <Snake.h>

long countr=0;
long lastmove=0;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
Snake mysnake = Snake(8,8);

void setup() {
  //control button connections (avoid 0,1 since serial comms may autofire buttons)
  pinMode (2,INPUT_PULLUP);
  pinMode (3,INPUT_PULLUP);
  pinMode (4,INPUT_PULLUP);
  pinMode (5,INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Snake Started");
  matrix.begin(0x70);  // pass in the address
  matrix.clear();
  matrix.drawRect(0,0, 8,8, LED_ON);
  matrix.drawRect(2,2, 4,4, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay (1000);
  matrix.clear();
  matrix.writeDisplay();  // write the changes we just made to the display
}
//smiles
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B00000000,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 };

void loop() {
  countr++;
  if (digitalRead(2)==LOW) {
    mysnake.setDirection('u');
    Serial.println("U");
  }
  if (digitalRead(3)==LOW) {
    mysnake.setDirection('l');
    Serial.println("L");
  }
  if (digitalRead(4)==LOW) {
    mysnake.setDirection('r');
    Serial.println("R");
  }
  if (digitalRead(5)==LOW) {
    mysnake.setDirection('d');
    Serial.println("D");
  }

  if (!mysnake.gameover) {
    if ((millis()-lastmove)>100) {
          matrix.drawPixel(mysnake.applex, mysnake.appley, LED_ON);
          matrix.drawPixel(mysnake.posx, mysnake.posy, LED_ON);
          matrix.drawPixel(mysnake.todelx, mysnake.todely, LED_OFF);
          mysnake.nextmove();
          lastmove=millis();
    }
  }
  else
  {
    matrix.clear();
    matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
    delay (1000);

    for (int8_t x=7; x>=-60; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
        matrix.print("Score:"+(String)mysnake.length);
        matrix.writeDisplay();
        delay(100);
    }
    delay(1000);
    countr=0;
  }
  matrix.writeDisplay();
}
