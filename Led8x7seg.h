#ifndef Led8x7seg_h
#define Led8x7seg_h

#include <Arduino.h>


typedef enum Characters
{
  Character_Zero  = 0x00,
  Character_One   = 0x01,
  Character_Two   = 0x02,
  Character_Three = 0x03,
  Character_Four  = 0x04,
  Character_Five  = 0x05,
  Character_Six   = 0x06,
  Character_Seven = 0x07,
  Character_Eight = 0x08,
  Character_Nine  = 0x09,
  Character_Dash  = 0x0A,
  Character_E     = 0x0B,
  Character_H     = 0x0C,
  Character_L     = 0x0D,
  Character_P     = 0x0E,
  Character_Blank = 0x0F
} Character_t;
class Led8x7seg {
  private:
    int m_DataPin, m_LoadPin, m_ClockPin;

    byte getNumFont(int Num);
    void putMax7219Byte(byte data);
    void putMax7219Reg( byte reg, byte col);
  public:
    Led8x7seg(int DataPin, int LoadPin, int ClockPin);
    void setBrightness(byte Brightness); //0~15 
    void ShowNum(int Pos, byte Num);
    void ShowInt(long Num);
    void ShowFloat(float Num);
};
#endif
