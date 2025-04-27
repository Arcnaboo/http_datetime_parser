/*
 * The MIT License
 *
 * Copyright 2025 Arda 'Arc' Akgur.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * HTTP Datetime Parser Library
 *
 * Based on the original concept and parsing logic of Arda 'Arc' Akgür's 2018 
 * HTTP Date parser implementation:
 * https://github.com/Arcnaboo/coms3200-Ass1-HTTP/blob/master/src/datetime.c
 *
 * This library provides a modern, portable, and lightweight way to parse,
 * manipulate, and format HTTP Date strings in C, fully supporting GMT offset
 * conversions, leap years, and date arithmetic.
 *
 * Features:
 * - Parse standard HTTP Date strings into a structured arcdate_t format.
 * - Generate current system time if no input is provided.
 * - Convert between different GMT timezones.
 * - Add or subtract hours, minutes, days, months, or years.
 * - Fully leap-year aware (including February 29 handling).
 * - C99 compliant with <stdbool.h> usage.
 *
 * License: MIT License
 * Author: Arda 'Arc' Akgür (Original Concept and Core Logic)
 * Version: 1.0 (2025 Edition)
 */

#ifndef HTTP_DATETIME_PARSER_H
#define HTTP_DATETIME_PARSER_H

#include <stdbool.h>

typedef struct {
    int year;       // Year, e.g., 2025
    int month;      // Month (1-12)
    int day;        // Day (1-31)
    int hour;       // Hour (0-23)
    int minute;     // Minute (0-59)
    int second;     // Second (0-59)
    int weekday;    // 0=Sun, 1=Mon, ..., 6=Sat
    int gmt_offset; // GMT offset, e.g., 0, +3, -5
} arcdate_t;

// Main functions
arcdate_t* generate_date(const char *httpDate, int gmt_offset);
char* to_date_string(const arcdate_t *date);
void convert(arcdate_t *date, int new_gmt_offset);
void free_date(arcdate_t *date);

// Date manipulation functions
void add_hours(arcdate_t *date, int hours);
void add_minutes(arcdate_t *date, int minutes);
void add_days(arcdate_t *date, int days);
void add_months(arcdate_t *date, int months);
void add_years(arcdate_t *date, int years);

#endif // HTTP_DATETIME_PARSER_H
