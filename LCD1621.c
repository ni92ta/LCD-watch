/*
 * File:   LCD1621.c
 * Author: Никита
 *
 * Created on 31 марта 2021 г., 13:45
 */
#include <xc.h>
#include "LCD1621.h"
unsigned char i;
//-------------------отправка команды------------------
void sendbyte(unsigned char adress){
    CS=0;
               WR=0;
         data = 1;
                 WR=1;
         __delay_us(1);
         WR=0;
         data = 0;
         
         WR=1;
         __delay_us(1);
         WR=0;
         data = 0;
        
         WR=1;
         __delay_us(1); 
         WR=0;
         data = 0;
        
         WR=1; 
  __delay_us(1);
     for(i=0;i<8;i++)
     {
               WR=0;
         if(adress&0x80)
         {
             data = 1;
             
         }
         else
         {
             data=0;
            
         }
       __delay_us(1); 
       adress=(adress<<1);
       __delay_us(1);
       WR=1;
  __delay_us(1);   
     }
 CS=1;   
}
//-----------------отправка адреса--------------------
void i1621_adre(unsigned char adresss)
{
         WR=0;
         data = 1;
         WR=1;
         __delay_us(1);
         WR=0;
         data = 0;
         WR=1;
         __delay_us(1);
         WR=0;
         data = 1;
         WR=1;
         __delay_us(1);   
                  for(i=0;i<6;i++)
     {
         WR=0;
         if(adresss&0x20)
         {
             data = 1;
         }
         else
         {
             data = 0;
         }
       __delay_us(1); 
       adresss=(adresss<<1);
       __delay_us(1);
       WR=1;
  __delay_us(1);   
     } 
}
//-----------------отправка байта данных--------------------
void init_1621(unsigned char inform){
    for(i=0;i<4;i++)
     {
               WR=0;
         if(inform&0x8)
         {
             data = 1;
         }
         else
         {
             data = 0;
         }
       __delay_us(1); 
       inform=(inform<<1);
       __delay_us(1);
       WR=1;
  __delay_us(1);   
     } 
}


