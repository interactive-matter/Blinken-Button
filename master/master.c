/**
 * SPI master example
 *
 * Sends frames to the Blinken-button (SPI slave)
 */

// Clock rate: should not exceed that of the slave
#define F_CPU 1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Adjust these to fit the MCU (ATmega8535)
#define SS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

/**
 * Initialize SPI in master mode
 */
void spi_init(void)
{
    volatile char IOReg;
    
    // Set MOSI, SCK and SS as output
    DDRB = (1<<MOSI) | (1<<SCK) | (1<<SS);
    
    // Enable SPI in Master Mode with SCK = CK/4
    SPCR = (1<<SPE) | (1<<MSTR);
    
    // Clear SPIF bit in SPSR
    IOReg   = SPSR;
    IOReg   = SPDR;
}

/**
 * Transmit a byte, returning the byte received by the slave
 */
uint8_t spi_transmit(uint8_t data)
{
    // Hold SS low
    PORTB &= ~(1 << SS);
    
    // Start transmission
    SPDR = data;
    
    // Wait for transmission complete
    while(!(SPSR & (1<<SPIF)));
    
    // Transmission complete, set SS high
    PORTB |= 1 << SS;
    
    // Return any received byte
    return SPDR;
}

/**
 * Send a byte, ignoring any received data
 */
void spi_send(char data) {
    spi_transmit(data);
}

/**
 * Receive a byte, sending 0x00
 */
uint8_t spi_receive() {
    return spi_transmit(0x00);
}

void spi_begin() {
    PORTB &= ~(1 << SS);
}

void spi_end() {
    PORTB |= 1 << SS;
}


int main() 
{
    int i, j;
    uint8_t d = 0;
    
    // Initialize SPI
    spi_init();
    
    // Set PORTD as output
    DDRD = 0xff;
    
    for(;;) {
        // Cycle through rows
        for (i = 0; i < 8; i++) {
            PORTD = ~(1 << i);
            for (j = 0; j < 8; j++) {
                if (i == j) {
                    spi_send(0xff);
                } else {
                    spi_send(0x00);
                }
            }
            
            _delay_ms(100);
        }
        
        // Cycle through columns
        for (i = 0; i < 8; i++) {
            PORTD = ~(1 << i);
            d = (1 << i);
            for (j = 0; j < 8; j++) {
                spi_send(d);
            }
            
            _delay_ms(100);
        }
        
        // Space invaders
        PORTD = 0xff;
        
        for (i = 0; i < 5; i++) {
            spi_send(0x18);
            spi_send(0x3C);
            spi_send(0x7E);
            spi_send(0xDB);
            spi_send(0xFF);
            spi_send(0x24);
            spi_send(0x5A);
            spi_send(0xA5);
            
            _delay_ms(500);
            
            spi_send(0x18);
            spi_send(0x3C);
            spi_send(0x7E);
            spi_send(0xDB);
            spi_send(0xFF);
            spi_send(0x24);
            spi_send(0x42);
            spi_send(0x24);
            
            _delay_ms(500);
        }
    }
}
