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

#ifndef MODE_TEXT_H
#define MODE_TEXT_H

#include "mode.h"

#define TEXT_BUFFER_SIZE 256

namespace neo
{

/**
 * A mode for displaying and editing text. No internationalization efforts are
 * made whatsoever.
 */
class mode_text : public mode
{
public:
    /**
     * A text color.
     */
    using color_t = uint32_t;

    /**
     * Default text background color.
     */
    static constexpr color_t DEFAULT_COLOR_BG = 0x00000000;

    /**
     * Default text foreground color.
     */
    static constexpr color_t DEFAULT_COLOR_FG = 0x00ffffff;

private:
    /**
     * The active text buffer. Plain ol' ASCII with some special in-band
     * formatting sequences for your viewing pleasure. Not null-terminated.
     */
    char text_m[TEXT_BUFFER_SIZE];

    /**
     * The length of the text string currently active in the buffer. This
     * includes formatting sequences.
     */
    size_t text_len_m;

    /**
     * The current substitute for obfuscated characters.
     */
    char current_obfuscated_char_m;

    /**
     * The current rainbow color.
     */
    color_t current_rainbow_color_m;

    /**
     * The current rainbow hue used to derive the rainbow color.
     */
    int current_rainbow_hue_m;

public:
    mode_text(input_ctrl& input_p, display_pair& displays_p);

    inline const char* text() const
    { return text_m; }

    inline size_t text_len() const
    { return text_len_m; }

    void update() override;
};

} // namespace neo

#endif // #ifndef MODE_TEXT_H
