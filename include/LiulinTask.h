#ifndef __TIMER_LIUL_TASK__
#define __TIMER_LIUL_TASK__

// !!! #define USE_SOFTWARE_SERIAL_LIUL_READ

#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
#include <SoftwareSerial.h> // https://github.com/plerup/espsoftwareserial
#define SER_INVERT_LOGIC false  // https://github.com/plerup/espsoftwareserial/blob/eb4b29074b75eacac3585bf84b5495b8f80a92cf/src/SoftwareSerial.h#L97
#define SER_BUF_SIZE 256
#define LIUL_SOFT_SER_RX 22
#define LIUL_SOFT_SER_TX 23
#else
#include <HardwareSerial.h>
#define LIUL_HARD_SER_RX 27
#define LIUL_HARD_SER_TX 26
#endif // USE_SOFTWARE_SERIAL_LIUL_READ

/* Liulin config params */
#define LIUL_BAUD_RATE 57600

#define TIMER_INTERVAL_LIUL_READ 10000U // ms

/* Liul error codes */ // todo
#define ERR_GPS_DATA_INVALID 0
#define ERR_NO_GPS_FIX 1
#define ERR_WIRING_SERIAL 2
#define ERR_GPS_ALL_GOOD 3

/* Config Liulin container */
typedef struct
{
#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
  SoftwareSerial *serialLiul;
#else
  HardwareSerial *serialLiul;
#endif // USE_SOFTWARE_SERIAL_LIUL_READ
  String name;

} LIULIN_t;

/* Data container for spectrum channels */
typedef struct
{
  uint8_t chan[2*256];
} SPECTRUM_t;

/* FUNCTIONS*/
void InitLiulinTask(
    LIULIN_t *l,
    uint32_t timeiInterval,
#ifdef USE_SOFTWARE_SERIAL_LIUL_READ
    SoftwareSerial *
#else
    HardwareSerial *
#endif
); // TimerHandle_t *hTimer );

static void LiulTimerCallback(TimerHandle_t timer_h);
static void LiulReadTask(void *params);

#endif
