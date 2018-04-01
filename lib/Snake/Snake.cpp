/*
Snake.cpp Library implementation
Created by Gabriele Tazzari 
*/

#include "Arduino.h"
#include "Snake.h"

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
  _direction='r'; //initial direction
  uint16_t bytes = w*h;
  if((buffer = (uint8_t *)malloc(bytes))) {
      memset(buffer, 0, bytes);
  }
  buffer[0]=posx+posy*_width; //set starting position
}
void Snake::setDirection(char dir) {
  //this avoid direction inversion
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
  //check target apple hit and create a new apple
  if (currentpos==(applex+appley*_width))
  {
    length++;
    applex=random(_width);
    appley=random(_height);
  }
  //check collision and move snake queue
  for (int i=length-1;i>0;i--) {
    buffer[i]=buffer[i-1];
    if (currentpos==buffer[i])
    {
      gameover=true;
      return;
    }
  }
  todelx=buffer[length-1] % _width;
  todely=buffer[length-1] /  _width;
  //delete last pixel at the end of cycle
  buffer[length-1]=0;
  //remove last pixel from array
  buffer[0]=currentpos; //base point set
  //insert new position into array head
}
Snake::~Snake(void) {
  if (buffer) free (buffer);
}
