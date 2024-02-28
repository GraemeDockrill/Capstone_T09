

#ifndef header_h
#define header_h

// struct for message to be added to serial task queue
struct QueueMessage{
    int time_stamp;
    float data1;
    float data2;
    short data3;
    short data4;
    short data5;
    short data6;
} queue_message;

// define union for UART message
union u_message {
   struct QueueMessage parsed_message ; 
   byte temp_byte[20] ;
} COM_message;

// define bit constants
int BIT0 = 0x0001;
int BIT1 = 0x0002;
int BIT2 = 0x0004;
int BIT3 = 0x0008;
int BIT4 = 0x0016;
int BIT5 = 0x0032;
int BIT6 = 0x0064;
int BIT7 = 0x0128;

int ESCByte = 0;

#define QUEUE_MAX_LENGTH 100


#endif