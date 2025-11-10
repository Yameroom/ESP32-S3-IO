#include <Arduino.h>

#define BUTTON1_PIN 7
#define BUTTON2_PIN 6

TaskHandle_t taskReadButton1;
TaskHandle_t taskReadButton2;

volatile bool button1Pressed = false;
volatile bool button2Pressed = false;

void readButton1Task(void *pvParameters) {
  Serial.print("readButton1Task berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  pinMode(BUTTON1_PIN, INPUT_PULLUP);

  for (;;) {
    bool state = digitalRead(BUTTON1_PIN) == LOW;
    if (state != button1Pressed) {
      button1Pressed = state;
      Serial.print("Core 0 → ");
      Serial.println(state ? "Button 1 DITEKAN" : "Button 1 DILEPAS");
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void readButton2Task(void *pvParameters) {
  Serial.print("readButton2Task berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  for (;;) {
    bool state = digitalRead(BUTTON2_PIN) == LOW;
    if (state != button2Pressed) {
      button2Pressed = state;
      Serial.print("Core 1 → ");
      Serial.println(state ? "Button 2 DITEKAN" : "Button 2 DILEPAS");
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("\n=== Program Dual Button Multi-Core ===");

  xTaskCreatePinnedToCore(
    readButton1Task,
    "ReadButton1",
    2048,
    NULL,
    1,
    &taskReadButton1,
    0
  );

  xTaskCreatePinnedToCore(
    readButton2Task,
    "ReadButton2",
    2048,
    NULL,
    1,
    &taskReadButton2,
    1
  );
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
