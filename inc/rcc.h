#ifndef RCC_H
#define RCC_H

#define XTAL_4MHZ	4000000
#define XTAL_8MHZ	8000000
#define XTAL_12MHZ	12000000
#define XTAL_16MHZ	16000000
#define XTAL_25MHZ	25000000

extern int sysclk;
extern int ahbclk;
extern int apb1clk;
extern int apb2clk;

#ifdef __cplusplus
extern "C" {
#endif

int rcc_configure_pll(int hse_freq, int input_hse, int hse_div, int pll_mul, int ahb_div, int apb1_div, int apb2_div);
void rcc_high_performance(int hse_freq);

#ifdef __cplusplus
}
#endif

#endif
