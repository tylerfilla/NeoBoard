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

#ifndef MODE_SERIAL_H
#define MODE_SERIAL_H

#include "mode.h"

namespace neo
{

class mode_text;

/**
 * A serial interface mode. If you have a computer handy, this is faster than
 * the built-in editor interface.
 */
class mode_serial : public mode
{
    /**
     * A text mode instance for indication purposes.
     */
    mode_text* indicator_text_m;

public:
    mode_serial(input_ctrl& input_p, display_pair& displays_p);

    ~mode_serial() override;

    void update() override;
};

} // namespace neo

#endif // #ifndef MODE_SERIAL_H
