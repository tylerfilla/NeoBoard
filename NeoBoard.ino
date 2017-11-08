/*
 * NeoBoardV2 - LED Panel Controller
 * Dedicated to FIRST Tech Challenge team #5703
 *
 * Copyright (c) 2017 Tyler Filla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Adafruit_NeoPixel.h>

#include "input_ctrl.h"
#include "mode_text.h"
#include "mode.h"

/**
 * Pin for the up directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_UP = 7;

/**
 * Pin for the down directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_DOWN = 8;

/**
 * Pin for the left directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_LEFT = 9;

/**
 * Pin for the right directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_RIGHT = 10;

/**
 * Pin for the select button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_SELECT = 11;

/**
 * Pin connected to the WS2812 LED strip data input line (5V logic, use a
 * low-ish value resistor). This daisy-chains through each panel.
 */
static constexpr auto PIN_LED_STRIP = 12;

/**
 * The number of daisy-chained LED panels.
 */
static constexpr auto NUM_LED_PANELS = 1;

/**
 * The number of WS2812 LED modules per panel.
 */
static constexpr auto NUM_LEDS_PER_PANEL = 60;

/**
 * Macro for determining if a pulled-up N/O switch is currently closed.
 */
#define IS_SWITCH_CLOSED(pin) ((pin) == LOW)

/** A reference to the current mode. */
static neo::mode* current_mode_g;

/** State for input control array. */
static neo::input_ctrl input_ctrl_g;

/** State for WS2812 LED modules. */
static Adafruit_NeoPixel leds_g(NUM_LED_PANELS * NUM_LEDS_PER_PANEL,
        PIN_LED_STRIP, NEO_GRB | NEO_KHZ800);

void setup()
{
    // Initialize startup mode
    current_mode_g = new neo::mode_text;

    // Initialize NeoPixel instance and clear displays
    leds_g.begin();
    leds_g.show();

    // Input control array pin initialization
    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
    pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_BTN_SELECT, INPUT_PULLUP);
}

void loop()
{
    // Query and update input control array state
    input_ctrl_g
            .btn_up(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_UP)))
            .btn_down(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_DOWN)))
            .btn_left(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_LEFT)))
            .btn_right(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_RIGHT)))
            .btn_select(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_SELECT)));

    for (int i = 0; i < 60; ++i)
    {
        leds_g.setPixelColor(i, leds_g.Color(1, 0, 0));
    }
    leds_g.show();
}
