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

#include <Arduino.h>

#include <ctype.h>
#include <string.h>

#include "display_pair.h"
#include "display_panel.h"
#include "input_ctrl.h"
#include "mode_serial.h"
#include "mode_text.h"
#include "string_store.h"

#define DEFAULT_BAUD 115200

neo::mode_serial::mode_serial(input_ctrl& p_input, display_pair& p_displays)
        : mode(p_input, p_displays)
        , m_indicator_text(p_input, p_displays)
        , m_line_buffer {}
        , m_line_buffer_len(0)
        , m_echo(true)
        , m_prompted(false)
{
    // Indicate serial mode
    m_indicator_text.show_string("SER", 3);

    // Open serial communication
    Serial.begin(DEFAULT_BAUD);

    // Print header
    Serial.println("NeoBoard Serial Interface");
}

neo::mode_serial::~mode_serial()
{
    // Close serial communication
    Serial.end();
}

void neo::mode_serial::cmd_crash()
{
    Serial.println("crashing the arduino");
    Serial.end();

    // Don't try this at home!
    reinterpret_cast<void (*)()>(0)();
}

void neo::mode_serial::cmd_ping()
{
    Serial.println("pong!");
}

void neo::mode_serial::cmd_show()
{
    // Get string index
    uint32_t index = atoi(m_line_buffer + 5);

    // Get and print string
    char str[neo::string_store::STRING_SIZE];
    if (neo::string_store::get(index, neo::string_store::STRING_SIZE, str))
    {
        m_indicator_text.show_string(str, strlen(str));
        Serial.println("ok");
    }
    else
    {
        Serial.println("failed");
    }
}

void neo::mode_serial::cmd_strings()
{
    if (strncmp(m_line_buffer + 8, "count", 5) == 0)
    {
        Serial.print("strings: ");
        Serial.println(neo::string_store::NUM_STRINGS);
        Serial.print("size: ");
        Serial.println(neo::string_store::STRING_SIZE);
    }
    else if (strncmp(m_line_buffer + 8, "erase", 5) == 0)
    {
        // Get string index
        uint32_t index = atoi(m_line_buffer + 14);

        // Erase the string
        if (neo::string_store::erase(index))
        {
            Serial.println("ok");
        }
        else
        {
            Serial.println("failed");
        }
    }
    else if (strncmp(m_line_buffer + 8, "get", 3) == 0)
    {
        // Get string index
        uint32_t index = atoi(m_line_buffer + 12);

        // Get and print string
        char str[neo::string_store::STRING_SIZE];
        if (neo::string_store::get(index, neo::string_store::STRING_SIZE, str))
        {
            Serial.println(str);
        }
        else
        {
            Serial.println("failed");
        }
    }
    else if (strncmp(m_line_buffer + 8, "put", 3) == 0)
    {
        // Get string index
        uint32_t index = atoi(m_line_buffer + 12);

        // Find the text argument
        char* text = m_line_buffer + 12;
        bool ready = false;
        for (;; ++text)
        {
            // Start looking after the first space
            if (isspace(*text))
            {
                ready = true;
                continue;
            }

            // Stop looking after the first non-space
            if (ready && !isspace(*text))
                break;
        }

        // Put given string
        if (neo::string_store::put(text, strlen(text), index))
        {
            Serial.println("ok");
        }
        else
        {
            Serial.println("failed");
        }
    }
    else
    {
        Serial.println("invalid action");
    }
}

void neo::mode_serial::exec_command()
{
    if (m_line_buffer_len == 0)
        return;

    if (strncmp(m_line_buffer, "crash", 4) == 0)
    {
        cmd_crash();
    }
    else if (strncmp(m_line_buffer, "ping", 4) == 0)
    {
        cmd_ping();
    }
    else if (strncmp(m_line_buffer, "show", 4) == 0)
    {
        cmd_show();
    }
    else if (strncmp(m_line_buffer, "strings", 7) == 0)
    {
        cmd_strings();
    }
    else
    {
        Serial.print("Command not found: ");
        Serial.write(m_line_buffer, m_line_buffer_len);
        Serial.println();
    }

    // Clear line buffer
    m_line_buffer_len = 0;

    // Begin another prompt
    m_prompted = false;
}

void neo::mode_serial::update()
{
    // Update the indicator text
    m_indicator_text.update();

    if (m_prompted)
    {
        if (Serial.available() > 0)
        {
            // Get byte
            char byte = Serial.read();

            // Handle backspace
            // This removes the last character and re-prompts
            if (byte == 8)
            {
                if (m_line_buffer_len > 0)
                {
                    m_line_buffer_len--;
                    Serial.println();
                    Serial.print("> ");
                    Serial.write(m_line_buffer, m_line_buffer_len);
                }
                return;
            }

            // Handle carriage return
            if (byte == '\r')
            {
                Serial.println();
                exec_command();
                return;
            }

            // Echo byte if desired
            if (m_echo)
            {
                Serial.write(byte);
            }

            // Read into line buffer if possible
            if (m_line_buffer_len < LINE_BUFFER_SIZE)
            {
                m_line_buffer[m_line_buffer_len++] = byte;
            }
        }
    }
    else
    {
        // Issue a prompt
        Serial.print("> ");
        m_prompted = true;
    }
}
