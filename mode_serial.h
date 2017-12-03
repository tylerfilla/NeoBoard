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

#ifndef MODE_SERIAL_H
#define MODE_SERIAL_H

#include <stddef.h>

#include "mode.h"
#include "mode_text.h"

namespace neo
{

/**
 * A serial interface mode. If you have a computer handy, this is faster than
 * the built-in editor interface.
 */
class mode_serial : public mode
{
public:
    /**
     * The size of the line buffer.
     */
    static constexpr auto LINE_BUFFER_SIZE = 128;

private:
    /**
     * A text mode instance for indication purposes.
     */
    mode_text m_indicator_text;

    /**
     * The line buffer to hold the current input line. Not null-terminated.
     */
    char m_line_buffer[LINE_BUFFER_SIZE];

    /**
     * The length of the text currently stored in the line buffer.
     */
    size_t m_line_buffer_len;

    /**
     * Whether text should be echoed back to the client.
     */
    bool m_echo;

    /**
     * Whether the client has been prompted.
     */
    bool m_prompted;

    /**
     * Command: crash
     * Crash the Arduino for science.
     */
    void cmd_crash();

    /**
     * Command: strings
     * Test the connection (as if it weren't already known).
     */
    void cmd_ping();

    /**
     * Command: show
     * Show the given text string on the display.
     */
    void cmd_show();

    /**
     * Command: strings
     * Edit text in the string store.
     */
    void cmd_strings();

    void exec_command();

public:
    mode_serial(input_ctrl& p_input, display_pair& p_displays);

    ~mode_serial() override;

    void update() override;
};

} // namespace neo

#endif // #ifndef MODE_SERIAL_H
