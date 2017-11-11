/*
 * NeoBoardV2 - LED Panel Controller
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

//
// This is a font primarily custom-made, but with some small influences from
// the public domain. It is regular weight, monospaced, and with 4x3 ASCII
// 7-bit printable characters.
//

#include "font_pixely.h"

const unsigned char neo::font::pixely[128][3] = {
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 }, // (space)
    { 0x00, 0x1d, 0x00 }, // !
    { 0x18, 0x00, 0x18 }, // "
    { 0x1f, 0x0a, 0x1f }, // #
    { 0x0a, 0x1f, 0x14 }, // $
    { 0x13, 0x04, 0x19 }, // %
    { 0x0a, 0x15, 0x0b }, // &
    { 0x00, 0x18, 0x00 }, // '
    { 0x00, 0x0e, 0x11 }, // (
    { 0x11, 0x0e, 0x00 }, // )
    { 0x15, 0x0e, 0x15 }, // *
    { 0x04, 0x0e, 0x04 }, // +
    { 0x01, 0x02, 0x00 }, // ,
    { 0x04, 0x04, 0x04 }, // -
    { 0x00, 0x03, 0x00 }, // .
    { 0x01, 0x0e, 0x10 }, // /
    { 0x1f, 0x11, 0x1f }, // 0
    { 0x09, 0x1f, 0x01 }, // 1
    { 0x17, 0x15, 0x1d }, // 2
    { 0x11, 0x15, 0x1f }, // 3
    { 0x1c, 0x04, 0x1f }, // 4
    { 0x1d, 0x15, 0x17 }, // 5
    { 0x1f, 0x15, 0x17 }, // 6
    { 0x10, 0x10, 0x1f }, // 7
    { 0x1f, 0x15, 0x1f }, // 8
    { 0x1d, 0x15, 0x1f }, // 9
    { 0x00, 0x0a, 0x00 }, // :
    { 0x01, 0x0a, 0x00 }, // ;
    { 0x04, 0x0a, 0x11 }, // <
    { 0x0a, 0x0a, 0x0a }, // =
    { 0x11, 0x0a, 0x04 }, // >
    { 0x10, 0x15, 0x18 }, // ?
    { 0x0e, 0x15, 0x0d }, // @
    { 0x0f, 0x14, 0x0f }, // A
    { 0x1f, 0x15, 0x0a }, // B
    { 0x0e, 0x11, 0x11 }, // C
    { 0x1f, 0x11, 0x0e }, // D
    { 0x1f, 0x15, 0x15 }, // E
    { 0x1f, 0x14, 0x14 }, // F
    { 0x0e, 0x15, 0x17 }, // G
    { 0x1f, 0x04, 0x1f }, // H
    { 0x11, 0x1f, 0x11 }, // I
    { 0x02, 0x01, 0x1e }, // J
    { 0x1f, 0x04, 0x1b }, // K
    { 0x1f, 0x01, 0x01 }, // L
    { 0x1f, 0x08, 0x1f }, // M
    { 0x1f, 0x0e, 0x1f }, // N
    { 0x0e, 0x11, 0x0e }, // O
    { 0x1f, 0x14, 0x08 }, // P
    { 0x0e, 0x13, 0x0f }, // Q
    { 0x1f, 0x16, 0x0d }, // R
    { 0x09, 0x15, 0x12 }, // S
    { 0x10, 0x1f, 0x10 }, // T
    { 0x1e, 0x01, 0x1f }, // U
    { 0x1e, 0x01, 0x1e }, // V
    { 0x1f, 0x06, 0x1f }, // W
    { 0x1b, 0x04, 0x1b }, // X
    { 0x18, 0x07, 0x18 }, // Y
    { 0x13, 0x15, 0x19 }, // Z
    { 0x00, 0x1f, 0x11 }, // [
    { 0x18, 0x04, 0x03 }, // \ (backslash)
    { 0x11, 0x1f, 0x00 }, // ]
    { 0x08, 0x10, 0x08 }, // ^
    { 0x01, 0x01, 0x01 }, // _
    { 0x10, 0x08, 0x00 }, // `
    { 0x03, 0x05, 0x07 }, // a
    { 0x1f, 0x05, 0x02 }, // b
    { 0x02, 0x05, 0x05 }, // c
    { 0x02, 0x05, 0x1f }, // d
    { 0x06, 0x0b, 0x0d }, // e
    { 0x04, 0x0f, 0x14 }, // f
    { 0x05, 0x0d, 0x0e }, // g
    { 0x1f, 0x04, 0x03 }, // h
    { 0x00, 0x17, 0x00 }, // i
    { 0x01, 0x01, 0x16 }, // j
    { 0x0f, 0x02, 0x05 }, // k
    { 0x11, 0x1f, 0x01 }, // l
    { 0x07, 0x06, 0x07 }, // m
    { 0x07, 0x04, 0x03 }, // n
    { 0x02, 0x05, 0x02 }, // o
    { 0x07, 0x0a, 0x04 }, // p
    { 0x04, 0x0a, 0x07 }, // q
    { 0x0f, 0x04, 0x04 }, // r
    { 0x05, 0x0f, 0x0a }, // s
    { 0x08, 0x1f, 0x08 }, // t
    { 0x06, 0x01, 0x07 }, // u
    { 0x06, 0x03, 0x06 }, // v
    { 0x07, 0x03, 0x07 }, // w
    { 0x05, 0x02, 0x05 }, // x
    { 0x09, 0x05, 0x0e }, // y
    { 0x0b, 0x0f, 0x0d }, // z
    { 0x04, 0x1b, 0x11 }, // {
    { 0x00, 0x1f, 0x00 }, // |
    { 0x11, 0x1b, 0x04 }, // }
    { 0x08, 0x18, 0x10 }, // ~
    { 0x00, 0x00, 0x00 }, // (delete)
};
