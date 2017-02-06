#include <lpc2292.h>
#include <common.h>
#include <systimer.h>

#define VIC_TIMER0_bit ( 1 << VIC_TIMER0 )

static volatile unsigned long __systime = 0;

static void timer0_interrupt( void )
{
    __systime++;

    T0IR = 0xff; // Clear timer 0 interrupt line.
}

void init_systimer0( void )
{
	VICProtection = 0;

	VICIntSelect &= ~VIC_TIMER0_bit;		// IRQ on timer 0 line.
	VICVectAddr0 = ( unsigned int )&timer0_interrupt;
	VICVectCntl0 = 0x20 | VIC_TIMER0;		// Enable vector interrupt for timer 0.
	VICIntEnable |= VIC_TIMER0_bit;			// Enable timer 0 interrupt.

	T0TCR = 0;			// Disable timer 0.
						// The prescaler refuses any value other than zero!?
	T0PC  = 0;			// Prescaler is set to no division. Cclk is 48 MHz, pclk is 12 MHz.
	T0MR0 = 12000;		// Count up to this value. Generate 1000 Hz interrupt.
//	T0MR0 = 3000;		// Count up to this value. Generate 4000 Hz interrupt.
	T0MCR = 3;			// Reset and interrupt on MR0 (match register 0).
	T0CCR = 0;			// Capture is disabled.
	T0EMR = 0;			// No external match output.
}


void start_timer0( void )
{
	T0TCR = 1; // Enable timer 0.
}

unsigned long clock_ms( void )
{
	return __systime;
}
unsigned long dtime_ms( unsigned long t2 )
{
    unsigned long t1 = ( unsigned long )clock_ms();

    return t1 - t2;
}
void delay_ms( unsigned long ms )
{
    unsigned long t1 = ( unsigned long )clock_ms();

	while ( 1 )
	{
		if ( dtime_ms( t1 ) > ms ) break;
	}  
}
void init_systimer1( void )
{
	T1TCR = 0; // Disable timer 1.
	T1PR = 11; // 12MHz/(11+1) = 1000000Hz
	T1PC = 0;
	T1TC = 0;
	T1MCR = 0; // Match is disabled.
	T1CCR = 0; // Capture is disabled.
	T1EMR = 0; // No external match output.
}
void start_timer1( void )
{
	T1TCR = 1; // Enable timer 1.
}
void reset_timer1( void )
{
	T1PC = 0;
	T1TC = 0;
}
unsigned long clock_mcs( void )
{
	return T1TC;
}
unsigned long dtime_mcs( unsigned long t2 )
{
    unsigned long t1 = ( unsigned long )clock_mcs();

    return t1 - t2;  
}
void delay_mcs( unsigned long mcs )
{
    unsigned long t1 = ( unsigned long )clock_mcs();

	while ( 1 )
	{
		if ( dtime_mcs( t1 ) > mcs ) break;
	}  
}