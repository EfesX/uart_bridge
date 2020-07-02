#ifndef __CDC_I2C_H
#define __CDC_I2C_H

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

enum{
	TX = 0x01,
	RX = 0x02
}I2C_DIR;


void cdcCtrlFeedback(uint8_t *data, I2C_DIR){

}


#endif /* __CDC_I2C_H */