
#ifndef HELPER_H
#define HELPER_H

#define SET_BIT(REG, bit) REG |= _BV(bit)
#define UNSET_BIT(REG,bit) REG &= ~_BV(bit)
#define TOGGLE_BIT(REG,bit) REG ^= _BV(bit)

#include <avr/io.h>

#include "uart.h"

#define DEBUG 1

void print_data(char *data);
void print_hex_value(char c);
void  print_hex(uint8_t val);
void print_status(char *str);


#endif