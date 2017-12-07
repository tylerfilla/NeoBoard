/*
 * NeoBoard - LED Panel Controller
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

#include "clock.h"
#include "display_neopixels.h"
#include "display_pair.h"
#include "input_ctrl.h"
#include "mode.h"
#include "mode_demo.h"
#include "mode_text.h"

/**
 * Pin for the up directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_UP = 11;

/**
 * Pin for the down directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_DOWN = 12;

/**
 * Pin for the left directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_LEFT = 13;

/**
 * Pin for the right directional button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_RIGHT = 10;

/**
 * Pin for the select button. Pulled up. Use a N/O switch to GND.
 */
static constexpr auto PIN_BTN_SELECT = 9;

/**
 * Pin connected to the WS2812 LED strip data input line (5V logic, use a
 * low-ish value resistor). This daisy-chains through each panel.
 */
static constexpr auto PIN_LED_STRIP = 8;

/**
 * The width of each panel.
 */
static constexpr auto PANEL_WIDTH = 12;

/**
 * The height of each panel.
 */
static constexpr auto PANEL_HEIGHT = 5;

/**
 * Macro for determining if a pulled-up N/O switch is currently closed.
 */
#define IS_SWITCH_CLOSED(pin) ((pin) == LOW)

/**
 * State for input control array.
 */
static neo::input_ctrl g_input_ctrl;

/**
 * State for WS2812 LED modules.
 */
static Adafruit_NeoPixel g_leds(2 * PANEL_WIDTH * PANEL_HEIGHT,
        PIN_LED_STRIP, NEO_GRB | NEO_KHZ800);

/**
 * The first display panel.
 */
static neo::display_neopixels g_display1(g_leds, 0, PANEL_WIDTH, PANEL_HEIGHT);

/**
 * The second display panel.
 */
static neo::display_neopixels g_display2(g_leds, PANEL_WIDTH * PANEL_HEIGHT,
        PANEL_WIDTH, PANEL_HEIGHT);

/**
 * A pairwise arrangement of both display panels.
 */
static neo::display_pair g_displays(g_display1, g_display2);

/**
 * The text mode.
 */
static neo::mode_text g_mode_text(g_input_ctrl, g_displays);

/**
 * The demo mode.
 */
static neo::mode_demo g_demo_mode(g_input_ctrl, g_displays, g_mode_text);

/**
 * The time at which the demo should end.
 */
static unsigned long g_demo_end_time;

/**
 * Whether the demo has ended yet.
 */
static bool g_demo_ended = false;

/**
 * External flag indicating a desire to keep the demo alive.
 */
extern bool g_keep_demo;

void setup()
{
    // Schedule conclusion of demo
    g_demo_end_time = neo::clock::uptime_millis() + 3000;

    // Initialize NeoPixel instance and clear displays
    g_leds.begin();
    g_leds.show();

    // Input control array pin initialization
    pinMode(PIN_BTN_UP, INPUT_PULLUP);
    pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
    pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
    pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_BTN_SELECT, INPUT_PULLUP);
}

void loop()
{
    // Update input control array state
    g_input_ctrl
            .btn_up(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_UP)))
            .btn_down(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_DOWN)))
            .btn_left(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_LEFT)))
            .btn_right(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_RIGHT)))
            .btn_select(IS_SWITCH_CLOSED(digitalRead(PIN_BTN_SELECT)));

    // Update the appropriate mode
    if (!g_keep_demo && neo::clock::uptime_millis() >= g_demo_end_time)
    {
        // Transition from demo to normal mode
        if (!g_demo_ended)
        {
            // Reset text mode
            g_mode_text.reset();
            g_demo_ended = true;
        }

        g_mode_text.update();
    }
    else
    {
        g_demo_mode.update();
    }
}
