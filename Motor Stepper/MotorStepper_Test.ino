#include <Arduino.h>

#define STEP_PIN 38
#define DIR_PIN  40
#define STEPS_PER_REV 200
#define CORE_TO_RUN 1

void stepperTask(void * pvParameters) {
  String coreID = String(xPortGetCoreID());
  Serial.print("stepperTask berjalan di Core: ");
  Serial.println(coreID);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  for (;;) {
    Serial.println("Core " + coreID + ": Putar 1x Searah Jarum Jam");
    digitalWrite(DIR_PIN, HIGH);

    for (int i = 0; i < STEPS_PER_REV; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(500);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(500);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    Serial.println("Core " + coreID + ": Putar 1x Berlawanan Arah");
    digitalWrite(DIR_PIN, LOW);

    for (int i = 0; i < STEPS_PER_REV; i++) {
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(500);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(500);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
      stepperTask,
      "StepperTask",
      2048,
      NULL,
      2,
      NULL,
      CORE_TO_RUN
  );

  Serial.println("Task Stepper telah dibuat untuk berjalan di Core " + String(CORE_TO_RUN));
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}