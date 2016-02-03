#include <xc.h>
#define LCD_DEFAULT
#include "CANlib.h"
#include "delay.h"
#include "delay.c"
#include "LCD_44780.h" 
#include "LCD_44780.c"
#include "idCan.h"
#include "pic18f4480_config.h"
CANmessage msg;
unsigned char id = 0;
unsigned char data = 0;

__interrupt(high_priority) void ISR_Alta(void) {
    if (CANisRxReady()) { //Se il messaggio è arrivato
        CANreceiveMessage(&msg); //leggilo e salvalo
        PIR3bits.RXB0IF = 0;
        PIR3bits.RXB1IF = 0;
    }
}
void configurazione(void);

void main(void) {
    configurazione();
    LCD_initialize(16);
    while (1) {
        id = msg.identifier;
        data = msg.data[0];
        LCD_goto_xy(1, 0);
        LCD_write_message("id");
        LCD_goto_xy(1, 3);
        LCD_write_char(id);
        LCD_goto_xy(2, 0);
        LCD_write_char(data);
        delay_ms(1000);
        LCD_clear();
    }
}

void configurazione(void) {
    CANInitialize(4, 6, 5, 1, 3, CAN_CONFIG_LINE_FILTER_OFF & CAN_CONFIG_SAMPLE_ONCE & CAN_CONFIG_ALL_VALID_MSG & CAN_CONFIG_DBL_BUFFER_ON);
    LATA = 0x00;
    TRISA = 0xff; //ALL IN
    LATB = 0x00;
    TRISB = 0xff; //RB1 e RB2 INPUT
    LATC = 0x00;
    TRISC = 0xff;
    LATD = 0x00;
    TRISD = 0x00; //OUTPUT LCD
    LATE = 0x00;
    TRISE = 0xFF;
    PIR3bits.RXB1IF = 0; //azzera flag interrupt can bus buffer1
    PIR3bits.RXB0IF = 0; //azzera flag interrupt can bus buffer0
    IPR3bits.RXB1IP = 1; //interrupt alta priorità per can
    IPR3bits.RXB0IP = 1; //interrupt alta priorità per can
    PIE3bits.RXB1IE = 1; //abilita interrupt ricezione can bus buffer1
    PIE3bits.RXB0IE = 1; //abilita interrupt ricezione can bus buffer0
}