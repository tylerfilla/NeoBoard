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

#ifndef PIXELY_FONT_H
#define PIXELY_FONT_H

namespace neo
{

namespace font
{

/**
 * The default (and only) font.
 */
namespace pixely
{

/**
 * Height for pixely.
 */
extern const int height;

/**
 * Width for pixely.
 */
extern const int width;

/**
 * Font data for pixely.
 */
extern const unsigned char data[128][3];

} // namespace pixely

} // namespace font

} // namespace neo

#endif // #ifndef PIXELY_FONT_H
