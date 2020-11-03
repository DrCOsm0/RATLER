#define D4 eS_PORTC2
#define D5 eS_PORTC3
#define D6 eS_PORTC4
#define D7 eS_PORTC5
#define RS eS_PORTC0
#define EN eS_PORTC1

#define F_CPU 8000000UL
#include <CEC_ADC.h>
#include <nrf24.h>
#include <nrf24.c>
#include <lcd.h>
#include <stdlib.h>

int indi = 0;
char buffer[50];

short y = 0;
short x = 0;
short ybuff = 0;
short xbuff = 0;

uint8_t temp;
uint8_t q = 0;
uint8_t data_array[2];
uint8_t tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
uint8_t rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};

int main(void){
DDRC = 0b11111111;
DDRD = 0b00000010;

Lcd4_Init();
Lcd4_Clear();

ADC_Init(ADC_REF_500V, ADC_8_BIT);
sei();

nrf24_init();
nrf24_config(2,2);
nrf24_tx_address(tx_address);
nrf24_rx_address(rx_address);   


while(1){

x = ADC_Read8Bit(0);
y = ADC_Read8Bit(1);

data_array[0] = x;
data_array[1] = y;

nrf24_send(data_array);        
while(nrf24_isSending());

temp = nrf24_lastMessageStatus();

if(temp == NRF24_TRANSMISSON_OK)
{
Lcd4_Clear();
Lcd4_Set_Cursor(1,1);                    
Lcd4_Write_String("TRANS OK");
}
else if(temp == NRF24_MESSAGE_LOST)
{
Lcd4_Clear();
Lcd4_Set_Cursor(1,1);                    
Lcd4_Write_String("TRANS LOST"); 
}
        
	
temp = nrf24_retransmissionCount();
indi = temp;
itoa(indi,buffer,10);
Lcd4_Set_Cursor(2,1);                    
Lcd4_Write_String(buffer);  
	
nrf24_powerUpRx();
_delay_ms(10);

ybuff = 0;
xbuff = 0;
}
}














