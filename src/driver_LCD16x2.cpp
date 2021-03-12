#include "driver_LCD16x2.hpp"

// global definitions
volatile uint8_t *LCD_DATA_PORT, *LCD_data_dir, 
                 *LCD_CONTROL_PORT, *LCD_control_dir;
uint8_t LCD_RS_pin, LCD_EN_pin;


// HIGH LEVEL FUNCTIONS
// data port is 8-bit, control port is 2-bit
// initializes LCD with the selected port in 8-bit mode
void lcd_init(volatile uint8_t *data_port, volatile uint8_t *control_port, 
              volatile uint8_t *data_dir, volatile uint8_t *control_dir,
              uint8_t RS_pin, uint8_t EN_pin)
{
    LCD_DATA_PORT = data_port;
    LCD_CONTROL_PORT = control_port;
    LCD_data_dir = data_dir;
    LCD_control_dir = control_dir;

    // set ports as outputs
    *LCD_data_dir = 0xFF;
    *LCD_control_dir |= (1 << RS_pin) | (1 << EN_pin);

}

void lcd_setpos(uint8_t row, uint8_t col){

}

// LOW LEVEL FUNCTIONS
void lcd_command(uint8_t command){

}
void lcd_char(unsigned char data){

}