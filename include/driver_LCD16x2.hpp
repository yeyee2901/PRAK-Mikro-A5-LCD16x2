#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <Arduino.h>

// GLOBAL VARIABLE DECLARATIONS
extern volatile uint8_t *LCD_DATA_PORT, *LCD_data_dir, 
                        *LCD_CONTROL_PORT, *LCD_control_dir;
extern uint8_t LCD_RS_pin, LCD_EN_pin;


// HIGH LEVEL COMMANDS
void lcd_init(volatile uint8_t *data_port, volatile uint8_t *control_port, 
              volatile uint8_t *data_dir, volatile uint8_t *control_dir,
              uint8_t RS_pin, uint8_t EN_pin);

void lcd_setpos(uint8_t row, uint8_t col);


// LOW LEVEL COMMANDS
void lcd_command(uint8_t command);
void lcd_char(unsigned char data);

#endif