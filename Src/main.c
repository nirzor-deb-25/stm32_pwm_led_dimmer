#include "led.h"
#include "button.h"
#include "timing.h"
#include "pwm.h"

static const uint32_t brightness_levels[] = {
    0U, 10U, 25U, 50U, 75U, 100U
};

#define LEVEL_COUNT \
    (sizeof(brightness_levels) / sizeof(brightness_levels[0]))

int main(void)
{
    uint32_t level_index = 0U;
    bool previous_pressed = false;

    led_init();
    pwm_init();
    timing_init();
    button_init();

    pwm_set_percent(brightness_levels[level_index]);

    while (1)
    {
        uint32_t now_ms = timing_millis();

        button_update(now_ms);
        bool pressed = button_is_pressed();

        led_blue_set(pressed);

        /* Advance once per debounced button press. */
        if (pressed && !previous_pressed)
        {
            level_index++;

            if (level_index >= LEVEL_COUNT)
            {
                level_index = 0U;
            }

            pwm_set_percent(brightness_levels[level_index]);
        }

        previous_pressed = pressed;
    }
}