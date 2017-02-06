#include <lpc2292.h>

void IRQ_Handler(void) __irq
{
	void (*interrupt_function)();
  
	interrupt_function = (void(*)())VICVectAddr;
	(*interrupt_function)();
	  
	VICVectAddr = 0; // Clear interrupt in VIC.
}
