
#include "i2c.h"

void i2c_init()
{
    TWBR = F_SCL; 
    TWSR = 0; // Prescaler à 1
}


void i2c_start()
{

    TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));    //wait to see if start has been transmitted

    print_status("i2c_start");
}

void i2c_stop()
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
    while (!(TWCR & (1<<TWSTO)));    //wait to see if start has been transmitted
}

void i2c_write(unsigned char data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));    //wait to see if start has been transmitted

    print_status("i2c_write");
}


void i2c_address(uint8_t addr)
{
    i2c_write(addr);
    print_status("i2c_address");

}

void i2c_read(uint8_t is_last) 
{
    if (is_last) 
    {
        TWCR = (1 << TWINT) | (1 << TWEN); //if last NACK
    } 
    else 
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    }
    while (!(TWCR & (1 << TWINT)));
    print_status("i2c_read");

}