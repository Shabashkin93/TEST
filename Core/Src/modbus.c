#include "modbus.h"


uint16_t currentWaitTime=0;
uint8_t modbusBuffer;
uint8_t endModbusMessage=0;

struct timesBetweenFrames{
    uint32_t baudRate;
    uint16_t waitTime;
};

struct timesBetweenFrames tableWaitTime[]={
    {2400, 16000},
    {4800,8000},
    {9600,4000},
    {19200,2000},
    {38400,1000},
    {115200,330}
};

void setCurrentWaitTime(UART_HandleTypeDef *currentUart){
    for (int i=0; i<NUMBER_BAUD_RATES; i++){
        if (currentUart->Init.BaudRate == tableWaitTime[i].baudRate){
            currentWaitTime = tableWaitTime[i].waitTime;
        }
    }
}

/* monitor - получает данные и кладёт их в кольцевой буфер, сигнализируя если принят '\n'*/
void monitor(values_ring_buffer *options){
    options->rx_buf[options->idxIN++]=options->buf;
    options->word_lenght++;
    if (endModbusMessage){
        for (int i=0;i<options->idxIN; i++){
            printf("%s",options->rx_buf[i]);   
        }
    }
}
