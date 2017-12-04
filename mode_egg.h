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

#ifndef MODE_EGG_H
#define MODE_EGG_H

#include "mode.h"

namespace neo
{

/**
 * An Easter egg, mayhaps?
 */
class mode_egg : public mode
{
    /**
     * The maximum size of the snake.
     */
    static constexpr auto SNAKE_MAX_SIZE = 32;

    /**
     * The time of the last game tick.
     */
    unsigned long m_last_tick_time;

    /**
     * Whether the current game has been initialized.
     */
    bool m_game_initialized;

    /**
     * Horizontal control sum.
     */
    int m_ctrl_h;

    /**
     * Vertical control sum.
     */
    int m_ctrl_v;

    /**
     * The horizontal velocity of the snake head.
     */
    int m_vel_x;

    /**
     * The vertical velocity of the snake head.
     */
    int m_vel_y;

    /**
     * The apple x-coordinate.
     */
    int m_apple_x;

    /**
     * The apple y-coordinate.
     */
    int m_apple_y;

    /**
     * Whether the apple has been placed on the board.
     */
    bool m_apple_placed;

    /**
     * The various parts of the snake.
     */
    struct snake_part
    {
        /**
         * The x-coordinate of the part.
         */
        int m_pos_x;

        /**
         * The y-coordinate of the part.
         */
        int m_pos_y;

        /**
         * Whether the part is waiting to be entered into the game. This is
         * used to do lazy insertion when an apple is eaten.
         */
        bool m_waiting;
    } m_snake[SNAKE_MAX_SIZE];

    /**
     * The current length of the snake.
     */
    int m_snake_len;

    /**
     * Perform a game tick.
     */
    void tick();

public:
    mode_egg(input_ctrl& p_input, display_pair& p_displays);

    void update() override;
};

} // namespace neo

#endif // #ifndef MODE_EGG_H
