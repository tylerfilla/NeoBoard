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

#ifndef EFFECT_TEXT_H
#define EFFECT_TEXT_H

namespace neo
{

/**
 * A special text string that supports spans of character effects.
 */
class effect_text
{
    /**
     * The raw text content.
     */
    const char* raw_text_m;

public:
    effect_text();

    effect_text(const char* raw_text_p);

    ~effect_text();

    /**
     * Get the raw text content.
     */
    inline const char* raw_text() const
    { return raw_text_m; }
};

} // namespace neo

#endif // #ifndef EFFECT_TEXT_H
