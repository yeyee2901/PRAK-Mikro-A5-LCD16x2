#include <Arduino.h> 
#include "driver_LCD16x2.hpp"

void setup() {
  // put your setup code here, to run once:
  lcd_init(&PORTA, &PORTC, &DDRA, &DDRC, PC0, PC1);
  *LCD_DATA_PORT = 0xFF;
}

void loop() {
  // put your main code here, to run repeatedly:

  *LCD_DATA_PORT ^= 0xFF;
  *LCD_CONTROL_PORT ^= ((1 << LCD_EN_pin) | (LCD_RS_pin));
  delay(500);
}