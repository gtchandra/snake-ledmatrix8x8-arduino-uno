#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>

volatile bool interr=false;

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
long countr=0;
long lastclick=0;
long lastmove=0;
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();
Snake mysnake = Snake(8,8);

void changeDirection() {
  interr=true;
}

void setup() {
  pinMode (2,INPUT_PULLUP);
  pinMode (3,INPUT_PULLUP);
  pinMode (4,INPUT_PULLUP);
  pinMode (5,INPUT_PULLUP);
  //attachInterrupt(0,changeDirection,FALLING); // interrupt 0 is connected to PIN 2
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
  //randomSeed(analogRead(0));

}
//smiles
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10111101,
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
  if ((_direction=='l' and dir=='r') or (_direction=='r' and dir=='l')) return;
  if ((_direction=='u' and dir=='d') or (_direction=='d' and dir=='u')) return;
  _direction=dir;
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
  //RANDOM MODE
  int ran2=random(3)+1;
  if (countr%ran2==0) {
    int ran=random(4);
    mysnake.setDirection(moves[ran]);
  }
*/
  //da fare circuito antirimbalzo a 4 pulsanti
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
    //Serial.println("posx: "+(String)mysnake.posx+" posy:"+(String)mysnake.posy);
    //Serial.println("Appx: "+(String)mysnake.applex+" Appy:"+(String)mysnake.appley);

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


    Serial.println("game over cycle");
    countr=0;
  }
    //
  matrix.writeDisplay();

}
