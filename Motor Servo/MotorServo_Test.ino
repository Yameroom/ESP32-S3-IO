#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_PIN 41
#define CORE_TO_RUN 0

Servo myServo;

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

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}