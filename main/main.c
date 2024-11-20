#include <stdio.h>
#include <time.h>
#include "ds3231.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SDA_PIN 21  // Change as per your setup
#define SCL_PIN 22  // Change as per your setup

static const char *TAG = "MAIN";

void app_main() {
    esp_err_t ret;
    struct tm timeinfo;
    time_t unix_time;

    // Initialize DS3231 RTC
    ret = ds3231_init(SDA_PIN, SCL_PIN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize DS3231");
        return;
    }

    // Set time (example: 20 Nov 2024, 15:30:45)
    timeinfo.tm_year = 2024 - 1900;  // tm_year is years since 1900
    timeinfo.tm_mon = 10;            // November (0-based)
    timeinfo.tm_mday = 20;
    timeinfo.tm_hour = 15;
    timeinfo.tm_min = 30;
    timeinfo.tm_sec = 45;

    ret = ds3231_set_time(&timeinfo);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set time");
    } else {
        ESP_LOGI(TAG, "Time set successfully");
    }

    // Get time and print it
    while (1) {
        ret = ds3231_get_time(&timeinfo);
        if (ret == ESP_OK) {
            char buffer[64];
            strftime(buffer, sizeof(buffer), "%c", &timeinfo);  // Format time as string
            ESP_LOGI(TAG, "Current time: %s", buffer);
        } else {
            ESP_LOGE(TAG, "Failed to get time");
        }

        // Get UNIX time
        ret = ds3231_get_unix_time(&unix_time);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "UNIX time: %lld", unix_time);
        } else {
            ESP_LOGE(TAG, "Failed to get UNIX time");
        }

        // Wait for 5 seconds before the next read
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
