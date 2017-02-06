#include <debug_out.h>
#include <common.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <serial.h>

char __buf[TYPE_MAX_STR];
unsigned char buf_number[ MAX_NUMBER_BUFFER_SIZE ];

void WSIO(unsigned char c)
{
    while ( wsio0(c)!=OK );
}

unsigned char RSIO(void)
{
    unsigned char tmp;
    while ( rsio0(&tmp)!=OK );
    
    return tmp;
}

int x_kbhit(void)
{
    if ( rsiostat0() )
        return 1;
    else
        return 0;
}


unsigned char x_getchar(void)
{
    return RSIO();
}
