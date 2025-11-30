#ifndef LED_H
#define LED_H

typedef enum {
    LED_SOLID, LED_FAST, LED_SLOW
} led_mode_t;

void led_init_pwm();
void led_isr();
void led_init_isr();
void led_config(led_mode_t led_mode, uint8_t data1, uint8_t data2);

#endif