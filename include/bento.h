#ifndef BENTO_H
#define BENTO_H

#include <nori.h>

typedef enum JSONValueType {
    JSON_VALUE_OBJECT,
    JSON_VALUE_ARRAY,
    JSON_VALUE_STRING,
    JSON_VALUE_NUMBER,
    JSON_VALUE_TRUE,
    JSON_VALUE_FALSE,
    JSON_VALUE_NULL,
    JSON_VALUE_INVALID,
    JSON_VALUE_COUNT
} JSONValueType;

typedef struct JSONValueCommon {
    JSONValueType type;
} JSONValueCommon;

typedef struct JSONObject {
    JSONValueCommon c;
    NList *keys;    // of const char *
    NList *values;  // of JSONValue
} JSONObject;

typedef struct JSONArray {
    JSONValueCommon c;
    NList *data;    // of JSONValue
} JSONArray;

typedef struct JSONString {
    JSONValueCommon c;
    uint32 qty;
    unsigned char *data;
} JSONString;

typedef struct JSONNumber {
    JSONValueCommon c;
    int16 exponent;
    int32 integer;
    int32 frac;
} JSONNumber;

// Values "true", "false", and "null" are given by their type.
typedef union JSONValue {
    JSONValueCommon c;
    JSONObject o;
    JSONArray a;
    JSONString s;
    JSONNumber n;
} JSONValue;

// Parser
typedef enum JSONTokenType {
    TOKEN_OBJ_BEGIN,
    TOKEN_OBJ_END,
    TOKEN_ARRAY_BEGIN,
    TOKEN_ARRAY_END,
    TOKEN_STRING_BEGIN,
    TOKEN_STRING_END,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_WS,    
    TOKEN_BOOL,
    TOKEN_NULL,
    TOKEN_UNKNOWN, // Means "depends on context." Could be valid.
    TOKEN_COUNT
} JSONTokenType;

typedef struct JSONParser {
    void *f;
    char *buffer;
    uint32 bufsize;
    JSONTokenType type;
    char *token;
} JSONParser;

/// \brief Trims extra whitespace from a string, leaving only one space
///    between tokens and none before/after
/// Whitespace affected: \\n, \\r, \\t, \\s
char *bparse_trim_ws(const void *blk, uint32 len);

JSONParser bparse_open(const char *filename);
boolean bparse_has_more_tokens(JSONParser p);
void bparse_advance(JSONParser p);
char *bparse_get_token(JSONParser p);
JSONTokenType bparse_get_token_type(JSONParser p);
void bparse_close(JSONParser p);

// Value building functions.
// Begin/end structure allows for incremental reading from buffer in
// case of objects much larger than a feasible string buffer.
// Read in tokens until none left, then call end() to initiate a final
// validity check & away you go.
JSONObject b_obj_begin(void);
void b_obj_push(JSONObject o, const char *key, JSONValue value);
void b_obj_end(JSONObject o);
boolean b_obj_entry_exists(JSONObject o, const char *key);
JSONValueType b_obj_entry_type(JSONObject o, const char *key);
JSONValue b_obj_entry_value(JSONObject o, const char *key);
void b_obj_free(JSONObject o);

JSONArray b_array_begin(void);
void b_array_push(JSONArray a, JSONValue value);
void b_array_end(JSONArray a);
uint32 b_array_length(JSONArray a);
JSONValue b_array_at(JSONArray a, uint32 pos);
void b_array_free(JSONArray a);

JSONString b_string_begin(void);
void b_string_push(JSONString s, const char *buf);
void b_string_end(JSONString s);
char *b_string_to_cstr(JSONString s);

JSONNumber b_number_begin(void);
void b_number_push(JSONNumber b, const char *buf);
void b_number_end(JSONNumber n);
int32 b_number_to_int32(JSONNumber n);
uint32 b_number_to_uint32(JSONNumber n);
#define b_number_to_float(n) b_number_to_real32((n))
real32 b_number_to_real32(JSONNumber n);
#define b_number_to_double(n) b_number_to_real64((n))
real64 b_number_to_real64(JSONNumber n);

#endif
