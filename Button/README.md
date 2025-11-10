## Percobaan Button
Wokwi Project [Link](https://wokwi.com/projects/447212070337922049)
Percobaan ini bertujuan untuk membaca status tombol (push button) menggunakan dua core prosesor ESP32.
Core 0 bertugas membaca input dari tombol, sementara Core 1 menampilkan status tombol ke Serial Monitor secara paralel.

## Penjelasan Kode
### 1. Definisi Pin dan Task
Pin yang digunakan untuk tombol serta deklarasi task didefinisikan di awal program.
```c
#include <Arduino.h>

#define BUTTON_PIN 7

TaskHandle_t taskReadButton;
TaskHandle_t taskDisplayStatus;

volatile bool buttonPressed = false;
```
`BUTTON_PIN` menentukan pin GPIO tempat tombol terhubung.
Dua variabel `TaskHandle_t` digunakan untuk menyimpan handle dari kedua task (`taskReadButton` dan `taskDisplayStatus`).
Variabel `buttonPressed` bertipe `volatile` agar aman diakses oleh dua core secara bersamaan.

### 2. Task `readButtonTask`
Task ini dijalankan di Core 0 dan bertugas membaca status tombol secara terus-menerus.
```c
void readButtonTask(void *pvParameters) {
  Serial.print("readButtonTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (;;) {
    bool state = digitalRead(BUTTON_PIN) == LOW; // tombol aktif LOW
    buttonPressed = state;
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
```
Fungsi ini menggunakan `digitalRead()` untuk membaca status pin tombol. Karena tombol menggunakan mode `INPUT_PULLUP`, maka kondisi aktif adalah `LOW`.
Fungsi `vTaskDelay()` memberikan jeda kecil agar pembacaan tombol lebih stabil dan tidak terbaca berulang akibat bouncing.

### 3. Task `displayStatusTask`
Task ini dijalankan di Core 1 dan bertanggung jawab menampilkan status tombol ke Serial Monitor.
```c
void displayStatusTask(void *pvParameters) {
  Serial.print("displayStatusTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    if (buttonPressed) {
      Serial.println("Button DITEKAN");
    } else {
      Serial.println("Button DILEPAS");
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```
Task ini memeriksa variabel `buttonPressed` dan menampilkan statusnya setiap 500 ms ke Serial Monitor.
Karena berjalan di core yang berbeda, task ini dapat menampilkan status tombol secara real-time tanpa menghambat proses pembacaan tombol di core lainnya.

### 4. Fungsi `setup()`
Fungsi `setup()` digunakan untuk menginisialisasi komunikasi serial dan membuat kedua task di dua core berbeda.
```c
void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    readButtonTask,     // Fungsi task
    "ReadButton",       // Nama task
    2048,               // Ukuran stack
    NULL,               // Parameter
    1,                  // Prioritas
    &taskReadButton,    // Handle task
    0                   // Core 0
  );

  xTaskCreatePinnedToCore(
    displayStatusTask,
    "DisplayStatus",
    2048,
    NULL,
    1,
    &taskDisplayStatus,
    1                   // Core 1
  );

  Serial.println("Task untuk Button berhasil dibuat di Core 0 & Core 1");
}
```
Fungsi `xTaskCreatePinnedToCore()` digunakan untuk menentukan task mana berjalan di core mana.
Dengan begitu, pembacaan tombol dan tampilan status benar-benar berjalan paralel.

### 5. Fungsi `loop()`  
Fungsi `loop()` tidak digunakan karena seluruh logika program telah dijalankan di dua task berbeda.
```c
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
```

## Hasil Percobaan
Hasil percobaan menunjukkan bahwa Core 0 mampu membaca tombol dengan baik, sedangkan Core 1 menampilkan status tombol secara real-time.
Output Serial Monitor akan berbeda tergantung pada kondisi tombol.

Hasil Serial Monitor pada Core 0:
```shell
readButtonTask berjalan di Core: 0 
displayStatusTask berjalan di Core: 1 
Button DILEPAS
Button DILEPAS
Button DILEPAS
Button DILEPAS
```

Hasil Serial Monitor pada Core 1:
```shell
readButtonTask berjalan di Core: 0 
displayStatusTask berjalan di Core: 1 
Button DILEPAS
Button DITEKAN
Button DITEKAN
Button DITEKAN
```

### Hasil Screenshot Core 0 (Pembacaan `readButtonTask`) & Core 1 (Status `displayStatusTask`)
---
![Hasil Screenshot Core 0 & 1](../assets/Button_Core_1&0_1.png)

![Hasil Screenshot Core 0 & 1](../assets/Button_Core_1&0_2.png)


Hasil Video Percobaan bisa diakses melalui link drive berikut:
[Percobaan Button](https://drive.google.com/file/d/1KIuYfP2UALllpA2vBfug6GYwfYf2TmNv/view?usp=drive_link)
