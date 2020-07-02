#include "i2c_cdc.h"



void cdc_i2c_feedback(cdcI2CHandleTypedef *hcdci2c, I2C_DIR dir, HAL_StatusTypeDef status){
	switch(dir){
		case TX: 
				CDC_Transmit_FS((uint8_t *)"===================== TX ==========================\r\n", strlen("===================== TX ==========================\r\n"));
				break;
		case RX:
				CDC_Transmit_FS((uint8_t *)"===================== RX ==========================\r\n", strlen("===================== RX ==========================\r\n"));
				break;
		default: break;
	}
	HAL_Delay(5);
	
	sprintf(hcdci2c->cdcTxBuf, "Device's address: 0x%x\r\n", hcdci2c->devAddr);
	while(CDC_Transmit_FS((uint8_t *)hcdci2c->cdcTxBuf, strlen(hcdci2c->cdcTxBuf)) != USBD_OK);
	HAL_Delay(5);	
	
	sprintf(hcdci2c->cdcTxBuf, "Register's address: 0x%x\r\n", hcdci2c->regAddr);
	while(CDC_Transmit_FS((uint8_t *)hcdci2c->cdcTxBuf, strlen(hcdci2c->cdcTxBuf)) != USBD_OK);
	HAL_Delay(5);	
	
	sprintf(hcdci2c->cdcTxBuf, "Bytes count: %d\r\n", hcdci2c->cntByte);	
	while(CDC_Transmit_FS((uint8_t *)hcdci2c->cdcTxBuf, strlen(hcdci2c->cdcTxBuf)) != USBD_OK);
	HAL_Delay(5);	
	
	for(uint8_t i = 0; i < hcdci2c->cntByte; i++){
		sprintf(hcdci2c->cdcTxBuf, "Data[%d]: 0x%x\r\n", i, hcdci2c->i2cDataBuf[i]);
		while(CDC_Transmit_FS((uint8_t *)hcdci2c->cdcTxBuf, strlen(hcdci2c->cdcTxBuf)) != USBD_OK);
		//CDC_Transmit_FS((uint8_t *)hcdci2c->cdcTxBuf, strlen(hcdci2c->cdcTxBuf));
		HAL_Delay(50);
	}
	HAL_Delay(50);
	switch(status){
		case HAL_OK: CDC_Transmit_FS((uint8_t *)"STATUS: OK\r\n", strlen("STATUS: OK\r\n")); break;
		case HAL_ERROR: CDC_Transmit_FS((uint8_t *)"STATUS: ERROR\r\n", strlen("STATUS: ERROR\r\n")); break;
		case HAL_BUSY: CDC_Transmit_FS((uint8_t *)"STATUS: BUSY\r\n", strlen("STATUS: BUSY\r\n")); break;
		case HAL_TIMEOUT: CDC_Transmit_FS((uint8_t *)"STATUS: TIMEOUT\r\n", strlen("STATUS: TIMEOUT\r\n")); break;
		default: break;
	}
	HAL_Delay(50);
	CDC_Transmit_FS((uint8_t *)"===================================================\r\n", strlen("===================================================\r\n"));
}


void cdc_parse(cdcI2CHandleTypedef *hcdci2c, uint8_t *buf){
	hcdci2c->devAddr = (uint8_t) strtol((char *)buf, &hcdci2c->ptrBuf, 16);
	hcdci2c->regAddr = (uint8_t) strtol(hcdci2c->ptrBuf, &hcdci2c->ptrBuf, 16);
	hcdci2c->cntByte = (uint8_t) strtol(hcdci2c->ptrBuf, &hcdci2c->ptrBuf, 10);
	for(uint8_t i = 0; i < hcdci2c->cntByte; i++){
		hcdci2c->i2cDataBuf[i] = (uint8_t) strtol(hcdci2c->ptrBuf, &hcdci2c->ptrBuf, 16);
	}
}


extern I2C_HandleTypeDef hi2c1;
cdcI2CHandleTypedef hcdci2c;
uint8_t rxdata = 0;;
uint8_t txdata = 0x1f;


void cdc_i2c_run(uint8_t *buf){
	HAL_StatusTypeDef status;
	switch(buf[0]){
		case 'W': 
			buf[0] = ' ';
			cdc_parse(&hcdci2c, &buf[0]);
			status = HAL_I2C_Mem_Write(&hi2c1, (uint16_t) hcdci2c.devAddr, hcdci2c.regAddr, I2C_MEMADD_SIZE_8BIT, hcdci2c.i2cDataBuf, hcdci2c.cntByte, HAL_MAX_DELAY);
			cdc_i2c_feedback(&hcdci2c, TX, status);
			break;
		
			case 'R':
				buf[0] = ' ';
				cdc_parse(&hcdci2c, &buf[0]);
				status = HAL_I2C_Mem_Read(&hi2c1, (uint16_t) hcdci2c.devAddr, hcdci2c.regAddr, I2C_MEMADD_SIZE_8BIT, hcdci2c.i2cDataBuf, hcdci2c.cntByte, HAL_MAX_DELAY);
				cdc_i2c_feedback(&hcdci2c, RX, status);
				break;
		default: break;
	}
	
}
	


