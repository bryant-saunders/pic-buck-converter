/*
MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ  32000000

uint8_t out_volt = 63;


void init_pg(){
    HIDRVCbits.HIDC4 = 0b1; //Set high current output
    TRISCbits.TRISC4 = 0b0; //Set RC4 as output
    LATCbits.LATC4 = 0; //Set output low
}

void init_comp(){
    TRISCbits.TRISC3 = 0b1; //Set pin RC3 to input mode
    ANSELCbits.ANSC3 = 0b1; //Set pin RC3 to ADC mode
    CM1CON0bits.C1POL = 0;
    CM1CON1bits.C1PCH = 0b01; //Connect positive comp input to DAC
    CM1CON1bits.C1NCH = 0b011; //Connect negative comp input to CXIN3(RC3)
    CM1CON0bits.C1HYS = 0; //Hysteresis disabled
    CM1CON0bits.C1ON = 1; //Enable Compare module 1
}

void init_dac(){
    DAC1CON0bits.DAC1EN = 1; //Enable DAC module
    DAC1CON0bits.DAC1PSS = 0b00; //Use VDD as input reference
    DAC1CON1 = out_volt; //Set DAC to 1.23V assuming 5V VDD from regulator
}


void main(void) {
    init_pg();
    OSCCONbits.IRCF = 0b1111; //Set internal OSC to 32mHz  
    TRISAbits.TRISA5 = 0b1; //Set button pin as input(RA5)
    TRISCbits.TRISC5 = 0b1; //Set button pin as input(RC5)
    init_dac();   
    init_comp();    
    
    uint16_t debounce_counter = 0;
    uint8_t debounce_lock = 0;
    //__delay_ms(2000);
    while(1){      
        LATCbits.LATC4 = CM1CON0bits.C1OUT;
        __delay_us(5);
        LATCbits.LATC4 = 0;
        if(PORTAbits.RA5 == 1 && debounce_lock == 0){
            out_volt += 13;
            debounce_lock = 1;
            DAC1CON1 = out_volt;
        }
        if(PORTCbits.RC5 == 1 && debounce_lock == 0){
            out_volt += 1;
            debounce_lock = 1;
            DAC1CON1 = out_volt;
        }
        if (debounce_lock == 1){
            debounce_counter++;
        }
        if (debounce_counter > 60000){
            debounce_counter = 0;
            debounce_lock = 0;
        }  
    }
}