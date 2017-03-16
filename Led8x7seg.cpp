#include <avr/pgmspace.h>
#include "Led8x7seg.h"


#define maxReg maxBitmapLine
// define max7219 registers
#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

typedef enum DecodeModes
{
  DecodeMode_NoDecode     = 0x00,
  DecodeMode_Digit0       = 0x01,
  DecodeMode_Digit1       = 0x02,
  DecodeMode_Digit2       = 0x04,
  DecodeMode_Digit3       = 0x08,
  DecodeMode_Digit4       = 0x10,
  DecodeMode_Digit5       = 0x20,
  DecodeMode_Digit6       = 0x40,
  DecodeMode_Digit7       = 0x80,
  DecodeMode_AllDigits    = 0xFF
} DecodeMode_t;

Led8x7seg::Led8x7seg(int DataPin, int LoadPin, int ClockPin)
  : m_DataPin(DataPin), m_LoadPin(LoadPin), m_ClockPin(ClockPin)
{
  pinMode(DataPin, OUTPUT);
  pinMode(LoadPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);

  //initiation of the max 7219

  putMax7219Reg(max7219_reg_scanLimit, 0x07);
  putMax7219Reg(max7219_reg_decodeMode, DecodeMode_AllDigits);  // using an led matrix (not digits)
  putMax7219Reg(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  putMax7219Reg(max7219_reg_displayTest, 0x00); // no display test
  for (int e = max7219_reg_digit0; e <= 8; e++)
  { // empty registers, turn all LEDs off
    putMax7219Reg(e, 0);
  }
  setBrightness(7);
  // range: 0x00 to 0x0f
}

void Led8x7seg::setBrightness(byte Brightness)
{
	putMax7219Reg(max7219_reg_intensity, Brightness & 0x0f); 
}


void Led8x7seg::putMax7219Byte(byte data)
{
  for (int i = 0; i < 8; i++, data <<= 1)
  {
    digitalWrite(m_ClockPin, LOW);   // tick
    digitalWrite(m_DataPin, (data & 0x80) ? HIGH : LOW); // send 1
    digitalWrite(m_ClockPin, HIGH);   // tock
  }
}

void Led8x7seg::putMax7219Reg( byte reg, byte col)
{
  //maxSingle is the "easy"  function to use for a single max7219

  digitalWrite(m_LoadPin, LOW);       // begin
  putMax7219Byte(reg);                  // specify register
  putMax7219Byte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(m_LoadPin, LOW);       // and load da stuff
  digitalWrite(m_LoadPin, HIGH);
}

void Led8x7seg::ShowNum(int Pos, byte Num)
{
  putMax7219Reg(Pos, Num);
}
void Led8x7seg::ShowInt(long Num)
{
  long n;
  int i;
  bool sign = Num < 0;
  if(sign)
  	n=-Num;
  else
  	n=Num;	
  for (i = 0; i < 8; i++)
  {
    byte d;
    if (n)
    {
      d = n % 10;
      n /= 10;
    }
    else
    {
      if (sign)
      {
        d = Character_Dash;
        sign = false;
      }
      else
        d = Character_Blank;
    }
    putMax7219Reg(i + 1, d);
  }
}
void Led8x7seg::ShowFloat(float Num)
{
  char buf[20];
  int len, i;
  dtostrf(Num, 8, 8, buf);
  len = strlen(buf);
  for (i = 0; i < 8 && len >0 ; i++)
  {
    if (buf[len - 1] == '0')
    {
      buf[len - 1] = 0;
      len--;
    }
    else
      break;
  }
  if (buf[len - 1] == '.')
  {
    buf[len - 1] = 0;
    len--;
    if (len > 8)
    {
      buf[8] = 0;
      len = 8;
    }

  }
  else
  {
    if (len > 9)
    {
      buf[9] = 0;
      len = 9;
      for (i = 0; i < 8 && len >0 ; i++)
  		{
    		if (buf[len - 1] == '0')
    		{
      		buf[len - 1] = 0;
      		len--;
    		}
    		else
      		break;
  		}
    }

  }
  

  for (i = 0; i < 8; i++)
  {
    byte d;
    if (len)
    {
      bool needDot = false;
      d = buf[len - 1];
      if (d == '.')
      {
        len--;
        d = buf[len - 1];
        needDot = true;
      }
      len--;
      if (d >= '0' && d <= '9')
      {
        d -= '0';
      }
      else if (d == '-')
        d = Character_Dash;
      if (needDot)
        d |= 0x80;
    }
    else
      d = Character_Blank;
    putMax7219Reg(i + 1, d);
  }
}