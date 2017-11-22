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

#ifndef STRING_STORE_H
#define STRING_STORE_H

#include <stddef.h>
#include <stdint.h>

namespace neo
{

/**
 * A device that tightly packs saved strings into EEPROM. All other uses of
 * device EEPROM are in conflict, as this uses every last drop of it.
 */
namespace string_store
{

/**
 * Maximum allocated size of each string.
 */
constexpr auto STRING_SIZE = 128;

/**
 * Copy the string at the given index from the store.
 */
bool get(uint32_t index, size_t length, char* out_string);

/**
 * Copy the given string into the store at the given index.
 */
bool put(const char* string, size_t length, uint32_t index);

} // namespace string_store

} // namespace neo

#endif // #ifndef STRING_STORE_H
