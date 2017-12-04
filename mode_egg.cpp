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

#include <stdlib.h>

#include "clock.h"
#include "display_pair.h"
#include "display_panel.h"
#include "input_ctrl.h"
#include "mode_egg.h"

neo::mode_egg::mode_egg(input_ctrl& p_input, display_pair& p_displays)
        : mode(p_input, p_displays)
        , m_last_tick_time(0)
        , m_game_initialized(false)
        , m_ctrl_h(0)
        , m_ctrl_v(0)
        , m_vel_x(0)
        , m_vel_y(0)
        , m_apple_x(0)
        , m_apple_y(0)
        , m_apple_placed(false)
        , m_snake {}
        , m_snake_len()
{
}

void neo::mode_egg::tick()
{
    //
    // Game Logic
    //

    // TODO: Collect entropy from the player

    // If not done already, initialize the game
    if (!m_game_initialized)
    {
        // Reset controls
        m_ctrl_h = 0;
        m_ctrl_v = 0;

        // Reset velocity
        m_vel_x = 0;
        m_vel_y = 0;

        // Position the snake parts
        // This creates an initial snake of length 3 (a 3-snake?)
        m_snake[0].m_pos_x = 2;
        m_snake[0].m_pos_y = 0;
        m_snake[0].m_waiting = false;
        m_snake[1].m_pos_x = 1;
        m_snake[1].m_pos_y = 0;
        m_snake[1].m_waiting = false;
        m_snake[2].m_pos_x = 0;
        m_snake[2].m_pos_y = 0;
        m_snake[2].m_waiting = false;
        m_snake_len = 3;

        // Mark apple to be placed on the board
        m_apple_placed = false;

        // Mark game initialized
        m_game_initialized = true;
    }

    // If not done already, randomly place the apple on the board
    if (!m_apple_placed)
    {
        // Choose random coordinates for apple
        m_apple_x = rand() % m_displays.surface_width();
        m_apple_y = rand() % m_displays.surface_height();

        // TODO: Generate different "apple colors"

        // Mark apple placed
        m_apple_placed = true;
    }

    // If snake is moving
    if (m_vel_x != 0 || m_vel_y != 0)
    {
        // Move snake piece-by-piece
        int last_part_x;
        int last_part_y;
        for (int i = 0; i < m_snake_len; ++i)
        {
            // If this part is the head
            if (i == 0)
            {
                // Move head according to velocity
                last_part_x = m_snake[0].m_pos_x;
                last_part_y = m_snake[0].m_pos_y;
                m_snake[0].m_pos_x += m_vel_x;
                m_snake[0].m_pos_y += m_vel_y;

                // Check for snake self-intersection
                for (int j = 1; j < m_snake_len; ++j)
                {
                    // If the head just hit another snake part
                    if (m_snake[0].m_pos_x == m_snake[j].m_pos_x
                            && m_snake[0].m_pos_y == m_snake[j].m_pos_y)
                    {
                        // FIXME
                        m_game_initialized = false;
                        return;
                    }
                }
            }
            else
            {
                // Move subsequent snake parts to earlier parts' last positions
                int old_x = m_snake[i].m_pos_x;
                int old_y = m_snake[i].m_pos_y;
                m_snake[i].m_pos_x = last_part_x;
                m_snake[i].m_pos_y = last_part_y;
                last_part_x = old_x;
                last_part_y = old_y;

                // Clear waiting flag
                m_snake[i].m_waiting = false;
            }
        }
    }

    // If snake head hit apple
    if (m_snake[0].m_pos_x == m_apple_x && m_snake[0].m_pos_y == m_apple_y)
    {
        // Consume apple and schedule a replace
        m_apple_placed = false;

        // Increase snake length if there is room to grow
        if (m_snake_len < SNAKE_MAX_SIZE)
        {
            m_snake[m_snake_len].m_waiting = true;
            m_snake_len++;
        }
    }

    //
    // Render Logic
    //

    // Begin drawing frame
    m_displays.surface_clear();

    // Draw apple
    m_displays.set_pixel(m_apple_x, m_apple_y, 0x00ff0000);

    // Draw snake
    for (int i = 0; i < m_snake_len; ++i)
    {
        // Ignore those parts lazily waiting for insertion
        if (m_snake[i].m_waiting)
            continue;

        m_displays.set_pixel(m_snake[i].m_pos_x, m_snake[i].m_pos_y, 0x0000ff00);
    }

    // Finish drawing frame
    m_displays.surface_flush();
}

void neo::mode_egg::update()
{
    // Handle directional controls
    if (m_input.btn_up_changed())
    {
        if (m_input.btn_up())
        {
            m_ctrl_v--;
        }
        else
        {
            m_ctrl_v++;
        }
    }
    if (m_input.btn_down_changed())
    {
        if (m_input.btn_down())
        {
            m_ctrl_v++;
        }
        else
        {
            m_ctrl_v--;
        }
    }
    if (m_input.btn_left_changed())
    {
        if (m_input.btn_left())
        {
            m_ctrl_h--;
        }
        else
        {
            m_ctrl_h++;
        }
    }
    if (m_input.btn_right_changed())
    {
        if (m_input.btn_right())
        {
            m_ctrl_h++;
        }
        else
        {
            m_ctrl_h--;
        }
    }

    // Bridge control sums and snake velocity
    // This affects the snake outside of the game tick cycle
    if (m_ctrl_h)
    {
        m_vel_x = m_ctrl_h;
        m_vel_y = 0;
    }
    if (m_ctrl_v)
    {
        m_vel_x = 0;
        m_vel_y = m_ctrl_v;
    }

    // Handle game tick
    unsigned long current_time = neo::clock::uptime_millis();
    if (m_last_tick_time == 0 || current_time - m_last_tick_time > 250)
    {
        m_last_tick_time = current_time;
        tick();
    }
}
