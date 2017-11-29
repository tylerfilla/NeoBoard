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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "display_pair.h"
#include "display_panel.h"
#include "font_pixely.h"
#include "input_ctrl.h"
#include "mode_text.h"
#include "string_store.h"

#define RENDERED_EFFECT_FLASH_DELAY_ON 500
#define RENDERED_EFFECT_FLASH_DELAY_OFF 250
#define EDIT_CARET_BLINK_DELAY 250
#define EDIT_CARET_DUCK_DELAY 750

neo::mode_text::mode_text(input_ctrl& input_p, display_pair& displays_p)
        : mode(input_p, displays_p),
          text_m {""},
          text_len_m(0),
          current_obfuscated_char_m(' '),
          current_rainbow_color_a_m(DEFAULT_COLOR_FG),
          current_rainbow_driver_hue_m(0),
          marquee_enable_m(false),
          marquee_step_m(0),
          flash_state_m(false),
          flash_timer_m(0),
          editing_m(false),
          overtype_m(true),
          edit_caret_pos_m(0),
          edit_caret_visible_m(true),
          edit_caret_blink_timer_m(0),
          saved_m(true),
          save_index_m(0)
{
    // Find first nonempty string
    while (true)
    {
        load_string();

        // Try next string
        if (text_len_m == 0)
        {
            save_index_m++;

            // If the end has been reached
            if (save_index_m == neo::string_store::NUM_STRINGS - 1)
            {
                // Go to the first string
                save_index_m = 0;

                // Drop to edit mode, overtype
                editing_m = true;
                overtype_m = true;

                // Stop here
                break;
            }
            else
            {
                // Go to next string
                continue;
            }
        }
        else
        {
            // Stop here
            break;
        }
    }
}

void neo::mode_text::insert_character(size_t index, char ch)
{
    // Don't exceed buffer size
    if (text_len_m > TEXT_BUFFER_SIZE)
        return;

    // If appending to existing text, simply tack it on
    // Appending requires no movement
    if (index == text_len_m)
    {
        text_m[index] = ch;
    }
    else
    {
        // Move characters after and including insertion point one character right
        // This exposes a garbage character at the insertion point
        memmove(text_m + index + 1, text_m + index, text_len_m - index);

        // Fill in the insertion point with the desired character
        text_m[index] = ch;
    }

    // Increment text length
    text_len_m++;

    // Mark buffer unsaved
    saved_m = false;
}

void neo::mode_text::delete_character(size_t index)
{
    // Require at least one character in buffer
    if (text_len_m == 0)
        return;
      
    // If not deleting the last character
    // Deleting the last character requires no movement
    if (index < text_len_m - 1)
    {
        // Move characters after deletion point one character left
        // This covers up the character to be deleted
        memmove(text_m + index, text_m + index + 1, text_len_m - index);
    }

    // Decrement text length
    text_len_m--;

    // Mark buffer unsaved
    saved_m = false;
}

void neo::mode_text::load_string()
{
    // Copy target string from string store into text buffer
    // This overwrites all text currently in the buffer, even unsaved changes!
    neo::string_store::get(save_index_m, TEXT_BUFFER_SIZE, text_m);

    // Find the length of the string
    text_len_m = strlen(text_m);

    // Flag as saved
    saved_m = true;

    // HACK: Disable marquee animation
    marquee_enable_m = false;
}

void neo::mode_text::save_string()
{
    // Quick sanity check
    if (text_len_m > TEXT_BUFFER_SIZE)
        return;

    // Copy string from text buffer into string store
    neo::string_store::put(text_m, text_len_m, save_index_m);

    // Flag as saved
    saved_m = true;
}

static neo::mode_text::color_t compute_hue_color(int hue)
{
    // Augmented hues for different centers of color
    int hue_aug_r = hue + 120;
    int hue_aug_g = hue;
    int hue_aug_b = hue - 120;

    // Wrap augmented hues within limits for degree angles
    // This can possibly be improved by checking for validity first
    // Would a comparison on each of these be faster or slower?
    hue_aug_r = (hue_aug_r + 360) % 360;
    hue_aug_g = (hue_aug_g + 360) % 360;
    hue_aug_b = (hue_aug_b + 360) % 360;

    // Unassembled RGB components
    neo::mode_text::color_t comp_red = 0;
    neo::mode_text::color_t comp_green = 0;
    neo::mode_text::color_t comp_blue = 0;

#define COMPUTE_COMPONENT(hue_aug, comp)                            \
    if (hue_aug < 60)                                               \
    {                                                               \
        comp = (int) (255.0f * 6.0f * (float) hue_aug / 360.0f);    \
    }                                                               \
    else if (hue_aug < 180)                                         \
    {                                                               \
        comp = 255;                                                 \
    }                                                               \
    else if (hue_aug < 240)                                         \
    {                                                               \
        comp = (int) (255.0f * (4.0f - (float) hue_aug / 60.0f));   \
    }

    // Compute components from augmented hues
    COMPUTE_COMPONENT(hue_aug_r, comp_red)
    COMPUTE_COMPONENT(hue_aug_g, comp_green)
    COMPUTE_COMPONENT(hue_aug_b, comp_blue)

#undef COMPUTE_COMPONENT

    // String together the components into the final color value
    return (comp_red << 16) | (comp_green << 8) | comp_blue;
}

void neo::mode_text::update()
{
    //
    // Handle User Input
    //

    // Handle edit mode-specific input
    if (editing_m)
    {
        //
        // Edit Mode Input
        //

        // Handle save and exit
        if (input_m.btn_select() && input_m.btn_up_changed()
            && input_m.btn_up())
        {
            // Save the text buffer if unsaved
            if (!saved_m)
            {
                save_string();
            }

            // Leave edit mode
            editing_m = false;

            // Skip this update
            return;
        }

        // Handle toggle overtype
        if (input_m.btn_select() && input_m.btn_down_changed()
            && input_m.btn_down())
        {
            // Toggle overtype
            overtype_m = !overtype_m;
        }

        // Handle backspace
        if (input_m.btn_select() && input_m.btn_left_changed()
            && input_m.btn_left())
        {
            // If there is a character behind the caret
            if (edit_caret_pos_m > 0)
            {
                // Delete the character
                delete_character(edit_caret_pos_m - 1);

                // Shift the caret to the left
                edit_caret_pos_m--;
            }
        }

        // Handle dollar sign shortcut
        // This is useful for entering formatting commands
        if (input_m.btn_select() && input_m.btn_right_changed()
            && input_m.btn_right())
        {
            // Insert a dollar sign
            insert_character(edit_caret_pos_m, '$');
        }

        // Handle caret movement
        if (input_m.btn_left_changed() && input_m.btn_left())
        {
            if (--edit_caret_pos_m < 0)
            {
                edit_caret_pos_m = 0;
            }

            // Show the caret immediately
            // This gives a solid text editor feel
            edit_caret_visible_m = true;
            edit_caret_blink_timer_m = neo::clock::uptime_millis()
                + EDIT_CARET_BLINK_DELAY;
        }
        else if (input_m.btn_right_changed() && input_m.btn_right())
        {
            // Allow caret to go one past the end
            if (++edit_caret_pos_m >= text_len_m + 1)
            {
                edit_caret_pos_m = text_len_m;
            }

            // Do not allow caret to exceed the text buffer size
            if (edit_caret_pos_m >= TEXT_BUFFER_SIZE)
            {
                edit_caret_pos_m = TEXT_BUFFER_SIZE - 1;
            }

            // Show the caret immediately
            // This gives a solid text editor feel
            edit_caret_visible_m = true;
            edit_caret_blink_timer_m = neo::clock::uptime_millis()
                + EDIT_CARET_BLINK_DELAY;
        }

        // Handle character editing
        if (input_m.btn_up_changed() && input_m.btn_up())
        {
            // If in overtype mode, edit character under caret
            if (overtype_m)
            {
                // If caret is positioned to append a character
                if (edit_caret_pos_m == text_len_m)
                {
                    // Shortcut: Add in a new character here
                    insert_character(edit_caret_pos_m, 'Z');
                }
                else
                {
                    // Decrement existing character (in Z to A direction)
                    text_m[edit_caret_pos_m]--;
                    if (!isprint(text_m[edit_caret_pos_m]))
                    {
                        // Lowest important printable ASCII character
                        text_m[edit_caret_pos_m] = ' ';
                    }
                }

                // Flag buffer as unsaved
                saved_m = false;

                // Temporarily hide the caret for an extended time
                edit_caret_visible_m = false;
                edit_caret_blink_timer_m = neo::clock::uptime_millis()
                    + EDIT_CARET_DUCK_DELAY;
            }
            else
            {
                // Currently in insert mode
                // Insert a suitable starting character
                insert_character(edit_caret_pos_m, 'Z');

                // Drop back to overtype
                overtype_m = true;
            }
        }
        else if (input_m.btn_down_changed() && input_m.btn_down())
        {
            // If in overtype mode, edit character under caret
            if (overtype_m)
            {
                // If caret is positioned to append a character
                if (edit_caret_pos_m == text_len_m)
                {
                    // Shortcut: Add in a new character here
                    insert_character(edit_caret_pos_m, 'A');
                }
                else
                {
                    // Increment existing character (in A to Z direction)
                    text_m[edit_caret_pos_m]++;
                    if (!isprint(text_m[edit_caret_pos_m]))
                    {
                        // Highest important printable ASCII character
                        text_m[edit_caret_pos_m] = '~';
                    }
                }

                // Flag buffer as unsaved
                saved_m = false;

                // Temporarily hide the caret for an extended time
                edit_caret_visible_m = false;
                edit_caret_blink_timer_m = neo::clock::uptime_millis()
                    + EDIT_CARET_DUCK_DELAY;
            }
            else
            {
                // Currently in insert mode
                // Insert a suitable starting character
                insert_character(edit_caret_pos_m, 'A');

                // Drop back to overtype
                overtype_m = true;
            }
        }
    }
    else
    {
        //
        // Rendered Mode Input
        //

        // Handle switch to edit mode in insert mode
        if (input_m.btn_select() && input_m.btn_up_changed()
            && input_m.btn_up())
        {
            // Enter edit mode
            editing_m = true;
            overtype_m = true;

            return;
        }

        // Handle erase the current string
        if (input_m.btn_select() && input_m.btn_down_changed()
            && input_m.btn_down())
        {
            // If the current string is empty and saved, do nothing
            if (text_len_m == 0 && saved_m)
                return;

            // Fake an unsaved erasure
            memset(text_m, 0, TEXT_BUFFER_SIZE);
            text_len_m = 0;
            saved_m = false;

            // And then save it
            save_string();

            return;
        }

        // Handle switch to string left
        if (input_m.btn_left_changed() && input_m.btn_left())
        {
            // If another string exists to the left
            if (save_index_m > 0)
            {
                // Decrement (move left) the save index
                save_index_m--;

                // Load the new string
                load_string();

                return;
            }
        }

        // Handle switch to string right
        if (input_m.btn_right_changed() && input_m.btn_right())
        {
            // If another string exists to the right
            if (save_index_m < neo::string_store::NUM_STRINGS - 1)
            {
                // Increment (move right) the save index
                save_index_m++;

                // Load the new string
                load_string();

                return;
            }
        }
    }

    //
    // Handle Editor State
    //

    // Handle edit caret blink
    if (neo::clock::uptime_millis() >= edit_caret_blink_timer_m)
    {
        edit_caret_visible_m = !edit_caret_visible_m;
        edit_caret_blink_timer_m = neo::clock::uptime_millis()
            + EDIT_CARET_BLINK_DELAY;
    }

    //
    // Handle Rainbow Color
    //

    // Increment rainbow driver hue
    current_rainbow_driver_hue_m += 3;
    current_rainbow_driver_hue_m %= 360;

    // Find hues for both rainbow channels
    int rainbow_hue_a = current_rainbow_driver_hue_m;
    int rainbow_hue_b = (current_rainbow_driver_hue_m + 180) % 360;

    // Compute RGB colors for fully-saturated, half-lit variants of these hues
    // FIXME: Only need to compute what's necessary for each frame
    current_rainbow_color_a_m = compute_hue_color(rainbow_hue_a);
    current_rainbow_color_b_m = compute_hue_color(rainbow_hue_b);

    //
    // Handle Obfuscated Text
    //

    // Change obfuscated character
    current_obfuscated_char_m = 0;
    while (!isprint(current_obfuscated_char_m))
    {
        current_obfuscated_char_m = rand() % 128;
    }

    //
    // Handle Marquee
    //

    // Step the marquee forward, if enabled
    if (marquee_enable_m)
    {
        marquee_step_m += 10;
    }

    //
    // Handle Flashing
    //

    // Handle flash state toggle
    if (neo::clock::uptime_millis() >= flash_timer_m)
    {
        flash_state_m = !flash_state_m;

        // Apply delay based on new state
        if (flash_state_m)
        {
            flash_timer_m = neo::clock::uptime_millis()
                + RENDERED_EFFECT_FLASH_DELAY_ON;
        }
        else
        {
            flash_timer_m = neo::clock::uptime_millis()
                + RENDERED_EFFECT_FLASH_DELAY_OFF;
        }
    }

    //
    // Draw the Frame
    //

    // Clear the drawing surface
    displays_m.surface_clear();

    // Alias the one and only font
    namespace font = neo::font::pixely;
    
    // Current text background and foreground colors
    color_t color_bg = DEFAULT_COLOR_BG;
    color_t color_fg = DEFAULT_COLOR_FG;

    // Indicates whether a formatting sequence is currently being processed
    bool format_escape = false;

    // The cumulative offset to apply to subsequent text characters
    int text_run_offset = 0;

    // Scoped render effect modes
    bool mode_obfuscated = false;
    bool mode_flash = false;

    // The text to display this frame
    // Also get its corresponding length
    const char* text = text_m;
    size_t text_len = text_len_m;

    // When editing, allow edit caret to go one past the end
    if (editing_m)
    {
        text_len++;
    }

    for (size_t char_idx = 0; char_idx < text_len; ++char_idx)
    {
        char ch;

        // If editing and we're one past the end
        if (editing_m && char_idx == text_len - 1)
        {
            // Display a space (i.e. nothing)
            ch = ' ';
        }
        else
        {
            // Get the real character
            ch = text[char_idx];
        }

        // If in rendered mode, handle formatting escapes
        // The text color then indicates the save state
        if (!editing_m)
        {
            // Match unescaped format escape character
            if (ch == '$' && !format_escape)
            {
                // Set format escape flag and move on to next character
                format_escape = true;
                continue;
            }

            if (format_escape)
            {
                // This character contributes to a formatting sequence
                switch (ch)
                {
                case '0':
                    // Set foreground color to black (clear on typical LED strips)
                    color_fg = 0x00000000;
                    goto escape_done;
                case '1':
                    // Set foreground color to dark blue
                    color_fg = 0x000000aa;
                    goto escape_done;
                case '2':
                    // Set foreground color to dark green
                    color_fg = 0x0000aa00;
                    goto escape_done;
                case '3':
                    // Set foreground color to dark aqua
                    color_fg = 0x0000aaaa;
                    goto escape_done;
                case '4':
                    // Set foreground color to dark red
                    color_fg = 0x00aa0000;
                    goto escape_done;
                case '5':
                    // Set foreground color to dark purple
                    color_fg = 0x00aa00aa;
                    goto escape_done;
                case '6':
                    // Set foreground color to gold
                    color_fg = 0x00ffaa00;
                    goto escape_done;
                case '7':
                    // Set foreground color to gray (or less bright white)
                    color_fg = 0x00aaaaaa;
                    goto escape_done;
                case '8':
                    // Set foreground color to dark gray (even less bright white)
                    color_fg = 0x00555555;
                    goto escape_done;
                case '9':
                    // Set foreground color to blue
                    color_fg = 0x000000ff;
                    goto escape_done;
                case 'a':
                case 'A':
                    // Set foreground color to green
                    color_fg = 0x0000ff00;
                    goto escape_done;
                case 'b':
                case 'B':
                    // Set foreground color to aqua
                    color_fg = 0x0000ffff;
                    goto escape_done;
                case 'c':
                case 'C':
                    // Set foreground color to red
                    color_fg = 0x00ff0000;
                    goto escape_done;
                case 'd':
                case 'D':
                    // Set foreground color to light purple
                    color_fg = 0x00ff00ff;
                    goto escape_done;
                case 'e':
                case 'E':
                    // Set foreground color to yellow
                    color_fg = 0x00ffff00;
                    goto escape_done;
                case 'f':
                case 'F':
                    // Set foreground color to white
                    color_fg = 0x00ffffff;
                    goto escape_done;
                case 'm':
                case 'M':
                    // Enable marquee mode
                    // This is a global flag
                    marquee_enable_m = true;
                    goto escape_done;
                case 'o':
                case 'O':
                    // Enable scoped obfuscated (glitter) mode
                    mode_obfuscated = true;
                    goto escape_done;
                case 'r':
                case 'R':
                    // Reset a bunch of stuff
                    mode_obfuscated = false;
                    color_bg = DEFAULT_COLOR_BG;
                    color_fg = DEFAULT_COLOR_FG;
                    goto escape_done;
                case 's':
                case 'S':
                    // Rainbow mode A
                    // Set foreground color to rainbow A color
                    color_fg = current_rainbow_color_a_m;
                    goto escape_done;
                case 't':
                case 'T':
                    // Rainbow mode B
                    // Set foreground color to rainbow B color
                    color_fg = current_rainbow_color_b_m;
                    goto escape_done;
                case 'u':
                case 'U':
                    // Enable scoped flashing mode
                    mode_flash = true;
                    goto escape_done;
                default:
                    goto escape_done;
                }

            escape_done:
                format_escape = false;
                continue;
            }

            // Handle obfuscated mode
            if (mode_obfuscated)
            {
                ch = current_obfuscated_char_m;
            }
        }
        else
        {
            // We are in edit mode at the moment
            // Change text color based on save state
            if (saved_m)
            {
                // Green for saved
                color_fg = 0x0000ff00;
            }
            else
            {
                // Red for unsaved
                color_fg = 0x00ff0000;
            }
        }

        // Get character bitmap data and dimensions
        // Use the character value itself as an index into the font buffer
        // This is incredibly primitive, but it satisfies requirements
        auto char_bitmap = font::data[static_cast<size_t>(ch)];
        auto char_height = font::height;
        auto char_width = font::width;

        // The total number of visible characters
        // This is used to calculate horizontal scroll offset for editing
        // We assume one blank to demonstrate a past-the-end overtype caret
        auto total_visible_chars = displays_m.surface_width() / char_width;

        // Offset to apply to text to provide editor scrolling
        int horiz_scroll_offset = 0;

        // If buffer length exceeds total visible string length
        if (text_len_m > total_visible_chars - 1)
        {
            // If edit caret is not on the first "page"
            if (edit_caret_pos_m > total_visible_chars - 1)
            {
                // Apply an offset to put edit caret at last position
                horiz_scroll_offset = char_width * (edit_caret_pos_m
                    - total_visible_chars + 1);
            }
        }

        for (int col = 0; col < char_width; ++col)
        {
            // Get column data
            auto col_data = char_bitmap[col];

            for (int row = 0; row < char_height; ++row)
            {
                // Get pixel coordinates on logical drawing surface
                int x = text_run_offset + col - horiz_scroll_offset;
                int y = font::height - row - 1;

                // Handle marquee animation if not editing
                if (marquee_enable_m && !editing_m)
                {
                    x -= marquee_step_m / 100;

                    // Wrap x into proper range
                    while (x < 0)
                    {
                        x += displays_m.surface_width();
                    }
                    x %= displays_m.surface_width();
                }

                // Determine the color to be applied to this pixel
                // Use foreground color for text and background color everywhere else
                auto color = ((col_data >> row) & 1) ? color_fg : color_bg;

                // If this character is affected by global flash state
                if (mode_flash)
                {
                    // If global flash state says NO!
                    if (!flash_state_m)
                    {
                        // There will be no light
                        color = 0;
                    }
                }

                // Buffer the pixel color
                displays_m.set_pixel(x, y, color);
            }
        }

        // Render edit caret in edit mode
        if (editing_m && char_idx == edit_caret_pos_m)
        {
            for (int row = 0; row < char_height; ++row)
            {
                // Draw appropriate caret style
                if (overtype_m)
                {
                    // Cover entire character
                    for (int col = 0; col < char_width; ++col)
                    {
                        int x = text_run_offset + col - horiz_scroll_offset;
                        int y = row;

                        auto cur = displays_m.get_pixel(x, y);
                        displays_m.set_pixel(x, y,
                            edit_caret_visible_m ? 0x000f0f0f : cur);
                    }
                }
                else
                {
                    // Cover leftmost column of character
                    int col = text_run_offset - horiz_scroll_offset;
                    auto cur = displays_m.get_pixel(col, row);
                    displays_m.set_pixel(col, row,
                        edit_caret_visible_m ? 0x000f0f0f : cur);
                }
            }
        }

        // Account for character dimensions in text run
        // This assumes text runs horizontally
        text_run_offset += char_width;
    }

    // Flush the drawing buffer
    displays_m.surface_flush();
}
