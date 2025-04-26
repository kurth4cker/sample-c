#include <stdlib.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "../nob.h"

int
main(int argc, const char **argv)
{
	NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "../nob.h");

	Nob_Cmd cmd = { 0 };
	nob_cmd_append(&cmd, "cc", "-std=c17");
	nob_cmd_append(&cmd, "-g");
	nob_cmd_append(&cmd, "-pedantic");
	nob_cmd_append(&cmd, "-Wall", "-Wextra");
	nob_cmd_append(&cmd, "-Werror");
	nob_cmd_append(&cmd, "-o", "sample");
	nob_cmd_append(&cmd, "main.c");
	nob_cmd_append(&cmd, "-lX11");

	if (!nob_cmd_run_sync(cmd)) {
		exit(EXIT_FAILURE);
	}
}
