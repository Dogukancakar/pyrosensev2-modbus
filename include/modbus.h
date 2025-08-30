#ifndef MODBUS_H
#define MODBUS_H

#include <ModbusMaster.h>

extern ModbusMaster node;
extern HardwareSerial mySerial;

void read_ss(uint8_t slaveAddress);
void read_flow(uint8_t slaveAddress);
void setup_modbus();
void write_slave_ss(uint8_t slaveAddress, uint16_t value);

#endif // MODBUS_H