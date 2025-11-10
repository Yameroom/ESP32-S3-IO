#include <Arduino.h>

#define LED1_PIN 1
#define LED2_PIN 2
#define LED3_PIN 40

#define CORE_TO_RUN 1 // 0 = Core, 1 = Core 0

// Task yang akan menjalankan I/O
void ledBlinkTask(void * pvParameters) {
  Serial.print("ledBlinkTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  for (;;) {
    Serial.println("Core " + String(xPortGetCoreID()) + ": LEDs ON");
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println("Core " + String(xPortGetCoreID()) + ": LEDs OFF");
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
      ledBlinkTask,   // Fungsi task
      "LEDTask",      // Nama task
      2048,           // Ukuran stack
      NULL,           // Parameter
      1,              // Prioritas
      NULL,           // Handle task
      CORE_TO_RUN     // Core ID (0 atau 1)
  );

  Serial.println("Task LED telah dibuat untuk berjalan di Core " + String(CORE_TO_RUN));
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}