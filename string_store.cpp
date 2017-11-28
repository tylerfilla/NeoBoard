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

#include <EEPROM.h>
#include "string_store.h"

bool neo::string_store::get(uint32_t index, size_t length, char* out_string)
{
    if (index > EEPROM.length() / STRING_SIZE)
        return false;

    if (length > STRING_SIZE)
    {
        length = STRING_SIZE;
    }

    // Copy string from EEPROM
    for (size_t i = 0; i < length; ++i)
    {
        out_string[i] = EEPROM[index * STRING_SIZE + i];
    }

    return true;
}

bool neo::string_store::put(const char* string, size_t length, uint32_t index)
{
    if (index > EEPROM.length() / STRING_SIZE)
        return false;

    if (length > STRING_SIZE)
    {
        length = STRING_SIZE;
    }

    // Copy string into EEPROM with trailing zeroes
    for (size_t i = 0; i < STRING_SIZE; ++i)
    {
        EEPROM[index * STRING_SIZE + i] = i < length ? string[i] : '\0';
    }

    return true;
}
