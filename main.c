#include <nrf_delay.h>

int main() {
    NRF_P0->DIRSET = 1 << 24 | 1 << 25 | 1 << 26 | 1 << 27;

    while (1) {
        NRF_P0->OUTSET = 1 << 24 | 1 << 25;
        NRF_P0->OUTCLR = 1 << 26 | 1 << 27;
        nrf_delay_ms(3000);

        NRF_P0->OUTSET = 1 << 26 | 1 << 27;
        NRF_P0->OUTCLR = 1 << 24 | 1 << 25;
        nrf_delay_ms(3000);
    }
}
