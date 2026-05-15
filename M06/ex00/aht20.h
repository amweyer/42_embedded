
#ifndef AHT20_H
#define AHT20_H

#define SLA_W 0b01110000
#define SLA_R 0b01110001


#include <avr/io.h>
#include <util/delay.h>


#include "uart.h"
#include "i2c.h"
#include "helper.h"

void aht20_calibrate();
void ath20_read_data(void);
void aht20_trigger_measurement();


#endif