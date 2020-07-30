#ifndef SEVENSEG_H
#define SEVENSEG_H
#include <xc.h>
#include <stdint.h>
#include "Dio.h"
#include "Port.h"
#include "Dio_Regs.h"





//void 7seg_ini

unsigned char display7s(unsigned char v);
void display(unsigned char value,unsigned char num);
void sevenSeg_init(void);

#endif