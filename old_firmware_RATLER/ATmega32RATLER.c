#include <CEC_UART.h>
#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)
long pulse = 0;
char i = 0;
long distance = 100;
long time = 0;
long clock = 0;
long travel = 0;


long left = 0;
long right = 0;
long forward = 0;

void getDistance();
void getMath();

#define SERVO_PERIOD 20000
#define SERVO_MIN    770
#define SERVO_MAX    2770
#define SERVO_MID    1770

int main(void){
DDRD = 0b11111010;
DDRC = 0b11111111;
UART_Init(9600);
sei();

//ultrasonic
SET_BIT(MCUCR, ISC00);//enabling any logical change makes an interrupt
SET_BIT(TCCR2, CS22);//setting 1024 prescaler timer 2
SET_BIT(TCCR2, CS21);
SET_BIT(TCCR2, CS20);
SET_BIT(GICR, INT0);//enabling interrupt0 (PD2)

//servo PWM       
SET_BIT(TCCR1B, CS11);//setting 8 prescaler timer 1  
SET_BIT(TCCR1A, WGM11);//setting timer settings to allow ICR
SET_BIT(TCCR1B, WGM12);
SET_BIT(TCCR1B, WGM13);
SET_BIT(TCCR1A, COM1A1);//setting timer to allow OCRA1

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

OCR0 = 0;
OCR2 = 0;

ICR1 = SERVO_PERIOD;

OCR1A = SERVO_MID;
_delay_ms(1000);

while(1){

PORTC = 0b00001111;
OCR2 = 255;
}
}





/*
OCR1A = SERVO_MIN;
_delay_ms(1000);
getDistance();
getMath();
right = distance;


OCR1A = SERVO_MAX;
_delay_ms(500);
getDistance();
getMath();
left = distance;


if(left > right){
PORTB = 0b00001011;
_delay_ms(500);
PORTB = 0b00001010;
}

if(left < right){
PORTB = 0b00001110;
_delay_ms(500);
PORTB = 0b00001010;
}


}//while
}//main

void getDistance(){
PORTC = 0b00000001;
_delay_us(10);
PORTC = 0b00000000;
_delay_ms(65);
}

void getMath(){
clock = pulse * 1024;     //gets clock cycles for pulse to no prescaler
time = clock / 8;        //clocks per microsecond
travel = time * 0.0343;   //speed of sound in cm per microsecond
distance = travel / 2;    //distance divided by two

if(distance > 400){
distance = 300;
}
UART_SEND_DECIMAL_INTEGER(distance);
UART_CRLF();
}









ISR(INT0_vect){

if(i==0){    
TCNT2=0;
i=1;

}else{
                                            
pulse=TCNT2;
i=0;
}
}
 
*/
