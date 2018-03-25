

/***************************************************
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks
  ----> http://www.adafruit.com/products/872
  ----> http://www.adafruit.com/products/871
  ----> http://www.adafruit.com/products/870

  These displays use I2C to communicate, 2 pins are required to
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>

class Snake {
 public:
  Snake(uint16_t w, uint16_t h);
  ~Snake(void);
  void setDirection(char dir);
  char getDirection();
  void nextmove ();
  bool gameover;
  int16_t posx;
  int16_t posy;
  int16_t applex;
  int16_t appley;
  int16_t todelx;
  int16_t todely;
  int16_t length;
 private:
  uint16_t _width;
  uint16_t _height;
  uint8_t *buffer;
  char _direction;
};

const char moves[]="ulrd";
int countr=0;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
Snake mysnake = Snake(8,8);

void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");
  matrix.begin(0x70);  // pass in the address
  matrix.clear();
  matrix.drawRect(0,0, 8,8, LED_ON);
  matrix.drawRect(2,2, 4,4, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay (1000);
  matrix.clear();
  matrix.writeDisplay();  // write the changes we just made to the display
  randomSeed(analogRead(0));
  pinMode (2,INPUT_PULLUP);
  pinMode (3,INPUT_PULLUP);
}

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  unic_bmp[] =
  { B00001000,
    B00101010,
    B00111110,
    B00011100,
    B00111110,
    B00111110,
    B00111110,
    B00011100 };

uint8_t l[64];




Snake::Snake (uint16_t w, uint16_t h)
{
  _width=w;
  _height=h;
  gameover=0;
  length=2;
  posx=4;   //initial snake position
  posy=4;
  applex=0; //initial apple position
  appley=4;
  todelx=0;
  todely=0;
  _direction='r';
  uint16_t bytes = w*h;
  if((buffer = (uint8_t *)malloc(bytes))) {
      memset(buffer, 0, bytes);
  }
  buffer[0]=posx+posy*_width; //set starting position
}
void Snake::setDirection(char dir) {
  //this avoid inversion of direction
  //if ((_direction=='l' and dir=='r') or (_direction=='r' and dir=='l')) return;
  //if ((_direction=='u' and dir=='d') or (_direction=='d' and dir=='u')) return;
  if (_direction=='r' and dir=='r') _direction='d';
  if (_direction=='d' and dir=='r') _direction='l';
  if (_direction=='u' and dir=='r') _direction='r';
  if (_direction=='l' and dir=='r') _direction='u';

  //_direction=dir;
}
char Snake::getDirection() {
      return _direction;
}
void Snake::nextmove()
{
  switch (_direction) {
    //compute next position
    case 'r':
    posx=((posx+1) % _width);
    break;
    case 'l':
    posx=(posx-1) % _width;
    break;
    case 'u':
    posy=(posy-1) % _height;
    break;
    case 'd':
    posy=(posy+1) % _height;
    break;
  }

  int16_t currentpos=posx+posy*_width;
  //update pixel to delete
  if ((mysnake.applex==mysnake.posx) and (mysnake.appley==mysnake.posy))
  {
    Serial.println("Apple target: HIT");
  }
  if (currentpos==(applex+appley*_width))
  {
    length++;
    applex=random(_width);
    appley=random(_height);
  }
  //check collision and move queue
  for (int i=length-1;i>0;i--) {
    buffer[i]=buffer[i-1];
    if (currentpos==buffer[i])
    {
      Serial.println("game over");
      Serial.println("posx: "+(String)mysnake.posx+" posy:"+(String)mysnake.posy);
      Serial.println("Appx: "+(String)mysnake.applex+" Appy:"+(String)mysnake.appley);
      gameover=true;
      return;
    }
  }
  todelx=buffer[length-1] % _width;
  todely=buffer[length-1] /  _width;
  buffer[length-1]=0;
  buffer[0]=currentpos; //base point set
  //Serial.println("todelx="+(String)todelx+" todely="+(String)todely);
}

Snake::~Snake(void) {
  if (buffer) free (buffer);
}

void loop() {
  countr++;
  /*
  int ran2=random(3)+1;
  if (countr%ran2==0) {
    int ran=random(4);
    mysnake.setDirection(moves[ran]);
  }
*/
  //da fare circuito antirimbalzo a 4 pulsanti
  if (digitalRead(2)==LOW) {
    mysnake.setDirection('l');
  }
  if (digitalRead(3)==LOW) {
    mysnake.setDirection('r');
  }

  if (!mysnake.gameover) {
    Serial.println("posx: "+(String)mysnake.posx+" posy:"+(String)mysnake.posy);
    Serial.println("Appx: "+(String)mysnake.applex+" Appy:"+(String)mysnake.appley);

    matrix.drawPixel(mysnake.applex, mysnake.appley, LED_ON);
    matrix.drawPixel(mysnake.posx, mysnake.posy, LED_ON);
    matrix.drawPixel(mysnake.todelx, mysnake.todely, LED_OFF);
    mysnake.nextmove();

  }
  else
  {
    delay (5000);
    Serial.println("game over cycle");
    countr=0;
    matrix.clear();
    randomSeed(analogRead(0));
    Snake mysnake = Snake(8,8);
  }
    //matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(150);

/*
  matrix.clear();      // clear display
  matrix.drawPixel(0, 0, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.clear();
  matrix.drawLine(0,0, 7,7, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.clear();
  matrix.drawRect(0,0, 8,8, LED_ON);
  matrix.fillRect(2,2, 4,4, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.clear();
  matrix.drawCircle(3,3, 3, LED_ON);
  matrix.writeDisplay();  // write the changes we just made to the display
  delay(500);

  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  for (int8_t x=0; x>=-36; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Hello");
    matrix.writeDisplay();
    delay(40);
  }
  //matrix.setRotation(3);
  for (int8_t x=7; x>=-36; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("World");
    matrix.writeDisplay();
    delay(40);
  }
  matrix.setRotation(0);
  */
}
