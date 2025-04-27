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
#include "http_datetime_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// how many days in given month
static const int month_days[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
// week day names
static const char *weekday_names[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
//  month names
static const char *month_names[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

/* 
 * Checks if a given year is a leap year.
 * Returns true if leap year, false otherwise.
 */
static bool is_leap_year(int year) {
    if (year % 4 != 0) return false;
    else if (year % 100 != 0) return true;
    else if (year % 400 != 0) return false;
    else return true;
}

/* 
 * Returns the number of days in a given month for a given year.
 * (February adjusts to 28 or 29 days depending on leap year status.)
 */
static int days_in_month(int month, int year) {
    if (month == 2) {
        return is_leap_year(year) ? 29 : 28;
    }
    return month_days[month - 1];
}

/* 
 * Parses a three-letter month abbreviation (e.g., "Oct") into its month number (1-12).
 * Defaults to January (1) if parsing fails.
 */
static int parse_month(const char *str) {
    for (int i = 0; i < 12; ++i) {
        if (strcmp(str, month_names[i]) == 0)
            return i + 1;
    }
    return 1; // Default January if parse fails
}

/* 
 * Parses a three-letter weekday abbreviation (e.g., "Wed") into its day index (0=Sun, 1=Mon, etc.).
 * Defaults to Sunday (0) if parsing fails.
 */
static int parse_weekday(const char *str) {
    for (int i = 0; i < 7; ++i) {
        if (strcmp(str, weekday_names[i]) == 0)
            return i;
    }
    return 0; // Default Sunday
}
/**
 * @brief Generates an arcdate_t object from a HTTP Date string or system time.
 *
 * @param httpDate A HTTP Date string (e.g., "Wed, 21 Oct 2015 07:28:00 GMT"), or NULL for current system UTC time.
 * @param gmt_offset Desired GMT offset to apply (e.g., 0, +3, -5).
 * @return Pointer to a dynamically allocated arcdate_t structure. Must be freed using free_date().
 */
arcdate_t* generate_date(const char *httpDate, int gmt_offset) {
    arcdate_t *date = (arcdate_t*)malloc(sizeof(arcdate_t));
    if (!date) return NULL;

    if (httpDate == NULL) {
        time_t now = time(NULL);
        struct tm *tm_utc = gmtime(&now);

        date->year = tm_utc->tm_year + 1900;
        date->month = tm_utc->tm_mon + 1;
        date->day = tm_utc->tm_mday;
        date->hour = tm_utc->tm_hour;
        date->minute = tm_utc->tm_min;
        date->second = tm_utc->tm_sec;
        date->weekday = tm_utc->tm_wday;
    } else {
        // Parse format: "Wed, 21 Oct 2015 07:28:00 GMT"
        char weekday[4], month[4];
        int day, year, hour, minute, second;
        sscanf(httpDate, "%3s, %d %3s %d %d:%d:%d", 
               weekday, &day, month, &year, &hour, &minute, &second);

        date->year = year;
        date->month = parse_month(month);
        date->day = day;
        date->hour = hour;
        date->minute = minute;
        date->second = second;
        date->weekday = parse_weekday(weekday);
    }
    
    date->gmt_offset = gmt_offset;
    add_hours(date, gmt_offset); // Adjust to desired GMT immediately
    return date;
}
/**
 * @brief Converts an arcdate_t structure to a formatted HTTP Date string.
 *
 * @param date Pointer to an arcdate_t structure.
 * @return Dynamically allocated string containing the formatted date. Must be freed by the caller.
 */
char* to_date_string(const arcdate_t *date) {
    char *buffer = (char*)malloc(100);
    if (!buffer) return NULL;

    snprintf(buffer, 100, "%s, %02d %s %04d %02d:%02d:%02d GMT%+d",
             weekday_names[date->weekday],
             date->day,
             month_names[date->month - 1],
             date->year,
             date->hour,
             date->minute,
             date->second,
             date->gmt_offset);
    return buffer;
}
/**
 * @brief Free an arcdate_t
 *
 * @param date Pointer to an arcdate_t structure to be free
 * 
 */
void free_date(arcdate_t *date) {
    if (date) {
        free(date);
    }
}

/**
 * @brief Converts an arcdate_t from its current GMT offset to a new GMT offset.
 *
 * @param date Pointer to an arcdate_t structure to be updated.
 * @param new_gmt_offset The target GMT offset (e.g., +3, -5).
 */
void convert(arcdate_t *date, int new_gmt_offset) {
    int diff = new_gmt_offset - date->gmt_offset;
    add_hours(date, diff);
    date->gmt_offset = new_gmt_offset;
}

/**
 * @brief Adds or subtracts minutes from an arcdate_t.
 *
 * @param date Pointer to the arcdate_t structure to modify.
 * @param minutes Number of minutes to add (positive) or subtract (negative).
 */
void add_minutes(arcdate_t *date, int minutes) {
    int total_minutes = date->minute + minutes;
    int hours_change = total_minutes / 60;
    if (total_minutes < 0) hours_change--;

    date->minute = (total_minutes % 60 + 60) % 60;
    add_hours(date, hours_change);
}
/**
 * @brief Adds or subtracts hours from an arcdate_t.
 *
 * @param date Pointer to the arcdate_t structure to modify.
 * @param hours Number of hours to add (positive) or subtract (negative).
 */
void add_hours(arcdate_t *date, int hours) {
    int total_hours = date->hour + hours;
    int days_change = total_hours / 24;
    if (total_hours < 0) days_change--;

    date->hour = (total_hours % 24 + 24) % 24;
    add_days(date, days_change);
}

/**
 * @brief Adds or subtracts days from an arcdate_t.
 *
 * @param date Pointer to the arcdate_t structure to modify.
 * @param days Number of days to add (positive) or subtract (negative).
 */
void add_days(arcdate_t *date, int days) {
    date->day += days;

    while (date->day > days_in_month(date->month, date->year)) {
        date->day -= days_in_month(date->month, date->year);
        date->month++;
        if (date->month > 12) {
            date->month = 1;
            date->year++;
        }
        date->weekday = (date->weekday + 1) % 7;
    }
    
    while (date->day < 1) {
        date->month--;
        if (date->month < 1) {
            date->month = 12;
            date->year--;
        }
        date->day += days_in_month(date->month, date->year);
        date->weekday = (date->weekday + 6) % 7; // -1 mod 7
    }
}
/**
 * @brief Adds or subtracts months from an arcdate_t.
 *
 * @param date Pointer to the arcdate_t structure to modify.
 * @param months Number of months to add (positive) or subtract (negative).
 */
void add_months(arcdate_t *date, int months) {
    int total_months = date->month + months - 1;
    int years_change = total_months / 12;
    if (total_months < 0) years_change--;

    date->month = (total_months % 12 + 12) % 12 + 1;
    add_years(date, years_change);

    int dim = days_in_month(date->month, date->year);
    if (date->day > dim) date->day = dim;
}

/**
 * @brief Adds or subtracts years from an arcdate_t.
 *
 * @param date Pointer to the arcdate_t structure to modify.
 * @param years Number of years to add (positive) or subtract (negative).
 */
void add_years(arcdate_t *date, int years) {
    date->year += years;
    if (date->month == 2 && date->day == 29 && !is_leap_year(date->year)) {
        date->day = 28;
    }
}
