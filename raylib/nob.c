#include <stdlib.h>
#include <stdbool.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "../nob.h"

static void
cc_from_env(Nob_Cmd *cmd)
{
	static const char *cc = NULL;
	if (cc == NULL) {
		cc = getenv("CC");
	}
	if (cc == NULL) {
		cc = "cc";
	}
	nob_cmd_append(cmd, cc);
}

int
main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF(argc, argv);
	#ifndef __linux__
	nob_log(NOB_WARNING, "raylib example only builds on linux");
	exit(EXIT_SUCCESS);
	#endif
	bool run = false;

	Nob_Cmd *cmd = &(Nob_Cmd){ 0 };
	cc_from_env(cmd);
	nob_cmd_append(cmd, "-std=c17", "-pedantic");
	nob_cmd_append(cmd, "-Wall", "-Werror");
	nob_cc_output(cmd, "main");
	nob_cc_inputs(cmd, "main.c");
	nob_cmd_append(cmd, "-lraylib", "-lm");
	if (!nob_cmd_run_sync_and_reset(cmd)) {
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "run") == 0) {
			run = true;
		}
	}

	if (run) {
		nob_cmd_append(cmd, "./main");
		if (!nob_cmd_run_sync_and_reset(cmd)) {
			exit(EXIT_FAILURE);
		}
	}
}
