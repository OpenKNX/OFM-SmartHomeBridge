#include "string.h"
#include "ISO8859_15ToUTF8.h"
#include "MemoryAllocator.h"
#ifndef  _GLIBCXX_USE_CHAR8_T
#define char8_t char
#endif
#ifndef iso8859_15_UTF8
#define DEFAULT_CHAR_MAPPING iso8859_15_UTF8
#endif

const char8_t* iso8859_15_UTF8[128] = {
    // 0x80–0x9F : C1 control codes (Unicode U+0080..U+009F)
    u8"\u0080", // 0x80 <control>
    u8"\u0081", // 0x81 <control>
    u8"\u0082", // 0x82 <control>
    u8"\u0083", // 0x83 <control>
    u8"\u0084", // 0x84 <control>
    u8"\u0085", // 0x85 <control> NEXT LINE (NEL)
    u8"\u0086", // 0x86 <control>
    u8"\u0087", // 0x87 <control>
    u8"\u0088", // 0x88 <control>
    u8"\u0089", // 0x89 <control>
    u8"\u008A", // 0x8A <control>
    u8"\u008B", // 0x8B <control>
    u8"\u008C", // 0x8C <control>
    u8"\u008D", // 0x8D <control>
    u8"\u008E", // 0x8E <control>
    u8"\u008F", // 0x8F <control>

    u8"\u0090", // 0x90 <control>
    u8"\u0091", // 0x91 <control>
    u8"\u0092", // 0x92 <control>
    u8"\u0093", // 0x93 <control>
    u8"\u0094", // 0x94 <control>
    u8"\u0095", // 0x95 <control>
    u8"\u0096", // 0x96 <control>
    u8"\u0097", // 0x97 <control>
    u8"\u0098", // 0x98 <control>
    u8"\u0099", // 0x99 <control>
    u8"\u009A", // 0x9A <control>
    u8"\u009B", // 0x9B <control>
    u8"\u009C", // 0x9C <control>
    u8"\u009D", // 0x9D <control>
    u8"\u009E", // 0x9E <control>
    u8"\u009F", // 0x9F <control>

    // 0xA0–0xFF : Latin-9 druckbare Zeichen
    u8"\u00A0", // 0xA0 NO-BREAK SPACE
    u8"\u00A1", // 0xA1 INVERTED EXCLAMATION MARK
    u8"\u00A2", // 0xA2 CENT SIGN
    u8"\u00A3", // 0xA3 POUND SIGN
    u8"\u20AC", // 0xA4 EURO SIGN
    u8"\u00A5", // 0xA5 YEN SIGN
    u8"\u0160", // 0xA6 S WITH CARON
    u8"\u00A7", // 0xA7 SECTION SIGN
    u8"\u0161", // 0xA8 s WITH CARON
    u8"\u00A9", // 0xA9 COPYRIGHT SIGN
    u8"\u00AA", // 0xAA FEMININE ORDINAL
    u8"\u00AB", // 0xAB LEFT-POINTING GUILLEMET
    u8"\u00AC", // 0xAC NOT SIGN
    u8"\u00AD", // 0xAD SOFT HYPHEN
    u8"\u00AE", // 0xAE REGISTERED SIGN
    u8"\u00AF", // 0xAF MACRON

    u8"\u00B0", // 0xB0 DEGREE SIGN
    u8"\u00B1", // 0xB1 PLUS-MINUS SIGN
    u8"\u00B2", // 0xB2 SUPERSCRIPT TWO
    u8"\u00B3", // 0xB3 SUPERSCRIPT THREE
    u8"\u017D", // 0xB4 Z WITH CARON
    u8"\u00B5", // 0xB5 MICRO SIGN
    u8"\u00B6", // 0xB6 PILCROW SIGN
    u8"\u00B7", // 0xB7 MIDDLE DOT
    u8"\u017E", // 0xB8 z WITH CARON
    u8"\u00B9", // 0xB9 SUPERSCRIPT ONE
    u8"\u00BA", // 0xBA MASCULINE ORDINAL
    u8"\u00BB", // 0xBB RIGHT-POINTING GUILLEMET
    u8"\u0152", // 0xBC Œ
    u8"\u0153", // 0xBD œ
    u8"\u0178", // 0xBE Ÿ
    u8"\u00BF", // 0xBF INVERTED QUESTION MARK

    u8"\u00C0", // 0xC0 À
    u8"\u00C1", // 0xC1 Á
    u8"\u00C2", // 0xC2 Â
    u8"\u00C3", // 0xC3 Ã
    u8"\u00C4", // 0xC4 Ä
    u8"\u00C5", // 0xC5 Å
    u8"\u00C6", // 0xC6 Æ
    u8"\u00C7", // 0xC7 Ç
    u8"\u00C8", // 0xC8 È
    u8"\u00C9", // 0xC9 É
    u8"\u00CA", // 0xCA Ê
    u8"\u00CB", // 0xCB Ë
    u8"\u00CC", // 0xCC Ì
    u8"\u00CD", // 0xCD Í
    u8"\u00CE", // 0xCE Î
    u8"\u00CF", // 0xCF Ï

    u8"\u00D0", // 0xD0 Ð
    u8"\u00D1", // 0xD1 Ñ
    u8"\u00D2", // 0xD2 Ò
    u8"\u00D3", // 0xD3 Ó
    u8"\u00D4", // 0xD4 Ô
    u8"\u00D5", // 0xD5 Õ
    u8"\u00D6", // 0xD6 Ö
    u8"\u00D7", // 0xD7 ×
    u8"\u00D8", // 0xD8 Ø
    u8"\u00D9", // 0xD9 Ù
    u8"\u00DA", // 0xDA Ú
    u8"\u00DB", // 0xDB Û
    u8"\u00DC", // 0xDC Ü
    u8"\u00DD", // 0xDD Ý
    u8"\u00DE", // 0xDE Þ
    u8"\u00DF", // 0xDF ß

    u8"\u00E0", // 0xE0 à
    u8"\u00E1", // 0xE1 á
    u8"\u00E2", // 0xE2 â
    u8"\u00E3", // 0xE3 ã
    u8"\u00E4", // 0xE4 ä
    u8"\u00E5", // 0xE5 å
    u8"\u00E6", // 0xE6 æ
    u8"\u00E7", // 0xE7 ç
    u8"\u00E8", // 0xE8 è
    u8"\u00E9", // 0xE9 é
    u8"\u00EA", // 0xEA ê
    u8"\u00EB", // 0xEB ë
    u8"\u00EC", // 0xEC ì
    u8"\u00ED", // 0xED í
    u8"\u00EE", // 0xEE î
    u8"\u00EF", // 0xEF ï

    u8"\u00F0", // 0xF0 ð
    u8"\u00F1", // 0xF1 ñ
    u8"\u00F2", // 0xF2 ò
    u8"\u00F3", // 0xF3 ó
    u8"\u00F4", // 0xF4 ô
    u8"\u00F5", // 0xF5 õ
    u8"\u00F6", // 0xF6 ö
    u8"\u00F7", // 0xF7 ÷
    u8"\u00F8", // 0xF8 ø
    u8"\u00F9", // 0xF9 ù
    u8"\u00FA", // 0xFA ú
    u8"\u00FB", // 0xFB û
    u8"\u00FC", // 0xFC ü
    u8"\u00FD", // 0xFD ý
    u8"\u00FE", // 0xFE þ
    u8"\u00FF"  // 0xFF ÿ
};

const char8_t* iso8859_1_UTF8[128] = {
    // 0x80–0x9F : C1 control codes (Unicode U+0080..U+009F)
    u8"\u0080", // 0x80 <control>
    u8"\u0081", // 0x81 <control>
    u8"\u0082", // 0x82 <control>
    u8"\u0083", // 0x83 <control>
    u8"\u0084", // 0x84 <control>
    u8"\u0085", // 0x85 <control> NEL
    u8"\u0086", // 0x86 <control>
    u8"\u0087", // 0x87 <control>
    u8"\u0088", // 0x88 <control>
    u8"\u0089", // 0x89 <control>
    u8"\u008A", // 0x8A <control>
    u8"\u008B", // 0x8B <control>
    u8"\u008C", // 0x8C <control>
    u8"\u008D", // 0x8D <control>
    u8"\u008E", // 0x8E <control>
    u8"\u008F", // 0x8F <control>

    u8"\u0090", // 0x90 <control>
    u8"\u0091", // 0x91 <control>
    u8"\u0092", // 0x92 <control>
    u8"\u0093", // 0x93 <control>
    u8"\u0094", // 0x94 <control>
    u8"\u0095", // 0x95 <control>
    u8"\u0096", // 0x96 <control>
    u8"\u0097", // 0x97 <control>
    u8"\u0098", // 0x98 <control>
    u8"\u0099", // 0x99 <control>
    u8"\u009A", // 0x9A <control>
    u8"\u009B", // 0x9B <control>
    u8"\u009C", // 0x9C <control>
    u8"\u009D", // 0x9D <control>
    u8"\u009E", // 0x9E <control>
    u8"\u009F", // 0x9F <control>

    // 0xA0–0xFF : Latin-1 druckbare Zeichen
    u8"\u00A0", // 0xA0 NO-BREAK SPACE
    u8"\u00A1", // 0xA1 ¡
    u8"\u00A2", // 0xA2 ¢
    u8"\u00A3", // 0xA3 £
    u8"\u00A4", // 0xA4 ¤ CURRENCY SIGN
    u8"\u00A5", // 0xA5 ¥
    u8"\u00A6", // 0xA6 ¦ BROKEN BAR
    u8"\u00A7", // 0xA7 §
    u8"\u00A8", // 0xA8 ¨ DIAERESIS
    u8"\u00A9", // 0xA9 ©
    u8"\u00AA", // 0xAA ª
    u8"\u00AB", // 0xAB «
    u8"\u00AC", // 0xAC ¬
    u8"\u00AD", // 0xAD SOFT HYPHEN
    u8"\u00AE", // 0xAE ®
    u8"\u00AF", // 0xAF ¯

    u8"\u00B0", // 0xB0 °
    u8"\u00B1", // 0xB1 ±
    u8"\u00B2", // 0xB2 ²
    u8"\u00B3", // 0xB3 ³
    u8"\u00B4", // 0xB4 ´ ACUTE
    u8"\u00B5", // 0xB5 µ
    u8"\u00B6", // 0xB6 ¶
    u8"\u00B7", // 0xB7 ·
    u8"\u00B8", // 0xB8 ¸ CEDILLA
    u8"\u00B9", // 0xB9 ¹
    u8"\u00BA", // 0xBA º
    u8"\u00BB", // 0xBB »
    u8"\u00BC", // 0xBC ¼
    u8"\u00BD", // 0xBD ½
    u8"\u00BE", // 0xBE ¾
    u8"\u00BF", // 0xBF ¿

    // 0xC0–0xFF (gleich wie Latin-9 ab hier)
    u8"\u00C0", // À
    u8"\u00C1", // Á
    u8"\u00C2", // Â
    u8"\u00C3", // Ã
    u8"\u00C4", // Ä
    u8"\u00C5", // Å
    u8"\u00C6", // Æ
    u8"\u00C7", // Ç
    u8"\u00C8", // È
    u8"\u00C9", // É
    u8"\u00CA", // Ê
    u8"\u00CB", // Ë
    u8"\u00CC", // Ì
    u8"\u00CD", // Í
    u8"\u00CE", // Î
    u8"\u00CF", // Ï

    u8"\u00D0", // Ð
    u8"\u00D1", // Ñ
    u8"\u00D2", // Ò
    u8"\u00D3", // Ó
    u8"\u00D4", // Ô
    u8"\u00D5", // Õ
    u8"\u00D6", // Ö
    u8"\u00D7", // ×
    u8"\u00D8", // Ø
    u8"\u00D9", // Ù
    u8"\u00DA", // Ú
    u8"\u00DB", // Û
    u8"\u00DC", // Ü
    u8"\u00DD", // Ý
    u8"\u00DE", // Þ
    u8"\u00DF", // ß

    u8"\u00E0", // à
    u8"\u00E1", // á
    u8"\u00E2", // â
    u8"\u00E3", // ã
    u8"\u00E4", // ä
    u8"\u00E5", // å
    u8"\u00E6", // æ
    u8"\u00E7", // ç
    u8"\u00E8", // è
    u8"\u00E9", // é
    u8"\u00EA", // ê
    u8"\u00EB", // ë
    u8"\u00EC", // ì
    u8"\u00ED", // í
    u8"\u00EE", // î
    u8"\u00EF", // ï

    u8"\u00F0", // ð
    u8"\u00F1", // ñ
    u8"\u00F2", // ò
    u8"\u00F3", // ó
    u8"\u00F4", // ô
    u8"\u00F5", // õ
    u8"\u00F6", // ö
    u8"\u00F7", // ÷
    u8"\u00F8", // ø
    u8"\u00F9", // ù
    u8"\u00FA", // ú
    u8"\u00FB", // û
    u8"\u00FC", // ü
    u8"\u00FD", // ý
    u8"\u00FE", // þ
    u8"\u00FF"  // ÿ
};


const char8_t* cp1252_UTF8[128] = {
    // 0x80–0x9F : zusätzliche druckbare Zeichen in CP1252
    u8"\u20AC", // 0x80 € EURO SIGN
    u8"\u0081", // 0x81 <control> (UNDEFINED in CP1252)
    u8"\u201A", // 0x82 ‚ SINGLE LOW-9 QUOTATION MARK
    u8"\u0192", // 0x83 ƒ LATIN SMALL LETTER F WITH HOOK
    u8"\u201E", // 0x84 „ DOUBLE LOW-9 QUOTATION MARK
    u8"\u2026", // 0x85 … HORIZONTAL ELLIPSIS
    u8"\u2020", // 0x86 † DAGGER
    u8"\u2021", // 0x87 ‡ DOUBLE DAGGER
    u8"\u02C6", // 0x88 ˆ MODIFIER LETTER CIRCUMFLEX
    u8"\u2030", // 0x89 ‰ PER MILLE SIGN
    u8"\u0160", // 0x8A Š LATIN CAPITAL LETTER S WITH CARON
    u8"\u2039", // 0x8B ‹ SINGLE LEFT-POINTING ANGLE QUOTE
    u8"\u0152", // 0x8C Œ LATIN CAPITAL LIGATURE OE
    u8"\u008D", // 0x8D <control> (UNDEFINED in CP1252)
    u8"\u017D", // 0x8E Ž LATIN CAPITAL LETTER Z WITH CARON
    u8"\u008F", // 0x8F <control> (UNDEFINED in CP1252)

    u8"\u0090", // 0x90 <control> (UNDEFINED in CP1252)
    u8"\u2018", // 0x91 ‘ LEFT SINGLE QUOTATION MARK
    u8"\u2019", // 0x92 ’ RIGHT SINGLE QUOTATION MARK
    u8"\u201C", // 0x93 “ LEFT DOUBLE QUOTATION MARK
    u8"\u201D", // 0x94 ” RIGHT DOUBLE QUOTATION MARK
    u8"\u2022", // 0x95 • BULLET
    u8"\u2013", // 0x96 – EN DASH
    u8"\u2014", // 0x97 — EM DASH
    u8"\u02DC", // 0x98 ˜ SMALL TILDE
    u8"\u2122", // 0x99 ™ TRADE MARK SIGN
    u8"\u0161", // 0x9A š LATIN SMALL LETTER S WITH CARON
    u8"\u203A", // 0x9B › SINGLE RIGHT-POINTING ANGLE QUOTE
    u8"\u0153", // 0x9C œ LATIN SMALL LIGATURE OE
    u8"\u009D", // 0x9D <control> (UNDEFINED in CP1252)
    u8"\u017E", // 0x9E ž LATIN SMALL LETTER Z WITH CARON
    u8"\u0178", // 0x9F Ÿ LATIN CAPITAL LETTER Y WITH DIAERESIS

    // 0xA0–0xFF : identisch zu ISO-8859-1
    u8"\u00A0", // 0xA0 NO-BREAK SPACE
    u8"\u00A1", // 0xA1 ¡
    u8"\u00A2", // 0xA2 ¢
    u8"\u00A3", // 0xA3 £
    u8"\u00A4", // 0xA4 ¤
    u8"\u00A5", // 0xA5 ¥
    u8"\u00A6", // 0xA6 ¦
    u8"\u00A7", // 0xA7 §
    u8"\u00A8", // 0xA8 ¨
    u8"\u00A9", // 0xA9 ©
    u8"\u00AA", // 0xAA ª
    u8"\u00AB", // 0xAB «
    u8"\u00AC", // 0xAC ¬
    u8"\u00AD", // 0xAD SOFT HYPHEN
    u8"\u00AE", // 0xAE ®
    u8"\u00AF", // 0xAF ¯

    u8"\u00B0", // 0xB0 °
    u8"\u00B1", // 0xB1 ±
    u8"\u00B2", // 0xB2 ²
    u8"\u00B3", // 0xB3 ³
    u8"\u00B4", // 0xB4 ´
    u8"\u00B5", // 0xB5 µ
    u8"\u00B6", // 0xB6 ¶
    u8"\u00B7", // 0xB7 ·
    u8"\u00B8", // 0xB8 ¸
    u8"\u00B9", // 0xB9 ¹
    u8"\u00BA", // 0xBA º
    u8"\u00BB", // 0xBB »
    u8"\u00BC", // 0xBC ¼
    u8"\u00BD", // 0xBD ½
    u8"\u00BE", // 0xBE ¾
    u8"\u00BF", // 0xBF ¿

    u8"\u00C0", // 0xC0 À
    u8"\u00C1", // 0xC1 Á
    u8"\u00C2", // 0xC2 Â
    u8"\u00C3", // 0xC3 Ã
    u8"\u00C4", // 0xC4 Ä
    u8"\u00C5", // 0xC5 Å
    u8"\u00C6", // 0xC6 Æ
    u8"\u00C7", // 0xC7 Ç
    u8"\u00C8", // 0xC8 È
    u8"\u00C9", // 0xC9 É
    u8"\u00CA", // 0xCA Ê
    u8"\u00CB", // 0xCB Ë
    u8"\u00CC", // 0xCC Ì
    u8"\u00CD", // 0xCD Í
    u8"\u00CE", // 0xCE Î
    u8"\u00CF", // 0xCF Ï

    u8"\u00D0", // 0xD0 Ð
    u8"\u00D1", // 0xD1 Ñ
    u8"\u00D2", // 0xD2 Ò
    u8"\u00D3", // 0xD3 Ó
    u8"\u00D4", // 0xD4 Ô
    u8"\u00D5", // 0xD5 Õ
    u8"\u00D6", // 0xD6 Ö
    u8"\u00D7", // 0xD7 ×
    u8"\u00D8", // 0xD8 Ø
    u8"\u00D9", // 0xD9 Ù
    u8"\u00DA", // 0xDA Ú
    u8"\u00DB", // 0xDB Û
    u8"\u00DC", // 0xDC Ü
    u8"\u00DD", // 0xDD Ý
    u8"\u00DE", // 0xDE Þ
    u8"\u00DF", // 0xDF ß

    u8"\u00E0", // 0xE0 à
    u8"\u00E1", // 0xE1 á
    u8"\u00E2", // 0xE2 â
    u8"\u00E3", // 0xE3 ã
    u8"\u00E4", // 0xE4 ä
    u8"\u00E5", // 0xE5 å
    u8"\u00E6", // 0xE6 æ
    u8"\u00E7", // 0xE7 ç
    u8"\u00E8", // 0xE8 è
    u8"\u00E9", // 0xE9 é
    u8"\u00EA", // 0xEA ê
    u8"\u00EB", // 0xEB ë
    u8"\u00EC", // 0xEC ì
    u8"\u00ED", // 0xED í
    u8"\u00EE", // 0xEE î
    u8"\u00EF", // 0xEF ï

    u8"\u00F0", // 0xF0 ð
    u8"\u00F1", // 0xF1 ñ
    u8"\u00F2", // 0xF2 ò
    u8"\u00F3", // 0xF3 ó
    u8"\u00F4", // 0xF4 ô
    u8"\u00F5", // 0xF5 õ
    u8"\u00F6", // 0xF6 ö
    u8"\u00F7", // 0xF7 ÷
    u8"\u00F8", // 0xF8 ø
    u8"\u00F9", // 0xF9 ù
    u8"\u00FA", // 0xFA ú
    u8"\u00FB", // 0xFB û
    u8"\u00FC", // 0xFC ü
    u8"\u00FD", // 0xFD ý
    u8"\u00FE", // 0xFE þ
    u8"\u00FF"  // 0xFF ÿ
};

const char8_t** defaultConverter = DEFAULT_CHAR_MAPPING;

const char* convertISO8859_15ToUTF8(const char* iso)
{
    int bufferlength = 0; 
    int i = 0;
    bool replacementNeeded = false;
    while(true)
    {
        char c = iso[i++];
        if (c >= 128)
        {
            bufferlength += strlen((const char*) defaultConverter[c - 128]);
            replacementNeeded = true;
        }
        else
        {
            bufferlength++;
            if (c == 0)
                break;
        }
    }
    if (!replacementNeeded)
        return iso;
    char* cUtf8 = (char*) HS_MALLOC(bufferlength);
    size_t bufferIndex = 0;
    i = 0;
    while(true)
    {
        char c = iso[i++];
        if (c >= 128)
        {
            const char* utf8 = (const char*) defaultConverter[c - 128];
            size_t utf8len = strlen(utf8);
            memcpy(cUtf8 + bufferIndex, utf8, utf8len);
            bufferIndex += utf8len;
        }
        else
        {
            cUtf8[bufferIndex++] = c; 
            if (c == 0)
                break;
        }
    }
    return cUtf8;
}

std::string convertISO8859_15ToUTF8_string(const char* c1252)
{
    const char* converted = convertISO8859_15ToUTF8(c1252);
    std::string result(converted);
    if (converted != c1252)
    {
        free((void*)converted);
    }
    return result;
}
