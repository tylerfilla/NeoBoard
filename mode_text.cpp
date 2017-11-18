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

#include <string.h>

#include "display_pair.h"
#include "display_panel.h"
#include "font_pixely.h"
#include "mode_text.h"

#include <Arduino.h>

neo::mode_text::mode_text(input_ctrl& input_p, display_pair& displays_p)
        : mode(input_p, displays_p),
          color_bg_m(DEFAULT_COLOR_BG),
          color_fg_m(DEFAULT_COLOR_FG),
          active_color_m(color_fg_m),
          text_m {"$sMEEPMEEP"},
          text_len_m(10),
          current_obfuscated_char_m('?'),
          current_rainbow_color_m(DEFAULT_COLOR_FG),
          current_rainbow_hue_m(0)
{
}

static float hue_to_rgb(float p, float q, float t)
{
    if (t < 0)
    {
        ++t;
    }

    if (t > 1)
    {
        --t;
    }

    if (t < (float) 1 / 6)
        return p + (q - p) * 6 * t;

    if (t < (float) 1 / 2)
        return q;

    if (t < (float) 2 / 3)
        return p + (q - p) * ((float) 2 / 3 - t) * 6;

    return p;
}

static void hsl_to_rgb(float h, float s, float l, int& out_r, int& out_g, int& out_b)
{
    h /= (float) 360;
    float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
    float p = 2 * l - q;

    float r = hue_to_rgb(p, q, h + (float) 1 / 3);
    float g = hue_to_rgb(p, q, h);
    float b = hue_to_rgb(p, q, h - (float) 1 / 3);

    out_r = (int) (255.f * r);
    out_g = (int) (255.f * g);
    out_b = (int) (255.f * b);
}

void neo::mode_text::update()
{
    // Increment rainbow hue
    current_rainbow_hue_m += 5;
    current_rainbow_hue_m %= 360;

    // FIXME: optimize this
    int r, g, b;
    hsl_to_rgb(current_rainbow_hue_m, 1.0f, 0.5f, r, g, b);

    current_rainbow_color_m = ((color_t) r << 16) | ((color_t) g << 8) | (color_t) b;

    // Alias the one and only font
    namespace font = neo::font::pixely;

    // Indicates whether a formatting sequence is currently being processed
    bool format_escape = false;

    // The cumulative offset to apply to subsequent text characters
    int text_run_offset = 0;

    for (size_t char_idx = 0; char_idx < text_len_m; ++char_idx)
    {
        // Get character value
        auto char_text = text_m[char_idx];

        // Match unescaped format escape character
        if (char_text == '$' && !format_escape)
        {
            // Set format escape flag and move on to next character
            format_escape = true;
            continue;
        }

        if (format_escape)
        {
            // This character contributes to a formatting sequence
            switch (char_text)
            {
            case '0':
                // Set active color to black (clear on typical LED strips)
                active_color_m = 0x00000000;
                continue;
            case '1':
                // Set active color to dark blue
                active_color_m = 0x000000aa;
                continue;
            case '2':
                // Set active color to dark green
                active_color_m = 0x0000aa00;
                continue;
            case '3':
                // Set active color to dark aqua
                active_color_m = 0x0000aaaa;
                continue;
            case '4':
                // Set active color to dark red
                active_color_m = 0x00aa0000;
                continue;
            case '5':
                // Set active color to dark purple
                active_color_m = 0x00aa00aa;
                continue;
            case '6':
                // Set active color to gold
                active_color_m = 0x00ffaa00;
                continue;
            case '7':
                // Set active color to gray (or less bright white)
                active_color_m = 0x00aaaaaa;
                continue;
            case '8':
                // Set active color to dark gray (even less bright white)
                active_color_m = 0x00555555;
                continue;
            case '9':
                // Set active color to blue
                active_color_m = 0x000000ff;
                continue;
            case 'a':
                // Set active color to green
                active_color_m = 0x0000ff00;
                continue;
            case 'b':
                // Set active color to aqua
                active_color_m = 0x0000ffff;
                continue;
            case 'c':
                // Set active color to red
                active_color_m = 0x00ff0000;
                continue;
            case 'd':
                // Set active color to light purple
                active_color_m = 0x00ff00ff;
                continue;
            case 'e':
                // Set active color to yellow
                active_color_m = 0x00ffff00;
                continue;
            case 'f':
                // Set active color to white
                active_color_m = 0x00ffffff;
                continue;
            case 'r':
                // Reset all formatting
                color_bg_m = DEFAULT_COLOR_BG;
                color_fg_m = DEFAULT_COLOR_FG;
                active_color_m = color_fg_m;
                continue;
            case 's':
                // Set active color to the current rainbow frame
                active_color_m = current_rainbow_color_m;
                continue;
            default:
                // Not a formatting character, so clear escape flag
                format_escape = false;
                break;
            }
        }

        // Get character bitmap data and dimensions
        // Use the character value itself as an index into the font buffer
        // This is incredibly primitive, but it satisfies requirements
        auto char_bitmap = font::data[static_cast<size_t>(char_text)];
        auto char_height = font::height;
        auto char_width = font::width;

        for (int column = 0; column < char_width; ++column)
        {
            // Get column data
            auto column_data = char_bitmap[column];

            for (int row = 0; row < char_height; ++row)
            {
                // Get pixel coordinates on logical drawing surface
                int x = text_run_offset + column;
                int y = font::height - row - 1;

                // Determine the color to be applied to this pixel
                // Use foreground color for text and background color everywhere else
                auto color = ((column_data >> row) & 1) ? color_fg_m : color_bg_m;

                // Buffer the pixel color
                displays_m.set_pixel(x, y, color);
            }
        }

        // Account for character dimensions in text run
        // This assumes text runs horizontally
        text_run_offset += char_width;
    }

    // Flush the drawing buffer
    displays_m.surface_flush();
}
