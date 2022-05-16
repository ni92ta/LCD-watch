/* PCF8563
 * File:   I2C.h
 * Author: Никита
 *
 * Created on 31 марта 2021 г., 13:34
 */

#ifndef I2C_H
#define	I2C_H
#define SCL TRISA1//
#define SDA TRISA0 //TRISC4
#define SCL_IN RA1 
#define SDA_IN RA0
#define _XTAL_FREQ 70000
#define dev_addrw 0b10100010 //запись
#define dev_addrr 0b10100011 //чтение
//----------------------------------------
void i2c_start(void);
void i2c_stop (void);
void I2C_SendByte (unsigned char d);
int I2C_ReadByte (void);
int I2C_ReadByte_last (void);

#endif	/* I2C_H */

