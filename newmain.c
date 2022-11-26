/*
 * File:   newmain.c
 * Author: Никита
 *Часы на pic16f628  и PCF8563
 * Created on 22 июля 2021 г., 20:00
 */
#define _XTAL_FREQ 70000// низкая тактовая частота для снижения энергопотребления
#define dev_addrw 0b10100010 //запись
#define dev_addrr 0b10100011 //чтение
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include "newfile.h"
#include "I2C.h"
#include "LCD1621.h"
     unsigned char t = 0;//Флаг нажатия кнопки устанавливаем в 0, чтоб при сбросе питания часы стабильно запускались
     unsigned char alrm;
     unsigned char sece;//единицы секунд
     unsigned char secd;//десятки секунд
     unsigned char sec;//секунды до преобразования
     unsigned char min;//минуты до преобразования
     unsigned char min_alar;//минуты будильника до преобразования
     unsigned char mine;//единицы минут после преобразования
     unsigned char mind;//десятки минут после преобразования
     unsigned char mine_alar;//единицы минут будильника после преобразования
     unsigned char mind_alar;//десятки минут будильника после преобразования     
     unsigned char hour;//часы до преобразования
     unsigned char hour_alar;//часы будильника до преобразования
     unsigned char houre;//единицы часов после преобразования
     unsigned char hourd;//десятки часов после преобразования
     unsigned char houre_alar;//единицы часов будильника после преобразования
     unsigned char hourd_alar;//десятки часов будильника после преобразования     
     unsigned char minee;//переменная для настройки минут 
     unsigned char houree;//переменная для настройки часов
     unsigned char minee_alar;//переменная для настройки минут 
     unsigned char houree_alar;//переменная для настройки часов
     unsigned char control_2;//переменная для настройки часов
     unsigned char Days;//переменная числа до преобразования
     unsigned char Daysset;//переменная для настройки числа
     unsigned char Weekdays;//переменная дня недели до преобразования
     //unsigned char Weekdaysset;//переменная для настройки дня недели
     unsigned char Months;//переменная месяца до преобразования
     unsigned char Monthsset;//переменная для настройки месяца
     unsigned char Years;//переменная года до преобразования
     unsigned char Yearsset;//переменная для настройки года
     unsigned char Days_pre;//переменная числа после преобразования(единицы)
     unsigned char Days_prd;//переменная числа после преобразования(десятки)
     unsigned char Weekdays_pr;//переменная дня недели после преобразования
     unsigned char Months_pre;//переменная месяца после преобразования(единицы)
     unsigned char Months_prd;//переменная месяца после преобразования(десятки)
     //unsigned char Months_pr;//переменная месяца после преобразования
     unsigned char Years_pre;//переменная года после преобразования(единицы)
     unsigned char Years_prd;//переменная года после преобразования(десятки)
//------------------------------------------------------------------------------   
//-------------------массив с цифрами-------------------------------------------
unsigned char a[8]={  0b0110,0b1110,//0.1  0b0110,0b1110,   0b1111,0b1111,
                      0b0000,0b0100,//2.3  0b0000,0b0100,  0b1111,0b1111,
                      0b0010,0b1100,//4.5  0b0010,0b1100,  0b1111,0b1111,
                      0b1000,0b1010//6.7  0b1000,0b1010
                      // с точкой
                      };
//-------инициализация микросхемы LCD-------------------------------------------
void HT1621init(void){
        __delay_ms(20);
sendbyte(0b00000010);//sys en
__delay_ms(20);
sendbyte(0b00101000);//RC 32K 0b00110000
__delay_ms(20);
sendbyte(0b01001010);//bias&com
__delay_ms(20);
sendbyte(0b00000110);//lcd on 
__delay_ms(20);
sendbyte(0b10000000);//TONE 4k 
}
//------------------------------------------------------------------------------
void PCF8563init (void){//инициализация микросхемы
        __delay_ms(200);
      unsigned char control_3; 
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00000010);//вызов регистра секунд
      i2c_stop ();//отправка посылки СТОП 
      i2c_start ();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrr);//адрес часовой микросхемы - чтение
      control_3 = I2C_ReadByte_last();//чтение регистра 
      i2c_stop ();//
      if (0b10000000 & control_3 ){//читаем бит контроля, если было снижение питания, то время не настраиваем      
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00000010);//вызов регистра секунд
    I2C_SendByte (0b01000000);//установка секунд 40
    I2C_SendByte (0b00000000);//установка минут 00
    I2C_SendByte (0b11000000);//установка часов 00
    I2C_SendByte (0b00000001);//установка числа месяца 15
    I2C_SendByte (0b00000001);//установка дня недели 1
    I2C_SendByte (0b00000001);//установка месяца 1
    I2C_SendByte (0b00100001);//установка года 21
    I2C_SendByte (0b00000001);//установка будильника минуты 1
    I2C_SendByte (0b00000000);//установка будильника часы 0
    I2C_SendByte (0b00000000);//отключение будильника по дню недели 7 бит в единицу
    I2C_SendByte (0b00000000);//отключение будильника по неделе 7 бит в единицу
   // i2c_stop (); 
    //i2c_start ();//отправка посылки СТАРТ
   // I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись 
    //I2C_SendByte (0b00001101);//вызов регистра clock out
    I2C_SendByte (0b00000011);//включение делителя частоты 1Hz
    i2c_stop (); 
      }
}
//------------------------------------------------------------------------------
unsigned char RTC_ConvertFromDecd(unsigned char c,unsigned char v){//Перевод из двоичного формата в десятичный
    unsigned char ch;
    if (v==1){
      c = c>>4;
       ch = (0b00000011&c);   
    }
    if (v==0){
    ch = (c>>4);     
    }
    if (v==2){
          c = c>>4;
       ch = (0b00000001&c);  
    }
    return ch; 
}
//--------------------перевод из двоичного в единицы минут и часов--------------
unsigned char RTC_ConvertFromDec(unsigned char c){//
    unsigned char ch = (0b00001111&c);
    return ch;
}
//--------------вывод цифры на дисплей-------------------------------------------
void LCD_OUT (unsigned char s,unsigned char ss){// если нужна цифра с десятичной точкой, то SS=1
 switch (s)
    {
        case 0:
init_1621(a[1]);
init_1621(a[0]);
if (ss) {
    init_1621(a[1]);//0 с точкой
}
else{
  init_1621(a[5]);//0  
}
        break;
        case 1:
init_1621(a[2]);
init_1621(a[2]);
if (ss) {
    init_1621(a[1]);//1 с точкой
}
else{
init_1621(a[5]);//1 
}
        break;
        case 2:
init_1621(a[6]);
init_1621(a[1]);
if (ss) {
    init_1621(a[0]);//2 с точкой
}
else{
init_1621(a[3]);//2 
}
        break;
        case 3:
init_1621(a[2]);
init_1621(a[1]);
if (ss) {
    init_1621(a[1]);//3 с точкой
}
else{
init_1621(a[5]);//3
}
        break;
        case 4:
init_1621(a[3]);
init_1621(a[6]);
if (ss) {
    init_1621(a[1]);//4 с точкой
}
else{
init_1621(a[5]);//4
}
        break;
        case 5:
init_1621(a[3]);
init_1621(a[1]);
if (ss) {
    init_1621(a[7]);//5 с точкой
}
else{
init_1621(a[6]);//5
}
        break;
        case 6:
init_1621(a[1]);
init_1621(a[1]);
if (ss) {
    init_1621(a[7]);//6 с точкой
}
else{
init_1621(a[6]);//6
}
        break;
        case 7:
init_1621(a[2]);
init_1621(a[3]);
if (ss) {
    init_1621(a[1]);//7 с точкой
}
else{
init_1621(a[5]);//7
}
        break;
        case 8:
init_1621(a[1]);
init_1621(a[1]);
if (ss) {
    init_1621(a[1]);//8 с точкой
}
else{
init_1621(a[5]);//8
}
        break;
        case 9:
init_1621(a[0]);
init_1621(a[1]);
if (ss) {
    init_1621(a[1]);//9 с точкой
}
else{
init_1621(a[5]);//9
}
        break;
    }
                    }
//-----------------------переключение десятков минут----------------------------
unsigned char vyb_raz (unsigned char u){
    minee = u;
    minee ++; 
    if (u == 0b00001001) minee = 0b00010000;//если больше 9 записываем в переменную 10
    if (u == 0b00011001) minee = 0b00100000;//если больше 19 записываем в переменную 20
    if (u == 0b00101001) minee = 0b00110000;//если больше 29 записываем в переменную 30
    if (u == 0b00111001) minee = 0b01000000;//если больше 39 записываем в переменную 40
    if (u == 0b01001001) minee = 0b01010000;//если больше 49 записываем в переменную 50
    if (u == 0b01011001) minee = 0b00000000;//если больше 59 то обнуляем   
return minee;
}
//-----------------------переключение десятков часов----------------------------
unsigned char vyb_raz_h (unsigned char u){
    houree = u;
    houree ++;
    if (u == 0b00001001) houree = 0b00010000;//если больше 9 то записываем в переменную 10
    if (u == 0b00011001) houree = 0b00100000;//если больше 19 то записываем в переменную 20
    if (u == 0b00100011) houree = 0b00000000;//если больше 23 то обнуляем
return houree;
}
//-----------------------переключение числа месяца------------------------------
unsigned char vyb_raz_dat (unsigned char u){
    Daysset = u;
    Daysset ++;
    if (u == 0b00001001) Daysset = 0b00010000;//если больше 9 то записываем в переменную 10
    if (u == 0b00011001) Daysset = 0b00100000;//если больше 19 то записываем в переменную 20
    if (u == 0b00101001) Daysset = 0b00110000;//если больше 29 то записываем в переменную 30
    if (u == 0b00110001) Daysset = 0b00000001;//если больше 31 записываем в переменную 1 
return Daysset;
}
//------------------------------------------------------------------------------
unsigned char vyb_raz_month (unsigned char u){
    Monthsset = u;
    Monthsset ++; 
    if (u == 0b00001001) Monthsset = 0b00010000;//если больше 9 записываем в переменную 10
    if (u == 0b00010010) Monthsset = 0b00000001;//если больше 12 записываем в переменную 1  
return Monthsset;
}
//------------------------------------------------------------------------------
unsigned char vyb_raz_year (unsigned char u){
    Yearsset = u;
    Yearsset ++; 
    if (u == 0b00001001) Yearsset = 0b00010000;//если больше 9 записываем в переменную 10
    if (u == 0b00011001) Yearsset = 0b00100000;//если больше 19 записываем в переменную 20
    if (u == 0b00101001) Yearsset = 0b00110000;//если больше 29 записываем в переменную 30
    if (u == 0b00111001) Yearsset = 0b01000000;//если больше 39 записываем в переменную 40
    if (u == 0b01001001) Yearsset = 0b01010000;//если больше 49 записываем в переменную 50 
    if (u == 0b01011001) Yearsset = 0b01100000;//если больше 59 записываем в переменную 60
    if (u == 0b01101001) Yearsset = 0b01110000;//если больше 69 записываем в переменную 70
    if (u == 0b01111001) Yearsset = 0b00000000;//если больше 79 записываем в переменную 0  
return Yearsset;
}
//-----------------------обработка нажатия кнопки (изменение значения)---------- 
void button (unsigned char u,unsigned char i){
  unsigned int butcount=0;
  while(!RB1)
  { 
    if(butcount < 400)
    {
      butcount++;
    }
    else
    {
    sendbyte(0b00010010);//TONE ON
    __delay_ms(1);
    sendbyte(0b00010000);//TONE OFF 
    if (i==1){//настройка минут
    vyb_raz (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00000011);//вызов регистра минут
    I2C_SendByte (minee);//установка минут
    i2c_stop (); 
    }
    if (i==2){//настройка часов
    vyb_raz_h (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00000100);//вызов регистра часов
    I2C_SendByte (houree);//установка часов
    i2c_stop (); 
    }
    if (i==3){//настройка минут будильника     
    vyb_raz (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00001001);//вызов регистра минут
    I2C_SendByte (minee);//установка минут
    i2c_stop (); 
    }       
    
    if (i==4){//настройка часов будильника
    vyb_raz_h (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00001010);//вызов регистра часов
    I2C_SendByte (houree);//установка часов
    i2c_stop (); 
    }  
    
    if (i==5){//настройка дня недели
    vyb_raz_dat (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00000101);//вызов регистра дня недели
    I2C_SendByte (Daysset);//установка дня недели
    i2c_stop (); 
    } 
  
    if (i==6){//настройка месяца
    vyb_raz_month (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00000111);//вызов регистра месяца
    I2C_SendByte (Monthsset);//установка месяца
    i2c_stop (); 
    } 
    
    if (i==7){//настройка года
    vyb_raz_year (u);
    i2c_start ();//отправка посылки СТАРТ
    I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
    I2C_SendByte (0b00001000);//вызов регистра года
    I2C_SendByte (Yearsset);//установка года
    i2c_stop (); 
             } 
break;
    }
  } 
}
//------------------------мигание сегмента--------------------------------------
void seg_blink (void){//мигание сегмента
      init_1621(0b0000);
      init_1621(0b0000); 
      init_1621(0b0000);
      init_1621(0b0000); 
      init_1621(0b0000);
      init_1621(0b0000); 
}
//----------------------------задержка------------------------------------------
void dela_y (void){
       __delay_ms(180);    
}
//---------------------------задержка2------------------------------------------
void dela_yy (void){
       __delay_ms(60);    
}
//-----------------------------вывод--------------------------------------------
void clk_out (void){//
         unsigned int butcount=0;
    while (!RB0){
    if(butcount < 400)
    {
      butcount++;
    }
    else{ 
sendbyte(0b00010010);//TONE ON
dela_y ();
sendbyte(0b00010000);//TONE OFF
        t++;
        if (t==8) t=0;//установка флага режима настройки
      break;     
    }
    }
//--------------------первое нажатие - настройка минут--------------------------      
      if (t==1){
       button(min,1);
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (hourd,0);
       LCD_OUT (houre,1); 
       LCD_OUT (mind,0);
       LCD_OUT (mine,1); 
       CS=1; 
dela_y(); 
      CS=0;
      i1621_adre(0b001110);
      seg_blink ();
      CS=1;
      CS=0;
      i1621_adre(0b010100);
dela_yy();
       LCD_OUT (secd,0);
       LCD_OUT (sece,0);
       CS=1; 
               }
//-------------------второе нажатие - настройка часов---------------------------
       if (t==2){//
        button(hour,2);
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (hourd,0);
       LCD_OUT (houre,1); 
       CS=1;
dela_y(); 
       CS=0;
       i1621_adre(0b001000);
       seg_blink ();
       CS=1;
dela_yy();
       CS=0;
       i1621_adre(0b001110);
       LCD_OUT (mind,0);
       LCD_OUT (mine,1); 
       LCD_OUT (secd,0);
       LCD_OUT (sece,0);
       CS=1;  
       } 
//-----------------третье нажатие - будильник настройка минут-------------------       
       if (t==3){// 
       button(min_alar,3);  
       CS=0;
       i1621_adre(0b001011);
       LCD_OUT (hourd_alar,0);
       LCD_OUT (houre_alar,1); 
       LCD_OUT (mind_alar,0);//вывод десятков минут будильника
       LCD_OUT (mine_alar,0);//вывод единиц минут будильника
       CS=1;
dela_y(); 
       CS=0;
       i1621_adre(0b010001);
seg_blink (); 
dela_yy();  
seg_blink ();
       CS=1;
       } 
//-----------------четвёртое нажатие - будильник настройка часа-----------------     
        if (t==4){//
        button(hour_alar,4);  
        CS=0;
        i1621_adre(0b001011);
        LCD_OUT (hourd_alar,0);
        LCD_OUT (houre_alar,1); 
        CS=1;
dela_y();        
      CS=0;
     i1621_adre(0b001011);
seg_blink (); 
dela_yy();  
       LCD_OUT (mind_alar,0);
       LCD_OUT (mine_alar,0); 
       CS=1;                
       } 
//-----------------пятое нажатие -  настройка числа месяца----------------------     
       if (t==5){//
       button(Days,5);  
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (Days_prd,0);//
       LCD_OUT (Days_pre,1); //
       CS=1;
dela_y(); 
       CS=0;
       i1621_adre(0b001000);
       seg_blink ();
       CS=1;
dela_yy();
       CS=0;
       i1621_adre(0b001110);
       LCD_OUT (Months_prd,0);//Months_prd
       LCD_OUT (Months_pre,1);//Months_pre
       LCD_OUT (Years_prd,0);//Years_prd
       LCD_OUT (Years_pre,0);//Years_pre
       CS=1;                
       } 
//-------------------шестое нажатие - настройка месяца-------------------------
if (t==6){//
       button(Months,6);  
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (Days_prd,0);//Days_prd
       LCD_OUT (Days_pre,1);//Days_pre
       LCD_OUT (Months_prd,0);//
       LCD_OUT (Months_pre,1);//
       LCD_OUT (Years_prd,0);//Years_prd
       LCD_OUT (Years_pre,0);//Years_pre
       CS=1; 
dela_y();
      CS=0;
      i1621_adre(0b001110);
      seg_blink ();
      CS=1;
      CS=0;
      i1621_adre(0b010100);
dela_yy();  
       LCD_OUT (Years_prd,0);
       LCD_OUT (Years_pre,0); 
       CS=1;                
       } 
//-------------------седьмое нажатие - настройка года---------------------------
 if (t==7){//
       button(Years,7);  
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (Days_prd,0);//Days_prd
       LCD_OUT (Days_pre,1);//Days_pre
       LCD_OUT (Months_prd,0);//Months_prd
       LCD_OUT (Months_pre,1);//Months_pre
       LCD_OUT (Years_prd,0);
       LCD_OUT (Years_pre,0);
       CS=1;
dela_y(); 
       CS=0;
       i1621_adre(0b010100);
       seg_blink ();
       CS=1;
dela_yy();
       }    
//-----------------восьмое нажатие - выход из режима настройки------------------      
      if (t==0){
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (hourd,0);//hourd
       LCD_OUT (houre,1); //houre
       LCD_OUT (mind,0);//mind
       LCD_OUT (mine,1); //mine
       if (alrm == 1){
       LCD_OUT (secd,1);//secd
        }
       else{
        LCD_OUT (secd,0);//sece   
      }
         LCD_OUT (sece,0);//sece  
       
       CS=1;
//--------------------------Показ даты------------------------------------------
       while (!RB3) {
       CS=0;
       i1621_adre(0b001000);
       LCD_OUT (Days_prd,0);//число месяца (старший разряд) 
       LCD_OUT (Days_pre,1);//число месяца (младший разряд)
       LCD_OUT (Months_prd,0);//месяц (старший разряд)
       LCD_OUT (Months_pre,1);//месяц (младший разряд)
       LCD_OUT (Years_prd,0);//год (старший разряд)
       LCD_OUT (Years_pre,0);//год (младший разряд)
       CS=1;     
                    } 
//--------------------Включение будильника--------------------------------------
while (!RB7) {
   
    alrm ++;
    if (alrm == 1){
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00001011);//вызов регистра контроля
      I2C_SendByte (0b10000000);
      I2C_SendByte (0b10000000);
      i2c_stop ();//отправка посылки СТОП      
    }
    if (alrm > 1){
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00001011);//вызов регистра контроля
      I2C_SendByte (0b00000000);
      I2C_SendByte (0b00000000);
      i2c_stop ();//отправка посылки СТОП 
      alrm = 0;
    }     
    }            
                }     
//---------------проверка условия включения будильника--------------------------
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00000001);//вызов регистра контроля
      i2c_stop ();//отправка посылки СТОП 
      i2c_start ();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrr);//адрес часовой микросхемы - чтение
      control_2 = I2C_ReadByte_last();//чтение регистра срабатывания будильника
      i2c_stop ();// ;
      if (control_2&0b00001000){//проверяем флаг срабатывания будильника
       sendbyte(0b00010010);//TONE ON
    //__delay_ms(500);
       dela_yy ();
sendbyte(0b00010000);//TONE OFF
//__delay_ms(500);
dela_y ();
while (!RB2) {
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00000000);//вызов регистра контроля
      I2C_SendByte (0b00000000);
      I2C_SendByte (0b00000000);
      i2c_stop ();//отправка посылки СТОП  
             } 
                                }
}
//------------------------------------------------------------------------------
void main(void) {
    dela_y();
    CM2 = 1;//отключение компаратора (порт А - цифровой)
    CM1 = 1;//отключение компаратора (порт А - цифровой)       
    CM0 = 1;//отключение компаратора (порт А - цифровой)
    T0CS = 1;
    PORTA = 0b00000000;//11111100
    TRISA = 0b11111100;//11111100
    PORTB = 0b10001111;//10001111
    TRISB = 0b10001111;// RB0-изменение значения,RB1-вход в режим настройки,//10001111
    //RB2-отключение сигнала будильника,RB3-показ даты 
    //ANSEL = 0b00000000;
     dela_y();
     dela_y();
     dela_y();
     HT1621init();
dela_y();
dela_y();
dela_y();
     PCF8563init(); 
sendbyte(0b00010000);//TONE OFF  
        
    while (1){
      i2c_start();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrw);//адрес часовой микросхемы - запись
      I2C_SendByte (0b00000010);//вызов регистра секунд
      i2c_stop ();//отправка посылки СТОП 
      i2c_start ();//отправка посылки СТАРТ
      I2C_SendByte (dev_addrr);//адрес часовой микросхемы - чтение
      sec = I2C_ReadByte();//чтение секунд
      min = I2C_ReadByte();//чтение минут
      hour = I2C_ReadByte();//чтение часов
      Days = I2C_ReadByte();//чтение числа
      Weekdays = I2C_ReadByte();//чтение дня недели
      Months = I2C_ReadByte();//чтение месяца
      Years = I2C_ReadByte();//чтение года
      min_alar = I2C_ReadByte();//чтение минут будильника
      hour_alar = I2C_ReadByte_last();//чтение часов будильника
      i2c_stop (); 
//--------------перевод значения времени в десятичный формат--------------------     
      sece = RTC_ConvertFromDec(sec);
      secd = RTC_ConvertFromDecd(sec,0);
      mine = RTC_ConvertFromDec(min);
      mind = RTC_ConvertFromDecd(min,0);
      houre = RTC_ConvertFromDec(hour);
      hourd = RTC_ConvertFromDecd(hour,1);
      mine_alar = RTC_ConvertFromDec(min_alar);
      mind_alar = RTC_ConvertFromDecd(min_alar,0);
      houre_alar = RTC_ConvertFromDec(hour_alar);
      hourd_alar = RTC_ConvertFromDecd(hour_alar,0);   
      Days_pre = RTC_ConvertFromDec(Days);
      Days_prd = RTC_ConvertFromDecd(Days,1);
      Weekdays_pr = RTC_ConvertFromDec(Weekdays);
      Months_pre = RTC_ConvertFromDec(Months);
      Months_prd = RTC_ConvertFromDecd(Months,2);
      Years_pre = RTC_ConvertFromDec(Years);
      Years_prd = RTC_ConvertFromDecd(Years,0);
//--------------------выбор режима настройки (время, будильник, дата)-----------
clk_out ();  
    }    
}
