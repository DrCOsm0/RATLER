
#define F_CPU 2000000UL
#define SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

#include <avr/io.h>
#include <util/delay.h>
#include "nrf24_lib.h"
#include "nrf24_lib.c"
#include "UART.h"


uint8_t data_array[2];

uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	
long x = 0;
long y = 0;

long xbuff = 0;
long ybuff = 0;



int main(){
	DDRD = 0b11111111;
	//DDRB = 0b11111111;
	
	UART_Init(9600);
	
	nrf24_init();
	nrf24_config(2,2);
	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);

	SET_BIT(TCCR1B, CS10);//setting 8 prescaler timer 1
	SET_BIT(TCCR1A, WGM11);//setting timer settings to allow ICR1
	SET_BIT(TCCR1B, WGM12);
	SET_BIT(TCCR1B, WGM13);
	SET_BIT(TCCR1A, COM1A1);//setting timer to allow OCR1A
	SET_BIT(TCCR1A, COM1B1);//setting timer to allow OCR1B
	ICR1 = 19999;
	
	/*OCR1A = 2000;
	OCR1B = 2000;
	_delay_ms(5000);
	OCR1A = 1000;
	OCR1B = 1000;
	_delay_ms(2000);*/
	OCR1A = 1500;
	OCR1B = 1500;
	_delay_ms(2000);

	while(1)
	{
		
			
		if(nrf24_dataReady())
		{
			
			nrf24_getData(data_array);
			x = data_array[0];
			y = data_array[1];
			
			
			xbuff = (3.92 * x) + 1200;
			
			OCR1A = xbuff;
			OCR1B = xbuff;
			
			
			UART_SEND_DECIMAL_INTEGER(xbuff);
			UART_CRLF();
			

		}
	}

	
	
}//main