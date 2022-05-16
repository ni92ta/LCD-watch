/* PIC16F628
 * File:   LCD1621.h
 * Author: Никита
 *
 * Created on 31 марта 2021 г., 13:46
 */

#ifndef LCD1621_H
#define	LCD1621_H
#define _XTAL_FREQ 70000
#define CS RB4//RB6 выбор кристалла микросхемы holtek1621b. 12 вывод мк. RC5 RA2
#define WR RB5//RB5 тактовый сигнал микросхемы holtek1621b. 11 вывод мк. RC4 RA1
#define data RB6//RB4 данные микросхемы holtek1621b. 10 вывод мк. RC3 RA0
//------------------------------------
    void sendbyte(unsigned char adress);
    void init_1621(unsigned char inform);
    void i1621_adre(unsigned char adresss);
#endif	/* LCD1621_H */

