/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 6/9/2021
Author  : 
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 11.059200 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <delay.h>

// Declare your global variables here
char SEG_DATA[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int num = 0;
char d[] = {0, 0, 0, 0};
char di = 0;

void setDigits()
{
    char i = 0;
    int j = 1;
    while (i < 4)
    {
        d[i] = (num / j) % 10;
        j *= 10;
        i++;
    }
}

void switchDigit()
{
    setDigits();

    PORTD.7 = 1;
    PORTD.6 = 1;
    PORTD.5 = 1;
    PORTD.4 = 1;

    if (di == 0)
        PORTD.7 = 0;
    if (di == 1)
        PORTD.6 = 0;
    if (di == 2)
        PORTD.5 = 0;
    if (di == 3)
        PORTD.4 = 0;

    PORTB = SEG_DATA[d[di]];
    di = (di + 1) % 4;
}

char getKey()
{
    char k = 255;
    DDRA = 0b11110000;

    PORTA = 0b11101111;
    while (PINA.0 == 0)
        k = 7;
    while (PINA.1 == 0)
        k = 8;
    while (PINA.2 == 0)
        k = 9;
    while (PINA.3 == 0)
        k = 10; // /

    PORTA = 0b11011111;
    while (PINA.0 == 0)
        k = 4;
    while (PINA.1 == 0)
        k = 5;
    while (PINA.2 == 0)
        k = 6;
    while (PINA.3 == 0)
        k = 11; // *

    PORTA = 0b10111111;
    while (PINA.0 == 0)
        k = 1;
    while (PINA.1 == 0)
        k = 2;
    while (PINA.2 == 0)
        k = 3;
    while (PINA.3 == 0)
        k = 12; // -

    PORTA = 0b01111111;
    while (PINA.0 == 0)
        k = 13; // on
    while (PINA.1 == 0)
        k = 0;
    while (PINA.2 == 0)
        k = 14; // =
    while (PINA.3 == 0)
        k = 15; // +

    return k;
}

void send_signal()
{
    PORTD.0 = 1;
    delay_ms(1);
    PORTD.0 = 0;
}

void keypadTo7segment()
{
    char k = getKey();
    if (k == 255)
        return;
    if (k < 10)
        num = (num % 1000) * 10 + k;
    else
        num = (num % 100) * 100 + k;
    send_signal();
}

// External Interrupt 0 service routine
interrupt[EXT_INT0] void ext_int0_isr(void)
{
    // Place your code here
    num = 0;
}

// Timer 0 overflow interrupt service routine
interrupt[TIM0_OVF] void timer0_ovf_isr(void)
{
    // Reinitialize Timer 0 value
    TCNT0 = 0x53;
    // Place your code here
    switchDigit();
}

void main(void)
{
    // Declare your local variables here

    // Input/Output Ports initialization
    // Port A initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out
    DDRA = (1 << DDA7) | (1 << DDA6) | (1 << DDA5) | (1 << DDA4) | (1 << DDA3) | (1 << DDA2) | (1 << DDA1) | (1 << DDA0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0
    PORTA = (0 << PORTA7) | (0 << PORTA6) | (0 << PORTA5) | (0 << PORTA4) | (0 << PORTA3) | (0 << PORTA2) | (0 << PORTA1) | (0 << PORTA0);

    // Port B initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out
    DDRB = (1 << DDB7) | (1 << DDB6) | (1 << DDB5) | (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
    // State: Bit7=1 Bit6=1 Bit5=1 Bit4=1 Bit3=1 Bit2=1 Bit1=1 Bit0=1
    PORTB = (1 << PORTB7) | (1 << PORTB6) | (1 << PORTB5) | (1 << PORTB4) | (1 << PORTB3) | (1 << PORTB2) | (1 << PORTB1) | (1 << PORTB0);

    // Port C initialization
    // Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In
    DDRC = (0 << DDC7) | (0 << DDC6) | (0 << DDC5) | (0 << DDC4) | (0 << DDC3) | (0 << DDC2) | (0 << DDC1) | (0 << DDC0);
    // State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T
    PORTC = (0 << PORTC7) | (0 << PORTC6) | (0 << PORTC5) | (0 << PORTC4) | (0 << PORTC3) | (0 << PORTC2) | (0 << PORTC1) | (0 << PORTC0);

    // Port D initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=Out Bit0=Out
    DDRD = (1 << DDD7) | (1 << DDD6) | (1 << DDD5) | (1 << DDD4) | (0 << DDD3) | (0 << DDD2) | (1 << DDD1) | (1 << DDD0);
    // State: Bit7=1 Bit6=1 Bit5=1 Bit4=1 Bit3=T Bit2=T Bit1=0 Bit0=0
    PORTD = (1 << PORTD7) | (1 << PORTD6) | (1 << PORTD5) | (1 << PORTD4) | (0 << PORTD3) | (0 << PORTD2) | (0 << PORTD1) | (0 << PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 172.800 kHz
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    // Timer Period: 1.0012 ms
    TCCR0 = (0 << WGM00) | (0 << COM01) | (0 << COM00) | (0 << WGM01) | (0 << CS02) | (1 << CS01) | (1 << CS00);
    TCNT0 = 0x53;
    OCR0 = 0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: Timer1 Stopped
    // Mode: Normal top=0xFFFF
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
    TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12) | (0 << CS12) | (0 << CS11) | (0 << CS10);
    TCNT1H = 0x00;
    TCNT1L = 0x00;
    ICR1H = 0x00;
    ICR1L = 0x00;
    OCR1AH = 0x00;
    OCR1AL = 0x00;
    OCR1BH = 0x00;
    OCR1BL = 0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR = 0 << AS2;
    TCCR2 = (0 << PWM2) | (0 << COM21) | (0 << COM20) | (0 << CTC2) | (0 << CS22) | (0 << CS21) | (0 << CS20);
    TCNT2 = 0x00;
    OCR2 = 0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << OCIE0) | (1 << TOIE0);

    // External Interrupt(s) initialization
    // INT0: On
    // INT0 Mode: Falling Edge
    // INT1: Off
    // INT2: Off
    GICR |= (0 << INT1) | (1 << INT0) | (0 << INT2);
    MCUCR = (0 << ISC11) | (0 << ISC10) | (1 << ISC01) | (0 << ISC00);
    MCUCSR = (0 << ISC2);
    GIFR = (0 << INTF1) | (1 << INTF0) | (0 << INTF2);

    // USART initialization
    // USART disabled
    UCSRB = (0 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (0 << RXEN) | (0 << TXEN) | (0 << UCSZ2) | (0 << RXB8) | (0 << TXB8);

    // Analog Comparator initialization
    // Analog Comparator: Off
    // The Analog Comparator's positive input is
    // connected to the AIN0 pin
    // The Analog Comparator's negative input is
    // connected to the AIN1 pin
    ACSR = (1 << ACD) | (0 << ACBG) | (0 << ACO) | (0 << ACI) | (0 << ACIE) | (0 << ACIC) | (0 << ACIS1) | (0 << ACIS0);
    SFIOR = (0 << ACME);

    // ADC initialization
    // ADC disabled
    ADCSRA = (0 << ADEN) | (0 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (0 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);

    // SPI initialization
    // SPI disabled
    SPCR = (0 << SPIE) | (0 << SPE) | (0 << DORD) | (0 << MSTR) | (0 << CPOL) | (0 << CPHA) | (0 << SPR1) | (0 << SPR0);

    // TWI initialization
    // TWI disabled
    TWCR = (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWEN) | (0 << TWIE);

// Global enable interrupts
#asm("sei")

    while (1)
    {
        // Place your code here
        keypadTo7segment();
    }
}
