/**
 * The Forgotten Server - a free and open-source MMORPG server emulator
 * Copyright (C) 2019-2021  Saiyans King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_STRING_H_C5003BEA4242FA1BF0EB01601538A14ABE32BBEF0EF9666214578864
#define FS_STRING_H_C5003BEA4242FA1BF0EB01601538A14ABE32BBEF0EF9666214578864

#include <string>
#include <cstring>

constexpr char ext_digits[] =
"0001020304050607080910111213141516171819"
"2021222324252627282930313233343536373839"
"4041424344454647484950515253545556575859"
"6061626364656667686970717273747576777879"
"8081828384858687888990919293949596979899";

namespace std
{
    class stringExtended
    {
    public:
        stringExtended() = default;
        stringExtended(const size_t reserveSize) { outStr.reserve(reserveSize); }

        string::iterator begin() noexcept {
            return outStr.begin();
        }

        string::iterator end() noexcept {
            return outStr.end();
        }

        string::const_iterator begin() const noexcept {
            return outStr.begin();
        }

        string::const_iterator end() const noexcept {
            return outStr.end();
        }

        size_t length() const noexcept {
            return outStr.length();
        }

        void clear() noexcept {
            outStr.clear();
        }

        void reserve(const size_t count) noexcept {
            outStr.reserve(count);
        }

        void insert(const string::iterator it, const string::value_type element) {
            outStr.insert(it, element);
        }

        void push_back(const string::value_type element) {
            outStr.push_back(element);
        }

        string substr(const size_t pos, const size_t len = std::string::npos) const
        {
            return outStr.substr(pos, len);
        }

        stringExtended& append(const string& str) {
            outStr.append(str);
            return *this;
        }

        stringExtended& append(const string& str, const size_t subpos, const size_t sublen) {
            outStr.append(str, subpos, sublen);
            return *this;
        }

        stringExtended& append(const char* str) {
            outStr.append(str);
            return *this;
        }

        stringExtended& append(const char* str, const size_t n) {
            outStr.append(str, n);
            return *this;
        }

        stringExtended& append(const size_t n, const char c) {
            outStr.append(n, c);
            return *this;
        }

        string::reference operator[](const size_t index) {
            return outStr.operator[](index);
        }

        string::const_reference operator[](const size_t index) const {
            return outStr.operator[](index);
        }

        stringExtended& appendInt(uint64_t value) {
            char str_buffer[22]; // Should be able to contain uint64_t max value + sign
            char* ptrBufferEnd = str_buffer + 21;
            char* ptrBuffer = ptrBufferEnd;

            while (value >= 100) {
                const auto index = static_cast<uint32_t>(value % 100 * 2);
                value /= 100;

                ptrBuffer -= 2;
                memcpy(ptrBuffer, ext_digits + index, 2);
            }
            if (value < 10) {
                *--ptrBuffer = static_cast<char>('0' + value);
                return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
            }

            const auto index = static_cast<uint32_t>(value * 2);
            ptrBuffer -= 2;
            memcpy(ptrBuffer, ext_digits + index, 2);
            return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
        }

        stringExtended& appendInt(const int64_t value) {
            auto abs_value = static_cast<uint64_t>(value);
            const bool neg = value < 0;
            if (neg) {
                abs_value = 0 - abs_value;
            }

            char str_buffer[22]; // Should be able to contain uint64_t max value + sign
            char* ptrBufferEnd = str_buffer + 21;
            char* ptrBuffer = ptrBufferEnd;

            while (abs_value >= 100) {
                const auto index = static_cast<uint32_t>(abs_value % 100 * 2);
                abs_value /= 100;

                ptrBuffer -= 2;
                memcpy(ptrBuffer, ext_digits + index, 2);
            }
            if (abs_value < 10) {
                *--ptrBuffer = static_cast<char>('0' + abs_value);
                if (neg) {
                    *--ptrBuffer = '-';
                }
                return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
            }

            const auto index = static_cast<uint32_t>(abs_value * 2);
            ptrBuffer -= 2;
            memcpy(ptrBuffer, ext_digits + index, 2);
            if (neg) {
                *--ptrBuffer = '-';
            }
            return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
        }

        stringExtended& appendInt(const uint32_t value) { return appendInt(static_cast<uint64_t>(value)); }
        stringExtended& appendInt(const uint16_t value) { return appendInt(static_cast<uint64_t>(value)); }
        stringExtended& appendInt(const uint8_t value) { return appendInt(static_cast<uint64_t>(value)); }
        stringExtended& appendInt(const int32_t value) { return appendInt(static_cast<int64_t>(value)); }
        stringExtended& appendInt(const int16_t value) { return appendInt(static_cast<int64_t>(value)); }
        stringExtended& appendInt(const int8_t value) { return appendInt(static_cast<int64_t>(value)); }

        stringExtended& appendIntShowPos(uint64_t value) {
            char str_buffer[22]; // Should be able to contain uint64_t max value + sign
            char* ptrBufferEnd = str_buffer + 21;
            char* ptrBuffer = ptrBufferEnd;

            while (value >= 100) {
                const auto index = static_cast<uint32_t>(value % 100 * 2);
                value /= 100;

                ptrBuffer -= 2;
                memcpy(ptrBuffer, ext_digits + index, 2);
            }
            if (value < 10) {
                *--ptrBuffer = static_cast<char>('0' + value);
                *--ptrBuffer = '+';
                return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
            }

            const auto index = static_cast<uint32_t>(value * 2);
            ptrBuffer -= 2;
            memcpy(ptrBuffer, ext_digits + index, 2);
            *--ptrBuffer = '+';
            return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
        }

        stringExtended& appendIntShowPos(const int64_t value) {
            auto abs_value = static_cast<uint64_t>(value);
            const bool neg = value < 0;
            if (neg) {
                abs_value = 0 - abs_value;
            }

            char str_buffer[22]; // Should be able to contain uint64_t max value + sign
            char* ptrBufferEnd = str_buffer + 21;
            char* ptrBuffer = ptrBufferEnd;

            while (abs_value >= 100) {
                const auto index = static_cast<uint32_t>(abs_value % 100 * 2);
                abs_value /= 100;

                ptrBuffer -= 2;
                memcpy(ptrBuffer, ext_digits + index, 2);
            }
            if (abs_value < 10) {
                *--ptrBuffer = static_cast<char>('0' + abs_value);
                if (neg) {
                    *--ptrBuffer = '-';
                } else {
                    *--ptrBuffer = '+';
                }
                return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
            }

            const auto index = static_cast<uint32_t>(abs_value * 2);
            ptrBuffer -= 2;
            memcpy(ptrBuffer, ext_digits + index, 2);
            if (neg) {
                *--ptrBuffer = '-';
            } else {
                *--ptrBuffer = '+';
            }
            return append(ptrBuffer, std::distance(ptrBuffer, ptrBufferEnd));
        }

        stringExtended& appendIntShowPos(const uint32_t value) { return appendIntShowPos(static_cast<uint64_t>(value)); }
        stringExtended& appendIntShowPos(const uint16_t value) { return appendIntShowPos(static_cast<uint64_t>(value)); }
        stringExtended& appendIntShowPos(const uint8_t value) { return appendIntShowPos(static_cast<uint64_t>(value)); }
        stringExtended& appendIntShowPos(const int32_t value) { return appendIntShowPos(static_cast<int64_t>(value)); }
        stringExtended& appendIntShowPos(const int16_t value) { return appendIntShowPos(static_cast<int64_t>(value)); }
        stringExtended& appendIntShowPos(const int8_t value) { return appendIntShowPos(static_cast<int64_t>(value)); }

        stringExtended& operator<< (const string& str) { return append(str); }
        stringExtended& operator<< (const char* str) { return append(str); }
        stringExtended& operator<< (const char c) { return append(1, c); }

        stringExtended& operator<< (const uint64_t value) { return appendInt(value); }
        stringExtended& operator<< (const uint32_t value) { return appendInt(value); }
        stringExtended& operator<< (const uint16_t value) { return appendInt(value); }
        stringExtended& operator<< (const uint8_t value) { return appendInt(value); }
        stringExtended& operator<< (const int64_t value) { return appendInt(value); }
        stringExtended& operator<< (const int32_t value) { return appendInt(value); }
        stringExtended& operator<< (const int16_t value) { return appendInt(value); }
        stringExtended& operator<< (const int8_t value) { return appendInt(value); }

        stringExtended& operator<<= (const uint64_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const uint32_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const uint16_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const uint8_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const int64_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const int32_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const int16_t value) { return appendIntShowPos(value); }
        stringExtended& operator<<= (const int8_t value) { return appendIntShowPos(value); }

        //Allow implicit conversion to std::string&
        operator string& () { return outStr; }

    private:
        string outStr;
    };
};

#endif
