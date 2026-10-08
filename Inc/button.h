#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

void button_init(void);
void button_update(uint32_t now_ms);
bool button_is_pressed(void);

#endif