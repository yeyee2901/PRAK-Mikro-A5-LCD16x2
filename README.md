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
1. Set RS pin = 0 (*command mode*)
2. siapkan *data* / *command* di data bus
3. pull EN pin HIGH
4. delay 450ns (lihat datasheet HD44780, bag. *Bus Timing Characteristics*)
5. pull EN pin LOW
6. Clear *data* / *command* di data bus

