/*
Snake.h Library. used to play snake on simple led matrix displays
*/
#ifndef Snake_h
#define Snake_h

#include "Arduino.h"

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
  uint16_t *buffer;
  char _direction;
};

#endif
