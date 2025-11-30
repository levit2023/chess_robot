#ifndef LED_H
#define LED_H

typedef enum {
    LED_SOLID = 0x03, 
    LED_SLOW = 0x04, 
    LED_FAST = 0x05
} led_mode_t;

void led_init_pwm();
void led_isr();
void led_init_isr();
void led_config(led_mode_t mode, uint8_t data1, uint8_t data2);

#endif