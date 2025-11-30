#include <utils.h>
#include <led.h>
#include "hardware/pwm.h"
#include "hardware/irq.h"

#define PWM_PERIOD 10000

#define SLOW_FREQ_HZ 2.0
#define FAST_FREQ_HZ 4.0

#define RGB_RED 8
#define RGB_GREEN 9
#define RGB_BLUE 10
static const uint8_t rgb_pins[3] = {RGB_RED, RGB_GREEN, RGB_BLUE};

static double rgb_scaler[3] = {0};

static double start_time; // to set phase of blinking, to start from "off"
static double freq; // blink frequency (Hz)
static bool solid;

void led_init_pwm() {
    for (int i = 0; i < 3; i++) {
        // According to page 1079 example in the datasheet:
        
        uint slice_num = pwm_gpio_to_slice_num(rgb_pins[i]);
        uint chan = pwm_gpio_to_channel(rgb_pins[i]);

        // Tell GPIO that it is allocated to the PWM
        gpio_set_function(rgb_pins[i], GPIO_FUNC_PWM);

        // Set slice clock divider to 150
        pwm_set_clkdiv(slice_num, 150.f);

        // Set top (wrap) value (counts 0 to period - 1)
        pwm_set_wrap(slice_num, PWM_PERIOD - 1);

        // Set counter-compare to the duty cycle (off to begin)
        pwm_set_chan_level(slice_num, chan, 0);

        // Set the PWM running
        pwm_set_enabled(slice_num, true);
    }
}

void led_isr() {
    for (int i = 0; i < 3; i++) {
        uint slice_num = pwm_gpio_to_slice_num(rgb_pins[i]);
        uint chan = pwm_gpio_to_channel(rgb_pins[i]);
        
        // ack interrupt
        pwm_clear_irq(slice_num);

        // Set duty cycle based on triangle-wave pattern
        // where it is f(t) = 2A/T times (t or T-t)
        double blink_period = 1.0/freq;
        double duty_cycle = fmod(us_to_s(timer0_hw->timerawl) - start_time, blink_period); // t, elapsed time within period
        if (duty_cycle > (blink_period / 2.0)) duty_cycle = blink_period - duty_cycle; // T - t, downslope for second half of period
        duty_cycle *= 2 / blink_period; // scale by 2/T
        duty_cycle *= rgb_scaler[i]; // scale by A (rgb scaler)

        if (solid) duty_cycle = 1.0; // always full duty cycle during solid mode

        // set the chosen color's duty cycle (duty cycle from 0.0 to 1.0)
        // counter_compare = current period (top) * duty_cycle
        pwm_set_chan_level(slice_num, chan, pwm_hw->slice[slice_num].top * duty_cycle);
    }
}

void led_init_isr() {
    uint slice_num;
    uint chan;
    
    // PWM peripheral register, enable interrupts
    for (int i = 0; i < 3; i++) {
        slice_num = pwm_gpio_to_slice_num(rgb_pins[i]);
        pwm_set_irq_enabled(slice_num, true);
    }

    // Set handler for PWM_IRQ_WRAP_0
    irq_set_exclusive_handler(PWM_IRQ_WRAP_0, led_isr);
    irq_set_enabled(PWM_IRQ_WRAP_0, true);

    // Set all three outputs duty cycle to zero (off)
    for (int i = 0; i < 3; i++) {
        slice_num = pwm_gpio_to_slice_num(rgb_pins[i]);
        chan = pwm_gpio_to_channel(rgb_pins[i]);
        // Set counter-compare
        pwm_set_chan_level(slice_num, chan, 0);
    }
}

void led_config(led_mode_t led_mode, uint8_t data1, uint8_t data2) {
    // {data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
    uint16_t data = (data1 << 8) | data2;

    // these scalers are from 0.0 to 1.0
    rgb_scaler[0] = ((data >> 11) & 0x1F) / (double)0x1F;
    rgb_scaler[1] = ((data >>  5) & 0x3F) / (double)0x3F;
    rgb_scaler[2] = ((data >>  0) & 0x1F) / (double)0x1F;

    start_time = us_to_s(timer0_hw->timerawl);

    freq = 0.0;
    if (led_mode == LED_FAST) freq = FAST_FREQ_HZ;
    if (led_mode == LED_SLOW) freq = SLOW_FREQ_HZ;

    solid = (led_mode == LED_SOLID);
}