/*
This source file has a UTF-8 BOM and UTF-8 encoded strings. It runs a test to
determine whether or not the compiler did an incorrect conversion on the
strings.

cl utf8_compiler_string_test.c && utf8_compiler_string_test
or:
gcc -o utf8_compiler_string_test utf8_compiler_string_test.c && utf8_compiler_string_test

Microsoft compiler (cl) specific:
If a UTF-8 BOM and pragma (below) are present then the Visual Studio compiler
will leave the UTF-8 encoded strings as is and won't do an incorrect
conversion. Alternately if neither BOM nor pragma is present then the compiler
won't do an incorrect conversion (if the developer's local codepage isn't
multibyte). However if only one or the other is present that will lead to an
incorrect conversion. Refer to this project's README for more information.

Copyright (C) 2017 Jay Satiro <raysatiro@yahoo.com>
http://curl.haxx.se/docs/copyright.html

https://github.com/jay/compiler_string_test
*/

#if defined(_MSC_VER) && (_MSC_VER == 1700)
#error "Visual Studio 2012 does not support execution_character_set"
#endif
#pragma execution_character_set("utf-8")

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
// actual: the actual unicode character in utf-8
// escaped: char string of escaped hex equal to the actual unicode character
#define CHECK(actual, escaped) \
{ \
  char x[] = actual; \
  char y[] = escaped; \
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

  CHECK("𤭢", "\xF0\xA4\xAD\xA2");

  return fail_count ? EXIT_FAILURE : EXIT_SUCCESS;
}
