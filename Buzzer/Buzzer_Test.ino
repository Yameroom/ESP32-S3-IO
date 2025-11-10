#include <Arduino.h>

#define BUZZER_PIN 42
#define CORE_TO_RUN 0

void buzzerTask(void * pvParameters) {
  String coreID = String(xPortGetCoreID());
  Serial.print("buzzerTask berjalan di Core: ");
  Serial.println(coreID);

  pinMode(BUZZER_PIN, OUTPUT);

  for (;;) {
    Serial.println("Core " + coreID + ": Buzzer ON");

    digitalWrite(BUZZER_PIN, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    Serial.println("Core " + coreID + ": Buzzer OFF");

    digitalWrite(BUZZER_PIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
      buzzerTask,
      "BuzzerTask",
      2048,
      NULL,
      1,
      NULL,
      CORE_TO_RUN
  );

  Serial.println("Task Buzzer telah dibuat untuk berjalan di Core " + String(CORE_TO_RUN));
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}