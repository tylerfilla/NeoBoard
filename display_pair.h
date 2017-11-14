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

#ifndef DISPLAY_PAIR_H
#define DISPLAY_PAIR_H

namespace neo
{

class display_panel;

/**
 * A planar display surface built from 2 display panels, because that's all I
 * have to deal with here. The displays can be rearranged, flipped, and
 * rotated.
 */
class display_pair
{
    /**
     * The first display panel.
     */
    display_panel& panel1_m;

    /**
     * The second display panel.
     */
    display_panel& panel2_m;

public:
    display_pair(display_panel& panel1_p, display_panel& panel2_p);

    inline display_panel& panel1()
    { return panel1_m; }

    inline display_panel& panel2()
    { return panel2_m; }
};

} // namespace neo

#endif // #ifndef DISPLAY_PAIR_H
