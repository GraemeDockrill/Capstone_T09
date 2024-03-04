

#include <dataLogger.h>

int load_cell1_rdy = 0;
int load_cell2_rdy = 0;
int load_cell_data_rdy = 0;

// define load cell objects
HX711_ADC load_cell1(LOAD_CELL1_DATA_PIN, LOAD_CELL1_SCK_PIN);
HX711_ADC load_cell2(LOAD_CELL2_DATA_PIN, LOAD_CELL2_SCK_PIN);

void LoggingDataReadThread(void* arg){
  while(1){

    // if(load_cell1.update() & load_cell2.update())
    //   load_cell_data_rdy = true;

    // if(load_cell_data_rdy){

    //   // read load cells
    //   float load_cell1_data = load_cell1.getData();
    //   float load_cell2_data = load_cell2.getData();

    //   // read encoders
    //   short enc1_data = (short) encoder1.read();
    //   short enc2_data = (short) encoder2.read();
    //   short enc3_data = (short) encoder3.read();
    //   short enc4_data = (short) encoder4.read();

    //   // take empty buffer semaphore
    //   xSemaphoreTake(empty, portMAX_DELAY);

    //   // create and add message to queue
    //   logger_queue_message.time_stamp = (int) millis();
    //   logger_queue_message.data1 = load_cell1_data;
    //   logger_queue_message.data2 = load_cell2_data;
    //   logger_queue_message.data3 = enc1_data;
    //   logger_queue_message.data4 = enc2_data;
    //   logger_queue_message.data5 = enc3_data;
    //   logger_queue_message.data6 = enc4_data;

    //   // enqueue message for serial thread
    //   xQueueSend(data_logger_queue, (void *) &logger_queue_message, 0);

    //   // return new full buffer semaphore
    //   xSemaphoreGive(full);

    // }

    y3 = computeY1(x);
    y2 = computeY2(x);

    // take empty buffer semaphore
    xSemaphoreTake(empty, portMAX_DELAY);

    // add data to message queue struct
    logger_queue_message.time_stamp = (int) millis();
    logger_queue_message.data1 = y3;
    logger_queue_message.data2 = y2;
    logger_queue_message.data3 = computeEncoderTest(x);
    logger_queue_message.data4 = computeEncoderTest(x);
    logger_queue_message.data5 = computeEncoderTest(x);
    logger_queue_message.data6 = computeEncoderTest(x);

    // enqueue message for serial thread
    xQueueSend(data_logger_queue, (void *) &logger_queue_message, 0);

    // return new full buffer semaphore
    xSemaphoreGive(full);

    x += 0.1;

    // Sleep for 100 milliseconds.
    vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);

  }
}

void initializeLoadCells(void){
    // initialize load cells
    load_cell1.begin();
    load_cell2.begin();

    // startup, stabilization, and tare each module at the same time
    while(load_cell1_rdy + load_cell2_rdy < 2){
        if(!load_cell1_rdy) load_cell1_rdy = load_cell1.startMultiple(LOAD_CELL_STABILIZATION_TIME_MS, LOAD_CELL_TARE);
        if(!load_cell2_rdy) load_cell2_rdy = load_cell2.startMultiple(LOAD_CELL_STABILIZATION_TIME_MS, LOAD_CELL_TARE);
    }

    // calibrate load cells
    load_cell1.setCalFactor(LOAD_CELL1_CALIBRATION_FACTOR);
    load_cell2.setCalFactor(LOAD_CELL2_CALIBRATION_FACTOR);
}

