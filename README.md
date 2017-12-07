Compiler String Test
====================

This repo is in memoriam of a problem I had compiling source files in Visual
Studio that contained UTF-8 encoded strings.

Basically if a source file has a UTF-8 BOM it must also have
[`#pragma execution_character_set("utf-8")`](https://msdn.microsoft.com/en-us/library/mt708823.aspx)
in order to preserve the UTF-8 encoded strings in the source file, otherwise
the compiler will attempt to do some incorrect conversion on the strings:

~~~
utf8_compiler_string_test.c(88) : warning C4566: character represented by
universal-character-name '\UDF62D852' cannot be represented in the current
code page (1252)
~~~

Alternately the compiler seems to preserve UTF-8 strings in a source file if it
has neither BOM nor execution_character_set. However, according to some
comments in this
[stackoverflow thread](https://stackoverflow.com/q/688760),
if that source is being compiled on a system where the local codepage is
multibyte (eg Japan) then some incorrect conversion may take place.

In file utf8_compiler_string_test.c I experimented with the combinations of
UTF-8 BOM and `#pragma execution_character_set("utf-8")` with a UTF-8 char
string of "𤭢" which is U+24B62 and is UTF-8 encoded hex F0 A4 AD A2.

[![capture1](screenshots/capture1.png?raw=true)](screenshots/capture1.png?raw=true)

[![capture2](screenshots/capture2.png?raw=true)](screenshots/capture2.png?raw=true)

The results, below, are the same in VS2010 and VS2015. Legend: *actual* is the
actual UTF-8 encoded character in the char string (refer to the first argument
in CHECK in the screenshots above) and *escaped* is that same character but
escaped in hex "\xF0\xA4\xAD\xA2" which I used to check against *actual*.

~~~
-------------------------------------------------------------------------------
BOM: yes
execution_character_set: yes

'actual' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
'escaped' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
SUCCESS

-------------------------------------------------------------------------------
BOM: yes
execution_character_set: no

'actual' length is 3 bytes (0x3)
00000000: 3f 3f 00                                        ??.
'escaped' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
FAILED

-------------------------------------------------------------------------------
BOM: no
execution_character_set: yes

'actual' length is 9 bytes (0x9)
00000000: c3 b0 c2 a4 c2 ad c2 a2 00                      .........
'escaped' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
FAILED

-------------------------------------------------------------------------------
BOM: no
execution_character_set: no

'actual' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
'escaped' length is 5 bytes (0x5)
00000000: f0 a4 ad a2 00                                  .....
SUCCESS

-------------------------------------------------------------------------------
~~~


Other
-----

### Resources

VS 2008
[may or may not support](https://support.microsoft.com/en-us/help/980263/fix-string-literals-are-misinterpreted-by-the-visual-c-compiler-in-vis)
`#pragma execution_character_set("utf-8")`.

VS 2012
[doesn't support](https://connect.microsoft.com/VisualStudio/feedback/details/773186)
`#pragma execution_character_set("utf-8")`. (Also see
[this thread](https://social.msdn.microsoft.com/Forums/vstudio/en-US/2f328917-4e99-40be-adfa-35cc17c9cdec/pragma-executioncharactersetutf8?forum=vcgeneral))

A stackoverflow question with a lot of answers related to this topic:
[How to create a UTF-8 string literal in Visual C++ 2008](https://stackoverflow.com/q/688760)

A cprogramming forum post:
[Extended characters in your source files](https://cboard.cprogramming.com/cplusplus-programming/145590-non-english-characters-cout-2.html#post1086757)
