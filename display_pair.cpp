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

#include "display_pair.h"

neo::display_panel& neo::display_pair::panel_for_point(dim_t x, dim_t y,
    dim_t& out_px, dim_t& out_py)
{
    auto& p_before = panel_before();
    auto& p_after = panel_after();
        
    auto before_x_start = 0;
    auto before_x_stop = before_x_start + p_before.width();
    auto before_y_start = 0;
    auto before_y_stop = before_y_start + p_before.height();

    auto after_x_start = stacked_m ? 0 : before_x_stop;
    auto after_x_stop = after_x_start + p_after.width();
    auto after_y_start = stacked_m ? before_y_stop : 0;
    auto after_y_stop = after_y_start + p_after.height();

    if (x >= before_x_start && x < before_x_stop
        && y >= before_y_start && y < before_y_stop)
    {
        out_px = x - before_x_start;
        out_py = y - before_y_start;
        return p_before;
    }
    else
    {
        out_px = x - after_x_start;
        out_py = y - after_y_start;
        return p_after;
    }
}

neo::display_pair::display_pair(display_panel& panel1_p,
    display_panel& panel2_p)
        : panel1_m(panel1_p),
          panel2_m(panel2_p)
{
}

neo::display_pair::color_t neo::display_pair::get_pixel(dim_t x, dim_t y) const
{
    dim_t px, py;
    auto& panel = panel_for_point(x, y, px, py);
    return panel.get_pixel(px, py);
}

void neo::display_pair::set_pixel(dim_t x, dim_t y, color_t color)
{
    dim_t px, py;
    auto& panel = panel_for_point(x, y, px, py);
    panel.set_pixel(px, py, color);
}
