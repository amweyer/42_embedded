

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mapping.h"
#include "helper.h"

// #define F_CPU 16000000
// #define UART_BAUDRATE 115200
// #define UBRRnL (F_CPU/16/UART_BAUDRATE-1)
// #define UBRRnL ((F_CPU) / (16 * UART_BAUDRATE)) - 1

#define UBRRnL 8 //p199




void uart_init(unsigned int ubrr);
void uart_tx(char c);
char uart_rx( void );
void uart_printstr(const char* str);
void uart_debug(const char* str);

//add
extern char color_str[13];

int valid_char(unsigned char c);
void uart_getstr(char field[8]);
int valid_color_str();
uint8_t atoi_hex(char *clr);
void fill_colors();
void to_upper(char *str);
void get_input();

int str_comp(const char *s1, const char *s2);


#endif
