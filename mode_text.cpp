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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "display_pair.h"
#include "display_panel.h"
#include "font_pixely.h"
#include "mode_text.h"

neo::mode_text::mode_text(input_ctrl& input_p, display_pair& displays_p)
        : mode(input_p, displays_p),
          text_m {"$aT$cE$aS$cT"},
          text_len_m(12),
          current_obfuscated_char_m(' '),
          current_rainbow_color_m(DEFAULT_COLOR_FG),
          current_rainbow_hue_m(0),
          marquee_enable_m(false),
          marquee_step_m(0)
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
    // Clear the drawing surface
    // FIXME: This is not necessary all the time
    displays_m.surface_clear();

    // Change obfuscated character
    current_obfuscated_char_m = 0;
    while (!isprint(current_obfuscated_char_m))
    {
        current_obfuscated_char_m = rand() % 128;
    }

    // Increment rainbow hue
    current_rainbow_hue_m += 1;
    current_rainbow_hue_m %= 360;

    // FIXME: optimize this
    int r, g, b;
    hsl_to_rgb(current_rainbow_hue_m, 1.0f, 0.5f, r, g, b);

    current_rainbow_color_m = ((color_t) r << 16) | ((color_t) g << 8) | (color_t) b;

    if (marquee_enable_m)
    {
        marquee_step_m++;
    }

    // Alias the one and only font
    namespace font = neo::font::pixely;
    
    // Current text background and foreground colors
    color_t color_bg = DEFAULT_COLOR_BG;
    color_t color_fg = DEFAULT_COLOR_FG;

    // Indicates whether a formatting sequence is currently being processed
    bool format_escape = false;

    // The cumulative offset to apply to subsequent text characters
    int text_run_offset = 0;

    // Flag indicating obfuscated mode
    bool mode_obfuscated = false;

    for (size_t char_idx = 0; char_idx < text_len_m; ++char_idx)
    {
        // Get character value
        char char_text = text_m[char_idx];

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
                // Set foreground color to black (clear on typical LED strips)
                color_fg = 0x00000000;
                goto escape_done;
            case '1':
                // Set foreground color to dark blue
                color_fg = 0x000000aa;
                goto escape_done;
            case '2':
                // Set foreground color to dark green
                color_fg = 0x0000aa00;
                goto escape_done;
            case '3':
                // Set foreground color to dark aqua
                color_fg = 0x0000aaaa;
                goto escape_done;
            case '4':
                // Set foreground color to dark red
                color_fg = 0x00aa0000;
                goto escape_done;
            case '5':
                // Set foreground color to dark purple
                color_fg = 0x00aa00aa;
                goto escape_done;
            case '6':
                // Set foreground color to gold
                color_fg = 0x00ffaa00;
                goto escape_done;
            case '7':
                // Set foreground color to gray (or less bright white)
                color_fg = 0x00aaaaaa;
                goto escape_done;
            case '8':
                // Set foreground color to dark gray (even less bright white)
                color_fg = 0x00555555;
                goto escape_done;
            case '9':
                // Set foreground color to blue
                color_fg = 0x000000ff;
                goto escape_done;
            case 'a':
            case 'A':
                // Set foreground color to green
                color_fg = 0x0000ff00;
                goto escape_done;
            case 'b':
            case 'B':
                // Set foreground color to aqua
                color_fg = 0x0000ffff;
                goto escape_done;
            case 'c':
            case 'C':
                // Set foreground color to red
                color_fg = 0x00ff0000;
                goto escape_done;
            case 'd':
            case 'D':
                // Set foreground color to light purple
                color_fg = 0x00ff00ff;
                goto escape_done;
            case 'e':
            case 'E':
                // Set foreground color to yellow
                color_fg = 0x00ffff00;
                goto escape_done;
            case 'f':
            case 'F':
                // Set foreground color to white
                color_fg = 0x00ffffff;
                goto escape_done;
            case 'm':
                // Enable marquee mode
                // This is a global flag
                marquee_enable_m = true;
                goto escape_done;
            case 'o':
                // Enable obfuscated mode
                mode_obfuscated = true;
                goto escape_done;
            case 'r':
                // Reset a bunch of stuff
                mode_obfuscated = false;
                color_bg = DEFAULT_COLOR_BG;
                color_fg = DEFAULT_COLOR_FG;
                goto escape_done;
            case 's':
                // Rainbow mode
                // Set foreground color to rainbow frame
                color_fg = current_rainbow_color_m;
                goto escape_done;
            default:
                goto escape_continue;
            }

        escape_done:
            format_escape = false;
        escape_continue:
            continue;
        }

        // Handle obfuscated mode
        if (mode_obfuscated)
        {
            char_text = current_obfuscated_char_m;
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

                // Handle marquee animation
                if (marquee_enable_m)
                {
                    x -= marquee_step_m;

                    // Wrap x into proper range
                    while (x < 0)
                    {
                        x += displays_m.surface_width();
                    }
                    x %= displays_m.surface_width();
                }

                // Determine the color to be applied to this pixel
                // Use foreground color for text and background color everywhere else
                auto color = ((column_data >> row) & 1) ? color_fg : color_bg;

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
