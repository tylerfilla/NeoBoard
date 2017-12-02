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

#include <Arduino.h>

#include "display_pair.h"
#include "display_panel.h"
#include "input_ctrl.h"
#include "mode_serial.h"
#include "mode_text.h"

#define DEFAULT_BAUD 9600

neo::mode_serial::mode_serial(input_ctrl& input_p, display_pair& displays_p)
        : mode(input_p, displays_p),
          indicator_text_m(new mode_text(input_p, displays_p))
{
    // Open serial communication
    Serial.begin(DEFAULT_BAUD);
}

neo::mode_serial::~mode_serial()
{
    // Close serial communication
    Serial.end();

    if (indicator_text_m)
        delete indicator_text_m;
}

void neo::mode_serial::update()
{
    Serial.println("boop");
}
