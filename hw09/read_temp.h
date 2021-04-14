// Define sensor name for one-wire DS18B20 thermometer
#define SENSOR_NAME "/sys/bus/w1/devices/28-000005aad857/w1_slave"

/* returns temperature in degrees C */
double read_temp(void);
