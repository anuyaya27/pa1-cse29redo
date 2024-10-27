For the 3-byte sequence 11100000 10000000 10100001:

1. What code point does it encode in UTF-8, and what character is that?
The first byte 11100000 (0xE0) indicates a 3-byte sequence because it starts with three 1 (1110). The second and third bytes (10000000 and 10100001) are continuation bytes (they start with 10), which carry the remaining bits of the code point.
So, the first byte will be: 0000
Second byte: 000000
Third byte: 100001
Concatenating the above, we get: 00000000100001 in binary, which converts to decimal 0x21, which is U+0081.

2. What are the three other ways to encode that character?
There are several other ways to encode a character, like:
a) UTF-16: this version uses 2 bytes to represent characters.
b) UTF-32: this version uses a fixed 4 byte representation for any character.
c) Windows-1252 encoding: this encoding uses 1 byte representation for each character

3. 