/*
 * controlb_atmega.c
 *
 * Created: 4/26/2025 5:44:54 PM
 * Author : njfg0
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "com_uart/com_uart.h"

volatile uint16_t axis_x=0;
volatile uint16_t axis_y=0;
volatile uint8_t channel_mux=0;
volatile uint8_t boton=0;
char message[50];
char control;
char l_control='x';
uint8_t l_boton=2;

void ADC_init(void){
	ADMUX = 0;
	//Vref = AVCC = 5V
	ADMUX |= (1<<REFS0);
	// Justificado a la izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	// Habilitar ADC
	ADCSRA |= (1<<ADEN);
	// Máscara de interrupción del ADC
	ADCSRA |= (1<<ADIE);
	// Prescaler del ADC a 128
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	
	ADCSRB = 0;
	
	// El ADC está configurado para correr en modo Single Conversion
	//Iniciar primera conversión
	ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect){
	uint16_t value=ADC;
	if(channel_mux==0){
		axis_x=value;
		channel_mux=1;
		ADMUX=(ADMUX&0xF0)|1; //A1
		}else{
		axis_y=value;
		channel_mux=0;
		ADMUX=(ADMUX&0xF0)|0; //A0
	}
	ADCSRA|=(1<<ADSC);
}

ISR(PCINT1_vect){
	if(!(PINC&(1<<PC2))){
		boton=1;
		}else{
		boton=0;
	}
}


int main(void)
{
	cli();
	initUART9600();
	ADC_init();
	DDRC&=~(1<<PC2);
	PORTC|=(1<<PC2);

	PCICR|=(1<<PCIE1);//Habilitar interrupciones del canal c
	PCMSK1|=(1<<PCINT10);//habilitar interrupción del botón en A2
	
	sei();
	/* Replace with your application code */
	while (1)
	{
		if (axis_x >40000) {
			control='I';
			}else if(axis_x ==0){
			control='D';
			}else if(axis_y > 40000){
			control='A';//arriba
			}else if(axis_y == 0){
			control='B';//abajo
			}else{
			control=' ';
		}
		
		if (boton == 1 && l_boton == 0 && control == ' ') {
			sprintf(message, "P\n");
			writetxtUART(message);
			_delay_ms(250);
		}
		l_boton = boton;

		if (control != ' ') {
			sprintf(message, "%c\n", control);
			writetxtUART(message);
			_delay_ms(250);
		}
		
		//_delay_ms(100);
		
	}
}

