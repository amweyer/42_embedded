
#ifndef AHT20_H
#define AHT20_H

#define SLA_W 0b01110000
#define SLA_R 0b01110001

#define _2P20 1048576

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdlib.h>

#include "uart.h"
#include "i2c.h"
#include "helper.h"

void aht20_calibrate();
void ath20_read_data(void);
void aht20_trigger_measurement();
uint32_t ath20_get_temp_milli(uint8_t *data);
uint32_t ath20_get_humidity(uint8_t *data);
void show_data(uint8_t *data);
void get_average(uint8_t **data_tab, uint8_t av);


#endif