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
#include "mode_egg.h"
#include "mode_serial.h"
#include "mode_text.h"

neo::mode_demo::mode_demo(input_ctrl& p_input, display_pair& p_displays)
        : mode(p_input, p_displays)
        , m_submode(new neo::mode_text(p_input, p_displays))
        , m_secret_progress(0)
{
}

neo::mode_demo::~mode_demo()
{
    if (m_submode)
        delete m_submode;
}

void neo::mode_demo::update()
{
    // OooooOOOoo!
    if (m_secret_progress == 0 && m_input.btn_up_changed()
            && m_input.btn_up(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 1 && m_input.btn_up_changed()
            && m_input.btn_up(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 2 && m_input.btn_down_changed()
            && m_input.btn_down(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 3 && m_input.btn_down_changed()
            && m_input.btn_down(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 4 && m_input.btn_left_changed()
            && m_input.btn_left(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 5 && m_input.btn_right_changed()
            && m_input.btn_right(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 6 && m_input.btn_left_changed()
            && m_input.btn_left(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 7 && m_input.btn_right_changed()
            && m_input.btn_right(1))
    {
        m_secret_progress++;
    }
    else if (m_secret_progress == 8 && m_input.btn_select_changed()
            && m_input.btn_select(1))
    {
        // Remove currently shown submode
        if (m_submode)
            delete m_submode;

        // Drop into egg submode
        m_submode = new mode_egg(m_input, m_displays);

        m_secret_progress = 0;
    }

    // Handle drop into serial mode
    if (m_input.btn_select() && m_input.btn_left_changed()
            && m_input.btn_left())
    {
        // Remove currently shown submode
        if (m_submode)
            delete m_submode;

        // Drop into serial submode
        m_submode = new mode_serial(m_input, m_displays);
    }

    // Forward update to backing mode
    if (m_submode)
    {
        m_submode->update();
    }
}
