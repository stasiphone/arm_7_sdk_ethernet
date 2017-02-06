#include <stdio.h>
#include <eth.h>
#include <eth_mii_phy.h>
#include <systimer.h>
#define MII_WRITE   1
#define MII_READ    2

static void mii_write( unsigned short x );
static int mii_read( void );
static void mii_start( unsigned char mode, unsigned char addr );
void init_eth_phy( void )
{

    ETH_BANK    = BANK1;
    ETH_CONFIG  = 0xA0B1;
}

void write_phy_reg( unsigned char addr, unsigned short data )
{
	int i;

    mii_start( MII_WRITE, addr );

    ETH_BASE = 3;

    for( i = 0; i < 16; i++ )
    {
        if( data & 0x8000 )
            mii_write( 1 );
        else
            mii_write( 0 );
            
        data <<= 1;
    }

    ETH_MGMT = 0x3330;   

}
unsigned short read_phy_reg( unsigned char addr )
{
	int i;
	unsigned short data;

    mii_start( MII_READ, addr );

    ETH_BANK = 3;

    data = 0;

    for( i = 0; i < 16; i++ )
    {
        data <<= 1;

        if( mii_read() )
            data |= 1;
    }

    mii_write( 2 );

    return data;
}
static int mii_read( void )
{
	int res;

    ETH_BANK = 3;

    ETH_MGMT = 0x3330;   
    ETH_MGMT = 0x3334;   
    
    res      = ETH_MGMT; 

    ETH_MGMT = 0x3330;   

    if( res & 0x0002 )
        return 1;
    else
        return 0;
}
static void mii_write( unsigned short value )
{
    ETH_BASE = 3;
    
    switch( value )
    {
    case 0: // '0'
        ETH_MGMT = 0x3338;   
        ETH_MGMT = 0x333C;   
        ETH_MGMT = 0x3338; break;  

    case 1: // '1'
        ETH_MGMT = 0x3339;   
        ETH_MGMT = 0x333D;   
        ETH_MGMT = 0x3339; break;  
    case 2: // 'Z'
        ETH_MGMT = 0x3330;   
        ETH_MGMT = 0x3334;   
        ETH_MGMT = 0x3330; break;  
    }
}
static void mii_start( unsigned char mode, unsigned char addr )
{
	int i;

    ETH_BANK = 3;
    for( i = 0; i < 32; i++ )
    {
        ETH_MGMT = 0x3339;   
        ETH_MGMT = 0x333D;   
    }
    mii_write( 0 );
    mii_write( 1 );

    if( mode == MII_WRITE )
    {
        mii_write( 0 );
        mii_write( 1 );
    }
    else
    {
        mii_write( 1 );
        mii_write( 0 );
    }

    for( i = 0; i < 5; i++ )
        mii_write( 0 );

    for( i = 0; i < 5; i++ )
    {
        if( addr & 0x10 )
            mii_write( 1 );
        else
            mii_write( 0 );
            
        addr <<= 1;
    }

    if( mode == MII_WRITE )
    {
        mii_write( 1 );
        mii_write( 0 );
    }
    else
    {
        mii_write( 2 );
    }
}
