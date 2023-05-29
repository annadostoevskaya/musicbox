/*
Author: github.com/annadostoevskaya
File: main.c
Date: 18/03/23 21:14:29

Description: <empty>
*/

#define F_CPU 9.6e6

#include <avr/io.h>
#include <util/delay.h>

#define TRIG_PIN    PB0
#define LED_PIN     PB2
#define ECHO_PIN    PB1
#define ADKEY_PIN   PB4    // Пин, к которому подключен контакт Adkey 1 DF Player mini

void ping_ultra_sonyc()
{
	PORTB |= _BV(TRIG_PIN);
	_delay_us(2.5f);
	PORTB &= ~(_BV(TRIG_PIN));
}

void enable_dft_player()
{
	PORTB &= ~(1 << ADKEY_PIN); // Устанавливаем логическую 0 на пин ADKEY_PIN
}

void led_init()
{
	DDRB |= (1 << LED_PIN);
}

#define G_DISTANCE 750
volatile uint16_t g_counter = 0;

int main(void)
{
	// Устанавливаем выводы TRIG и LED как выходы
	DDRB |= _BV(LED_PIN);
	DDRB |= _BV(TRIG_PIN); // UltraSonyc init
	DDRB &= ~(_BV(ECHO_PIN)); // Устанавливаем вывод ECHO как вход

	for (;;)
	{
		ping_ultra_sonyc();
		_delay_us(200.0f); // NOTE(annad): UltraSonyc calculates
		g_counter = 0;
		while (PINB & (1 << ECHO_PIN))
		{
			g_counter++;
		}

		if (g_counter != 0 && g_counter < G_DISTANCE)
		{
			// NOTE(annad): Enable DFT in this code block!
			PORTB |= (1 << LED_PIN);
			_delay_ms(100.0f/8.0f);
			PORTB &= ~(1 << LED_PIN);
			_delay_ms(100.0f/8.0f);
			PORTB |= (1 << ADKEY_PIN);
			_delay_ms(50);
			PORTB &= ~(1 << ADKEY_PIN);
			_delay_ms(28800);
		}

		_delay_ms(50.0f/8.0f);
	}

	return 0;
}