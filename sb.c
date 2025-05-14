// SPDX-License-Identifier: MIT OR Unlicense
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <assert.h>

#include "sb.h"

static bool sb__init_if_needed(Sb *sb)
{
    assert(sb != NULL);
    const size_t DEFAULT_CAPACITY = 16;

    if (sb->data != NULL) {
        return true;
    }
    char *data = malloc(sizeof(*sb->data) * DEFAULT_CAPACITY);
    if (data == NULL) {
        return false;
    }
    sb->data = data;
    sb->capacity = DEFAULT_CAPACITY;
    return true;
}

static bool sb__grow_if_needed(Sb *sb)
{
    assert(sb->data != NULL);
}
