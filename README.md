# PRAKTIKUM SISTEM MIKRO - A5 - LCD 16x2

- IDE: PlatformIO on VSCode (https://platformio.org/)
- Device: Atmel AVR ATMega16
- CPU: Internal 1 MHz
- Programmer: USBasp
- Demonstrates simple interfacing of generic LCD 16x2

**Datasheets**
- LCD 16x2 HD44780 : https://www.alldatasheet.com/datasheet-pdf/pdf/63673/HITACHI/HD44780.html
- ATMega16 : https://www.alldatasheet.com/datasheet-pdf/pdf/78532/ATMEL/ATMEGA16.html
    

**Brief Explanations**

LCD 16x2 generic sebagian besar yang ditemui di pasaran menggunakan kontroler HD44780. Tidak hanya LCD 16x2 saja,
20x4 juga sekarang banyak menggunakan kontroler HD44780. Jadi driver LCD di project ini (hpp & cpp) seharusnya
bisa digunakan juga untuk ukuran LCD yang umum yang menggunakan HD44780.

LCD ini mempunyai 8-bit data bus, dan 3 pin untuk kontrol, yaitu RS (*Register Select*), R/W' (*Read/Write mode*),
dan EN (*Enable Pin*). Untuk mengoperasikan LCD ini, kita harus memberi informasi ke LCD,apakah yang akan kita kirimkan
adalah *command* atau *data*. Daftar *command* LCD sudah tercantum pada driver.

Untuk pengoperasiannya, LCD 16x2 mempunyai 2 mode pengoperasian. Yang pertama adalah metode delay, dimana setiap di akhir *command*
LCD dijalankan diberi delay untuk memberi waktu LCD memproses *command*-nya. Daftar delay setiap *command* dapat dilihat pada datasheet.
Untuk metode pengoperasian ini, pin R/W' dapat di *pull* ke LOW agar LCD selalu berada pada mode *write*, karena tidak diperlukan membaca data dari LCD.

Metode yang kedua adalah metode *busy flag*. Untuk metode ini sedikit kompleks dimana LCD harus bisa di toggle *mode* antara *read & write*. Pertama,
setiap **sebelum** ingin mengirimkan *command*, LCD harus diubah ke mode *read mode* & *command mode* dahulu (RS = 0 ; R/W' = 1). 
Lalu, baca *busy flag* yang terdapat pada *data bus* bit ke-7. *Busy flag* ini menandakan bahwa driver LCD sedang mengerjakan sesuatu (di background), sehingga segala
*command* / *data* yang di kirim ke LCD pada waktu ini tidak akan diterima & diproses. Kita harus menunggu hingga *busy flag* menjadi 0 yang menandakan LCD
sudah selesai mengerjakan hal tersebut. Bisa menggunakan perintah while sederhana:
```cpp
// BUSY_FLAG_BIT = 7

// tunggu busy flag = 0
while( DATA_BUS & (1 << BUSY_FLAG_BIT) == 1);

// send the command / data
// .
// .
```

**PSEUDOCODE**
1. Set RS = 0 (*command mode*) / RS = 1 (*data mode*)
2. siapkan *data* / *command* di data bus
3. pull EN pin HIGH
4. delay 450ns (lihat datasheet HD44780, bag. *Bus Timing Characteristics*)
5. pull EN pin LOW

**FUNCTIONS**
1. lcd_command() : kirim command ke LCD. Daftar command terdapat pada datasheet.

Parameter:
- uint8_t command : hex dari command yang ingin dikirim
```cpp
lcd_command(0x01); // clear display
```


2. lcd_char() : kirim 1 karakter ke LCD untuk di display.

Parameter:
- char data : karakter yang ingin di display (single character, bila array, pastikan hanya akses 1 element saja)
```cpp
char karakter[2] = "A";
lcd_char(karakter[0]); // kirim element ke-0, karena element ke-1 adalah NULL byte
```


3. lcd_string() : kirim string ke LCD untuk di display.

Parameter:
- char *char_array_pointer : address dari char array ATAU char pointer yang menunjuk ke suatu char array
```cpp
char *str_test = "Test 2";
char buf[10]; 
sprintf(buf, "Test %d", 3);

lcd_string("Test");     // output: Test
lcd_string(str_test);   // output: Test 2
lcd_string(buf);        // output: Test 3
```


4. lcd_init() : inisialisasi LCD menggunakan PORT yang ingin digunakan pada ATMega.

Parameter:
- volatile uint8_t *data_port : Address dari PORTx yang ingin digunakan sebagai data bus LCD
- volatile uint8_t *control_port : Address dari PORTx yang ingin digunakan sebagai control bus LCD (untuk RS & EN pin)
- volatile uint8_t *data_port_dir : Address dari DDRx yang ingin digunakan sebagai kontrol I/O direction dari data bus LCD
- volatile uint8_t *control_port_dir : Address dari DDRx yang ingin digunakan sebagai kontrol I/O direction dari control bus
- uint8_t RS_pin : nomor pin dari control bus yang ingin digunakan sebagai pin RS (integer value, bisa menggunakan pin definition datasheet)
- uint8_t EN_pin : nomor pin dari control bus yang ingin digunakan sebagai pin EN (integer value, bisa menggunakan pin definition datasheet)
```cpp
/* Inisialisasi LCD:
- PORTA : Data bus (8-bit)
- PORTB : control bus (2-bit)
- PB0 : pin RS
- PB1 : pin EN
*/
lcd_init( &PORTA, &PORTB, &DDRA, &DDRB, PB0, PB1 );
```

**HOW TO USE THIS PROJECT (THE FAST WAY)**
1. Download Visual Studio Code
2. Download the PlatformIO extension (from VSCode extension tab)
3. Clone this repo / download this repo in .zip & unzip it anywhere you want
4. Navigate to PlatformIO - Home tab
5. Select "Open Project"
6. Navigate the folder containing this repo / unzipped folder, select and open it.
7. You are now inside this PlatformIO project, to build the project, either:
    - Click the "tick" symbol at the bottom left part of VS Code
    - Ctrl + Shift + B , select "PlatformIO: Build"
9. If you followed the steps correctly, the project should build with no problems
10. To upload the code to your board, edit the "platformio.ini" file first
11. Change the "board" ID to the board ID you want to upload to (See https://docs.platformio.org/en/latest/platforms/atmelavr.html for list of board ID)
12. Change the upload protocol to suit your USB programmer (again, see https://docs.platformio.org/en/latest/platforms/atmelavr.html for more info, I used USBasp)
