#include <Arduino.h>

#define POT_PIN 5

TaskHandle_t TaskReadPot;
TaskHandle_t TaskDisplayPot;

volatile int potValue = 0;

void readPotTask(void *pvParameters) {
  Serial.print("readPotTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    potValue = analogRead(POT_PIN);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void displayPotTask(void *pvParameters) {
  Serial.print("displayPotTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    Serial.print("Nilai Potensiometer: ");
    Serial.println(potValue);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  pinMode(POT_PIN, INPUT);

  xTaskCreatePinnedToCore(
    readPotTask,
    "ReadPot",
    2048,
    NULL,
    1,
    &TaskReadPot,
    0
  );

  xTaskCreatePinnedToCore(
    displayPotTask,
    "DisplayPot",
    2048,
    NULL,
    1,
    &TaskDisplayPot,
    1
  );

  Serial.println("Task Potensiometer telah dibuat di Core 0 & Core 1");
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
