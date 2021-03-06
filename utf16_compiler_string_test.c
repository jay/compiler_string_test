/*
This source file has a UTF-16 BOM and UTF-16 encoded strings. It runs a test to
determine whether or not the compiler did an incorrect conversion on the
strings.

cl utf16_compiler_string_test.c && utf16_compiler_string_test

Copyright (C) 2017 Jay Satiro <raysatiro@yahoo.com>
http://curl.haxx.se/docs/copyright.html

https://github.com/jay/compiler_string_test
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

void dump(const char *text, FILE *stream,
          const unsigned char *ptr, unsigned long long size)
{
  unsigned long long i;
  unsigned int c, width = 0x10;

  fprintf(stream, "%s length is %llu bytes (0x%llx)\n",
          text, size, size);

  for(i = 0; i < size; i += width) {

    fprintf(stream, "%8.8llx: ", i);

    /* show hex to the left */
    for(c = 0; c < width; c++) {
      if(i+c < size)
        fprintf(stream, "%02x ", ptr[i+c]);
      else
        fputs("   ", stream);
    }

    /* show data on the right */
    for(c = 0; (c < width) && (i+c < size); c++) {
      char x = (ptr[i+c] >= 0x20 && ptr[i+c] < 0x80) ? ptr[i+c] : '.';
      fputc(x, stream);
    }

    fputc('\n', stream); /* newline */
  }
}

// CHECK(actual, escaped)
// actual: the actual unicode character in utf-16
// escaped: char string of escaped hex equal to the actual unicode character
#define CHECK(actual, escaped) \
{ \
  wchar_t x[] = actual; \
  char y[] = escaped ## "\x00"; \
  \
  fprintf(stderr, "\n"); \
  dump("'actual'", stderr, (unsigned char *)x, sizeof x); \
  dump("'escaped'", stderr, (unsigned char *)y, sizeof y); \
  if(sizeof x == sizeof y && !memcmp(x, y, sizeof x)) \
    fprintf(stderr, "SUCCESS\n"); \
  else { \
    fprintf(stderr, "FAILED\n"); \
    ++fail_count; \
  } \
}

int main(int argc, char *argv[])
{
  int fail_count = 0;

  /* Notepad++ can't display UTF-16:
     The check below is for Chinese character U+24B62 which is a surrogate pair
     in UTF-16 and therefore will not appear properly in Notepad++ since
     Notepad++ only supports UCS-2 (ie fixed length). Furthermore, attempting
     to drag and drop the line may make the character appear properly but
     actually it isn't saved properly.
     https://github.com/notepad-plus-plus/notepad-plus-plus/issues/801#issuecomment-348832085
   */
  CHECK(L"𤭢", "\x52\xD8\x62\xDF");

  return fail_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
