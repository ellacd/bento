#include "bento.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// lol
#define assert(x) (x)

static boolean is_ws(char c)
{
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
        return TRUE;
    default:
        return FALSE;
    }
}

static boolean is_esc_seq(char c)
{
    switch(c) {
    case '"':
    case '\\':
    case '/':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
    case 'u':
        return TRUE;
    default:
        return FALSE;
    }
}

char *bparse_trim_ws(const void *blk, uint32 len)
{
    char *s2;
    uint32 i;
    uint32 j = 0;

    s2 = malloc(len);
    memcpy(s2, blk, len);
    for (i = 0; i < len; i++) {
        if (is_ws(*(char *) blk + i)) {
            // case 1: start of string
            if (j < 1) continue;

            while (is_ws(*(char *) blk + i)) i++;

            if (*(char *) blk + i == '\0') {
                // case 2: trailing ws at end of string
                s2[j] = '\0';
                break;
            } else {
                // case 3: midstring ws, replace with exactly 1 space
                s2[j++] = ' ';
                s2[j++] = *(char *) blk + i;
            }
        } else {
            s2[j++] = *(char *) blk + i;
        }
    }
    return s2;
}

// Trims all ws to left and right of string contents;
// reduces all mid-string whitespace to a single space.
void bparse_trim_ws_inplace(char *s, uint32 len)
{
    uint32 i;
    uint32 j = 0;
    boolean ws = FALSE;

    for (i = 0; i < len; i++) {
        if (!is_ws(s[i])) {
            if (ws) {
                s[j++] = ' ';
                ws = !ws;
            }
            s[j++] = s[i];
            continue;
        }

        // ws at string start
        if (j < 1) continue;

        ws = TRUE;
    }
    s[j] = '\0';
}

JSONParser bparse_open(const char *filename)
{
    JSONParser p;
    p.f = fopen(filename, "r");
    p.bufsize = 128;
    p.buffer = malloc(128);
    p.token = p.buffer;
    return p;
}

boolean bparse_has_more_tokens(JSONParser p)
{
    return !feof(p.f);
}

void bparse_advance(JSONParser p)
{
    if (p.token < p.buffer) {
        // TODO
        // Advance to next token in buffer    
    } else {
        fgets(p.buffer, p.bufsize, p.f);
        bparse_trim_ws_inplace(p.buffer, p.bufsize);
    }

    // TODO
    // Discern token type based on available information
}

char *bparse_get_token(JSONParser p)
{
    return p.buffer;
}

JSONTokenType bparse_get_token_type(JSONParser p)
{
    return p.type;
}

void bparse_close(JSONParser p)
{
    fclose(p.f);
}
