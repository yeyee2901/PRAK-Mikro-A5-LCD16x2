#include <Arduino.h> 
#include "driver_LCD16x2.hpp"
#include <stdio.h>

void setup() {

  lcd_init(&PORTA, &PORTD, &DDRA, &DDRD, PD0, PD1);

  // coba string
  lcd_setpos(1,2);
  char buf[10];
  sprintf(buf, " test %d", 16);
  char *strings = "Test";

  lcd_string(strings);
  lcd_string(buf);
}

void loop() {
}