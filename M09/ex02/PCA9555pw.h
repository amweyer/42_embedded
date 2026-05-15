#ifndef PCA9555PW_H
#define PCA9555PW_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

#include "i2c.h"



#define SLA 0b01000000
#define DEFAULT 0b00000000
#define A0 0b00000010
#define A1 0b00000100
#define A2 0b00001000


typedef enum s_pca9555pw_cmd_lst
{
    INPUT_PORT_0,
    INPUT_PORT_1,
    OUTPUT_PORT_0,
    OUTPUT_PORT_1,
    POLARITY_INVERSION_PORT_0,
    POLARITY_INVERSION_PORT_1,
    CONFIGURATION_PORT_0,
    CONFIGURATION_PORT_1
} t_pca9555pw_cmd_lst;

void PCA955_i2c_connect(uint8_t addr, uint8_t prog, uint8_t mode);

void PCA955_set_register(t_pca9555pw_cmd_lst cmd, uint8_t value);

#endif