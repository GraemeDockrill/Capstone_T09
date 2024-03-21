// serialCommunicator.h for MECH45X cell stretcher capstone
// Written by Graeme Dockrill - 2024

#ifndef serialCommunicator_h
#define serialCommunicator_h

#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <HX711_ADC.h>
#include <Encoder.h>
#include <AccelStepper.h>
#include <math.h>
#include <defines.h>
#include <dataLogger.h>
#include <motorControl.h>

#define ZERO_STRETCH_POS_STEPS 30000
#define MAX_TARGET_STEPS ZERO_STRETCH_POS_STEPS + 3680
#define MAX_AVG_SPD_SPS 

// declare semaphore handles
extern SemaphoreHandle_t motor_mutex, empty, full;

// declare queue
extern QueueHandle_t data_logger_queue;
extern QueueHandle_t motor_traj_queue;

// struct for message to be added to serial task queue
struct LoggerQueueMessage{
    int time_stamp;
    float data1;
    float data2;
    short data3;
    short data4;
    short data5;
    short data6;
} extern logger_queue_message;

// define union for UART message
union u_serial_message {
    struct LoggerQueueMessage parsed_message ; 
    byte temp_byte[20] ;
} extern COM_message;

// enum for control program command bytes
enum incoming_message{
    HOME_AXES,
    MANUAL_POS_MOVE,
    MANUAL_NEG_MOVE,
    SET_PARAMETERS,
    STRETCH_TO_MAX,
    RETURN_TO_ZERO,
    CYCLIC_STRETCHING,
    STOP
};

// declare thread function for thread 2
// takes any full buffer spots and sends data over UART
void SerialThread(void* arg);

// implementation of a circular buffer for dequeueing serial data
class CircularBuffer{
public:
    
    // function for creating the buffer
    void createCircularBuffer(int size);

    // returns 1 if buffer is full, else 0
    int isFull();

    // returns 1 if buffer empty, else 0
    int isEmpty();

    // returns number of buffer items
    int queueCount();

    // enqueues a given int
    void enqueue(int data);

    // dequeues an int from the buffer, returns int if works, -1 otherwise
    int dequeue();

private:
    // buffer variables
    unsigned int* _buffer;
    int _size;
    int _front;
    int _rear;
    int _count;
};

// declare circular buffer
extern CircularBuffer serial_buffer;

#endif