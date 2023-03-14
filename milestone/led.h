#define LED0_TRIGGER "/sys/class/leds/beaglebone:green:usr0/trigger"
#define LED1_TRIGGER "/sys/class/leds/beaglebone:green:usr1/trigger"
#define LED2_TRIGGER "/sys/class/leds/beaglebone:green:usr2/trigger"
#define LED3_TRIGGER "/sys/class/leds/beaglebone:green:usr3/trigger"

#define LED0_BRIGHTNESS "/sys/class/leds/beaglebone:green:usr0/brightness"
#define LED1_BRIGHTNESS "/sys/class/leds/beaglebone:green:usr1/brightness"
#define LED2_BRIGHTNESS "/sys/class/leds/beaglebone:green:usr2/brightness"
#define LED3_BRIGHTNESS "/sys/class/leds/beaglebone:green:usr3/brightness"

void LED_initialize(void);
void LED_setTrigger(char *fileName, char *triggerOption);
void LED_powerOn(char *fileName);
void LED_powerOff(char *fileName);
void LED_powerAll(void);
void LED_dimAll(void);