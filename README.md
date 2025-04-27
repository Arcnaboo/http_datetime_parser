# HTTP Datetime Parser

A lightweight C library for parsing, manipulating, and formatting HTTP Date strings.

Built on the original 2018 work of [Arda 'Arc' Akgür](https://github.com/Arcnaboo/coms3200-Ass1-HTTP/blob/master/src/datetime.c),  
completely modernized for 2025: portable, fast, and C99 compliant.

---

## ✨ Features

- 📅 Parse standard HTTP Date headers into structured `arcdate_t` objects.
- 🕒 Convert between different GMT offsets (+3, -5, etc.).
- ➕ Add or subtract days, hours, minutes, months, or years.
- 👨‍🚀 Fully leap-year aware (February 29th support).
- ⚡ Modern C (C99 standard, `<stdbool.h>` based).
- 🛡️ Minimal, dependency-free, easy to integrate into any project.

---

## 📦 Files

| File                  | Description                        |
|------------------------|------------------------------------|
| `http_datetime_parser.h` | Public header for the API         |
| `http_datetime_parser.c` | Core implementation file          |
| `test.c`               | Example usage and simple tests    |

---

## 🚀 Quick Start

### Compile & Run

```bash
gcc -Wall -Wextra -std=c99 test.c http_datetime_parser.c -o test_datetime
./test_datetime
