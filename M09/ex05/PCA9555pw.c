
#include "PCA9555pw.h"

void PCA955_i2c_connect(uint8_t addr, uint8_t prog, uint8_t mode)
{
    i2c_address(addr | prog, mode);
}

void PCA955_set_register(t_pca9555pw_cmd_lst cmd, uint8_t value)
{
    i2c_start();
    PCA955_i2c_connect(SLA, DEFAULT, W);
    i2c_write(cmd);
    i2c_write(value);
    i2c_stop();
}
