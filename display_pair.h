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

#include "display_panel.h"

namespace neo
{

/**
 * A planar display surface built from 2 display panels, because that's all I
 * have to deal with here. The displays can be swapped for flexibility in
 * physical mounting.
 */
class display_pair
{
public:
    /**
     * Color value alias.
     */
    using color_t = neo::display_panel::color_t;

    /**
     * Dimension value alias.
     */
    using dim_t = neo::display_panel::dim_t;

private:
    /**
     * The first display panel.
     */
    display_panel& panel1_m;

    /**
     * The second display panel.
     */
    display_panel& panel2_m;

    /**
     * Whether the display panels are swapped. Normally panel 1 is said to be
     * the "before" panel (e.g. when displaying text) and 2 the "after" panel.
     * When swapped, 2 comes before 1.
     */
    bool swapped_m;

    /**
     * Whether the display panels are vertically stacked. Normally panel 1 is
     * to the left of panel 2 (when not swapped). Stacking places panel 1
     * normally above panel 2 (vice-versa when swapped).
     */
    bool stacked_m;

    /**
     * Determine if a point is visible on either panel.
     */
    bool is_point_visible(dim_t x, dim_t y) const;

    /**
     * Get whichever panel has the given point on the drawing surface.
     */
    display_panel& panel_for_point(dim_t x, dim_t y, dim_t& out_px,
        dim_t& out_py) const;

public:
    display_pair(display_panel& panel1_p, display_panel& panel2_p);

    inline display_panel& panel1()
    { return panel1_m; }

    inline display_panel& panel2()
    { return panel2_m; }

    inline bool swapped() const
    { return swapped_m; }

    inline bool stacked() const
    { return stacked_m; }

    /**
     * Get whichever panel logically comes first (the "before" panel).
     */
    inline display_panel& panel_before() const
    { return swapped_m ? panel2_m : panel1_m; }

    /**
     * Get whichever panel logically comes second (the "after" panel).
     */
    inline display_panel& panel_after() const
    { return swapped_m ? panel1_m : panel2_m; }

    /**
     * Swap (or un..swap?) the panels' logical order.
     */
    inline void swap()
    { swapped_m = !swapped_m; }

    /**
     * Toggle the vertically-stacked state of the panels.
     */
    inline void stack()
    { stacked_m = !stacked_m; }

    inline dim_t surface_width() const
    { return panel1_m.width() + (stacked_m ? 0 : panel2_m.width()); }

    inline dim_t surface_height() const
    { return panel1_m.height() + (stacked_m ? panel2_m.height() : 0); }

    /**
     * Clear the drawing surface.
     */
    inline void surface_clear()
    {
        panel1_m.clear();
        panel2_m.clear();
    }

    /**
     * Flush all buffered updates to the drawing surface.
     */
    inline void surface_flush()
    {
        panel1_m.flush();
        panel2_m.flush();
    }

    /**
     * Get the color of a particular pixel on the drawing surface.
     */
    color_t get_pixel(dim_t x, dim_t y) const;

    /**
     * Set the color of a particular pixel on the drawing surface.
     */
    void set_pixel(dim_t x, dim_t y, color_t color);
};

} // namespace neo

#endif // #ifndef DISPLAY_PAIR_H
