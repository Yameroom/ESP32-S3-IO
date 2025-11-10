#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Lebar OLED dalam px
#define SCREEN_HEIGHT 64 // Tinggi OLED dalam px
#define OLED_RESET -1
#define CORE_TO_RUN 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Handle untuk Mutex.
SemaphoreHandle_t i2cMutex;

void oledTask(void * pvParameters) {
  String coreID = String(xPortGetCoreID());
  Serial.print("oledTask berjalan di Core: ");
  Serial.println(coreID);

  for (;;) {
    // 1. Ambil "key" Mutex sebelum menggunakan I2C
    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Percobaan OLED");

      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print("CORE: ");
      display.println(coreID);

      display.setTextSize(1);
      display.setCursor(0, 40);
      display.println("Task berjalan...");

      display.display();

      // 2. Kembalikan "key" Mutex
      xSemaphoreGive(i2cMutex);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("===== Percobaan OLED Display =====");

  i2cMutex = xSemaphoreCreateMutex();

  if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("Alokasi SSD1306 gagal"));
      for (;;);
    }
    Serial.println("OLED SSD1306 ditemukan!");
    display.clearDisplay();
    display.display();

    xSemaphoreGive(i2cMutex);
  }

  if(CORE_TO_RUN == 0) {
    Serial.println("setup() berjalan di Core 1.");
  }
  Serial.println("setup() berjalan di Core 0.");

  xTaskCreatePinnedToCore(
      oledTask,       // Fungsi task
      "OLEDTask",     // Nama task
      4096,           // Ukuran stack (OLED butuh > 2048)
      NULL,           // Parameter
      1,              // Prioritas
      NULL,           // Handle task
      CORE_TO_RUN     // Core ID (0 atau 1)
  );

  Serial.println("Task OLED telah dibuat untuk berjalan di Core " + String(CORE_TO_RUN));
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}