#ifndef __DEBUG_OUT_H
#define __DEBUG_OUT_H

// Режимы работы type_buf

#define DEBUG_OUT

#define ASCII   0x00    // Вывод в ASCII
#define HEX     0x01    // Вывод в HEX
#define SBR     0x02    // Вывод в [скобках]
#define CR      0x04    // CRLF в конце

#define SYSTEM  0x80    // использовать system_type

#define TYPE_MAX_STR    128 // максимально возможная длина строки для функций 
                            // type и type_buf
#define MAX_NUMBER_BUFFER_SIZE		16

extern char __buf[TYPE_MAX_STR];

void WSIO(unsigned char c);
unsigned char RSIO(void);
int x_kbhit(void);
unsigned char x_getchar(void);
void type( char * s );
void type_buf( unsigned char * buf, unsigned char len, unsigned char mode );
void print( char * format, ... );

unsigned char x_get_number( signed short *num, signed short min_val, signed short max_val);

#define cr()        { WSIO( 13 ); WSIO( 10 ); }
#define sp()        WSIO( 32 ) 
//#define type( s )   print( s )

#endif

