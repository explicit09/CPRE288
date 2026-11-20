/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include "lcd.h"

void uart_init(int baud){
    SYSCTL_RCGCGPIO_R |= 0b00000010;      // enable clock GPIOB (page 340)
        SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
        GPIO_PORTB_AFSEL_R = 0x03;      // sets PB0 and PB1 as peripherals (page 671)
        GPIO_PORTB_PCTL_R  = 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
        GPIO_PORTB_DEN_R   |= 0b00000011;        // enables pb0 and pb1
        GPIO_PORTB_DIR_R   |= 0b00000001;
        GPIO_PORTB_DIR_R   &= 0b11111101;// sets pb0 as output, pb1 as input

        //compute baud values [UART clock= 16 MHz]
        double fbrd;
        int    ibrd;


        ibrd = 8;
        fbrd = 44;

        UART1_CTL_R &= 0b11111111111110;      // disable UART1 (page 918)
        UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
        UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
        UART1_LCRH_R = 0b01110000;        // write serial communication parameters (page 916) * 8bit and no parity
        UART1_CC_R   = 0b0000;          // use system clock as clock source (page 939)
        UART1_CTL_R |= 0b00000001;        // enable UART1


}

void uart_sendChar(char data){

	while(UART1_FR_R & 0x20){}
	UART1_DR_R = data;
}

char uart_receive(void){

	char data = 0;
	while (UART1_FR_R & 0x10){}
	data = (char)(UART1_DR_R & 0xFF);
	return data;
}

void uart_sendStr(const char *data){
    int i;

      for(i = 0; i < strlen(data); i++)
      {
         uart_sendChar(data[i]);
      }
}
