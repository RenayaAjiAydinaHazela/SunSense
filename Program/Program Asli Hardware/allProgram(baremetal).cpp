#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// Pin Definitions
#define DHT_PIN     PB0     // Digital pin 8 = PB0 pada ATmega328P
#define RAIN_PIN    PD2     // Pin 8
#define MODE_BUTTON PD3     // Tombol mode (pin 3)
#define CTRL_BUTTON PD4     // Tombol control (pin 2)
#define SERVO_PIN   PB1     // OC1A (Pin 9)

// Servo PWM
#define MIN_PULSE   1000    // Mikrodetik
#define MAX_PULSE   2000
#define SERVO_DEG_0   MIN_PULSE
#define SERVO_DEG_45 ((MIN_PULSE + MAX_PULSE) / 2) // ~1500 us

// Thresholds
#define LDR_DARK_THRESHOLD 300
#define HUMIDITY_THRESHOLD 70

// Delay for DHT communication
void dht_delay_us(uint16_t us) {
    while (us--) _delay_us(1);
}

// ========== USART Functions ==========
void uart_init() {
    UBRR0H = (uint8_t)(103 >> 8); // 9600 bps
    UBRR0L = (uint8_t)(103);
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

void uart_transmit(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_print(const char *str) {
    while (*str) uart_transmit(*str++);
}

void print_num(int num) {
    char buffer[10];
    itoa(num, buffer, 10);
    uart_print(buffer);
}

// ========== ADC Functions ==========
void adc_init() {
    ADMUX = (1 << REFS0);                    // AVcc reference
    ADCSRA = (1 << ADEN) | (1 << ADPS2);     // Enable ADC, prescaler 16
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

// ========== Rain Sensor ==========
void rain_sensor_init() {
    DDRD &= ~(1 << RAIN_PIN); // Input
}

bool is_raining() {
    return !(PIND & (1 << RAIN_PIN));
}

// ========== DHT22 Bit-Banging ==========
bool dht_start() {
    DDRB |= (1 << DHT_PIN);          // Set as output
    PORTB &= ~(1 << DHT_PIN);        // Pull low
    _delay_ms(18);
    PORTB |= (1 << DHT_PIN);         // Pull high
    dht_delay_us(40);
    DDRB &= ~(1 << DHT_PIN);         // Set as input

    dht_delay_us(80);
    if ((PINB & (1 << DHT_PIN))) return false; // No response
    dht_delay_us(80);
    return true;
}

uint8_t dht_receive_byte() {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        while (!(PINB & (1 << DHT_PIN))); // Wait for low
        dht_delay_us(40);
        if (PINB & (1 << DHT_PIN)) {
            byte |= (1 << (7 - i));
        }
        while (PINB & (1 << DHT_PIN)); // Wait for next bit
    }
    return byte;
}

bool dht_read(float *temp, float *humi) {
    if (!dht_start()) return false;

    uint8_t data[5];
    for (int i = 0; i < 5; i++) {
        data[i] = dht_receive_byte();
    }

    uint16_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) return false;

    *humi = (data[0] << 8 | data[1]) / 10.0;
    *temp = (data[2] << 8 | data[3]) / 10.0;

    return true;
}

// ========== Servo Control ==========
void servo_init() {
    DDRB |= (1 << SERVO_PIN); // OC1A sebagai output
    TCCR1A = (1 << COM1A1) | (1 << WGM11); // Fast PWM, non-inverting
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8
    ICR1 = 4999; // Period = 20ms (50Hz)
}

void servo_set_pulse(uint16_t pulse) {
    OCR1A = pulse;
}

// ========== Button Debounce ==========
bool debounce(volatile uint8_t *port, uint8_t pin) {
    if (((*port) & (1 << pin)) == 0) {
        _delay_ms(50);
        if (((*port) & (1 << pin)) == 0) return true;
    }
    return false;
}

// ========== Main Function ==========
int main(void) {
    cli();

    uart_init();
    adc_init();
    servo_init();
    rain_sensor_init();

    DDRD &= ~((1 << MODE_BUTTON) | (1 << CTRL_BUTTON)); // Input
    PORTD |= (1 << MODE_BUTTON) | (1 << CTRL_BUTTON);   // Pull-up enable

    float temp = 0, humi = 0;
    bool manualMode = false;
    bool manualDecision = false;

    servo_set_pulse(SERVO_DEG_0); // Start position

    uint32_t lastUpdate = 0;
    const uint32_t updateInterval = 1000;

    sei();

    while (1) {
        // Baca tombol mode
        if (debounce(&PIND, MODE_BUTTON)) {
            manualMode = !manualMode;
            uart_print("MODE:");
            uart_print(manualMode ? "MANUAL\r\n" : "AUTO\r\n");
            _delay_ms(200);
        }

        // Baca tombol control (jika mode manual)
        if (manualMode && debounce(&PIND, CTRL_BUTTON)) {
            manualDecision = !manualDecision;
            uart_print("CONTROL:");
            uart_print(manualDecision ? "OUT\r\n" : "IN\r\n");
            _delay_ms(200);
        }

        // Update setiap 1 detik
        if (_delay_ms(1000)) { // dummy delay
            if (dht_read(&temp, &humi)) {
                int light = adc_read(1);
                bool raining = is_raining();

                // Keputusan otomatis
                bool dark = light < LDR_DARK_THRESHOLD;
                bool cold = temp < 25.0;
                bool humid = humi > HUMIDITY_THRESHOLD;
                bool autoDecision = !(raining || (dark && cold) || (dark && humid));

                bool currentDecision = manualMode ? manualDecision : autoDecision;
                servo_set_pulse(currentDecision ? SERVO_DEG_45 : SERVO_DEG_0);

                // Kirim ke serial
                uart_print("<T:");
                print_num((int)temp);
                uart_print("|H:");
                print_num((int)humi);
                uart_print("|L:");
                print_num(light);
                uart_print("|R:");
                uart_print(raining ? "RAINING" : "DRY");
                uart_print("|M:");
                uart_print(manualMode ? "MANUAL" : "AUTO");
                uart_print("|D:");
                uart_print(currentDecision ? "OUT" : "IN>\r\n");
            } else {
                uart_print("DHT ERROR\r\n");
            }
        }
    }
}
