
#ifndef I2C_H
#define I2C_H


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "helper.h"

#define F_SCL 72

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(unsigned char data);
void i2c_address(uint8_t addr);
void i2c_read(uint8_t is_last);

#endif