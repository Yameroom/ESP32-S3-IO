#include <Arduino.h>

#define CLK_PIN 35
#define DT_PIN 36

TaskHandle_t taskCore0;
TaskHandle_t taskCore1;

volatile int lastStateCLK = LOW;
volatile int direction = 0;

void core0Task(void *pvParameters) {
  for (;;) {
    if (direction == -1) {
      Serial.print("Putaran CCW dijalankan di Core: ");
      Serial.println(xPortGetCoreID());
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void core1Task(void *pvParameters) {
  for (;;) {
    if (direction == 1) {
      Serial.print("Putaran CW dijalankan di Core: ");
      Serial.println(xPortGetCoreID());
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void readEncoderTask(void *pvParameters) {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK_PIN);

  for (;;) {
    int currentStateCLK = digitalRead(CLK_PIN);
    if (currentStateCLK != lastStateCLK) {
      if (digitalRead(DT_PIN) != currentStateCLK) {
        direction = 1;
      } else {
        direction = -1;
      }
    }
    lastStateCLK = currentStateCLK;
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    readEncoderTask,
    "EncoderReader",
    2048,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    core0Task,
    "Core0Task",
    2048,
    NULL,
    1,
    &taskCore0,
    0
  );

  xTaskCreatePinnedToCore(
    core1Task,
    "Core1Task",
    2048,
    NULL,
    1,
    &taskCore1,
    1
  );

  Serial.println("Task Encoder dan Core Switch telah dibuat!");
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
