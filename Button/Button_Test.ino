#include <Arduino.h>

#define BUTTON_PIN 7

TaskHandle_t taskReadButton;
TaskHandle_t taskDisplayStatus;

volatile bool buttonPressed = false;

void readButtonTask(void *pvParameters) {
  Serial.print("readButtonTask berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  for (;;) {
    bool state = digitalRead(BUTTON_PIN) == LOW;
    buttonPressed = state;
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void displayStatusTask(void *pvParameters) {
  Serial.print("\ndisplayStatusTask berjalan di Core: ");
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

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    readButtonTask,
    "ReadButton",
    2048,
    NULL,
    1,
    &taskReadButton,
    0
  );

  xTaskCreatePinnedToCore(
    displayStatusTask,
    "DisplayStatus",
    2048,
    NULL,
    1,
    &taskDisplayStatus,
    1
  );

}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
