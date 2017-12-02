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

#include "display_pair.h"
#include "display_panel.h"
#include "input_ctrl.h"
#include "mode_demo.h"
#include "mode_serial.h"

neo::mode_demo::mode_demo(input_ctrl& input_p, display_pair& displays_p)
        : mode(input_p, displays_p),
          mode_shown_m(nullptr)
{
}

neo::mode_demo::~mode_demo()
{
    if (mode_shown_m)
        delete mode_shown_m;
}

void neo::mode_demo::update()
{
    // Handle drop into serial mode
    if (input_m.btn_select() && input_m.btn_down_changed()
        && input_m.btn_down())
    {
        // Remove currently shown mode
        if (mode_shown_m)
            delete mode_shown_m;

        // Drop into serial mode
        mode_shown_m = new mode_serial(input_m, displays_m);
    }

    // Forward update to backing mode
    if (mode_shown_m)
    {
        mode_shown_m->update();
    }
}
