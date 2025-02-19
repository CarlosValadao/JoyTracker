#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "lib/rgb.h"
#include "lib/joystick.h"
#include "lib/oledgfx.h"
#include "lib/push_button.h"
#include "hardware/pwm.h"

#define I2C_PORT i2c1

#define OLED_SDA 14
#define OLED_SCL 15
#define OLED_ADDR 0x3C
#define OLED_BAUDRATE 400000

#define JOYSTICK_VRX 27
#define JOYSTICK_VRY 26
#define JOYSTICK_PB  22

#define RED_PIN   13
#define BLUE_PIN  12
#define GREEN_PIN 11

#define JOYSTICK_SW_PRESSED gpio == 22
#define BUTTON_A_PRESSED gpio == BUTTON_A
#define BUTTON_B_PRESSED gpio == BUTTON_B

#define set_bootsel_mode() reset_usb_boot(0, 0)

static volatile uint8_t border_type = BORDER_LIGHT;

static volatile bool led_red_active = false;
static volatile bool led_green_active = false;
static volatile bool led_blue_active = false;
static volatile bool led_control_override = false;
static ssd1306_t *ssd_global = NULL;

/**
 * @brief Normaliza um valor de uma faixa original para uma nova faixa.
 *
 * @param x O valor original a ser normalizado.
 * @param x_min O valor mínimo da faixa original.
 * @param x_max O valor máximo da faixa original.
 * @param new_min O valor mínimo da nova faixa desejada.
 * @param new_max O valor máximo da nova faixa desejada.
 * @return O valor normalizado dentro da nova faixa.
 */
static uint16_t normalize_joystick_to_display(uint16_t joystick_vr, uint8_t new_max);
static uint16_t adjuste_pwm_led_intensity(uint16_t pwm_led_value);
static void gpio_irq_callback(uint gpio, uint32_t event);
static uint16_t adjust_pwm_led_value(uint16_t pwm_value);

int main()
{
    stdio_init_all();
    rgb_t rgb;
    uint16_t adj_led_red_pwm_value, adj_led_blue_pwm_value;
    uint8_t joystick_vrx_norm, joystick_vry_norm;
    uint16_t joystick_vrx, joystick_vry;
    joystick_t joy;
    ssd1306_t ssd;
    rgb_init_all(&rgb, RED_PIN, GREEN_PIN, BLUE_PIN, 1.0, 2048);
    joystick_init_all(&joy, JOYSTICK_VRX, JOYSTICK_VRY, JOYSTICK_PB);
    oledgfx_init_all(&ssd, I2C_PORT, OLED_BAUDRATE, OLED_SDA, OLED_SCL, OLED_ADDR);
    ssd_global = &ssd;

    pb_config(JOYSTICK_PB, true);
    pb_config_btn_a();
    pb_config_btn_b();
    pb_set_irq_callback(&gpio_irq_callback);
    pb_enable_irq(BUTTON_A);
    pb_enable_irq(JOYSTICK_PB);
    pb_enable_irq(BUTTON_B);
    oledgfx_draw_border(&ssd, 1);
    while(true)
    {
        joystick_vrx = joystick_get_x(&joy);
        joystick_vry = joystick_get_y(&joy);
        joystick_vrx_norm = normalize_joystick_to_display(joystick_vrx, 127 - CURSOR_SIDE - border_type);
        joystick_vry_norm = (63 - CURSOR_SIDE) - normalize_joystick_to_display(joystick_vry, 63 - CURSOR_SIDE - border_type);
        oledgfx_update_cursor(&ssd, joystick_vrx_norm, joystick_vry_norm);
        oledgfx_draw_border(&ssd, border_type);
        oledgfx_render(&ssd);
        if(!led_control_override)
        {
            adj_led_red_pwm_value = adjust_pwm_led_value(joystick_vrx);
            adj_led_blue_pwm_value = adjust_pwm_led_value(joystick_vry);
            pwm_set_gpio_level(BLUE_PIN, adj_led_blue_pwm_value);
            pwm_set_gpio_level(RED_PIN, adj_led_red_pwm_value);
        }
        sleep_ms(100);
    }
    return EXIT_SUCCESS;
}

static uint16_t normalize_joystick_to_display(uint16_t joystick_vr, uint8_t new_max)
{
    return (joystick_vr * new_max) / 4095;
}

static void gpio_irq_callback(uint gpio, uint32_t event)
{
    if(pb_is_debounce_delay_over())
    {
        if(BUTTON_B_PRESSED) set_bootsel_mode();
        else if(BUTTON_A_PRESSED) 
        {
            pwm_set_gpio_level(RED_PIN, 0);
            pwm_set_gpio_level(BLUE_PIN, 0);
            led_control_override = !led_control_override;
        }
        else if(JOYSTICK_SW_PRESSED)
        {
            oledgfx_clear_screen(ssd_global);
            if(led_green_active)
            {
                border_type = BORDER_LIGHT;
                oledgfx_draw_border(ssd_global, BORDER_LIGHT);
                pwm_set_gpio_level(GREEN_PIN, 0);
            }
            else
            {
                border_type = BORDER_THICK;
                oledgfx_draw_border(ssd_global, BORDER_THICK);
                pwm_set_gpio_level(GREEN_PIN, 1024);
            }
            led_green_active = !led_green_active;
        }
    }
}

static uint16_t adjust_pwm_led_value(uint16_t pwm_value)
{
    if(pwm_value >= 2048) return pwm_value - 2048;
    else if(pwm_value < 2048) return 2048 - pwm_value;
}