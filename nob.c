#include <stdlib.h>

#define NOB_IMPLEMENTATION
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

static void
add_cc_from_env_or(Nob_Cmd *cmd, const char *default_cc)
{
    static const char *cc = NULL;
    if (cc == NULL) {
        cc = getenv("CC");
    }
    if (cc == NULL) {
        cc = default_cc;
    }
    nob_cmd_append(cmd, cc);
}

#define run(cmd, ...) \
    _run(cmd, \
         ((const char*[]){__VA_ARGS__}), \
         (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))
static void
_run(Nob_Cmd *cmd, const char **args, size_t count)
{
    if (count < 1) {
        fprintf(stderr, "[ERROR] run should be called with at least two argument: cmd and program\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < count; i++) {
        nob_cmd_append(cmd, args[i]);
    }
    if (!nob_cmd_run_sync_and_reset(cmd)) {
        exit(EXIT_FAILURE);
    }
}

#define compile(cmd, output, ...) \
    _compile(cmd, output, \
             ((const char*[]){__VA_ARGS__}), \
             (sizeof((const char*[]){__VA_ARGS__})/sizeof(const char*)))
#ifdef _WIN32
static void
_compile(Nob_Cmd *cmd, const char *output, const char **args, size_t count)
{
    add_cc_from_env_or(cmd, "cl");
    nob_cmd_append(cmd, "/nologo", "/std:c17");
    nob_cmd_append(cmd, "/Wall");
    // disabled by default, there are warnings I do not know what they means
    // nob_cmd_append(cmd, "/WX");
    nob_cc_output(cmd, output);
    for (size_t i = 0; i < count; i++) {
        nob_cmd_append(cmd, args[i]);
    }
    if (!nob_cmd_run_sync_and_reset(cmd)) {
        exit(EXIT_FAILURE);
    }
}
#else
static void
_compile(Nob_Cmd *cmd, const char *output, const char **args, size_t count)
{
    add_cc_from_env_or(cmd, "cc");
    nob_cmd_append(cmd, "-std=c17", "-pedantic",
                   "-g",
                   "-Wall",
                   "-Wextra",
                   "-Werror");
    nob_cc_output(cmd, output);
    for (size_t i = 0; i < count; i++) {
        nob_cmd_append(cmd, args[i]);
    }
    if (!nob_cmd_run_sync_and_reset(cmd)) {
        exit(EXIT_FAILURE);
    }
}
#endif // _WIN32

int
main(int argc, char **argv)
{
	NOB_GO_REBUILD_URSELF_PLUS(argc, argv, "nob.h");

	Nob_Cmd *cmd = &(Nob_Cmd){ 0 };

	compile(cmd, "sample", "sample.c");
	compile(cmd, "sb-test", "sb.c", "sb-test.c");
	compile(cmd, "cc-test", "sb.c", "cc.c", "cc-test.c");
#ifdef __linux__
	compile(cmd, "xlib", "xlib.c", "-lX11");
#else
	nob_log(NOB_WARNING, "xlib only compiles on linux");
#endif // __linux__

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "test") == 0) {
			run(cmd, "./sample");
			run(cmd, "./sb-test");
			run(cmd, "./cc-test");
		}
	}
}
