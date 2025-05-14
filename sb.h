#ifndef SB_H_INCLUDED
#define SB_H_INCLUDED

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    size_t capacity, size;
    char *data;
} Sb; // String Builder

bool sb_append_str(Sb *sb, const char *str);
bool sb_append_char(Sb *sb, int ch);
char *sb_cstr(const Sb *sb);

#endif // SB_H_INCLUDED
