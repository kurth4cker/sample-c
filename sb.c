// SPDX-License-Identifier: MIT OR Unlicense
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "sb.h"

static bool sb__init_if_needed(Sb *sb)
{
    assert(sb != NULL);
    const size_t INITIAL_CAPACITY = 16;

    if (sb->data != NULL) {
        return true;
    }
    char *data = malloc(sizeof(*sb->data) * INITIAL_CAPACITY);
    if (data == NULL) {
        return false;
    }
    sb->data = data;
    sb->capacity = INITIAL_CAPACITY;
    return true;
}

static bool sb__grow_if_needed(Sb *sb, size_t plus_size)
{
    assert(sb->data != NULL);

    const size_t ESTIMATED_SIZE = sb->size + plus_size;
    if (sb->capacity > ESTIMATED_SIZE) {
        return true;
    }
    size_t new_capacity = sb->capacity * 2;
    while (new_capacity < ESTIMATED_SIZE) {
        new_capacity *= 2;
    }
    char *data = realloc(sb->data, new_capacity);
    if (data == NULL) {
        return false;
    }
    sb->data = data;
    sb->capacity = new_capacity;
    return true;
}

bool sb_append_str(Sb *sb, const char *str)
{
    const size_t length = strlen(str);
    if (!sb__init_if_needed(sb) || !sb__grow_if_needed(sb, length)) {
        return false;
    }
    memcpy(sb->data + sb->size, str, length);
    sb->size += length;
    return true;
}

bool sb_append_char(Sb *sb, int ch)
{
    if (!sb__init_if_needed(sb) || !sb__grow_if_needed(sb, 1)) {
        return false;
    }
    sb->data[sb->size] = ch;
    sb->size++;
    return true;
}

char *sb_cstr(const Sb *sb)
{
    const size_t length = sb->size + 1;
    char *str = malloc(sizeof(*str) * length);
    memcpy(str, sb->data, length - 1);
    str[length-1] = '\0';
    return str;
}
