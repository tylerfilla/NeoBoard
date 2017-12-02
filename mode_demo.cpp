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

neo::mode_demo::mode_demo(input_ctrl& p_input, display_pair& p_displays)
        : mode(p_input, p_displays)
        , m_submode(nullptr)
{
}

neo::mode_demo::~mode_demo()
{
    if (m_submode)
        delete m_submode;
}

void neo::mode_demo::update()
{
    // Handle drop into serial mode
    if (input_m.btn_select() && input_m.btn_down_changed()
            && input_m.btn_down())
    {
        // Remove currently shown mode
        if (m_submode)
            delete m_submode;

        // Drop into serial mode
        m_submode = new mode_serial(input_m, displays_m);
    }

    // Forward update to backing mode
    if (m_submode)
    {
        m_submode->update();
    }
}
