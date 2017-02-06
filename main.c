#include <lpc2292.h>
#include <init.h>
#include <common.h>
#include <systimer.h>
#include <eth.h>
#include <eth_mii_phy.h>
#include <debug_out.h>

void show_eth_reg( void );
void show_phy_reg( void );
void read_packs( void );
void send_pack( void );
void eth_test_eeprom( void );
void enable_receive_all( void );
unsigned long eth_test_eeprom_check( unsigned short val, unsigned char incr );
unsigned long eth_test_eeprom_write( unsigned short val, unsigned char incr );

unsigned char packet[ 2048 ];

static unsigned char test_pack[2048] = {
	0x01, 0x00, 0x5E, 0x00, 0x00, 0x01, 0x00, 0x16, 0x76, 0xC5, 0xFA, 0x23, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



void test_eth_help( void )
{
    print("\n\n");
    print("Тест Ethernet\n");
    print("____________________________________________________________\n");
    print("\n");

    print("1   Регистры MAC\n");
    print("2   Регистры PHY\n");
    print("3   Прием всех пакетов\n");
    print("4   Отправка тестового пакета\n");
    print("5   Отправка 100 тестовых пакетов\n");
    print("6   Тест конфигурационного EEPROM\n");

    print("\n");
    print("h   Вывод справки \n");
    print("q   Выход\n\n");
}


void test_eth( void )
{
	unsigned long i;

    test_eth_help();

    for (;;)
    {
        print("\rETHERNET[time = %.8d]> ", clock_ms() );

        switch (x_getchar())
        {

            case '1': show_eth_reg();       break;
            case '2': show_phy_reg();       break;
            case '3': read_packs();         break;  
            case '4': send_pack();          break;
            case '5': 
           		for ( i = 0; i < 100; i++ ) send_pack();
            								break;
            case '6': eth_test_eeprom();    break;

            case 'h': test_eth_help();    	break;
            case 'q':                     	return;

            default:                      	break;
        }
    }
}


void show_eth_reg( void )
{
    print("\n");

    ETH_BANK = 0; print( "BANK0  ETH_TCR        = 0x%.4X\n", ETH_TCR        );        
    ETH_BANK = 0; print( "BANK0  ETH_EPH_STATUS = 0x%.4X\n", ETH_EPH_STATUS );
    ETH_BANK = 0; print( "BANK0  ETH_RCR        = 0x%.4X\n", ETH_RCR        );
    ETH_BANK = 0; print( "BANK0  ETH_COUNTER    = 0x%.4X\n", ETH_COUNTER    );
    ETH_BANK = 0; print( "BANK0  ETH_MIR        = 0x%.4X\n", ETH_MIR        );
    ETH_BANK = 0; print( "BANK0  ETH_RPCR       = 0x%.4X\n", ETH_RPCR       );

    print("\n");
  
    ETH_BANK = 1; print( "BANK1  ETH_CONFIG     = 0x%.4X\n", ETH_CONFIG     );
    ETH_BANK = 1; print( "BANK1  ETH_BASE       = 0x%.4X\n", ETH_BASE       );
    ETH_BANK = 1; print( "BANK1  ETH_IA_01      = 0x%.4X\n", ETH_IA_01      );
    ETH_BANK = 1; print( "BANK1  ETH_IA_23      = 0x%.4X\n", ETH_IA_23      );
    ETH_BANK = 1; print( "BANK1  ETH_IA_45      = 0x%.4X\n", ETH_IA_45      );
    ETH_BANK = 1; print( "BANK1  ETH_GENERAL    = 0x%.4X\n", ETH_GENERAL    );
    ETH_BANK = 1; print( "BANK1  ETH_CONTROL    = 0x%.4X\n", ETH_CONTROL    );

    print("\n");
                                                 
    ETH_BANK = 2; print( "BANK2  ETH_MMU_COMMAND= 0x%.4X\n", ETH_MMU_COMMAND);
    ETH_BANK = 2; print( "BANK2  ETH_PNR        = 0x%.4X\n", ETH_PNR        );
    ETH_BANK = 2; print( "BANK2  ETH_FIFO_PORTS = 0x%.4X\n", ETH_FIFO_PORTS );
    ETH_BANK = 2; print( "BANK2  ETH_POINTER    = 0x%.4X\n", ETH_POINTER    );
    ETH_BANK = 2; print( "BANK2  ETH_DATA0      = 0x%.4X\n", ETH_DATA0      );
    ETH_BANK = 2; print( "BANK2  ETH_DATA1      = 0x%.4X\n", ETH_DATA1      );
    ETH_BANK = 2; print( "BANK2  ETH_INTERRUPT  = 0x%.4X\n", ETH_INTERRUPT  );

    print("\n");
                                           
    ETH_BANK = 3; print( "BANK3  ETH_MT_01      = 0x%.4X\n", ETH_MT_01      );
    ETH_BANK = 3; print( "BANK3  ETH_MT_23      = 0x%.4X\n", ETH_MT_23      );
    ETH_BANK = 3; print( "BANK3  ETH_MT_45      = 0x%.4X\n", ETH_MT_45      );
    ETH_BANK = 3; print( "BANK3  ETH_MT_67      = 0x%.4X\n", ETH_MT_67      );
    ETH_BANK = 3; print( "BANK3  ETH_MGMT       = 0x%.4X\n", ETH_MGMT       );
    ETH_BANK = 3; print( "BANK3  ETH_REVISION   = 0x%.4X\n", ETH_REVISION   );
    ETH_BANK = 3; print( "BANK3  ETH_ERCV       = 0x%.4X\n", ETH_ERCV       );

    print("\n");

}


void show_phy_reg( void )
{
    print("\n");

    print( "PHY_CONTROL           = 0x%.4X\n", read_phy_reg( PHY_CONTROL      ) );
    print( "PHY_STATUS            = 0x%.4X\n", read_phy_reg( PHY_STATUS       ) );
    print( "PHY_ID1               = 0x%.4X\n", read_phy_reg( PHY_ID1          ) );
    print( "PHY_ID2               = 0x%.4X\n", read_phy_reg( PHY_ID2          ) );
    print( "PHY_AUTONEG_ADV       = 0x%.4X\n", read_phy_reg( PHY_AUTONEG_ADV  ) );
    print( "PHY_AUTONEG_REM       = 0x%.4X\n", read_phy_reg( PHY_AUTONEG_REM  ) );
    print( "PHY_CONF1             = 0x%.4X\n", read_phy_reg( PHY_CONF1        ) );
    print( "PHY_CONF2             = 0x%.4X\n", read_phy_reg( PHY_CONF2        ) );
    print( "PHY_STATUS_OUT        = 0x%.4X\n", read_phy_reg( PHY_STATUS_OUT   ) );
    print( "PHY_MASK              = 0x%.4X\n", read_phy_reg( PHY_MASK         ) );

    print("\n");

}


void show_packet( unsigned char * buf, unsigned short n, unsigned short npack )
{
	unsigned long i, j, m;

    print( "\nReceive packet( %.5d ): size = %d\n", npack, n );

    m = n >> 4;		// / 16
    if ( n & 0x0F ) m++;

    for ( i = 0; i < m; i++ )
    {
    	for ( j = 0; j < 16; j++ )
    	{
    		if ( ( ( i * 16 ) + j ) >= n )
	    		print( "   " );
    		else
            	print( "%.2X ", buf[ ( i * 16 ) + j ] );
    	}
		print("  ");

    	for ( j = 0; j < 16; j++ )
    	{
    		if ( ( ( i * 16 ) + j ) >= n ) break;
			if ( buf[ ( i * 16 ) + j ] < 0x20 ) print( "." );
			else print( "%c", buf[ ( i * 16 ) + j ] );

    	}
		print( "\n" );

   		if ( ( ( i * 16 ) + j ) >= n ) break;
    }
}

void read_packs( void )
{
	unsigned short len, count = 0;

	print( "\n[ ETH ] Receive packets:" );
    for ( ; ; )
    {
		if ( OK == read_eth_pack( packet, &len ) )
			show_packet( packet, len, count++ );

		if ( x_kbhit() )
		{
			if ( x_getchar() == 'q' )
			{
				ETH_BANK = 0; ETH_RCR = 0x0000;
                break;
            }
		}
	}  
}


void send_pack( void )
{
	static unsigned char pack_num = 0x00;
	unsigned long i;

	for ( i = 14; i < 64; i++ ) test_pack[ i ] = pack_num;

	print( "\n[ ETH ] Send [ %02X ] ", pack_num );
	if ( write_eth_pack( test_pack, 64 ) == OK )
		print( "OK\n" );
	else
		print( "ERROR\n" );

	pack_num++;
}


void eth_test_eeprom( void )
{
    print( "\n[ ETH ] Start Clear EEPROM " );
	if ( eth_test_eeprom_write( 0xFFFF, 0 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

    print( "\n[ ETH ] Start Check EEPROM " );
	if ( eth_test_eeprom_check( 0xFFFF, 0 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

    print( "\n[ ETH ] Start Write EEPROM " );
	if ( eth_test_eeprom_write( 0x8800, 1 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

    print( "\n[ ETH ] Start Check EEPROM " );
	if ( eth_test_eeprom_check( 0x8800, 1 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

    print( "\n[ ETH ] Start Clear EEPROM " );
	if ( eth_test_eeprom_write( 0xFFFF, 0 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

    print( "\n[ ETH ] Start Check EEPROM " );
	if ( eth_test_eeprom_check( 0xFFFF, 0 ) == OK ) print( "OK" );
	else { print( "ERROR\n" ); return; }

	print( "\n[ ETH ] Тест EEPROM OK\n" );
}


unsigned long eth_test_eeprom_check( unsigned short val, unsigned char incr )
{
	unsigned short i, wtmp;
	unsigned long t;

	for ( i = 0x20; i < 0x30; i++ )
	{
	    ETH_BANK = 2; ETH_POINTER = i;

	    ETH_BANK = 1; ETH_CONTROL |= 0x0004; // EEPROM SELECT = 1
	    ETH_BANK = 1; ETH_CONTROL |= 0x0002; // RELOAD = 1
	    delay_ms( 10 );

	    t = clock_ms();
	    while ( ETH_CONTROL & 0x0002 )
	    {
	    	if ( dtime_ms( t ) >= 3000 )
	    	{
	    		print( "\n[ ETH ] Тест EEPROM Read Timeout ERROR!\n" );
	    		return ERR;
	    	}
	    }

	    ETH_BANK = 1; wtmp = ETH_GENERAL;
	    if ( incr )
	    	{ if ( wtmp != ( val + i ) ) return ERR; }
		else
	    	{ if ( wtmp != val ) return ERR; }
	}

	return OK;
}


unsigned long eth_test_eeprom_write( unsigned short val, unsigned char incr )
{
	unsigned short i;
	unsigned long t;

	for ( i = 0x20; i < 0x30; i++ )
	{
	    ETH_BANK = 2; ETH_POINTER = i;
	    if ( incr )
	    	{ ETH_BANK = 1; ETH_GENERAL = val + i; }
    	else
	    	{ ETH_BANK = 1; ETH_GENERAL = val; }


	    ETH_BANK = 1; ETH_CONTROL |= 0x0004; // EEPROM SELECT = 1
	    ETH_BANK = 1; ETH_CONTROL |= 0x0001; // STORE = 1
	    delay_ms( 10 );

	    t = clock_ms();
	    while ( ETH_CONTROL & 0x0001 )
	    {
	    	if ( dtime_ms( t ) >= 3000 )
	    	{
	    		print( "\n[ ETH ] Тест EEPROM Write Timeout ERROR!\n" );
	    		return ERR;
	    	}
	    }
	}

	return OK;
}


void main( void )
{
    init_system();

    while ( 1 )
    {
    	test_eth();
	}
}                                
