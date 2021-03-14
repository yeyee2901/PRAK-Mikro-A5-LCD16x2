#include <Arduino.h> 
#include "driver_LCD16x2.hpp"
#include <stdio.h>

// GLOBAL MACROS --------------------------------------------
#define RUNNING_SPEED 970

// GLOBAL VARIABLES------------------------------------------
volatile int detik = 0,
             menit = 0,
             jam = 0;
int detik_last;
char jam_buffer[9];

uint8_t clear_flag = 0, 
        position = 0,
        direction = 0;

// FUNCTION PROTOTYPE----------------------------------------
void update_jam();

// MAIN PROGRAM ---------------------------------------------
void setup() {

  // Initialize LCD 16x2, 8-bit mode, 2 line,
  // Increment mode, Cursor shift mode, Cursor OFF
  // PORTA -> Data bus
  // PORTD -> PD0: RS pin , PD1: EN pin
  lcd_init(&PORTA, &PORTD, &DDRA, &DDRD, PD0, PD1);

  // Initialize External Interrupt (INT0 & INT1)

  // Initialize Timer-1 (16-bit)
  // - Overflow Interrupt Mode
  // - Overflow setiap 1 detik
  // - F_CPU = 1MHz
  // - prescaler = 64
  TCCR1A = 0;
  TIMSK = (1 << TOIE1);
  TCNT1 = 49900;
  TCCR1B = (1 << CS11) | (1 << CS10);

  // Initialize External Interrupt
  // INT0 & INT1
  // Falling Edge mode (set ISC11 & ISC01)
  GICR = (1 << INT0) | (1 << INT1);
  MCUCR = (1 << ISC11) | (1 << ISC01);

}

void loop() {
  
  // Running Text ke kiri-kanan bolak balik
  if(position >= 3){
    position = 3;
    direction = 0;  // set arah geser ke KIRI
  }
  if(position <= 0){
    position = 0;
    direction = 1;  // set arah geser ke KANAN
  }

  // running ke kiri
  if(direction == 0){
    delay(RUNNING_SPEED);
    lcd_setpos(0,0);
    lcd_string("                ");
    lcd_setpos(0, position);
    lcd_string("-JAM DIGITAL-");
    position--;
  }
  // running ke kanan
  if(direction == 1){
    delay(RUNNING_SPEED);
    lcd_setpos(0,0);
    lcd_string("                ");
    lcd_setpos(0, position);
    lcd_string("-JAM DIGITAL-");
    position++;
  }
  
  // if clear flag is set, clear the clock
  if(clear_flag){
    detik = 0;
    menit = 0;
    jam = 0;
    clear_flag = 0;
    update_jam();
  }

  // update jam jika detik sekarang != detik terakhir
  if(detik != detik_last){
    update_jam();
  }


}

// ISR ------------------------------------------------------
// Start / Stop jam
ISR(INT0_vect){
  TCCR1B ^= ((1 << CS11) | (1 << CS10));
}

// Clear Jam ke 00:00:00
ISR(INT1_vect){
  clear_flag = 1;
}

// Overflow setiap 1 detik, reset TCNT1 ke 49900
ISR(TIMER1_OVF_vect){
  detik++;
  TCNT1 = 49900;
}

// FUNCTION DEFINITION -------------------------------------
void update_jam(){
  
  // update nilai detik, menit, jam
  if(jam >= 23 && menit >= 59 && detik >= 60){
    jam = 0;
    menit = 0;
    detik = 0;
  }
  else if(menit >= 59 && detik >= 60){
    jam++;
    menit = 0;
    detik = 0;
  }
  if(detik >= 60){
    menit++;
    detik = 0;
  }

  // pecah satuan & puluhan, lalu simpan di char buffer
  sprintf(jam_buffer, "%d%d:%d%d:%d%d",
          jam/10, jam%10,
          menit/10, menit%10,
          detik/10, detik%10
  );
  
  // kirim ke LCD
  lcd_setpos(1,0);
  lcd_string(jam_buffer);
  lcd_string("  @yeyee");

  // update nilai detik last
  detik_last = detik;
}