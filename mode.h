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

#ifndef MODE_H
#define MODE_H

namespace neo
{

class display_pair;
class input_ctrl;

/**
 * A mode of operation.
 */
class mode
{
protected:
    /**
     * The target display group.
     */
    display_pair& displays_m;

    /**
     * The input control array.
     */
    input_ctrl& input_m;

    mode(input_ctrl& input_p, display_pair& displays_p);

    mode(mode& mode_p);

public:
    /**
     * Update the mode.
     */
    virtual void update() = 0;
};

} // namespace neo

#endif // #ifndef MODE_H
