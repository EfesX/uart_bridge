#ifndef __I2C_CDC_H
#define __I2C_CDC_H

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

typedef enum{
	TX = 0x01,
	RX = 0x02
} I2C_DIR;

typedef struct{
	uint8_t devAddr;
	uint8_t regAddr;
	uint8_t cntByte;
	char *ptrBuf;
	uint8_t i2cDataBuf[10];
	char cdcTxBuf[255];
}cdcI2CHandleTypedef;

void cdc_i2c_run(uint8_t *buf);



#endif /* __I2C_CDC_H */
