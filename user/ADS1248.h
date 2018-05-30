#ifndef _ADS1248_H_
#define _ADS1248_H_
#include "stm32f4xx.h"


#define MUX0 		0x00
#define VBIAS 		0x01
#define MUX1 		0x02
#define SYS0 		0x03
#define OFC0 		0x04
#define OFC1 		0x05
#define OFC2 		0x06
#define FSC0 		0x07
#define FSC1 		0x08
#define FSC2 		0x09
#define IDAC0 		0x0A
#define IDAC1 		0x0B
#define GPIOCFG 	0x0C
#define GPIODIR 	0x0D
#define GPIODAT 	0x0E




#define NOP			0xFF
#define RDATA		0x12		//0x13
#define RDATAC		0X14
#define RREG		0x20
#define WREG		0x40
#define SYNC		0x04


void Read_ADCData(void);
void Init_ADC1248(void);
extern int ADC1248_data[8];


#endif  













