#ifndef __DEBUG_OUT_H
#define __DEBUG_OUT_H

// ������ ������ type_buf

#define DEBUG_OUT

#define ASCII   0x00    // ����� � ASCII
#define HEX     0x01    // ����� � HEX
#define SBR     0x02    // ����� � [�������]
#define CR      0x04    // CRLF � �����

#define SYSTEM  0x80    // ������������ system_type

#define TYPE_MAX_STR    128 // ����������� ��������� ����� ������ ��� ������� 
                            // type � type_buf
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

