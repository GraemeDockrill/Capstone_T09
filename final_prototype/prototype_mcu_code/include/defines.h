

#ifndef header_h
#define header_h

// pin definitions
#define LOAD_CELL1_SCK_PIN 27
#define LOAD_CELL2_SCK_PIN 13
#define LOAD_CELL1_DATA_PIN 28
#define LOAD_CELL2_DATA_PIN 41


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

// declare received UART message bytes
int start_byte;
int cmd_byte0;
int data_byte0;
int data_byte1;
int data_byte2;
int data_byte3;
int esc_byte;

// declare combined UART data bytes
int data_int0;
int data_int1;

int new_data = 0;
int received_char;

int ESCByte = 0;        // for sending over UART

#define QUEUE_MAX_LENGTH 100
#define BUFFER_MAX_LENGTH 100
#define RECEIVE_MSG_SIZE 7
#define LOAD_CELL_STABILIZATION_TIME_MS 2000
#define LOAD_CELL_TARE true
#define LOAD_CELL1_CALIBRATION_FACTOR 1000
#define LOAD_CELL2_CALIBRATION_FACTOR 1000

int load_cell1_rdy = 0;
int load_cell2_rdy = 0;
static boolean load_cell_data_rdy = false;

class CircularBuffer{
public:
    
    // function for creating the buffer
    void CircularBuffer::createCircularBuffer(int size){
        _buffer = (unsigned int*) malloc(size * sizeof(unsigned int));
        _size = size;
        _front = -1;
        _rear = 0;
        _count = 0;
    }

    // returns 1 if buffer is full, else 0
    int CircularBuffer::isFull(){
        return (_rear + 1) % _size == _front;
    }

    // returns 1 if buffer empty, else 0
    int CircularBuffer::isEmpty(){
        return _front == -1;
    }

    // returns number of buffer items
    int CircularBuffer::queueCount(){
        return _count;
    }

    // enqueues a given int
    void CircularBuffer::enqueue(int data){
        if(isFull())
            return;
        if(isEmpty())
            _front = _rear = 0;
        else
            _rear = (_rear + 1) % _size;

        _buffer[_rear] = data;
        _count = _count + 1;
        return;
    }

    // dequeues an int from the buffer, returns int if works, -1 otherwise
    int CircularBuffer::dequeue(){
        if(isEmpty())
            return -1;
        int data = _buffer[_front];
        if(_front == _rear)
            _front = _rear = -1;
        else
            _front = (_front + 1) % _size;

        _count = _count - 1;
        return data;
    }

    private:
        // buffer variables
        unsigned int* _buffer;
        int _size;
        int _front;
        int _rear;
        int _count;
};

#endif