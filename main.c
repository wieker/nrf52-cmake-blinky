#include <nrf_delay.h>

int main() {
    NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;

    while (1) {
        NRF_P0->OUTSET = 1 << 30 | 1 << 20;
        NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
        nrf_delay_ms(3000);

        NRF_P0->OUTSET = 1 << 18 | 1 << 19;
        NRF_P0->OUTCLR = 1 << 30 | 1 << 20;
        nrf_delay_ms(3000);
    }
}
