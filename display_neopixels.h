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

#ifndef DISPLAY_NEOPIXELS_H
#define DISPLAY_NEOPIXELS_H

#include "display_panel.h"

class Adafruit_NeoPixel;

namespace neo
{

/**
 * A display panel based on Adafruit NeoPixel-compatible LED strips.
 */
class display_neopixels : public display_panel
{
    /**
     * The Adafruit NeoPixel backend instance. This instance buffers LED states
     * internally.
     */
    Adafruit_NeoPixel& neopixels_m;

    /**
     * The LED strip offset.
     */
    unsigned int offset_m;

public:
    display_neopixels(Adafruit_NeoPixel& neopixels_p, unsigned int offset_p,
        unsigned int width_p, unsigned int height_p);

    /**
     * Get the Adafruit NeoPixel backend instance.
     */
    inline Adafruit_NeoPixel& neopixels()
    { return neopixels_m; }

    /**
     * Get the LED strip offset.
     */
    inline unsigned int offset() const
    { return offset_m; }

    void clear() override;

    void flush() override;

    color_t get_pixel(dim_t x, dim_t y) const override;

    void set_pixel(dim_t x, dim_t y,  color_t color) override;
};

} // namespace neo

#endif // #ifndef DISPLAY_NEOPIXELS_H
