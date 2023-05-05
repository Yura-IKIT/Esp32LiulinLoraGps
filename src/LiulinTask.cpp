#include <Arduino.h>
#include "LiulinTask.h"

static TimerHandle_t LiulTimeHandle = NULL; // Timer handle

static TaskHandle_t LiulReadTaskHandle = NULL;

LIULIN_t *pLiul;

/*************************************** SETUP ****************************************/
void InitLiulinTask(
    LIULIN_t *Liulin_t,
    uint32_t timeiInterval, // = TIMER_INTERVAL_LIUL_READ,
#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
    SoftwareSerial *SerialPort
#else
    HardwareSerial *SerialPort
#endif
    ) // TimerHandle_t *hTimer)
{

    // SerialPort init
    Liulin_t->serialLiul = SerialPort;
#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
    Liulin_t->serialLiul->begin(LIUL_BAUD_RATE, SERIAL_8N1, LIUL_SOFT_SER_RX, LIUL_SOFT_SER_TX, SER_INVERT_LOGIC, SER_BUF_SIZE);
#else
    Liulin_t->serialLiul->begin(LIUL_BAUD_RATE, SERIAL_8N1, LIUL_HARD_SER_RX, LIUL_HARD_SER_TX);
#endif

    // Create the software timer that'll trigger the read of the Liulin data
    LiulTimeHandle = xTimerCreate("LiulTimer", pdMS_TO_TICKS(timeiInterval), pdTRUE, (void *)0, LiulTimerCallback);

    // Create the task to read Liulin in core 1
    xTaskCreatePinnedToCore(LiulReadTask, "readLiul", 2048, NULL, 1, &LiulReadTaskHandle, 1);

    // Start the time interval for the Liulin reading
    xTimerStart(LiulTimeHandle, portMAX_DELAY); // Rewind the timer
}
/**/

// Read time interval notifyer
static void LiulTimerCallback(TimerHandle_t LiulTimeHandle)
{

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(LiulReadTaskHandle, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken == pdTRUE)
        portYIELD_FROM_ISR();
}

// Task that performs the sequencial reads of the Liulin
static void LiulReadTask(void *params)
{
    int i = 0;
    // const TickType_t max_blocking_time = pdMS_TO_TICKS(5000); // ms

    while (1)
    {
        // 1.
        // ulTaskNotifyTake(pdTRUE, max_blocking_time);
        //   xTimerStart LiulTimeHandle, portMAX_DELAY); // Rewind the timer
        // 2.
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != 0) // wait to be notifyed from time interval function
        {
            xTimerStart(LiulTimeHandle, portMAX_DELAY); // Rewind the timer to time interval
            
            Serial.printf("ReadSpectrum: %d\n", i);
            i++;
            //todo: notify GPS task to get last valid data of the DateTime, Location, Distance from central and e.t.
        }

        // todo
    }
}