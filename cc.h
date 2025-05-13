#ifndef CC_H_INCLUDED
#define CC_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>


typedef struct {
    const char *name;
    const char *data;
} Cc_Flag;

typedef struct {
    size_t size;
    const char **strings;
} Cc_Str_Array;

typedef struct {
    const char *name;
    const Cc_Flag *flags;
    Cc_Str_Array output;
} Cc;
extern const Cc CC_CC;
extern const Cc CC_GCC;
extern const Cc CC_CLANG;
Cc cc_detect(void);

typedef struct {
    const Cc *cc;

    size_t size;
    size_t capacity;
    const char **args;
} Cc_Cmd;
bool cc_cmd_std(Cc_Cmd *cmd, const char *cstd);
bool cc_cmd_output(Cc_Cmd *cmd, const char *output);
bool cc_cmd_run(Cc_Cmd *cmd);
#define cc_cmd_flags(cmd, ...) cc_cmd__flags(cmd, __VA_ARGS__, NULL)
bool cc_cmd__flags(Cc_Cmd *cmd, ...);
#define cc_cmd_inputs(cmd, ...) cc_cmd__inputs(cmd, __VA_ARGS__, NULL)
bool cc_cmd__inputs(Cc_Cmd *cmd, ...);

#endif // CC_H_INCLUDED
