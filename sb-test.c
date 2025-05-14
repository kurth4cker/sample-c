// SPDX-License-Identifier: MIT OR Unlicense
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "sb.h"

int main(void)
{
	Sb *sb = &(Sb){ 0 };
	sb_append_str(sb, "hello");
	sb_append_char(sb, ' ');
	sb_append_str(sb, "world");
	const char *greeting = sb_cstr(sb);
	assert(strcmp(greeting, "hello world") == 0);
	printf("%s\n", greeting);
}
