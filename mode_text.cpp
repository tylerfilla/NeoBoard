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

#include "mode_text.h"

neo::mode_text::mode_text(display_panel& display_p)
        : display_m(display_p)
{
}

neo::mode_text::mode_text(mode_text& rhs)
        : display_m(rhs.display_m)
{
}

neo::mode_text::~mode_text()
{
}

void neo::mode_text::update()
{
    display_m.set_pixel(0, 0, 0x00ff0000);
    display_m.set_pixel(1, 1, 0x00ff0000);
    display_m.set_pixel(2, 2, 0x00ff0000);
    display_m.set_pixel(3, 3, 0x00ff0000);
    display_m.set_pixel(4, 4, 0x00ff0000);
    display_m.flush();
}
