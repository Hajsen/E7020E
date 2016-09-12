#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

typedef enum {RED, GREEN, BLUE}colour;

typedef struct {
	int temp_interval[2];
	int colour_combination[3];
}LedTemperature;

LedTemperature temperatureInterval[10];


extern int colour_combinations[10][3];


void initPWMGPIO();
void initPWMTimer();
void init_RGBColours();
void setSingleLEDColour(colour LED, int colour_strength);
void setRGBLEDColour(int temperature);

#endif
