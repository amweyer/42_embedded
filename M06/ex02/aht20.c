
#include "aht20.h"

void aht20_calibrate()
{
    uart_debug("sensor calibration\r\n");

    uint8_t status;

    i2c_start();
    i2c_address(SLA_W);
    i2c_write(0x71);
    i2c_stop();

    _delay_ms(10);

    i2c_start();
    i2c_address(SLA_R);
    i2c_read(1);
    status = TWDR;
    i2c_stop();
    
    if(!(status & 0x08))
    {
        print_status("aht20_calibrate");
        i2c_start();
        i2c_address(SLA_W);
        i2c_write(0xBE);
        i2c_write(0x08);
        i2c_write(0x00);
        i2c_stop();
    }
    _delay_ms(10);
}


void aht20_trigger_measurement()
{

    uart_debug("trigger measurements\r\n");

    i2c_start();
    i2c_write(SLA_W);
    
    _delay_ms(10);
    i2c_write(0xAC);
    i2c_write(0x33);
    i2c_write(0x00);
    // _delay_ms(80);

    i2c_stop();
    
    _delay_ms(80);

    // print_status();
}

void  print(int32_t val)
{
    int flag = 0;
    for(uint32_t i = 1000000000; i > 0 ; i/=10 )
    {
        if(val / i != 0)
            flag = 1;          
        if((val/i==0) && (!flag) && (i != 1))
            continue;
        uart_tx(val/i  + '0');
        val %= i;
    }
}

void ath20_read_data(void)
{
    uint8_t data[7];

    i2c_start();
    i2c_write(SLA_R);

    for(int i = 0; i < 7; i++)
    {
        i2c_read(i==6);
        data[i] = TWDR;
    }

    i2c_stop();

    show_data(data);
}

void show_data(uint8_t *data)
{


    static uint8_t average = 0;
    float temp = 0;
    float humidity = 0;

    char temp_str[10];
    char humidity_str[10];

    static float temp_tab[3] = {0}; 
    static float humidity_tab[3] = {0}; 

    temp_tab[2] = temp_tab[1];
    temp_tab[1] = temp_tab[0];
    temp_tab[0] = ath20_get_temp_milli(data);
    uart_debug("passe\r\n");


    humidity_tab[2] = humidity_tab[1];
    humidity_tab[1] = humidity_tab[0];
    humidity_tab[0] = ath20_get_humidity(data);
    
    if (average < 3) average++;

   for (uint8_t i = 0; i < average; i++)
    {
        temp += temp_tab[i];
        humidity += humidity_tab[i];
    } 


    dtostrf(temp/average, 1,0, temp_str ); //precison -+2
    dtostrf(humidity/average, 1,1, humidity_str ); //precison+-0.3

    uart_printstr("Temperature:");
    uart_printstr(temp_str);
    uart_printstr("C, ");

    uart_printstr("Humidity:");
    uart_printstr(humidity_str);
    uart_printstr("\%\r\n");
    
}


uint32_t ath20_get_temp_milli(uint8_t *data)
{
    uint32_t t_signal;
     
    t_signal = (((uint32_t)(data[3] & 0b00001111) << 16) | (((uint32_t)data[4]) << 8) | (uint32_t)data[5]);
    return ((t_signal*200/_2P20)-50);
}

uint32_t ath20_get_humidity(uint8_t *data)
{
    uint32_t h_signal = 0;
     
    h_signal = (((uint32_t)data[1]  << 12) | (((uint32_t)data[2]) << 4) | (uint32_t)(data[3] >> 4)  );
    return (h_signal*100/_2P20);
}

