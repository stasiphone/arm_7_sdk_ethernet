/**----------------------------------------------------------------------------
Проект:     SDK-2.0 
Название:   Драйвер последовательного канала
Файл:       serial.h
Версия:     1.0.1
-----------------------------------------------------------------------------*/

#ifndef __SERIAL_H
#define __SERIAL_H

extern void init_serial0( void );
extern unsigned char wsio0( unsigned char ch );
extern unsigned char rsio0 ( unsigned char* ch );
extern int rsiostat0( void );
extern void type0( char * s );

#endif // __SERIAL_H
