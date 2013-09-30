#include "stm32f4xx.h"
#include <stm32f4xx_gpio.h>
#include "lcd.hpp"
#include "math.h"

void delay(unsigned int s){
	while(--s > 0) {
		__NOP();
	}
}

void LiquidCrystal::writeCmd(uint16_t cmd)
{
	GPIO_ResetBits(portGPIO, rsPin);
	writeData(cmd);
	delay(500);
	GPIO_SetBits(portGPIO, rsPin);
}

void LiquidCrystal::writeData(uint16_t data)
{
	GPIO_SetBits(portGPIO, enPin);
	shiftRegister.set(data);
	delay(500);
	GPIO_ResetBits(portGPIO, enPin);

}

void LiquidCrystal::init()
{
	GPIO_ResetBits(portGPIO, rsPin);  // rs = 0
	int del = 9999;
	delay(del);

	writeData(0b00110000); //init1
	delay(del);

	writeData(0b00110000); //init2
	delay(del);

	writeData(0b00110000); //init3
	delay(del);

	writeData(0b00111000); // function set  8bit 2line 5x8 dots
	delay(del);

	writeData(0b00001100); // display on + cursor underline + blinking
	delay(del);

	writeData(0b00000001);
	delay(del);

	writeData(0b00000110); //entry mode set
	delay(del);

	writeData(0b00000010); // return to home
	delay(del);

	GPIO_SetBits(portGPIO, rsPin);  //rs = 1
}

void LiquidCrystal::setCursor(uint8_t line, uint8_t pos)
{
	pos |= 0b10000000;
	if (line == 1)
		pos += 0x40;
	else if (line == 2)
		pos += 0x14;
	else if (line == 3)
		pos += 0x54;
	writeCmd(pos);
}

void LiquidCrystal::clear()
{
	writeCmd(0b00000001);
}

void LiquidCrystal::blink()
{
	writeCmd(0b00001101);
}

void LiquidCrystal::noBlink()
{
	writeCmd(0b00001100);

}

void LiquidCrystal::print(const char* str)
{
	do {
		printChr(str);
	}while(*++str);
}

void LiquidCrystal::printChr(const char* chr)
{
  switch(*chr) {
  case ' ':
    writeData(0x20); break;
  case '!':
    writeData(0x21); break;
  case '"':
    writeData(0x22); break;
  case '#':
    writeData(0x23); break;
  case '$':
    writeData(0x24); break;
  case '%':
    writeData(0x25); break;
  case '&':
    writeData(0x26); break;
  case '\'':
    writeData(0x27); break;
  case '(':
    writeData(0x28); break;
  case ')':
    writeData(0x29); break;
  case '*':
    writeData(0x2A); break;
  case '+':
    writeData(0x2B); break;
  case ',':
    writeData(0x2C); break;
  case '-':
    writeData(0x2D); break;
  case '.':
    writeData(0x2E); break;
  case '/':
    writeData(0x2F); break;
  case '0':
    writeData(0x30); break;
  case '1':
    writeData(0x31); break;
  case '2':
    writeData(0x32); break;
  case '3':
    writeData(0x33); break;
  case '4':
    writeData(0x34); break;
  case '5':
    writeData(0x35); break;
  case '6':
    writeData(0x36); break;
  case '7':
    writeData(0x37); break;
  case '8':
    writeData(0x38); break;
  case '9':
    writeData(0x39); break;
  case ':':
    writeData(0x3A); break;
  case ';':
    writeData(0x3B); break;
  case '<':
    writeData(0x3C); break;
  case '=':
    writeData(0x3D); break;
  case '>':
    writeData(0x3E); break;
  case '?':
    writeData(0x3F); break;
  case 'A':
    writeData(0x41); break;
  case 'À': // cyrillic
    writeData(0x41); break;
  case 'B':
    writeData(0x42); break;
  case 'Â': // cyrillic
    writeData(0x42); break;
  case 'C':
    writeData(0x43); break;
  case 'Ñ': // cyrillic
    writeData(0x43); break;
  case 'D':
    writeData(0x44); break;
  case 'E':
    writeData(0x45); break;
  case 'Å': // cyrillic
    writeData(0x45); break;
  case 'F':
    writeData(0x46); break;
  case 'G':
    writeData(0x47); break;
  case 'H':
    writeData(0x48); break;
  case 'Í': // cyrillic
    writeData(0x48); break;
  case 'I':
    writeData(0x49); break;
  case 'J':
    writeData(0x4A); break;
  case 'K':
    writeData(0x4B); break;
  case 'Ê': // cyrillic
    writeData(0x4B); break;
  case 'L':
    writeData(0x4C); break;
  case 'M':
    writeData(0x4D); break;
  case 'Ì': // cyrillic
    writeData(0x4D); break;
  case 'N':
    writeData(0x4E); break;
  case 'O':
    writeData(0x4F); break;
  case 'Î': // cyrillic
    writeData(0x4F); break;
  case 'P':
    writeData(0x50); break;
  case 'Ð': // cyrillic
    writeData(0x50); break;
  case 'Q':
    writeData(0x51); break;
  case 'R':
    writeData(0x52); break;
  case 'S':
    writeData(0x53); break;
  case 'T':
    writeData(0x54); break;
  case 'U':
    writeData(0x55); break;
  case 'V':
    writeData(0x56); break;
  case 'W':
    writeData(0x57); break;
  case 'X':
    writeData(0x58); break;
  case 'Õ': // cyrillic
    writeData(0x58); break;
  case 'Y':
    writeData(0x59); break;
  case 'Z':
    writeData(0x5A); break;
  case '[':
    writeData(0x5B); break;
  case ']':
    writeData(0x5D); break;
  case '^':
    writeData(0x5E); break;
  case '_':
    writeData(0x5F); break;
  case '`':
    writeData(0x60); break;
  case 'a':
    writeData(0x61); break;
  case 'à': // cyrillic
    writeData(0x61); break;
  case 'b':
    writeData(0x62); break;
  case 'c':
    writeData(0x63); break;
  case 'ñ': // cyrillic
    writeData(0x63); break;
  case 'd':
    writeData(0x64); break;
  case 'e':
    writeData(0x65); break;
  case 'å': // cyrillic
    writeData(0x65); break;
  case 'f':
    writeData(0x66); break;
  case 'g':
    writeData(0x67); break;
  case 'h':
    writeData(0x68); break;
  case 'i':
    writeData(0x69); break;
  case 'j':
    writeData(0x6A); break;
  case 'k':
    writeData(0x6B); break;
  case 'l':
    writeData(0x6C); break;
  case 'm':
    writeData(0x6D); break;
  case 'n':
    writeData(0x6E); break;
  case 'o':
    writeData(0x6F); break;
  case 'î': // cyrillic
    writeData(0x6F); break;
  case 'p':
    writeData(0x70); break;
  case 'ð': // cyrillic
    writeData(0x70); break;
  case 'q':
    writeData(0x71); break;
  case 'r':
    writeData(0x72); break;
  case 's':
    writeData(0x73); break;
  case 't':
    writeData(0x74); break;
  case 'u':
    writeData(0x75); break;
  case 'v':
    writeData(0x76); break;
  case 'w':
    writeData(0x77); break;
  case 'x':
    writeData(0x78); break;
  case 'õ': // cyrillic
    writeData(0x78); break;
  case 'y':
    writeData(0x79); break;
  case 'ó': // cyrillic
    writeData(0x79); break;
  case 'z':
    writeData(0x7A); break;
  case 'Á':
    writeData(0xA0); break;
  case 'Ã':
    writeData(0xA1); break;
  case '¨':
    writeData(0xA2); break;
  case 'Æ':
    writeData(0xA3); break;
  case 'Ç':
    writeData(0xA4); break;
  case 'È':
    writeData(0xA5); break;
  case 'É':
    writeData(0xA6); break;
  case 'Ë':
    writeData(0xA7); break;
  case 'Ï':
    writeData(0xA8); break;
  case 'Ó':
    writeData(0xA9); break;
  case 'Ô':
    writeData(0xAA); break;
  case '×':
    writeData(0xAB); break;
  case 'Ø':
    writeData(0xAC); break;
  case 'Ú':
    writeData(0xAD); break;
  case 'Û':
    writeData(0xAE); break;
  case 'Ý':
    writeData(0xAF); break;
  case 'Þ':
    writeData(0xB0); break;
  case 'ß':
    writeData(0xB1); break;
  case 'á':
    writeData(0xB2); break;
  case 'â':
    writeData(0xB3); break;
  case 'ã':
    writeData(0xB4); break;
  case '¸':
    writeData(0xB5); break;
  case 'æ':
    writeData(0xB6); break;
  case 'ç':
    writeData(0xB7); break;
  case 'è':
    writeData(0xB8); break;
  case 'é':
    writeData(0xB9); break;
  case 'ê':
    writeData(0xBA); break;
  case 'ë':
    writeData(0xBB); break;
  case 'ì':
    writeData(0xBC); break;
  case 'í':
    writeData(0xBD); break;
  case 'ï':
    writeData(0xBE); break;
  case 'ò':
    writeData(0xBF); break;
  case '÷':
    writeData(0xC0); break;
  case 'ø':
    writeData(0xC1); break;
  case 'ú':
    writeData(0xC2); break;
  case 'û':
    writeData(0xC3); break;
  case 'ü':
    writeData(0xC4); break;
  case 'ý':
    writeData(0xC5); break;
  case 'þ':
    writeData(0xC6); break;
  case 'ÿ':
    writeData(0xC7); break;
  case '«':
    writeData(0xC8); break;
  case '»':
    writeData(0xC9); break;
  case '“':
    writeData(0xCA); break;
  case '”':
    writeData(0xCB); break;
  case 'Ä':
    writeData(0xE0); break;
  case 'Ö':
    writeData(0xE1); break;
  case 'Ù':
    writeData(0xE2); break;
  case 'ä':
    writeData(0xE3); break;
  case 'ô':
    writeData(0xE4); break;
  case 'ö':
    writeData(0xE5); break;
  case 'ù':
    writeData(0xE6); break;
  case '§':
    writeData(0xFD); break;
  case '¶':
    writeData(0xFE); break;
  }
}
