#include "font.h"

static char ascii_rus_decode(char ansii)
{
  if(ansii >= 0xC0 && ansii <= 0xFF)
  {
    switch(ansii)
    {
      case 0xC0 : return 0x41;
      case 0xC1 : return 0xA0;
      case 0xB3 : return 0x69;
      case 0xC2 : return 0x42;
      case 0xC3 : return 0xA1;
      case 0xC4 : return 0xE0;
      case 0xC5 : return 0x45;
      case 0xC6 : return 0xA3;
      case 0xC7 : return 0xA4;
      case 0xC8 : return 0xA5;
      case 0xC9 : return 0xA6;
      case 0xCA : return 0x4B;
      case 0xCB : return 0xA7;
      case 0xCC : return 0x4D;
      case 0xCD : return 0x48;
      case 0xCE : return 0x4F;
      case 0xCF : return 0xA8;
      case 0xD0 : return 0x50;
      case 0xD1 : return 0x43;
      case 0xD2 : return 0x54;
      case 0xD3 : return 0xA9;
      case 0xD4 : return 0xAA;
      case 0xD5 : return 0x58;
      case 0xD6 : return 0xE1;
      case 0xD7 : return 0xAB;
      case 0xD8 : return 0xAC;
      case 0xD9 : return 0xE2;
      case 0xDA : return 0xAD;
      case 0xDB : return 0xAE;
      case 0xDC : return 0xC4;
      case 0xDD : return 0xAF;
      case 0xDE : return 0xB0;
      case 0xDF : return 0xB1;
      case 0xE0 : return 0x61;
      case 0xE1 : return 0xB2;
      case 0xE2 : return 0xB3;
      case 0xE3 : return 0xB4;
      case 0xE4 : return 0xE3;
      case 0xE5 : return 0x65;
      case 0xE6 : return 0xB6;
      case 0xE7 : return 0xB7;
      case 0xE9 : return 0xB9;
      case 0xEA : return 0xBA;
      case 0xEB : return 0xBB;
      case 0xEC : return 0xBC;
      case 0xED : return 0xBD;
      case 0xEE : return 0x6F;
      case 0xEF : return 0xBE;
      case 0xF0 : return 0x70;
      case 0xF1 : return 0x63;
      case 0xF2 : return 0xBF;
      case 0xF3 : return 0x79;
      case 0xF4 : return 0xE4;
      case 0xF5 : return 0x78;
      case 0xF6 : return 0xE5;
      case 0xF7 : return 0xC0;
      case 0xF8 : return 0xC1;
      case 0xF9 : return 0xE6;
      case 0xFA : return 0xC2;
      case 0xFB : return 0xC3;
      case 0xFC : return 0xC4;
      case 0xFD : return 0xC5;
      case 0xFE : return 0xC6;
      case 0xFF : return 0xC7;
    }
  }
  return ansii;
}


uint8_t font_get(char addr, uint8_t n)
{
  uint8_t fnt[7];
  addr = ascii_rus_decode(addr);
  switch(addr)
  {
    case 0x00 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x30 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b110010;
      fnt[3] = 0b101010;
      fnt[4] = 0b100110;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case '%' :
      fnt[0] = 0b000110;
      fnt[1] = 0b100110;
      fnt[2] = 0b010000;
      fnt[3] = 0b001000;
      fnt[4] = 0b000100;
      fnt[5] = 0b110010;
      fnt[6] = 0b110000;
      break;
    case '/' :
      fnt[0] = 0b000000;
      fnt[1] = 0b100000;
      fnt[2] = 0b010000;
      fnt[3] = 0b001000;
      fnt[4] = 0b000100;
      fnt[5] = 0b000010;
      fnt[6] = 0b000000;
      break;
    case '\\' :
      fnt[0] = 0b000000;
      fnt[1] = 0b000010;
      fnt[2] = 0b000100;
      fnt[3] = 0b001000;
      fnt[4] = 0b010000;
      fnt[5] = 0b100000;
      fnt[6] = 0b000000;
      break;
    case 0x31 :
      fnt[0] = 0b001000;
      fnt[1] = 0b001100;
      fnt[2] = 0b001000;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b011100;
      break;
    case 0x32 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100000;
      fnt[3] = 0b010000;
      fnt[4] = 0b001000;
      fnt[5] = 0b000100;
      fnt[6] = 0b111110;
      break;
    case 0x33 :
      fnt[0] = 0b111110;
      fnt[1] = 0b010000;
      fnt[2] = 0b001000;
      fnt[3] = 0b010000;
      fnt[4] = 0b100000;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x34 :
      fnt[0] = 0b010000;
      fnt[1] = 0b011000;
      fnt[2] = 0b010100;
      fnt[3] = 0b010010;
      fnt[4] = 0b111110;
      fnt[5] = 0b010000;
      fnt[6] = 0b010000;
      break;
    case 0x35 :
      fnt[0] = 0b111110;
      fnt[1] = 0b000010;
      fnt[2] = 0b011110;
      fnt[3] = 0b100000;
      fnt[4] = 0b100000;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x36 :
      fnt[0] = 0b011000;
      fnt[1] = 0b000100;
      fnt[2] = 0b000010;
      fnt[3] = 0b011110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x37 :
      fnt[0] = 0b111110;
      fnt[1] = 0b100000;
      fnt[2] = 0b010000;
      fnt[3] = 0b001000;
      fnt[4] = 0b000100;
      fnt[5] = 0b000100;
      fnt[6] = 0b000100;
      break;
    case 0x38 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b011100;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x39 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b111100;
      fnt[4] = 0b100000;
      fnt[5] = 0b100000;
      fnt[6] = 0b011100;
      break;
    case 0x20 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x41 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b111110;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x42 :
      fnt[0] = 0b011110;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b011110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011110;
      break;
    case 0x43 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b000010;
      fnt[3] = 0b000010;
      fnt[4] = 0b000010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x44 :
      fnt[0] = 0b001110;
      fnt[1] = 0b010010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b010010;
      fnt[6] = 0b001110;
      break;
    case 0x45 :
      fnt[0] = 0b111110;
      fnt[1] = 0b000010;
      fnt[2] = 0b000010;
      fnt[3] = 0b111110;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b111110;
      break;
    case 0x46 :
      fnt[0] = 0b011111;
      fnt[1] = 0b000010;
      fnt[2] = 0b000010;
      fnt[3] = 0b011110;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0x47 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b000010;
      fnt[3] = 0b111010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b111100;
      break;
    case 0x48 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b111110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x49 :
      fnt[0] = 0b011100;
      fnt[1] = 0b001000;
      fnt[2] = 0b001000;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b011100;
      break;
    case 0x4A :
      fnt[0] = 0b111000;
      fnt[1] = 0b010000;
      fnt[2] = 0b010000;
      fnt[3] = 0b010000;
      fnt[4] = 0b010000;
      fnt[5] = 0b010010;
      fnt[6] = 0b001100;
      break;
    case 0x4B :
      fnt[0] = 0b100010;
      fnt[1] = 0b010010;
      fnt[2] = 0b001010;
      fnt[3] = 0b000110;
      fnt[4] = 0b001010;
      fnt[5] = 0b010010;
      fnt[6] = 0b100010;
      break;
    case 0x4C :
      fnt[0] = 0b000010;
      fnt[1] = 0b000010;
      fnt[2] = 0b000010;
      fnt[3] = 0b000010;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b111110;
      break;
    case 0x4D :
      fnt[0] = 0b100010;
      fnt[1] = 0b110110;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x4E :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100110;
      fnt[3] = 0b101010;
      fnt[4] = 0b110010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x4F :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x50 :
      fnt[0] = 0b011110;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b011110;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0x51 :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b101010;
      fnt[5] = 0b010010;
      fnt[6] = 0b101100;
      break;
    case 0x52 :
      fnt[0] = 0b011110;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b011110;
      fnt[4] = 0b001010;
      fnt[5] = 0b010010;
      fnt[6] = 0b100010;
      break;
    case 0x53 :
      fnt[0] = 0b111100;
      fnt[1] = 0b000010;
      fnt[2] = 0b000010;
      fnt[3] = 0b011100;
      fnt[4] = 0b100000;
      fnt[5] = 0b100000;
      fnt[6] = 0b011110;
      break;
    case 0x54 :
      fnt[0] = 0b111110;
      fnt[1] = 0b001000;
      fnt[2] = 0b001000;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b001000;
      break;
    case 0x55 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x56 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b010100;
      fnt[6] = 0b001000;
      break;
    case 0x57 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b010100;
      break;
    case 0x58 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b010100;
      fnt[3] = 0b001000;
      fnt[4] = 0b010100;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x59 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b010100;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b001000;
      break;
    case 0x5A :
      fnt[0] = 0b111110;
      fnt[1] = 0b100000;
      fnt[2] = 0b010000;
      fnt[3] = 0b001000;
      fnt[4] = 0b000100;
      fnt[5] = 0b000010;
      fnt[6] = 0b111110;
      break;
    case 0x61 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100000;
      fnt[4] = 0b111100;
      fnt[5] = 0b100010;
      fnt[6] = 0b111100;
      break;
    case 0x62 :
      fnt[0] = 0b000010;
      fnt[1] = 0b000010;
      fnt[2] = 0b011010;
      fnt[3] = 0b100110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011110;
      break;
    case 0x63 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b000010;
      fnt[4] = 0b000010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x64 :
      fnt[0] = 0b100000;
      fnt[1] = 0b100000;
      fnt[2] = 0b101100;
      fnt[3] = 0b110010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b111100;
      break;
    case 0x65 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100010;
      fnt[4] = 0b111110;
      fnt[5] = 0b000010;
      fnt[6] = 0b011100;
      break;
    case 0xB8 :
      fnt[0] = 0b010100;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100010;
      fnt[4] = 0b111110;
      fnt[5] = 0b000010;
      fnt[6] = 0b011100;
      break;
    case 0x66 :
      fnt[0] = 0b011000;
      fnt[1] = 0b100100;
      fnt[2] = 0b000100;
      fnt[3] = 0b001110;
      fnt[4] = 0b000100;
      fnt[5] = 0b000100;
      fnt[6] = 0b000100;
      break;
    case 0x67 :
      fnt[0] = 0b000000;
      fnt[1] = 0b111100;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b111100;
      fnt[5] = 0b100000;
      fnt[6] = 0b011100;
      break;
    case 0x68 :
      fnt[0] = 0b000010;
      fnt[1] = 0b000010;
      fnt[2] = 0b011010;
      fnt[3] = 0b100110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x69 :
      fnt[0] = 0b001000;
      fnt[1] = 0b000000;
      fnt[2] = 0b001100;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b011100;
      break;
    case 0x6A :
      fnt[0] = 0b010000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011000;
      fnt[3] = 0b010000;
      fnt[4] = 0b010000;
      fnt[5] = 0b010010;
      fnt[6] = 0b001100;
      break;
    case 0x6B :
      fnt[0] = 0b000010;
      fnt[1] = 0b000010;
      fnt[2] = 0b010010;
      fnt[3] = 0b001010;
      fnt[4] = 0b000110;
      fnt[5] = 0b001010;
      fnt[6] = 0b010010;
      break;
    case 0x6C :
      fnt[0] = 0b001100;
      fnt[1] = 0b001000;
      fnt[2] = 0b001000;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b011100;
      break;
    case 0x6D :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b010110;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x6E :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011010;
      fnt[3] = 0b100110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0x6F :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0x70 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011110;
      fnt[3] = 0b100010;
      fnt[4] = 0b011110;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0x71 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b101100;
      fnt[3] = 0b110010;
      fnt[4] = 0b111100;
      fnt[5] = 0b100000;
      fnt[6] = 0b100000;
      break;
    case 0x72 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011010;
      fnt[3] = 0b100110;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0x73 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b000010;
      fnt[4] = 0b011100;
      fnt[5] = 0b100000;
      fnt[6] = 0b011100;
      break;
    case 0x74 :
      fnt[0] = 0b000100;
      fnt[1] = 0b000100;
      fnt[2] = 0b001110;
      fnt[3] = 0b000100;
      fnt[4] = 0b000100;
      fnt[5] = 0b100100;
      fnt[6] = 0b011000;
      break;
    case 0x75 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b110010;
      fnt[6] = 0b101100;
      break;
    case 0x76 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b010100;
      fnt[6] = 0b001000;
      break;
    case 0x77 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b010100;
      break;
    case 0x78 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b010100;
      fnt[4] = 0b001000;
      fnt[5] = 0b010100;
      fnt[6] = 0b100010;
      break;
    case 0x79 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b111100;
      fnt[5] = 0b100000;
      fnt[6] = 0b011100;
      break;
    case 0x7A :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111110;
      fnt[3] = 0b010000;
      fnt[4] = 0b001000;
      fnt[5] = 0b000100;
      fnt[6] = 0b111110;
      break;
    case 0xA0 :
      fnt[0] = 0b111110;
      fnt[1] = 0b100010;
      fnt[2] = 0b000010;
      fnt[3] = 0b011110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011110;
      break;
    case 0xA1 :
      fnt[0] = 0b111110;
      fnt[1] = 0b100010;
      fnt[2] = 0b000010;
      fnt[3] = 0b000010;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0xA2 :
      fnt[0] = 0b010100;
      fnt[1] = 0b000000;
      fnt[2] = 0b011110;
      fnt[3] = 0b000010;
      fnt[4] = 0b001110;
      fnt[5] = 0b000010;
      fnt[6] = 0b011110;
      break;
    case 0xA3 :
      fnt[0] = 0b101010;
      fnt[1] = 0b101010;
      fnt[2] = 0b101010;
      fnt[3] = 0b011100;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b101010;
      break;
    case 0xA4 :
      fnt[0] = 0b011110;
      fnt[1] = 0b100000;
      fnt[2] = 0b100000;
      fnt[3] = 0b111100;
      fnt[4] = 0b100000;
      fnt[5] = 0b100000;
      fnt[6] = 0b011110;
      break;
    case 0xA5 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b110010;
      fnt[3] = 0b101010;
      fnt[4] = 0b100110;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0xA6 :
      fnt[0] = 0b010100;
      fnt[1] = 0b001000;
      fnt[2] = 0b100010;
      fnt[3] = 0b110010;
      fnt[4] = 0b101010;
      fnt[5] = 0b100110;
      fnt[6] = 0b100010;
      break;
    case 0xA7 :
      fnt[0] = 0b111100;
      fnt[1] = 0b101000;
      fnt[2] = 0b101000;
      fnt[3] = 0b101000;
      fnt[4] = 0b101000;
      fnt[5] = 0b101010;
      fnt[6] = 0b100100;
      break;
    case 0xA8 :
      fnt[0] = 0b111110;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0xA9 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b010100;
      fnt[4] = 0b001000;
      fnt[5] = 0b000100;
      fnt[6] = 0b000010;
      break;
    case 0xAA :
      fnt[0] = 0b001000;
      fnt[1] = 0b011100;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b011100;
      fnt[6] = 0b001000;
      break;
    case 0xAB :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b111100;
      fnt[4] = 0b100000;
      fnt[5] = 0b100000;
      fnt[6] = 0b100000;
      break;
    case 0xAC :
      fnt[0] = 0b101010;
      fnt[1] = 0b101010;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b111110;
      break;
    case 0xAD :
      fnt[0] = 0b000110;
      fnt[1] = 0b000100;
      fnt[2] = 0b000100;
      fnt[3] = 0b011100;
      fnt[4] = 0b100100;
      fnt[5] = 0b100100;
      fnt[6] = 0b011100;
      break;
    case 0xAE :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100110;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b100110;
      break;
    case 0xAF :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100000;
      fnt[3] = 0b111000;
      fnt[4] = 0b100000;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0xB0 :
      fnt[0] = 0b010010;
      fnt[1] = 0b101010;
      fnt[2] = 0b101010;
      fnt[3] = 0b101110;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b010010;
      break;
    case 0xB1 :
      fnt[0] = 0b111100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b111100;
      fnt[4] = 0b101000;
      fnt[5] = 0b100100;
      fnt[6] = 0b100010;
      break;
    case 0xB2 :
      fnt[0] = 0b110000;
      fnt[1] = 0b001100;
      fnt[2] = 0b000010;
      fnt[3] = 0b011110;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0xB3 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b001110;
      fnt[3] = 0b010010;
      fnt[4] = 0b001110;
      fnt[5] = 0b010010;
      fnt[6] = 0b001110;
      break;
    case 0xB4 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111110;
      fnt[3] = 0b100010;
      fnt[4] = 0b000010;
      fnt[5] = 0b000010;
      fnt[6] = 0b000010;
      break;
    case 0xB5 :
      fnt[0] = 0b010100;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100010;
      fnt[4] = 0b111110;
      fnt[5] = 0b000010;
      fnt[6] = 0b011100;
      break;
    case 0xB6 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b011100;
      fnt[5] = 0b101010;
      fnt[6] = 0b101010;
      break;
    case 0xB7 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011110;
      fnt[3] = 0b100000;
      fnt[4] = 0b011000;
      fnt[5] = 0b100000;
      fnt[6] = 0b011110;
      break;
    case 0xE8 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b110010;
      fnt[4] = 0b101010;
      fnt[5] = 0b100110;
      fnt[6] = 0b100010;
      break;
    case 0xB9 :
      fnt[0] = 0b000000;
      fnt[1] = 0b010100;
      fnt[2] = 0b001000;
      fnt[3] = 0b100010;
      fnt[4] = 0b110010;
      fnt[5] = 0b101010;
      fnt[6] = 0b100110;
      break;
    case 0xBA :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b010010;
      fnt[3] = 0b001010;
      fnt[4] = 0b000110;
      fnt[5] = 0b001010;
      fnt[6] = 0b010010;
      break;
    case 0xBB :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111100;
      fnt[3] = 0b101000;
      fnt[4] = 0b101000;
      fnt[5] = 0b101010;
      fnt[6] = 0b100100;
      break;
    case 0xBC :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b110110;
      fnt[4] = 0b101010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0xBD :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b111110;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0xBE :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111110;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b100010;
      fnt[6] = 0b100010;
      break;
    case 0xBF :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111110;
      fnt[3] = 0b001000;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b001000;
      break;
    case 0xC0 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b111100;
      fnt[5] = 0b100000;
      fnt[6] = 0b100000;
      break;
    case 0xC1 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b101010;
      fnt[6] = 0b111110;
      break;
    case 0xC2 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000110;
      fnt[3] = 0b000100;
      fnt[4] = 0b011100;
      fnt[5] = 0b100100;
      fnt[6] = 0b011100;
      break;
    case 0xC3 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100110;
      fnt[5] = 0b101010;
      fnt[6] = 0b100110;
      break;
    case 0xC4 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000010;
      fnt[3] = 0b000010;
      fnt[4] = 0b001110;
      fnt[5] = 0b010010;
      fnt[6] = 0b001110;
      break;
    case 0xC5 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b011100;
      fnt[3] = 0b100010;
      fnt[4] = 0b111000;
      fnt[5] = 0b100010;
      fnt[6] = 0b011100;
      break;
    case 0xC6 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b010010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101110;
      fnt[5] = 0b101010;
      fnt[6] = 0b010010;
      break;
    case 0xC7 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111100;
      fnt[3] = 0b100010;
      fnt[4] = 0b111100;
      fnt[5] = 0b101000;
      fnt[6] = 0b100100;
      break;
    case 0xE0 :
      fnt[0] = 0b111100;
      fnt[1] = 0b101000;
      fnt[2] = 0b101000;
      fnt[3] = 0b100100;
      fnt[4] = 0b100010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100010;
      break;
    case 0xE1 :
      fnt[0] = 0b100010;
      fnt[1] = 0b100010;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100000;
      break;
    case 0xE2 :
      fnt[0] = 0b101010;
      fnt[1] = 0b101010;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100000;
      break;
    case 0xE3 :
      fnt[0] = 0b000000;
      fnt[1] = 0b111110;
      fnt[2] = 0b101000;
      fnt[3] = 0b100100;
      fnt[4] = 0b100010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100010;
      break;
    case 0xE4 :
      fnt[0] = 0b000000;
      fnt[1] = 0b001000;
      fnt[2] = 0b011100;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b011100;
      fnt[6] = 0b001000;
      break;
    case 0xE5 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b100010;
      fnt[3] = 0b100010;
      fnt[4] = 0b100010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100000;
      break;
    case 0xE6 :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b101010;
      fnt[3] = 0b101010;
      fnt[4] = 0b101010;
      fnt[5] = 0b111110;
      fnt[6] = 0b100000;
      break;
    case 0x21 :
      fnt[0] = 0b000100;
      fnt[1] = 0b000100;
      fnt[2] = 0b000100;
      fnt[3] = 0b000100;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b000100;
      break;
    case 0x27 :
      fnt[0] = 0b001100;
      fnt[1] = 0b001000;
      fnt[2] = 0b000100;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x28 :
      fnt[0] = 0b010000;
      fnt[1] = 0b001000;
      fnt[2] = 0b000100;
      fnt[3] = 0b000100;
      fnt[4] = 0b000100;
      fnt[5] = 0b001000;
      fnt[6] = 0b010000;
      break;
    case 0x29 :
      fnt[0] = 0b000100;
      fnt[1] = 0b001000;
      fnt[2] = 0b010000;
      fnt[3] = 0b010000;
      fnt[4] = 0b010000;
      fnt[5] = 0b001000;
      fnt[6] = 0b000100;
      break;
    case 0x2A :
      fnt[0] = 0b000000;
      fnt[1] = 0b001000;
      fnt[2] = 0b101010;
      fnt[3] = 0b011100;
      fnt[4] = 0b101010;
      fnt[5] = 0b001000;
      fnt[6] = 0b000000;
      break;
    case 0x2B :
      fnt[0] = 0b000000;
      fnt[1] = 0b001000;
      fnt[2] = 0b001000;
      fnt[3] = 0b111110;
      fnt[4] = 0b001000;
      fnt[5] = 0b001000;
      fnt[6] = 0b000000;
      break;
    case 0x2C :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b000110;
      fnt[5] = 0b000100;
      fnt[6] = 0b000010;
      break;
    case 0x2D :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b011100;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x2E :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b001100;
      fnt[6] = 0b001100;
      break;
    case 0x3A :
      fnt[0] = 0b000000;
      fnt[1] = 0b001100;
      fnt[2] = 0b001100;
      fnt[3] = 0b000000;
      fnt[4] = 0b001100;
      fnt[5] = 0b001100;
      fnt[6] = 0b000000;
      break;
    case 0x3B :
      fnt[0] = 0b000000;
      fnt[1] = 0b001100;
      fnt[2] = 0b001100;
      fnt[3] = 0b000000;
      fnt[4] = 0b001100;
      fnt[5] = 0b001000;
      fnt[6] = 0b000100;
      break;
    case 0x3C :
      fnt[0] = 0b010000;
      fnt[1] = 0b001000;
      fnt[2] = 0b000100;
      fnt[3] = 0b000010;
      fnt[4] = 0b000100;
      fnt[5] = 0b001000;
      fnt[6] = 0b010000;
      break;
    case 0x3D :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b111110;
      fnt[3] = 0b000000;
      fnt[4] = 0b111110;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x3E :
      fnt[0] = 0b000100;
      fnt[1] = 0b001000;
      fnt[2] = 0b010000;
      fnt[3] = 0b100000;
      fnt[4] = 0b010000;
      fnt[5] = 0b001000;
      fnt[6] = 0b000100;
      break;
    case 0x3F :
      fnt[0] = 0b011100;
      fnt[1] = 0b100010;
      fnt[2] = 0b100000;
      fnt[3] = 0b010000;
      fnt[4] = 0b001000;
      fnt[5] = 0b000000;
      fnt[6] = 0b001000;
      break;
    case 0x5E :
      fnt[0] = 0b001000;
      fnt[1] = 0b010100;
      fnt[2] = 0b100010;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b000000;
      break;
    case 0x5F :
      fnt[0] = 0b000000;
      fnt[1] = 0b000000;
      fnt[2] = 0b000000;
      fnt[3] = 0b000000;
      fnt[4] = 0b000000;
      fnt[5] = 0b000000;
      fnt[6] = 0b111110;
      break;
    case 0x5B :
      fnt[0] = 0b011100;
      fnt[1] = 0b000100;
      fnt[2] = 0b000100;
      fnt[3] = 0b000100;
      fnt[4] = 0b000100;
      fnt[5] = 0b000100;
      fnt[6] = 0b011100;
      break;
    case 0x5D :
      fnt[0] = 0b011100;
      fnt[1] = 0b010000;
      fnt[2] = 0b010000;
      fnt[3] = 0b010000;
      fnt[4] = 0b010000;
      fnt[5] = 0b010000;
      fnt[6] = 0b011100;
      break;
    default : return 0;
  }
  switch(n)
  {
    case 0 : return fnt[0];
    case 1 : return fnt[1];
    case 2 : return fnt[2];
    case 3 : return fnt[3];
    case 4 : return fnt[4];
    case 5 : return fnt[5];
    case 6 : return fnt[6];
  }
  return 0;
}
