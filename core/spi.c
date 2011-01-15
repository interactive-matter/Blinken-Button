/**
 * SPI support
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "display.h"

#define SS PB2
#define MOSI PB3
#define MISO PB4
#define SCK PB5

// Sprite buffer
uint8_t sprite[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t i = 0;

/**
 * Initialize SPI in slave mode
 */
void spi_init(void)
{
    volatile char IOReg;
    
    // Set MISO output
    DDRB |= (1<<MISO);
    
    // Enable SPI (slave) and SPI interrupts
    SPCR = (1<<SPE) | (1<<SPIE);
    
    // Clear SPIF bit in SPSR
    IOReg   = SPSR;
    IOReg   = SPDR;
}

/**
 * SPI interrupt routine
 */
ISR(SPI_STC_vect)
{
    // Save sprite into buffer
    sprite[i++] = SPDR;
    
    if (i == 8) {
        // Sprite completed
        i = 0;
        
        display_load_sprite(sprite);
        display_advance_buffer();
    }
}


