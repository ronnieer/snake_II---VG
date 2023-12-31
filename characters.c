// #############################################################################
// #################### CONSTANTS ##############################################
// #############################################################################

const unsigned char myCharacters[128][8] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* NULL*/
{0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03},       /* ""*/
{0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* ""*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},       /* " "*/
{0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00},       /* "!"*/
{0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00},       /* """*/
{0x14, 0x14, 0x3E, 0x14, 0x3E, 0x14, 0x14, 0x00},       /* "#"*/
{0x08, 0x3C, 0x0A, 0x1C, 0x28, 0x1E, 0x08, 0x00},       /* "$"*/
{0x06, 0x26, 0x10, 0x08, 0x04, 0x32, 0x30, 0x00},       /* "%"*/
{0x04, 0x0A, 0x0A, 0x24, 0x2A, 0x12, 0x2C, 0x00},       /* "&"*/
{0x0C, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00},       /* "'"*/
{0x10, 0x08, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00},       /* "("*/
{0x04, 0x08, 0x10, 0x10, 0x10, 0x08, 0x04, 0x00},       /* ")"*/
{0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08, 0x00},       /* "*"*/
{0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00},       /* "+"*/
{0x00, 0x00, 0x0C, 0x08, 0x04, 0x00, 0x00, 0x00},       /* ","*/
{0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00},       /* "-"*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},       /* "."*/
{0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00},       /* "/"*/
{0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00},       /* "0"*/
{0x08, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00},       /* "1"*/
{0x1C, 0x22, 0x20, 0x10, 0x08, 0x04, 0x3E, 0x00},       /* "2"*/
{0x1E, 0x20, 0x20, 0x1C, 0x20, 0x20, 0x1E, 0x00},       /* "3"*/
{0x22, 0x22, 0x22, 0x3E, 0x20, 0x20, 0x20, 0x00},       /* "4"*/
{0x3E, 0x02, 0x1E, 0x20, 0x20, 0x22, 0x1C, 0x00},       /* "5"*/
{0x18, 0x04, 0x02, 0x1E, 0x22, 0x22, 0x1C, 0x00},       /* "6"*/
{0x3E, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x00},       /* "7"*/
{0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00},       /* "8"*/
{0x1C, 0x22, 0x22, 0x3C, 0x20, 0x10, 0x0C, 0x00},       /* "9"*/
{0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00, 0x00},       /* ":"*/
{0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x08, 0x04, 0x00},       /* ";"*/
{0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x00},       /* "<"*/
{0x00, 0x00, 0x3E, 0x00, 0x00, 0x3E, 0x00, 0x00},       /* "="*/
{0x02, 0x04, 0x08, 0x10, 0x08, 0x04, 0x02, 0x00},       /* ">"*/
{0x1C, 0x22, 0x20, 0x10, 0x08, 0x00, 0x08, 0x00},       /* "?"*/
{0x1C, 0x22, 0x3A, 0x2A, 0x3A, 0x02, 0x3C, 0x00},       /* "@"*/
{0x1C, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00},       /* "A"*/
{0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E, 0x00},       /* "B"*/
{0x1C, 0x22, 0x02, 0x02, 0x02, 0x22, 0x1C, 0x00},       /* "C"*/
{0x1E, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1E, 0x00},       /* "D"*/
{0x3E, 0x02, 0x02, 0x3E, 0x02, 0x02, 0x3E, 0x00},       /* "E"*/
{0x3E, 0x02, 0x02, 0x3E, 0x02, 0x02, 0x02, 0x00},       /* "F"*/
{0x1C, 0x22, 0x02, 0x3A, 0x22, 0x22, 0x1C, 0x00},       /* "G"*/
{0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22, 0x00},       /* "H"*/
{0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00},       /* "I"*/
{0x3C, 0x10, 0x10, 0x10, 0x10, 0x12, 0x0C, 0x00},       /* "J"*/
{0x22, 0x12, 0x0A, 0x06, 0x0A, 0x12, 0x22, 0x00},       /* "K"*/
{0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3E, 0x00},       /* "L"*/
{0x22, 0x36, 0x2A, 0x22, 0x22, 0x22, 0x22, 0x00},       /* "M"*/
{0x22, 0x22, 0x26, 0x2A, 0x32, 0x22, 0x22, 0x00},       /* "N"*/
{0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00},       /* "O"*/
{0x1E, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x02, 0x00},       /* "P"*/
{0x1C, 0x22, 0x22, 0x22, 0x2A, 0x12, 0x2C, 0x00},       /* "Q"*/
{0x1E, 0x22, 0x22, 0x1E, 0x0A, 0x12, 0x22, 0x00},       /* "R"*/
{0x1C, 0x22, 0x02, 0x1C, 0x20, 0x22, 0x1C, 0x00},       /* "S"*/
{0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00},       /* "T"*/
{0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00},       /* "U"*/
{0x22, 0x22, 0x22, 0x22, 0x22, 0x14, 0x08, 0x00},       /* "V"*/
{0x22, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x22, 0x00},       /* "W"*/
{0x22, 0x22, 0x14, 0x08, 0x14, 0x22, 0x22, 0x00},       /* "X"*/
{0x22, 0x22, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00},       /* "Y"*/
{0x3E, 0x20, 0x10, 0x08, 0x04, 0x02, 0x3E, 0x00},       /* "Z"*/
{0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1C, 0x00},       /* "["*/
{0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00, 0x00},       /* "\"*/
{0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C, 0x00},       /* "]"*/
{0x08, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00},       /* "^"*/
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00},       /* "_"*/
{0x18, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},       /* "`"*/
{0x00, 0x00, 0x1C, 0x20, 0x3C, 0x22, 0x3C, 0x00},       /* "a"*/
{0x02, 0x02, 0x02, 0x1E, 0x22, 0x22, 0x1E, 0x00},       /* "b"*/
{0x00, 0x00, 0x00, 0x3C, 0x02, 0x02, 0x3C, 0x00},       /* "c"*/
{0x20, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x3C, 0x00},       /* "d"*/
{0x00, 0x00, 0x1C, 0x22, 0x3E, 0x02, 0x1C, 0x00},       /* "e"*/
{0x30, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x00},       /* "f"*/
{0x00, 0x00, 0x3C, 0x22, 0x3C, 0x20, 0x1C, 0x00},       /* "g"*/
{0x02, 0x02, 0x1A, 0x26, 0x22, 0x22, 0x22, 0x00},       /* "h"*/
{0x08, 0x00, 0x0C, 0x08, 0x08, 0x08, 0x1C, 0x00},       /* "i"*/
{0x10, 0x00, 0x18, 0x10, 0x10, 0x12, 0x0C, 0x00},       /* "j"*/
{0x02, 0x02, 0x12, 0x0A, 0x06, 0x0A, 0x12, 0x00},       /* "k"*/
{0x0C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00},       /* "l"*/
{0x00, 0x00, 0x00, 0x16, 0x2A, 0x2A, 0x2A, 0x00},       /* "m"*/
{0x00, 0x00, 0x1A, 0x26, 0x22, 0x22, 0x22, 0x00},       /* "n"*/
{0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x1C, 0x00},       /* "o"*/
{0x00, 0x00, 0x1E, 0x22, 0x1E, 0x02, 0x02, 0x00},       /* "p"*/
{0x00, 0x00, 0x3C, 0x22, 0x3C, 0x20, 0x20, 0x00},       /* "q"*/
{0x00, 0x00, 0x00, 0x1A, 0x06, 0x02, 0x02, 0x00},       /* "r"*/
{0x00, 0x00, 0x1C, 0x02, 0x1C, 0x20, 0x1E, 0x00},       /* "s"*/
{0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x30, 0x00},       /* "t"*/
{0x00, 0x00, 0x22, 0x22, 0x22, 0x32, 0x2C, 0x00},       /* "u"*/
{0x00, 0x00, 0x22, 0x22, 0x22, 0x14, 0x08, 0x00},       /* "v"*/
{0x00, 0x00, 0x22, 0x22, 0x2A, 0x2A, 0x14, 0x00},       /* "w"*/
{0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00},       /* "x"*/
{0x00, 0x00, 0x22, 0x22, 0x3C, 0x20, 0x1C, 0x00},       /* "y"*/
{0x00, 0x00, 0x3E, 0x10, 0x08, 0x04, 0x3E, 0x00},       /* "z"*/
{0x30, 0x08, 0x08, 0x04, 0x08, 0x08, 0x30, 0x00},       /* "{"*/
{0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00},       /* "|"*/
{0x06, 0x08, 0x08, 0x10, 0x08, 0x08, 0x06, 0x00},       /* "}"*/
{0x02, 0x15, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00},       /* "~"*/
{0x00, 0x08, 0x14, 0x22, 0x3E, 0x00, 0x00, 0x00}        /* ""*/
};
