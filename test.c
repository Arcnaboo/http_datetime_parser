include <stdio.h>
#include "http_datetime_parser.h"
/*
aakgur@instance-20250423-141426:~/datetime$ ./test_datetime 
Testing HTTP Datetime Parser Library...
Parsed and shifted date: Wed, 21 Oct 2015 10:28:00 GMT+3
Converted to GMT+5: Wed, 21 Oct 2015 12:28:00 GMT+5
After adding 2 days: Wed, 23 Oct 2015 12:28:00 GMT+5
After subtracting 90 minutes: Wed, 23 Oct 2015 10:58:00 GMT+5
Current UTC time: Sun, 27 Apr 2025 18:16:11 GMT+0
All tests completed.
*/
int main() {
    printf("Testing HTTP Datetime Parser Library...\n");

    // Test 1: Parse a given HTTP Date string
    const char *http_date = "Wed, 21 Oct 2015 07:28:00 GMT";
    arcdate_t *date = generate_date(http_date, 3); // parse and shift to GMT+3

    char *str = to_date_string(date);
    printf("Parsed and shifted date: %s\n", str);
    free(str);

    // Test 2: Convert to GMT+5
    convert(date, 5);
    str = to_date_string(date);
    printf("Converted to GMT+5: %s\n", str);
    free(str);

    // Test 3: Add 2 days
    add_days(date, 2);
    str = to_date_string(date);
    printf("After adding 2 days: %s\n", str);
    free(str);

    // Test 4: Subtract 90 minutes
    add_minutes(date, -90);
    str = to_date_string(date);
    printf("After subtracting 90 minutes: %s\n", str);
    free(str);

    free_date(date);

    // Test 5: Generate from current system time
    arcdate_t *now = generate_date(NULL, 0); // System time UTC
    str = to_date_string(now);
    printf("Current UTC time: %s\n", str);
    free(str);

    free_date(now);

    printf("All tests completed.\n");
    return 0;
}