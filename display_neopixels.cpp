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
#include "display_neopixels.h"

#define MAP2D(offset, width, x, y) ((offset) + ((y) % 2 ? (width) - (x) - 1 : \
    (x)) % (width) + (y) * (width))

neo::display_neopixels::display_neopixels(Adafruit_NeoPixel& neopixels_p,
    unsigned int offset_p, unsigned int width_p, unsigned int height_p)
        : display_panel(width_p, height_p),
          neopixels_m(neopixels_p),
          offset_m(offset_p)
{
}

neo::display_neopixels::~display_neopixels()
{
}

void neo::display_neopixels::clear()
{
    // Delegate to NeoPixel instance
    neopixels_m.clear();
}

void neo::display_neopixels::flush()
{
    // Read the buffer out to the LEDs
    neopixels_m.show();
}

neo::display_neopixels::color_t neo::display_neopixels::get_pixel(dim_t x,
    dim_t y) const
{
    // Delegate to NeoPixel instance
    return neopixels_m.getPixelColor(MAP2D(offset_m, width_m, x, y));
}

void neo::display_neopixels::set_pixel(dim_t x, dim_t y, color_t color)
{
    // Delegate to NeoPixel instance
    neopixels_m.setPixelColor(MAP2D(offset_m, width_m, x, y), color);
}
