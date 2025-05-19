// SPDX-License-Identifier: MIT OR Unlicense
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef _POSIX_C_SOURCE
#endif

#include "cc.h"

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(*arr))


static const Cc_Flag CC_CC_FLAGS[] = {
    {"debug", "-g"},

    {"werror", "-Werror"},
    {"wall", "-Wall"},
    {"wextra", "-Wextra"},

    {"syntax-only", "-fsyntax-only"},

    {"std=c89", "-std=c89"},
    {"std=c99", "-std=c99"},
    {"std=c11", "-std=c11"},
    {"std=c17", "-std=c17"},
    {"std=c23", "-std=c23"},

    {NULL, NULL},
};

const Cc CC_CC = {
    .name = "cc",
    .flags = CC_CC_FLAGS,
    .output = {
        .size = 2,
        .strings = (const char *[]){
            "-o",
            "%s",
        },
    },
};
const Cc CC_GCC = {
    .name = "gcc",
    .flags = CC_CC_FLAGS
};
const Cc CC_CLANG = {
    .name = "clang",
    .flags = CC_CC_FLAGS,
};
static Cc cc__current = { 0 };

Cc cc_detect(void)
{
    const char *name = getenv("CC");
    if (name == NULL) {
        return CC_CC;
    }
    if (strcmp(name, CC_GCC.name) == 0) {
        return CC_GCC;
    }
    if (strcmp(name, CC_CLANG.name) == 0) {
        return CC_CLANG;
    }
    if (strcmp(name, CC_CC.name) == 0) {
        return CC_CC;
    }
    Cc cc = {
        .name = name,
    };
    return cc;
}

static const char *cc__lookup_flag(const Cc *cc, const char *needle)
{
    assert(cc != NULL);
    if (cc->flags == NULL) {
        return NULL;
    }
    for (const Cc_Flag *flag = cc->flags; !(flag->name == NULL && flag->data == NULL); flag++) {
        if (strcmp(flag->name, needle) == 0) {
            return flag->data;
        }
    }
    return NULL;
}

static bool cc_cmd__grow_if_needed(Cc_Cmd *cmd)
{
    assert(cmd != NULL);
    assert(cmd->capacity >= cmd->size);
    if (cmd->capacity == cmd->size) {
        const size_t NEW_CAPACITY = cmd->capacity * 2;
        const char **args = realloc(cmd->args, NEW_CAPACITY);
        if (args == NULL) {
            return false;
        }
        cmd->args = args;
        cmd->capacity = NEW_CAPACITY;
    }
    return true;
}

static bool
cc__cmd_append1(Cc_Cmd *cmd, const char *arg)
{
    cc_cmd__grow_if_needed(cmd);
    cmd->args[cmd->size++] = arg;
    return true;
}

static bool cc_cmd__init_if_needed(Cc_Cmd *cmd)
{
    assert(cmd != NULL);
    assert(cmd->capacity >= cmd->size);

    const size_t INITIAL_CAPACITY = 16;

    if (!cmd->cc) {
        cc__current = cc_detect();
        cmd->cc = &cc__current;
    }

    // initialize args
    if (cmd->args == NULL) {
        const char **args = calloc(INITIAL_CAPACITY, sizeof(*args));
        if (args == NULL) {
            return false;
        }
        cmd->args = args;
        cmd->capacity = INITIAL_CAPACITY;
    }

    if (cmd->args[0] == NULL) {
        return cc__cmd_append1(cmd, cmd->cc->name);
    }
    return true;
}

bool cc_cmd__flags(Cc_Cmd *cmd, ...)
{
    cc_cmd__init_if_needed(cmd);

    va_list ap;

    va_start(ap, cmd);
    for (;;) {
        const char *arg = va_arg(ap, const char *);
        if (arg == NULL) {
            break;
        }
        const char *flag = cc__lookup_flag(cmd->cc, arg);
        if (flag == NULL) {
            fprintf(stderr, "[WARNING] unknown flag: %s\n", arg);
        }
        cc__cmd_append1(cmd, flag);
    }
    va_end(ap);
    return true;
}

bool cc_cmd__inputs(Cc_Cmd *cmd, ...)
{
    va_list ap;

    va_start(ap, cmd);
    for (;;) {
        const char *arg = va_arg(ap, const char *);
        if (arg == NULL) {
            break;
        }
        if (!cc__cmd_append1(cmd, arg)) {
            return false;
        }
    }
    va_end(ap);
    return true;
}

bool cc_cmd_output(Cc_Cmd *cmd, const char *output)
{
    cc_cmd__init_if_needed(cmd);
    for (size_t i = 0; i < cmd->cc->output.size; i++) {
        const char *fmt = cmd->cc->output.strings[i];
        if (strstr(fmt, "%s") != NULL) {
            const size_t flagsize = strlen(fmt) + strlen(output) - 2 + 1;
            char *flag = malloc(sizeof(*flag) * flagsize);
            if (flag == NULL) {
                fprintf(stderr, "[ERROR] cannot allocate memory for cc_cmd_output\n");
                return false;
            }
            snprintf(flag, flagsize, fmt, output);
            if (!cc__cmd_append1(cmd, flag)) {
                return false;
            }
        } else {
            if (!cc__cmd_append1(cmd, fmt)) {
                return false;
            }
        }
    }
    return true;
}

bool cc_cmd_std(Cc_Cmd *cmd, const char *cstd)
{
    cc_cmd__init_if_needed(cmd);
    char std_flag[8] = { 0 };
    snprintf(std_flag, ARRAY_LENGTH(std_flag), "std=%s", cstd);
    const char *flag = cc__lookup_flag(cmd->cc, std_flag);
    if (flag == NULL) {
        fprintf(stderr, "[WARNING] unknown C std (%s) by %s, ignoring\n", cstd, cmd->cc->name);
        return false;
    }
    return cc__cmd_append1(cmd, flag);
}

static char *cc__strdup(const char *str)
{
    const size_t str_len = strlen(str);
    const size_t dest_size = str_len + 1;
    char *result = malloc(sizeof(*result) * dest_size);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, str, str_len);
    result[str_len] = '\0';
    return result;
}

char **cc__cmd_to_arg_array(const Cc_Cmd *cmd)
{
    const size_t dest_size = cmd->size + 1;
    char **result = malloc(sizeof(*result) * dest_size);
    if (result == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < cmd->size; i++) {
        char *str = cc__strdup(cmd->args[i]);
        if (str == NULL) {
            for (size_t k = 0; k < i; k++) {
                free(result[k]);
            }
            free(result);
            return NULL;
        }
        result[i] = str;
    }
    return result;
}

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/wait.h>

bool cc_cmd_run(Cc_Cmd *cmd)
{
    cc_cmd__init_if_needed(cmd);
    pid_t pid = fork();
    if (pid == -1) {
        fprintf(stderr, "[ERROR] cannot run CMD\n");
        return false;
    }
    if (pid == 0) {
        char **args = cc__cmd_to_arg_array(cmd);
        if (args == NULL) {
            fprintf(stderr, "[ERROR] cannot exec CMD");
            exit(EXIT_FAILURE);
        }
        execvp(cmd->args[0], args);
        fprintf(stderr, "[ERROR] cannot exec CMD\n");
        exit(EXIT_FAILURE);
    } else {
        // fprintf(stderr, "[INFO] CMD: %s\n", cc__cmd_render(cmd));
        fprintf(stderr, "[TODO] %s\n", "implement cmd rendering");
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            fprintf(stderr, "[ERROR] cannot wait for child process\n");
            return false;
        }
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                fprintf(stderr, "[ERROR] process exited with %d\n", WEXITSTATUS(status));
            }
        } else {
            fprintf(stderr, "[ERROR] process exited abnormally with %d\n", WEXITSTATUS(status));
        }
    }
    return true;
}
#elif defined(_WIN32)
// cmd_run is not supported on Windows
#endif
