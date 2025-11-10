## Percobaan Motor Servo

Percobaan untuk mengontrol sebuah motor servo agar bergerak bolak-balik antara 0 dan 180 derajat, yang menguji kemampuan ESP32 untuk menghasilkan sinyal PWM (Pulse Width Modulation) yang stabil dari dalam sebuah task FreeRTOS yang berjalan di core spesifik.

## Penjelasan Kode

### 1. Inisialisasi Library, Pin, dan Core

Percobaan ini memerlukan library ESP32Servo.h, yang merupakan pengganti library Servo.h standar dan dioptimalkan untuk hardware timer ESP32.

```c
#include <ESP32Servo.h>

#define SERVO_PIN 41
#define CORE_TO_RUN 1

Servo myServo;
```

SERVO_PIN diatur ke GPIO 41. CORE_TO_RUN diatur ke 1, yang berarti task akan dieksekusi di Core 1. myServo adalah objek yang akan digunakan untuk mengontrol servo.

### 2. Task servoTask

Fungsi servoTask adalah task yang bertanggung jawab untuk logika pergerakan motor.

```c
void servoTask(void * pvParameters) {
  String coreID = String(xPortGetCoreID());
  Serial.print("servoTask berjalan di Core: ");
  Serial.println(coreID);

  myServo.attach(SERVO_PIN);

  for (;;) {
    Serial.println("Core " + coreID + ": Gerak ke 0 derajat");
    myServo.write(0);
    vTaskDelay(1500 / portTICK_PERIOD_MS);

    Serial.println("Core " + coreID + ": Gerak ke 180 derajat");
    myServo.write(180);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}
```

Setelah task dimulai dan mencetak Core ID-nya, program memanggil myServo.attach(). Di dalam loop utama (for (;;)), program menggerakkan servo ke 0 derajat dengan myServo.write(0), kemudian menunggu 1.5 detik, lalu menggerakkan ke 180 derajat dengan myServo.write(180).

### 3. Fungsi setup()

Fungsi setup() menginisialisasi Serial dan membuat task servo.

```c
void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
      servoTask,      
      "ServoTask",    
      2048,          
      NULL,          
      1,              
      NULL,          
      CORE_TO_RUN    
  );

  Serial.println("Task Servo telah dibuat untuk berjalan di Core " + String(CORE_TO_RUN));
}
```

Fungsi setup() berjalan di Core 1, yang menginisialisasi Serial Monitor, lalu membuat servoTask dan menempatkannya di core yang ditentukan oleh CORE_TO_RUN.

### 4. Fungsi loop()

Fungsi loop() dibiarkan kosong, karena semua logika pergerakan motor ditangani oleh servoTask.

```c
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
```

## Hasil Percobaan

Hasil percobaan menunjukkan bahwa baik Core 0 maupun Core 1 mampu mengalokasikan timer perangkat keras dan menghasilkan sinyal PWM yang stabil untuk mengontrol motor servo secara akurat.

Hasil Serial Monitor pada Core 0:

```shell
servoTask berjalan di Core: 0
Task Servo telah dibuat untuk berjalan di Core 0
Core 0: Gerak ke 0 derajat
Core 0: Gerak ke 180 derajat
Core 0: Gerak ke 0 derajat
```


Hasil Serial Monitor pada Core 1 (Sesuai kode):

```shell
servoTask berjalan di Core: 1
Task Servo telah dibuat untuk berjalan di Core 1
Core 1: Gerak ke 0 derajat
Core 1: Gerak ke 180 derajat
Core 1: Gerak ke 0 derajat
```


## Hasil Screenshot Core 0

![Hasil Screenshot Core 0](../assets/Servo_Core_0.png)

## Hasil Screenshot Core 1

![Hasil Screenshot Core 1](../assets/Servo_Core_1.png)

Hasil Video Percobaan bisa diakses melalui link drive berikut:
[Percobaan Servo](https://drive.google.com/file/d/1mUMzly3cSU9e0WwtdwtqEowXoirBX8SG/view?usp=drive_link)