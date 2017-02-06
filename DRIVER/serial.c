#include <lpc2292.h>
#include <serial.h>
#include <common.h>

unsigned char wsio0( unsigned char ch )  
{         
    while ( ! ( U0LSR & 0x20 ) );
    U0THR = ch;

    return OK;
}
unsigned char rsio0 ( unsigned char* ch )
{   
    while ( ! ( U0LSR & 0x01 ) );
    *ch = U0RBR;

    return OK;
}
int rsiostat0( void )
{
    return ( U0LSR & 0x01 );
}
void type0( char * s )
{
    while ( *s )
        wsio0( *s++ );
}

void init_serial0( void )
{
    U0LCR   = 0x83;
    U0DLL   = 78;    
    U0DLM   = 0x00;
    U0LCR   = 0x03;
}

