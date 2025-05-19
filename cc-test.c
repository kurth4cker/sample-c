// SPDX-License-Identifier: MIT OR Unlicense
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <stdlib.h>

#include "cc.h"

int main(void)
{
	Cc_Cmd *cmd = &(Cc_Cmd) { 0 };
	cc_cmd_std(cmd, "c17");
	cc_cmd_flags(cmd, "debug");
	cc_cmd_flags(cmd, "werror", "wall", "wextra");
	cc_cmd_output(cmd, "sample");
	cc_cmd_inputs(cmd, "sample.c");
#if defined(__unix__) || defined(__APPLE__)
	if (!cc_cmd_run(cmd)) {
		exit(EXIT_FAILURE);
	}
#else
	fprintf(stderr, "[WARNING] running cc is only supported on UNIX-like systems\n");
#endif
}
