/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amweyer <amweyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 20:10:21 by amweyer           #+#    #+#             */
/*   Updated: 2026/04/30 20:54:35 by amweyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "uart.h"

char *cmd_lst[6] = {"STATUS", "SET_ID", "SET_PRIO", "SET_TAG", "FACTORY_RESET", 0};

typedef enum s_cmd_lst
{
    STATUS,
    SET_ID,
    SET_PRIO,
    SET_TAG,
    FACTORY_RESET
} t_cmd_lst;





typedef struct s_net_node
{
    uint32_t magic_number;
    //identifier bloc de donnee (adresse ?)
    uint32_t ID;
    //
    int16_t priority;
    //
    char tag[33];
    //32 char alnum + _- + terminateur nul ?
    // unkwn_t integrity;
    //16 bits min
    //Checksum additif 16-bit
    // ou Checksum Fletcher
    // ou CRC-16
}   t_net_node;

//16 char long buff only
void fill_command(uint8_t *command)
{

    char c;
    uint8_t buf = 0;
    while(1)
    {
        c = uart_rx();
        if(c == 127 && buf > 0) //del
        {
            buf--;
            uart_printstr("\b \b");

            continue;
        }
        if(c == '\r')
        {
            command[buf] = '\0';
            uart_printstr("\r\n");
            return;
        }
        else if ((c >=33 && c <= 127) && buf < 15)
        {
            command[buf] = c;
            uart_tx(c);
            buf++;
        }
    
    }
  
}

int str_comp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int cmd_id(char *cmd)
{
    for (int i = 0; cmd_lst[i]; i++)
    {
        if (!str_comp(cmd, cmd_lst[i]))
            return (i);
    }
    return (-1);
}

int main()
{
    char command[16];
    
    uart_init(UBRRnL);
    
    while (1)
    {  
        uart_printstr("> ");
        fill_command(command);
        switch (cmd_id(command))
        {
			case STATUS:
				uart_printstr("Status \r\n");
				break;
			case SET_ID:
				uart_printstr("Set id \r\n");
				break;
			case SET_PRIO:
				uart_printstr("Set prio \r\n");
				break;
			case SET_TAG:
				uart_printstr("Set tag \r\n");
				break;
			case FACTORY_RESET:
				uart_printstr("Factory reset \r\n");
				break;
        	default:
				uart_printstr("Invalid command \r\n");
        	    break;
        }
        

        
    }
}

