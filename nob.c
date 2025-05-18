#include <stdlib.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

static void
cc(Nob_Cmd *cmd)
{
	const char *cc = getenv("CC");
	if (cc == NULL) {
		cc = "cc";
	}
	nob_cmd_append(cmd, cc);
}

static void
cflags(Nob_Cmd *cmd)
{
	nob_cmd_append(cmd, "-std=c17");
	nob_cmd_append(cmd, "-g");
	nob_cmd_append(cmd, "-pedantic");
	nob_cmd_append(cmd, "-Wall", "-Wextra");
	nob_cmd_append(cmd, "-Werror");
}

static bool
xlib(Nob_Cmd *cmd)
{
#ifndef __APPLE__
	cc(cmd);
	cflags(cmd);
	nob_cc_output(cmd, "xlib");
	nob_cc_inputs(cmd, "xlib.c");
	nob_cmd_append(cmd, "-lX11");
	return nob_cmd_run_sync_and_reset(cmd);
#else
	nob_log(NOB_WARNING, "xlib won't build on macos");
	return true;
#endif // __apple__
}

static bool
sample(Nob_Cmd *cmd)
{
	cc(cmd);
	cflags(cmd);
	nob_cc_output(cmd, "sample");
	nob_cc_inputs(cmd, "sample.c");
	return nob_cmd_run_sync_and_reset(cmd);
}

static bool
cc_dot_c(Nob_Cmd *cmd)
{
	cc(cmd);
	cflags(cmd);
	nob_cmd_append(cmd, "-fsyntax-only");
	nob_cc_inputs(cmd, "cc.c");
	return nob_cmd_run_sync_and_reset(cmd);
}

static bool
cc_test(Nob_Cmd *cmd)
{
	cc(cmd);
	cflags(cmd);
	nob_cc_output(cmd, "cc-test");
	nob_cc_inputs(cmd, "sb.c", "cc.c", "cc-test.c");
	if (!nob_cmd_run_sync_and_reset(cmd)) {
		return false;
	}

	nob_cmd_append(cmd, "./cc-test");
	return nob_cmd_run_sync_and_reset(cmd);
}

static bool
sb_dot_c(Nob_Cmd *cmd)
{
	cc(cmd);
	cflags(cmd);
	nob_cmd_append(cmd, "-fsyntax-only");
	nob_cc_inputs(cmd, "sb.c");
	return nob_cmd_run_sync_and_reset(cmd);
}

static bool
sb_test(Nob_Cmd *cmd)
{
	cc(cmd);
	cflags(cmd);
	nob_cc_output(cmd, "sb-test");
	nob_cc_inputs(cmd, "sb.c", "sb-test.c");
	if (!nob_cmd_run_sync_and_reset(cmd)) {
		return false;
	}

	nob_cmd_append(cmd, "./sb-test");
	return nob_cmd_run_sync_and_reset(cmd);
}

int
main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

	Nob_Cmd *cmd = &(Nob_Cmd){ 0 };

	if (!sample(cmd) || !xlib(cmd) ||
	    !sb_dot_c(cmd) || !sb_test(cmd) ||
	    !cc_dot_c(cmd) || !cc_test(cmd)) {
		exit(EXIT_FAILURE);
	}
}
