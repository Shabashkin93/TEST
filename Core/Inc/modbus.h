#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdint.h>
#include "usart.h"

#define NUMBER_BAUD_RATES 6
#define RX_BUF_SIZE 256
#define NUMBER_OF_TEAM 12
#define BUF_SIZE 64

extern uint16_t currentWaitTime;
extern uint8_t modbusBuffer;
extern uint8_t endModbusMessage;

typedef struct {
uint8_t idxIN;
uint8_t idxOUT;
uint8_t rx_buf[RX_BUF_SIZE];    //кольцевой буфер
uint8_t buffer[BUF_SIZE];       // буфер комманды
uint8_t word_lenght;          //длина принятого слова
uint8_t buf;                  //переменная хранящая принимаемый символ
uint8_t command_on;           //флаг что команда прочитана, те встречен "\n"
} values_ring_buffer;

void setCurrentWaitTime(UART_HandleTypeDef *currentUart);
void monitor(values_ring_buffer *options);

#endif/* __MODBUS_H */
