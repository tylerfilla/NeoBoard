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

#include "input_ctrl.h"

bool neo::input_ctrl::btn_up(int skim)
{
    if (!skim)
    {
        btn_up_changed_m = false;
    }
    return btn_up_m;
}

neo::input_ctrl& neo::input_ctrl::btn_up(bool btn_up_p)
{
    btn_up_changed_m = btn_up_m != btn_up_p;
    btn_up_m = btn_up_p;
    return *this;
}

bool neo::input_ctrl::btn_down(int skim)
{
    if (!skim)
    {
        btn_down_changed_m = false;
    }
    return btn_down_m;
}

neo::input_ctrl& neo::input_ctrl::btn_down(bool btn_down_p)
{
    btn_down_changed_m = btn_down_m != btn_down_p;
    btn_down_m = btn_down_p;
    return *this;
}

bool neo::input_ctrl::btn_left(int skim)
{
    if (!skim)
    {
        btn_left_changed_m = false;
    }
    return btn_left_m;
}

neo::input_ctrl& neo::input_ctrl::btn_left(bool btn_left_p)
{
    btn_left_changed_m = btn_left_m != btn_left_p;
    btn_left_m = btn_left_p;
    return *this;
}

bool neo::input_ctrl::btn_right(int skim)
{
    if (!skim)
    {
        btn_right_changed_m = false;
    }
    return btn_right_m;
}

neo::input_ctrl& neo::input_ctrl::btn_right(bool btn_right_p)
{
    btn_right_changed_m = btn_right_m != btn_right_p;
    btn_right_m = btn_right_p;
    return *this;
}

bool neo::input_ctrl::btn_select(int skim)
{
    if (!skim)
    {
        btn_select_changed_m = false;
    }
    return btn_select_m;
}

neo::input_ctrl& neo::input_ctrl::btn_select(bool btn_select_p)
{
    btn_select_changed_m = btn_select_m != btn_select_p;
    btn_select_m = btn_select_p;
    return *this;
}
