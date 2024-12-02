
#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
#define TASK_RUNNING_CORE 0
#else
#define TASK_RUNNING_CORE 1
#endif

#define ANALOG_INPUT_PIN A0

#ifndef LED_BUILTIN
#define LED_BUILTIN 13  // Specify the pin for the LED
#endif

// Define two tasks for Blink & AnalogRead.
void TaskBlink(void *pvParameters);
void TaskAnalogRead(void *pvParameters);
TaskHandle_t analog_read_task_handle;  // Optional: Task handle to manipulate a task from elsewhere.

// The setup function runs once when you press reset or power on the board.
void setup() {
    // Initialize serial communication at 115200 bits per second:
    Serial.begin(115200);

    // Set up two tasks to run independently.
    uint32_t blink_delay = 1000;  // Delay for LED blinking (1 second)
    xTaskCreate(
        TaskBlink, "Task Blink",  // Task name for debugging
        2048,  // Stack size in bytes
        (void *)&blink_delay,  // Pass delay as parameter
        1,  // Priority
        NULL  // No task handle needed
    );

    // Create another task for analog reading
    xTaskCreatePinnedToCore(
        TaskAnalogRead, "Analog Read",  // Task name
        2048,  // Stack size
        NULL,  // No parameters to pass
        5,  // Higher priority for Analog Read
        &analog_read_task_handle,  // Store handle for later manipulation
        0  // Run on core 0
    );

    Serial.println("Basic Multi-Threading Example");
}

void loop() {
    if (analog_read_task_handle != NULL) {
        delay(5000);  // Let the task run for a while
        vTaskDelete(analog_read_task_handle);  // Delete the analog read task
        Serial.println("TaskAnalogRead Deleted");
        analog_read_task_handle = NULL;  // Prevent deletion of a non-existing task
    }
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

// Task 1: Blink LED
void TaskBlink(void *pvParameters) {
    uint32_t blink_delay = *((uint32_t *)pvParameters);  // Get the delay from setup()
    pinMode(LED_BUILTIN, OUTPUT);  // Configure LED pin as output

    Serial.println("TaskBlink created");
    Serial.print("TaskBlink running on Core ");
    Serial.println(xPortGetCoreID());

    for (;;) {
        digitalWrite(LED_BUILTIN, HIGH);  // Turn LED on
        delay(blink_delay);
        digitalWrite(LED_BUILTIN, LOW);  // Turn LED off
        delay(blink_delay);
    }
}

// Task 2: Read Analog Input
void TaskAnalogRead(void *pvParameters) {
    (void)pvParameters;  // Unused parameter
    Serial.println("TaskAnalogRead created");

    if (digitalPinToAnalogChannel(ANALOG_INPUT_PIN) == -1) {
        Serial.printf("Invalid analog pin: %d. Task will terminate.\n", ANALOG_INPUT_PIN);
        vTaskDelete(NULL);
    }

    Serial.print("TaskAnalogRead running on Core ");
    Serial.println(xPortGetCoreID());

    for (;;) {
        int sensorValue = analogRead(ANALOG_INPUT_PIN);  // Read analog value
        Serial.printf("Analog Value: %d\n", sensorValue);
        delay(100);  // 100ms delay
    }
}
