// serialCommunicator.cpp for MECH45X cell stretcher capstone
// deals with communication between PC and MCU and between tasks
// Written by Graeme Dockrill - 2024

#include <serialCommunicator.h>

// declare received UART message bytes
int start_byte;
int cmd_byte0;
int data_byte0;
int data_byte1;
int data_byte2;
int data_byte3;
int esc_byte;           // for receiving over UART

// declare combined UART data bytes
int data_int0;
int data_int1;

int new_data = 0;
int received_char;

int ESCByte = 0;        // for sending over UART

int target_stretch_pos_steps = 0;
int avg_spd_sps = 0;
bool parameters_valid = false;

// create semaphores and queue for thread safe queue
SemaphoreHandle_t motor_mutex = xSemaphoreCreateCounting(1,1);
SemaphoreHandle_t full = xSemaphoreCreateCounting(QUEUE_MAX_LENGTH, 0);
SemaphoreHandle_t empty = xSemaphoreCreateCounting(QUEUE_MAX_LENGTH, QUEUE_MAX_LENGTH);
QueueHandle_t data_logger_queue = xQueueCreate(QUEUE_MAX_LENGTH, 20);
QueueHandle_t motor_traj_queue = xQueueCreate(QUEUE_MAX_LENGTH, 20);

LoggerQueueMessage logger_queue_message;
u_serial_message COM_message;
CircularBuffer serial_buffer;

void SerialThread(void* arg){
  
  pinMode(LED_BUILTIN, OUTPUT);

  while(1){
    // check if byte waiting in serial
    if(Serial.available() > 0){
      serial_buffer.enqueue(Serial.read());     // enqueue received byte
    }
    // if length of buffer is long enough for a message
    if(serial_buffer.queueCount() >= RECEIVE_MSG_SIZE){
      // dequeue received bytes
      start_byte = serial_buffer.dequeue();
      cmd_byte0 = serial_buffer.dequeue();
      data_byte0 = serial_buffer.dequeue();
      data_byte1 = serial_buffer.dequeue();
      data_byte2 = serial_buffer.dequeue();
      data_byte3 = serial_buffer.dequeue();
      esc_byte = serial_buffer.dequeue();

      // digitalWrite(LED_BUILTIN, HIGH);

      // if switching back using esc_byte
      if(esc_byte & BIT3)
        data_byte0 = 255;
      if(esc_byte & BIT2)
        data_byte1 = 255;
      if(esc_byte & BIT1)
        data_byte2 = 255;
      if(esc_byte & BIT0)
        data_byte3 = 255;

      // combining bytes back together
      data_int0 = data_byte0 << 8;
      data_int0 = data_int0 + data_byte1;
      data_int1 = data_byte2 << 8;
      data_int1 + data_int1 + data_byte3;

      // if start byte is valid
      if(start_byte == 255){
        // take mutex for updating motor target
        xSemaphoreTake(motor_mutex, portMAX_DELAY);

        // parse data_int0 and data_int1 according to cmd_byte0
        switch(cmd_byte0){
          case HOME_AXES:         // cmd_byte0 = 0
            digitalWrite(LED_BUILTIN, HIGH);
          break;
          case MANUAL_POS_MOVE:   // cmd_byte0 = 1
            
          break;
          case MANUAL_NEG_MOVE:   // cmd_byte0 = 2
            
          break;
          case SET_PARAMETERS:    // cmd_byte0 = 3
            target_stretch_pos_steps = ZERO_STRETCH_POS_STEPS + data_int0;
            avg_spd_sps = data_int0 * ((double)data_int1 / 100.0);

            // ADD CHECKS IF PARAMETERS ARE VALID
            // if(....)
            parameters_valid = true;
            // else
            // parameters_valid = false;
          break;
          case STRETCH_TO_MAX:    // cmd_byte0 = 4
            // initialize message for motor trajectory
            Trajectory_Params_t motor_traj_message;

            // if membrane parameters valid and no messages in queue
            if(parameters_valid && !xQueuePeek(motor_traj_queue, (Trajectory_Params_t *) &motor_traj_message, 0)){
              // create trajectory message to send to motor control thread
              // motor 1
              motor_traj_message.motor1.initial_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor1.target_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor1.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor1.cycles = 1;
              // motor 2
              motor_traj_message.motor2.initial_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor2.target_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor2.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor2.cycles = 1;
              // motor 3
              motor_traj_message.motor3.initial_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor3.target_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor3.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor3.cycles = 1;
              // motor 4
              motor_traj_message.motor4.initial_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor4.target_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor4.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor4.cycles = 1;
              xQueueSend(motor_traj_queue, (Trajectory_Params_t *) &motor_traj_message, 0);
            }
          break;
          case RETURN_TO_ZERO:    // cmd_byte0 = 5
            // initialize message for motor trajectory
            Trajectory_Params_t motor_traj_message;

            // if membrane parameters valid and no messages in queue
            if(parameters_valid && !xQueuePeek(motor_traj_queue, (Trajectory_Params_t *) &motor_traj_message, 0)){
              // create trajectory message to send to motor control thread
              // motor 1
              motor_traj_message.motor1.initial_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor1.target_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor1.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor1.cycles = 1;
              // motor 2
              motor_traj_message.motor2.initial_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor2.target_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor2.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor2.cycles = 1;
              // motor 3
              motor_traj_message.motor3.initial_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor3.target_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor3.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor3.cycles = 1;
              // motor 4
              motor_traj_message.motor4.initial_pos_steps = target_stretch_pos_steps;
              motor_traj_message.motor4.target_pos_steps = ZERO_STRETCH_POS_STEPS;
              motor_traj_message.motor4.avg_speed_sps = avg_spd_sps;
              motor_traj_message.motor4.cycles = 1;
              xQueueSend(motor_traj_queue, (Trajectory_Params_t *) &motor_traj_message, 0);
            }
          break;
          case CYCLIC_STRETCHING: // cmd_byte0 = 6
            
          break;
          case STOP:              // cmd_byte0 = 7
            digitalWrite(LED_BUILTIN, LOW);

            // initialize message for motor trajectory
            Trajectory_Params_t motor_traj_message;

            // clear trajectory queue to stop any new movements
            while(xQueueReceive(motor_traj_queue, (Trajectory_Params_t *) &motor_traj_message, 0));
          break;
          default:
          break;
        }

        // return mutex
        xSemaphoreGive(motor_mutex);
      }
    }
    // check if there is something in the send queue
    if(uxQueueMessagesWaiting(data_logger_queue) > 0){

      // signal full spot is removed
      xSemaphoreTake(full, portMAX_DELAY);

      // remove message from queue
      xQueueReceive(data_logger_queue, (u_serial_message *) &COM_message.parsed_message, portMAX_DELAY);

      // send message over UART
      Serial.write(255);
      for(int i = 0; i < 20; i++){
        Serial.write(COM_message.temp_byte[i]);         // write data bytes
      }
      Serial.write(ESCByte);

      // signal a new queue spot has opened
      xSemaphoreGive(empty);

    }

  }
}

void CircularBuffer::createCircularBuffer(int size){
    _buffer = (unsigned int*) malloc(size * sizeof(unsigned int));
    _size = size;
    _front = -1;
    _rear = 0;
    _count = 0;
}

int CircularBuffer::isFull(){
    return (_rear + 1) % _size == _front;
}

int CircularBuffer::isEmpty(){
    return _front == -1;
}

int CircularBuffer::queueCount(){
    return _count;
}

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