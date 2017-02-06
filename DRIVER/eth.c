#include <stdio.h>
#include <eth.h>
#include <eth_mii_phy.h>
#include <systimer.h>
#include <common.h>
#include <debug_out.h>

#define ETH_ALLOCATE_TO 3000
#define ETH_TRANSMIT_TO 3000    
#define ETH_RELEASE_TO  3000

#define ETH_INIT_TRY_CNT			2
#define PHY_STATUS_LINK_BIT			0x0004
#define PHY_STATUS_ANEG_ACK_BIT		0x0020

#define PHY_STATUS_OUT_SPDDET_BIT	0x0080
#define PHY_STATUS_OUT_DPLXDET_BIT	0x0040

volatile unsigned short *eth_reg = (unsigned short *)ETH_BASE_ADDRESS;

unsigned long init_eth( void )
{
	unsigned long i, t;
	unsigned short tmp, tcr;

	delay_ms( 50 );				

    ETH_BANK = 0; ETH_RCR       = 0x8000;
    ETH_BANK = 0; ETH_RCR       = 0x0000;

	delay_ms( 50 );				

    ETH_BANK = 0; ETH_RPCR      = 0x0810;
    write_phy_reg( PHY_CONTROL, 0x8000 );
    write_phy_reg( PHY_CONTROL, 0x1000 );

	print( "\n[ ETH ] Wait start Auto_Negatiation process" );

    for ( i = 0; i < ETH_INIT_TRY_CNT; i++ )
    {
	    delay_ms( 1500 );	

	    t = clock_ms();
	    do
	    {
		    tmp = read_phy_reg( PHY_STATUS );
		    if ( dtime_ms( t ) >= 2000 ) break;
	    } while ( ( !( tmp & PHY_STATUS_LINK_BIT ) || !( tmp & PHY_STATUS_ANEG_ACK_BIT ) ) );

	    if ( !( tmp & PHY_STATUS_LINK_BIT ) || !( tmp & PHY_STATUS_ANEG_ACK_BIT ) )
	    {
	    	print( "\n[ ETH ] Restart Auto_Negatiation process" );
		    write_phy_reg( PHY_CONTROL, 0x3200 );
	    	continue;
	    }

	    tmp = read_phy_reg( PHY_STATUS_OUT );
	    tcr = 0;
	    if ( tmp & PHY_STATUS_OUT_SPDDET_BIT )
	    	print( "\n[ ETH ] Device in 100Mbps mode" );
	    else
	    	print( "\n[ ETH ] Device in 10Mbps mode" );

	    if ( tmp & PHY_STATUS_OUT_DPLXDET_BIT )
	    {
	    	print( "\n[ ETH ] Device in Full Duplex mode" );
	    	tcr |= 0x8000;
	    }
	    else
	    {
	    	print( "\n[ ETH ] Device in Half Duplex mode" );
	    }
	    ETH_BANK = 0; ETH_TCR       = tcr;
		  ETH_BANK = 1; ETH_CONTROL   = 0x0800;
	    write_phy_reg( PHY_CONTROL, 0x3000 );

		ETH_BANK = 2; ETH_INTERRUPT = 0x00FF;

	    return OK;
	}

	return ERR;
}
int read_eth_pack( unsigned char * buf, unsigned short * len )
{
	unsigned short wtmp1 = 0, wtmp2 = 0, n, i, k, m, sw;
	unsigned short interrupt_status, fifo_ports;

    ETH_BANK = 0; ETH_RCR          = 0x0102;

    ETH_BANK = 2; interrupt_status = ETH_INTERRUPT;
    ETH_BANK = 2; fifo_ports       = ETH_FIFO_PORTS;

    if ( ( interrupt_status & 0x0001 ) && !( fifo_ports & 0x8000 ) )
    {  
        ETH_BANK = 2; ETH_POINTER = 0xE000;
        delay_mcs( 10 );
        ETH_BANK = 2; sw          = ETH_DATA0;
        ETH_BANK = 2; n           = ETH_DATA1;	

        *len = n - 10;

        n -= 6;
        n += 2;	

        if ( n & 0x03 )
			m = ( n >> 2 ) + 1;
        else
			m = n >> 2;

        k = 0;
        for ( i = 0; i < m; i++ )
        {
            ETH_BANK = 2; wtmp1 = ETH_DATA0;
            buf[ k++ ] = wtmp1;
            buf[ k++ ] = wtmp1 >> 8;
            ETH_BANK = 2; wtmp2 = ETH_DATA1;
            buf[ k++ ] = wtmp2;
            buf[ k++ ] = wtmp2 >> 8;
        }
        if ( n & 0x03 )
        {
        	if ( wtmp1 & 0x2000 ) *len = *len + 1;
        }
        else
        {
        	if ( wtmp2 & 0x2000 ) *len = *len + 1;
        }

        ETH_BANK = 2; ETH_MMU_COMMAND = 0x0080;

        return OK;
    }       

	return ERR;
}
int write_eth_pack( unsigned char * buf, unsigned short len )
{
	unsigned short wtmp, n, i, k;
	unsigned short interrupt_status, allocate_number;
	unsigned long t1;

	ETH_BANK = 2; ETH_MMU_COMMAND = 0x0020;

    t1 = clock_ms();
    for ( ; ; )
    {
        ETH_BANK = 2; interrupt_status = ETH_INTERRUPT;

        if ( interrupt_status & 0x0008 ) break;

        if ( dtime_ms( t1 ) >= ETH_ALLOCATE_TO )
        {
            print( "\n[ ETH ] write_eth_pack(): ERR of allocation memory for transmitting" );
            return ERR;
        }
    }

    ETH_BANK = 2; allocate_number = ETH_PNR >> 8;
	ETH_BANK = 2; ETH_PNR         = allocate_number;

    ETH_BANK = 2; ETH_POINTER     = 0x4000;

    ETH_BANK = 2; ETH_DATA0       = 0x0000;
    ETH_BANK = 2; ETH_DATA0       = len + 6;

    n = len;
    if ( n % 2 )
        n = ( n >> 1 ) + 1;
    else
        n = n >> 1;
        
    k = 0;
    for ( i = 0; i < n; i++ )
    {
        wtmp =  buf[ k + 1 ]; wtmp <<= 8;
        wtmp += buf[ k ];
        k += 2;  
        ETH_BANK = 2; ETH_DATA0 = wtmp;
    }

    ETH_BANK = 2; ETH_DATA0       = 0x0000;

    ETH_BANK = 0; ETH_TCR        |= 0x0001;
	ETH_BANK = 2; ETH_MMU_COMMAND = 0x00C0;
    t1 = clock_ms();
    for ( ; ; )
    {
        ETH_BANK = 2; interrupt_status = ETH_INTERRUPT;

        if ( interrupt_status & 0x0002 )
        {
			ETH_BANK = 2; ETH_INTERRUPT |= 0x0002;
        	break;
        }
        ETH_BANK = 0; interrupt_status = ETH_EPH_STATUS;
        if ( interrupt_status & 0x0001 ) break;

        if( dtime_ms( t1 ) > ETH_TRANSMIT_TO )
        {
            print( "\n[ ETH ] write_eth_pack(): TO of transmitting" );
            return ERR;
        }
    }

    return OK;
}
