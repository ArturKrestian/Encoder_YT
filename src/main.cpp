#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Encoder.h>

int const encA_Pin = 5;
int const encC_Pin = 4;
int const encB_Pin = 3;
int const encBtn_Pin = 2;

LiquidCrystal_I2C lcd(0x27, 20, 4);

HardwareTimer timer(TIM2);

Encoder encoder(encA_Pin, encB_Pin, encC_Pin, encBtn_Pin);

int count = 0;

void TimerInterrupt()
{
  encoder.IrqEncoder();
}

void PrintLCD(String _input, int _line = 0)
{
  lcd.setCursor(0, _line);
  lcd.print(_input+"                   ");
}

void setup()
{
  pinMode(encA_Pin, INPUT_PULLUP);
  pinMode(encB_Pin, INPUT_PULLUP);
  pinMode(encC_Pin, OUTPUT);
  pinMode(encBtn_Pin, INPUT_PULLUP);

  timer.setPrescaleFactor(3200);
  timer.setOverflow(10);
  timer.attachInterrupt(TimerInterrupt);
  timer.refresh();
  timer.resume();

  lcd.init();
  lcd.backlight();

  PrintLCD("cnchobby.pl :-)",3);
}
void loop()
{
  int _encValue = encoder.ReadEnc();
  if (_encValue != 0)
  {
    count = count + _encValue;
    PrintLCD(String(count));
  }
}