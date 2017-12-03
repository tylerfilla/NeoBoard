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

#ifndef MODE_TEXT_H
#define MODE_TEXT_H

#include <stddef.h>
#include "mode.h"

namespace neo
{

/**
 * A mode for displaying and editing text. No internationalization efforts are
 * made whatsoever.
 */
class mode_text : public mode
{
public:
    /**
     * A text color.
     */
    using color_t = uint32_t;

    /**
     * The maximum allocated size for the text buffer.
     */
    static constexpr auto TEXT_BUFFER_SIZE = 128;

    /**
     * Default text background color.
     */
    static constexpr color_t DEFAULT_COLOR_BG = 0x00000000;

    /**
     * Default text foreground color.
     */
    static constexpr color_t DEFAULT_COLOR_FG = 0x00ffffff;

private:
    /**
     * The active text buffer. Plain ol' ASCII with some special in-band
     * formatting sequences for your viewing pleasure. Not null-terminated.
     */
    char m_text[TEXT_BUFFER_SIZE];

    /**
     * The length of the text string currently active in the buffer. This
     * includes formatting sequences.
     */
    size_t m_text_len;

    /**
     * The current substitute for obfuscated characters.
     */
    char m_current_obfuscated_char;

    /**
     * The current rainbow A color.
     */
    color_t m_current_rainbow_color_a;

    /**
     * The current rainbow B color.
     */
    color_t m_current_rainbow_color_b;

    /**
     * The current rainbow hue used to drive both rainbow colors.
     */
    int m_current_rainbow_driver_hue;

    /**
     * Whether to enable marquee mode.
     */
    bool m_marquee_enable;

    /**
     * The marquee step counter.
     */
    uint32_t m_marquee_step;

    /**
     * The current flash state.
     */
    bool m_flash_state;

    /**
     * The time, in uptime milliseconds, at which the flash state should be
     * toggled.
     */
    unsigned long m_flash_timer;

    /**
     * Whether we're in edit mode at the moment. If not, we're in rendered mode
     * instead.
     */
    bool m_editing;

    /**
     * Whether we're in overtype mode at the moment. If not, we're in insert
     * mode instead.
     */
    bool m_overtype;

    /**
     * The index of the edit caret. In insert mode, the caret is before the
     * character with this index. In overtype mode, the caret os over the
     * character with this index.
     */
    long m_edit_caret_pos;

    /**
     * Whether the edit caret is momentarily visible.
     */
    bool m_edit_caret_visible;

    /**
     * The time, in uptime milliseconds, at which the edit caret should be
     * toggled.
     */
    unsigned long m_edit_caret_blink_timer;

    /**
     * Whether the text buffer is currently saved.
     */
    bool m_saved;

    /**
     * The index of the current saved string in the string store.
     */
    uint32_t m_save_index;

public:
    mode_text(input_ctrl& p_input, display_pair& p_displays);

    inline const char* text() const
    { return m_text; }

    inline size_t text_len() const
    { return m_text_len; }

    void update() override;

    /**
     * Insert the given character at the given index in the text buffer.
     */
    void insert_character(size_t index, char ch);

    /**
     * Delete the character at the given index in the text buffer.
     */
    void delete_character(size_t index);

    /**
     * Load or reload the string identified by the current save index.
     */
    void load_string();

    /**
     * Save the current text buffer into the string store at the current save
     * index.
     */
    void save_string();

    /**
     * Show the given text string directly.
     */
    void show_string(const char* str, size_t len);
};

} // namespace neo

#endif // #ifndef MODE_TEXT_H
