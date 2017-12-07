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

#ifndef CLOCK_H
#define CLOCK_H

namespace neo
{

/**
 * An abstraction over system timekeeping tool.
 */
namespace clock
{

/**
 * Get the number of milliseconds elapsed since boot time.
 */
unsigned long uptime_millis();

/**
 * Wait for the specified number of milliseconds.
 */
unsigned long delay_millis(unsigned long millis);

} // namespace clock

} // namespace neo

#endif // #ifndef CLOCK_H
