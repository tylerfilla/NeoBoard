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

#ifndef DISPLAY_PANEL_H
#define DISPLAY_PANEL_H

#include <stdint.h>

namespace neo
{

/**
 * A representation of a physical display panel.
 */
class display_panel
{
public:
    /**
     * A pixel color.
     */
    using color_t = uint32_t;

    /**
     * A dimension value.
     */
    using dim_t =  uint16_t;

protected:
    /**
     * The display width.
     */
    dim_t width_m;
    
    /**
     * The display height.
     */
    dim_t height_m;

    display_panel(dim_t width_p, dim_t height_p);

public:
    inline dim_t width() const
    { return width_m; }
    
    inline dim_t height() const
    { return height_m; }

    /**
     * Clear the buffer, but not the physical display.
     */
    virtual void clear() = 0;

    /**
     * Flush all buffered updates to the physical display.
     */
    virtual void flush() = 0;

    /**
     * Get the color of a particular pixel.
     */
    virtual color_t get_pixel(dim_t x, dim_t y) const = 0;

    /**
     * Set the color of a particular pixel. Not required to update immediately.
     */
    virtual void set_pixel(dim_t x, dim_t y,  color_t color) = 0;
};

} // namespace neo

#endif // #ifndef DISPLAY_PANEL_H
