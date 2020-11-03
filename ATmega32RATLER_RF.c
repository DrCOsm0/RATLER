#define F_CPU 8000000UL
#include <CEC_UART.h>
#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

#include <nrf24.h>
#include <nrf24.c>


long x = 0;
long y = 0;

long xbuff = 0;
long ybuff = 0;




uint8_t temp;
uint8_t data_array[2];

uint8_t tx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};
uint8_t rx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};

int main(){
DDRD = 0b11111111;
DDRC = 0b11111111;
DDRB = 0b11111111;
UART_Init(9600);
//sei();


//Moter PWM Left OC0
SET_BIT(TCCR0, WGM00);//sets fast PWM
SET_BIT(TCCR0, WGM01);
SET_BIT(TCCR0, COM01);//allows OCR0
SET_BIT(TCCR0, CS00);//setting 64 prescaler timer 0
SET_BIT(TCCR0, CS01);

//Moter PWM Right OC2
SET_BIT(TCCR2, WGM20);//sets fast PWM
SET_BIT(TCCR2, WGM21);
SET_BIT(TCCR2, COM21);//allows OCR2
SET_BIT(TCCR2, CS22);//setting 64 prescaler timer 2

//PC4 = brake
//0101 forward
//1010 back	
//0110 clockwise
//1001 counterclockwise

nrf24_init();
nrf24_config(2,2);
nrf24_tx_address(tx_address);
nrf24_rx_address(rx_address);

OCR2 = 0;
OCR0 = 0;
PORTC = 0b00000101;

while(1){



if(nrf24_dataReady()){
		
nrf24_getData(data_array);
x = data_array[0];
y = data_array[1];		        
         	

if(x >= 170){                   //f
PORTC = 0b00000101;
xbuff = x - 127;
xbuff = xbuff * 2;

if(xbuff >= 255){
xbuff = 255;
}
OCR0 = xbuff;
OCR2 = xbuff;
}

if(x <= 130){                          //b
PORTC = 0b00001010;
xbuff = 150 - x;
xbuff = xbuff * 2;

if(xbuff > 255){
xbuff = 255;
}
OCR0 = xbuff;
OCR2 = xbuff;
}

if(y >= 170){                            //c
PORTC = 0b00000110;
ybuff = y - 127;
ybuff = ybuff * 2;

if(ybuff > 255){
ybuff = 255;
}
OCR0 = ybuff;
OCR2 = ybuff;
}

if(y <= 130){
PORTC = 0b00001001;
ybuff = 150 - y;
ybuff = ybuff * 2;

if(ybuff > 255){
ybuff = 255;
}
OCR0 = ybuff;
OCR2 = ybuff;
}

if((x < 170 && x > 130 && y < 170 && y > 130)|(x > 170 && y > 170)|(x < 130 && y > 170)|(x < 130 && y < 130)|(x > 170 && y < 130)){
UART_SendString("brake");
UART_CRLF();
OCR2 = 255;
OCR0 = 255;
PORTC = 0b00011111;
}

UART_SEND_DECIMAL_INTEGER(xbuff);
UART_CRLF();
UART_SEND_DECIMAL_INTEGER(ybuff);
UART_CRLF();
UART_CRLF();

xbuff = 0;
ybuff = 0;


}
}
}//main









