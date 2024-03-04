

#ifndef dataLogger_h
#define dataLogger_h

#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <HX711_ADC.h>
#include <Encoder.h>
#include <math.h>
#include <defines.h>
#include <serialCommunicator.h>
#include <motorControl.h>

// declare HX711 objects
extern HX711_ADC load_cell1;
extern HX711_ADC load_cell2;

// declare thread function for thread 1
// computes y = x*x every 100 ms and adds it to buffer
void LoggingDataReadThread(void* arg);

void initializeLoadCells(void);

#endif