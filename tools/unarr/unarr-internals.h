/* Copyright 2014 the unarr project authors (see AUTHORS file).
   License: GPLv3 */

#ifndef unarr_internals_h
#define unarr_internals_h

#include "unarr.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

/***** crc32 *****/

uint32_t ar_crc32(uint32_t crc32, const unsigned char *data, size_t data_len);

/***** stream *****/

typedef void (* ar_stream_close_fn)(void *data);
typedef size_t (* ar_stream_read_fn)(void *data, void *buffer, size_t count);
typedef bool (* ar_stream_seek_fn)(void *data, ptrdiff_t offset, int origin);
typedef size_t (* ar_stream_tell_fn)(void *data);

struct ar_stream_s {
    ar_stream_close_fn close;
    ar_stream_read_fn read;
    ar_stream_seek_fn seek;
    ar_stream_tell_fn tell;
    void *data;
};

ar_stream *ar_open_stream(void *data, ar_stream_close_fn close, ar_stream_read_fn read, ar_stream_seek_fn seek, ar_stream_tell_fn tell);

/***** string ****/

WCHAR *ar_conv_utf8_to_utf16(const char *str);
char *ar_conv_utf16_to_utf8(const WCHAR *wstr);
char *ar_conv_ansi_to_utf8_utf16(const char *astr, WCHAR **wstr_opt);

/***** unarr *****/

#define warn(...) ar_log("!", __FILE__, __LINE__, __VA_ARGS__)
#ifdef DEBUG
#define log(...) ar_log("-", __FILE__, __LINE__, __VA_ARGS__)
#else
#define log(...) ((void)0)
#endif
void ar_log(const char *prefix, const char *file, int line, const char *msg, ...);

typedef void (* ar_archive_close_fn)(ar_archive *ar);
typedef bool (* ar_parse_entry_fn)(ar_archive *ar);
typedef const char *(* ar_entry_get_name_fn)(ar_archive *ar);
typedef const WCHAR *(* ar_entry_get_name_w_fn)(ar_archive *ar);
typedef bool (* ar_entry_uncompress_fn)(ar_archive *ar, void *buffer, size_t count);

struct ar_archive_s {
    ar_archive_close_fn close;
    ar_parse_entry_fn parse_entry;
    ar_entry_get_name_fn get_name;
    ar_entry_get_name_w_fn get_name_w;
    ar_entry_uncompress_fn uncompress;

    ar_stream *stream;
    bool at_eof;
    size_t entry_offset;
    size_t entry_size_block;
    size_t entry_size_uncompressed;
};

ar_archive *ar_open_archive(ar_stream *stream, size_t struct_size, ar_archive_close_fn close, ar_parse_entry_fn parse_entry,
                            ar_entry_get_name_fn get_name, ar_entry_get_name_w_fn get_name_w, ar_entry_uncompress_fn uncompress);

#endif
