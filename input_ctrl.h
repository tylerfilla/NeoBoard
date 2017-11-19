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

#ifndef INPUT_CTRL_H
#define INPUT_CTRL_H

namespace neo
{

/**
 * A representation of the input control array (the physicall arrangement of
 * buttons on the shield).
 */
class input_ctrl
{
    /**
     * The current state of the d-pad up button.
     */
    bool btn_up_m;

    /**
     * Whether btn_up_m has recently changed.
     */
    bool btn_up_changed_m;

    /**
     * The current state of the d-pad down button.
     */
    bool btn_down_m;

    /**
     * Whether btn_down_m has recently changed.
     */
    bool btn_down_changed_m;

    /**
     * The current state of the d-pad left button.
     */
    bool btn_left_m;

    /**
     * Whether btn_left_m has recently changed.
     */
    bool btn_left_changed_m;

    /**
     * The current state of the d-pad right button.
     */
    bool btn_right_m;

    /**
     * Whether btn_right_m has recently changed.
     */
    bool btn_right_changed_m;

    /**
     * The current state of the select button.
     */
    bool btn_select_m;

    /**
     * Whether btn_select_m has recently changed.
     */
    bool btn_select_changed_m;

public:
    bool btn_up();

    input_ctrl& btn_up(bool btn_up_p);

    inline bool btn_up_changed() const
    { return btn_up_changed_m; }

    bool btn_down();

    input_ctrl& btn_down(bool btn_down_p);

    inline bool btn_down_changed() const
    { return btn_down_changed_m; }

    bool btn_left();

    input_ctrl& btn_left(bool btn_left_p);

    inline bool btn_left_changed() const
    { return btn_left_changed_m; }

    bool btn_right();

    input_ctrl& btn_right(bool btn_right_p);

    inline bool btn_right_changed() const
    { return btn_right_changed_m; }

    bool btn_select();

    input_ctrl& btn_select(bool btn_select_p);

    inline bool btn_select_changed() const
    { return btn_select_changed_m; }
};

} // namespace neo

#endif // #ifndef INPUT_CTRL_H
