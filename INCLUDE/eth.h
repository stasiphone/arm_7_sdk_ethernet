/**----------------------------------------------------------------------------
Проект:     SDK-2.0 
Название:   Драйвер контроллера ethernet LAN91C111
Файл:       eth.h
Версия:     1.0.1
------------------------------------------------------------------------------*/

#ifndef __ETH_H
#define __ETH_H

extern volatile unsigned short *eth_reg;

#define ETH_BASE_ADDRESS        0x81000000L

#define BANK0   				0
#define BANK1   				1
#define BANK2   				2
#define BANK3   				3

#define ETH_REG0		 		(*((volatile unsigned short *) 0x81000000))
#define ETH_REG1         		(*((volatile unsigned short *) 0x81000002))
#define ETH_REG2         		(*((volatile unsigned short *) 0x81000004))
#define ETH_REG3         		(*((volatile unsigned short *) 0x81000006))
#define ETH_REG4         		(*((volatile unsigned short *) 0x81000008))
#define ETH_REG5         		(*((volatile unsigned short *) 0x8100000A))
#define ETH_REG6         		(*((volatile unsigned short *) 0x8100000C))
#define ETH_REG7         		(*((volatile unsigned short *) 0x8100000E))

// BANK0        
#define ETH_TCR         		ETH_REG0
#define ETH_EPH_STATUS  		ETH_REG1
#define ETH_RCR         		ETH_REG2
#define ETH_COUNTER     		ETH_REG3
#define ETH_MIR         		ETH_REG4
#define ETH_RPCR        		ETH_REG5
#define ETH_BANK        		ETH_REG7
// BANK1
#define ETH_CONFIG      		ETH_REG0
#define ETH_BASE        		ETH_REG1
#define ETH_IA_01       		ETH_REG2
#define ETH_IA_23       		ETH_REG3
#define ETH_IA_45       		ETH_REG4
#define ETH_GENERAL     		ETH_REG5
#define ETH_CONTROL     		ETH_REG6
// BANK2                		       
#define ETH_MMU_COMMAND 		ETH_REG0
#define ETH_PNR         		ETH_REG1
#define ETH_PNR_2       		(*((volatile unsigned char *) 0x81000002))
#define ETH_PNR_3       		(*((volatile unsigned char *) 0x81000003))
#define ETH_FIFO_PORTS  		ETH_REG2
#define ETH_POINTER     		ETH_REG3
#define ETH_DATA0       		ETH_REG4
#define ETH_DATA1       		ETH_REG5
#define ETH_INTERRUPT   		ETH_REG6
#define ETH_INTERRUPT_C 		(*((volatile unsigned char *) 0x8100000C))
#define ETH_INTERRUPT_D 		(*((volatile unsigned char *) 0x8100000D))
// BANK3                		       
#define ETH_MT_01       		ETH_REG0
#define ETH_MT_23       		ETH_REG1
#define ETH_MT_45       		ETH_REG2
#define ETH_MT_67       		ETH_REG3
#define ETH_MGMT        		ETH_REG4
#define ETH_REVISION    		ETH_REG5
#define ETH_ERCV        		ETH_REG6

extern unsigned long init_eth( void );
extern int read_eth_pack( unsigned char* buf, unsigned short* len );
extern int write_eth_pack( unsigned char* buf, unsigned short len );

#endif // __ETH_H
