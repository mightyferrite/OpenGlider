# OpenGlider
##Arduino based glider control software

This is Arduino code running a home built underwater glider.  It is currently functional and working however
there are some rough spots.  The glider states change based on a timer - the next phase of this will be 
to change states based on actual water depth by adding an external water sensor.

This runs on an Arduino Nano IOT 33 with built in wifi.  It is a new board and the libraries do not support websockets nicely yet, so I used SSE with much thanks to Oscar from his project https://github.com/ocrdu/arduino-webinterface-sse

The pinouts used are:

SD CARD (SCK)	1	D13	Digital	GPIO
		2	#ERROR!	Power Out	Internally generated power output to external devices
		3	AREF	Analog	Analog Reference; can be used as GPIO
		4	A0/DAC0	Analog	ADC in/DAC out; can be used as GPIO
Vlve Neg RLY	5	A1	Analog	ADC in; can be used as GPIO
	            6	A2	Analog	ADC in; can be used as GPIO
	            7	A3	Analog	ADC in; can be used as GPIO
              8	A4/SDA	Analog	ADC in; I2C SDA; Can be used as GPIO (*)
              9	A5/SCL	Analog	ADC in; I2C SCL; Can be used as GPIO(*)
              10	A6	Analog	ADC in; can be used as GPIO
              11	A7	Analog	ADC in; can be used as GPIO
              12	VUSB	Power In/Out	Normally NC; can be connected to VUSB pin of the USB connector by shorting a jumper
              13	RST	Digital In	Active low reset input (duplicate of pin 18)
              14	GND	Power	Power Ground
              15	VIN	Power In	Vin Power input
              16	TX	Digital	USART TX; can be used as GPIO
              17	RX	Digital	USART RX; can be used as GPIO
              18	RST	Digital	Active low reset input (duplicate of pin 13)
              19	GND	Power	Power Ground
              20	D2	Digital	GPIO
              21	D3/PWM	Digital	GPIO; can be used as PWM
SD CARD CS	  22	D4	Digital	GPIO
Buzzer 	      23	D5/PWM	Digital	GPIO; can be used as PWM
Pump Pwr RLY	24	D6/PWM	Digital	GPIO; can be used as PWM
Pump Pos RLY	25	D7	Digital	GPIO
Pump Neg RLY	26	D8	Digital	GPIO
Valve Pwr RLY	27	D9/PWM	Digital	GPIO; can be used as PWM
Valve Pos RLY	28	D10/PWM	Digital	GPIO; can be used as PWM
SD CARD MISO  29	D11/MOSI	Digital	SPI MOSI; can be used as GPIO
SD CARD MOSI  30	D12/MISO	Digital	SPI MISO; can be used as GPIO
