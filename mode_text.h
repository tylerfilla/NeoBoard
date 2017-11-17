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

#define TEXT_BUFFER_SIZE 512

namespace neo
{

/**
 * A mode for displaying and editing text. No internationalization efforts are
 * made whatsoever.
 */
class mode_text : public mode
{
    /**
     * The active text buffer. Plain ol' ASCII with some special in-band
     * formatting sequences for your viewing pleasure. Not null-terminated.
     */
    char text_m[TEXT_BUFFER_SIZE];

    /**
     * The length of the text string currently active in the buffer.
     */
    size_t text_len_m;

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
