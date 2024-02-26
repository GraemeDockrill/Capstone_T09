

#ifndef header_h
#define header_h

// define union for UART message
union u_message {
   long temp_long ; 
   byte temp_byte[8] ;
} message;

union u_timestamp {
    int temp_int ;
    byte temp_byte[4] ;
} timestamp;

struct number{
    float x;
    float y;
} num;

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