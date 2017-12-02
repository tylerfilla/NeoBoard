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

#ifndef MODE_DEMO_H
#define MODE_DEMO_H

#include "mode.h"

namespace neo
{

/**
 * A mode for introduction and friendly startups.
 */
class mode_demo : public mode
{
    /**
     * The current mode being demoed at the moment.
     */
    mode* m_submode;

public:
    mode_demo(input_ctrl& p_input, display_pair& p_displays);

    ~mode_demo() override;

    void update() override;
};

} // namespace neo

#endif // #ifndef MODE_DEMO_H
