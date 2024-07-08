/*
  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.

  This source code is the property of Michael S. D'Errico and is
  protected under international copyright laws.

  This program is free software: you can redistribute it and/or modify
  it under the terms of version 3 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Options for Contacting the Author:

    email name:   mikestoolbox
    email domain: pobox.com
    X/Twitter:    @mikestoolbox
    mail:         Michael D'Errico
                  10161 Park Run Drive, Suite 150
                  Las Vegas, NV 89145
*/

//+---------------------------------------------------------------------------
//  File:       String.cpp
//
//  Synopsis:   Character set conversion arrays
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

#define __ 255

static const uchar auc_HexValue[256] = {
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, __, __, __, __, __, __,
    __, 10, 11, 12, 13, 14, 15, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, 10, 11, 12, 13, 14, 15, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __
};

static const uchar auc_Base64Decode[256] = {
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, 62, __, __, __, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, __, __, __, __, __, __,
    __,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, __, __, __, __, __,
    __, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
    __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __
};

#undef __

// table of lengths of UTF-8 byte sequences indexed by the first byte

static const uchar auc_UTF8Length[256] = {
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2, 2,    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,    5, 5, 5, 5, 6, 6, 7, 8
};

// a comparison function that should not be possible for the compiler to
// optimize to memcmp and thus lose its constant-time behavior

bool String::ByteCompare (const String& str_Other, uintsys& u_Matches,
                          uintsys& u_Mismatches) const
{
    uintsys u_Length1 = Length();
    uintsys u_Length2 = str_Other.Length();

    uintsys u_MinLength = Minimum (u_Length1, u_Length2);

    u_Mismatches = (u_Length1 - u_MinLength) + (u_Length2 - u_MinLength);
    u_Matches    = 0;

    const uchar* puc1 = PointerToFirstByte();
    const uchar* puc2 = str_Other.PointerToFirstByte();

    for (uintsys u=0; u<u_MinLength; ++u)
    {
        uchar u_XOR  = *puc1 ^ *puc2;
        bool  b_Same = (u_XOR == 0);

        u_Matches    += b_Same ? 1 : 0;
        u_Mismatches += b_Same ? 0 : 1;

        ++puc1;
        ++puc2;
    }

    return (u_Mismatches == 0);
}

static const String ConvertToHex (const String& str, const char* pz_HexChars)
{
    uintsys u_Length = str.Length();

    if (IsTopBitSet (u_Length))
    {
        throw Exception ("String::HEX: String too long");
    }

    String str_HEX;

    if (u_Length != 0)
    {
        const uchar* puc_Src  = str.PointerToFirstByte();
        uchar*       puc_Dest = str_HEX.Allocate (2*u_Length);

        for (uintsys u=0; u<u_Length; ++u)
        {
            uchar uc = *puc_Src++;

            *puc_Dest++ = pz_HexChars[uc >> 4];
            *puc_Dest++ = pz_HexChars[uc & 0x0F];
        }
    }

    return str_HEX;
}

const String String::Hex () const
{
    return ConvertToHex (*this, gpz_Hex);
}

const String String::HEX () const
{
    return ConvertToHex (*this, gpz_HEX);
}

void String::AND (const String& str)
{
    uintsys u_LengthThis = Length();

    if (u_LengthThis == 0)
    {
        return;
    }

    uintsys u_MinLength = Minimum (u_LengthThis, str.Length());
    uchar*  puc_This    = mem_.EditInPlace();

    if (u_MinLength != 0)
    {
        const uchar* puc_That = str.PointerToFirstByte();

        for (uintsys u=0; u<u_MinLength; ++u)
        {
            *puc_This++ &= *puc_That++;
        }
    }

    if (u_LengthThis > u_MinLength)
    {
        ZeroMemory (puc_This, u_LengthThis - u_MinLength);
    }
}

void String::OR (const String& str)
{
    uintsys u_LengthThis = Length();
    uintsys u_LengthThat = str.Length();

    if (u_LengthThis >= u_LengthThat)
    {
        if (u_LengthThat != 0)
        {
            uchar*       puc_This = mem_.EditInPlace();
            const uchar* puc_That = str.PointerToFirstByte();

            for (uintsys u=0; u<u_LengthThat; ++u)
            {
                *puc_This++ |= *puc_That++;
            }
        }
    }
    else
    {
        String str_Longer (str);

        str_Longer.OR (*this);

        Swap (str_Longer);
    }
}

void String::XOR (const String& str)
{
    uintsys u_LengthThis = Length();
    uintsys u_LengthThat = str.Length();

    if (u_LengthThis >= u_LengthThat)
    {
        if (u_LengthThat != 0)
        {
            uchar*       puc_This = mem_.EditInPlace();
            const uchar* puc_That = str.PointerToFirstByte();

            for (uintsys u=0; u<u_LengthThat; ++u)
            {
                *puc_This++ ^= *puc_That++;
            }
        }
    }
    else
    {
        String str_Longer (str);

        str_Longer.XOR (*this);

        Swap (str_Longer);
    }
}

void StringIter::MoveTo (const Index& offset)
{
    uintsys u_Length = p_End_ - p_Start_;
    uintsys u_Offset = 0;

    if (offset.Calculate (u_Length, u_Offset))
    {
        p_Char_ = p_Start_ + u_Offset;
    }
    else
    {
        p_Char_ = p_End_;
    }
}

bool StringIter::Extract (uintsys u_NumBytes, String& str, ParseError& error)
{
    if (u_NumBytes > Capacity())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    if (u_NumBytes == 0)
    {
        str.Clear();
    }
    else
    {
        std::memcpy (str.Allocate (u_NumBytes), p_Char_, u_NumBytes);

        p_Char_ += u_NumBytes;
    }

    return true;
}

bool StringIter::Extract (String& str)
{
    uintsys u_NumBytes = Capacity();

    if (u_NumBytes == 0)
    {
        str.Clear();
    }
    else
    {
        std::memcpy (str.Allocate (u_NumBytes), p_Char_, u_NumBytes);

        p_Char_ += u_NumBytes;
    }

    return true;
}

bool StringIter::SkipChar (char c, ParseError& error)
{
    uchar uc = 0;

    Backup<StringIter> backup (*this);

    if (!ExtractByte (uc, error))
    {
        return false;
    }

    if (uc == (uchar)c)
    {
        return true;
    }

    error.SetIllegalByteSequence();

    backup.Restore();

    return false;
}

static inline bool AsciiHexDigitToBinary (uchar& uc, ParseError& error)
{
    uchar uc_Nibble = auc_HexValue[uc];

    if (uc_Nibble > 0x0F)
    {
        error.SetIllegalByteSequence();

        return false;
    }

    uc = uc_Nibble;

    return true;
}

static inline bool AsciiDigitToBinary (uchar& uc, ParseError& error)
{
    uchar uc_Nibble = auc_HexValue[uc];

    if (uc_Nibble > 9)
    {
        error.SetIllegalByteSequence();

        return false;
    }

    uc = uc_Nibble;

    return true;
}

static inline bool AppendBase10Digit (uintsys u_Digit, uintsys& u_Number, 
                                      ParseError& error)
{
    uintsys u_Temp = (u_Number * 10) + u_Digit;

    if (u_Number != (u_Temp / 10))
    {
        error.SetNumericOverflow();

        return false;
    }

    u_Number = u_Temp;

    return true;
}

bool StringIter::ParseNumber (uintsys u_Digits, uintsys& u_Result,
                              ParseError& error)
{
    StringIter iter (*this);

    u_Result = 0;

    uchar uc = 0;

    while (u_Digits > 0)
    {
        if (!(iter.ExtractByte (uc, error) &&
             AsciiDigitToBinary (uc, error) &&
             AppendBase10Digit  (uc, u_Result, error)))
        {
            return false;
        }

        --u_Digits;
    }

    Swap (iter);

    return true;
}

bool StringIter::ParseNumber (uintsys& u_Result, ParseError& error)
{
    Backup<StringIter> backup (*this);

    u_Result = 0;

    uchar uc = 0;

    uintsys u_Digits = 0;

    while (ExtractByte (uc, error))
    {
        if (!AsciiDigitToBinary (uc, error))
        {
            --p_Char_;

            break;
        }

        if (!AppendBase10Digit (uc, u_Result, error))
        {
            backup.Restore();

            return false;
        }

        ++u_Digits;
    }

    if (u_Digits == 0)
    {
        backup.Restore();

        return false;
    }

    error.Clear();

    return true;
}

bool StringIter::ParseHexNibble (uchar& uc_Result, ParseError& error)
{
    Backup<StringIter> backup (*this);

    uchar uc_Nibble = 0;

    if (ExtractByte (uc_Nibble, error) &&
        AsciiHexDigitToBinary (uc_Nibble, error))
    {
        uc_Result = uc_Nibble;

        return true;
    }

    backup.Restore();

    return false;
}

bool StringIter::ParseHexByte (uchar& uc_Result, ParseError& error)
{
    Backup<StringIter> backup (*this);

    uchar uc_High = 0;
    uchar uc_Low  = 0;

    if (ExtractByte           (uc_High, error) &&
        AsciiHexDigitToBinary (uc_High, error) &&
        ExtractByte           (uc_Low,  error) &&
        AsciiHexDigitToBinary (uc_Low,  error))
    {
        uc_Result = (uc_High << 4) | uc_Low;

        return true;
    }

    backup.Restore();

    return false;
}

bool StringIter::ExtractUTF8Char1_ (uintsys& u_CodePoint, ParseError& error)
{
    Backup<StringIter> backup (*this);

    if (!ExtractUint8 (u_CodePoint, error))
    {
        return false;
    }

    if (IsBitSet (u_CodePoint, 7))
    {
        error.SetIllegalByteSequence();

        backup.Restore();

        return false;
    }

    return true;
}

bool StringIter::ExtractUTF8Char2_ (uintsys& u_CodePoint, ParseError& error)
{
    Backup<StringIter> backup (*this);

    if (!ExtractUint16 (u_CodePoint, error))
    {
        return false;
    }

    if ((u_CodePoint & 0xE0C0) != 0xC080)
    {
        error.SetIllegalByteSequence();

        backup.Restore();

        return false;
    }

    uintsys u2 = u_CodePoint & 0x1F00;
    uintsys u1 = u_CodePoint & 0x003F;

    u_CodePoint = (u2 >> 2) | u1;

    if (u_CodePoint < 0x80)
    {
        error.SetResultOutOfRange();

        backup.Restore();

        return false;
    }

    return true;
}

bool StringIter::ExtractUTF8Char3_ (uintsys& u_CodePoint, ParseError& error)
{
    Backup<StringIter> backup (*this);

    if (!ExtractUint24 (u_CodePoint, error))
    {
        return false;
    }

    if ((u_CodePoint & 0xF0C0C0) != 0xE08080)
    {
        error.SetIllegalByteSequence();

        backup.Restore();

        return false;
    }

    uintsys u3 = u_CodePoint & 0x0F0000;
    uintsys u2 = u_CodePoint & 0x003F00;
    uintsys u1 = u_CodePoint & 0x00003F;

    u_CodePoint = (u3 >> 4) | (u2 >> 2) | u1;

    if (u_CodePoint < 0x0800)
    {
        error.SetResultOutOfRange();

        backup.Restore();

        return false;
    }

    return true;
}

bool StringIter::ExtractUTF8Char4_ (uintsys& u_CodePoint, ParseError& error)
{
    Backup<StringIter> backup (*this);

    if (!ExtractUint32 (u_CodePoint, error))
    {
        return false;
    }

    if ((u_CodePoint & 0xF8C0C0C0) != 0xF0808080)
    {
        error.SetIllegalByteSequence();

        backup.Restore();

        return false;
    }

    uintsys u4 = u_CodePoint & 0x07000000;
    uintsys u3 = u_CodePoint & 0x003F0000;
    uintsys u2 = u_CodePoint & 0x00003F00;
    uintsys u1 = u_CodePoint & 0x0000003F;

    u_CodePoint = (u4 >> 6) | (u3 >> 4) | (u2 >> 2) | u1;

    if ((u_CodePoint > MAX_UNICODE_CODE_POINT) || (u_CodePoint < 0x010000))
    {
        error.SetResultOutOfRange();

        backup.Restore();

        return false;
    }

    return true;
}

bool StringIter::ParseUTF8Char (uintsys& u_CodePoint, ParseError& error)
{
    if (!IsValid())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    switch (auc_UTF8Length[*p_Char_])
    {
        case 1: return ExtractUTF8Char1_ (u_CodePoint, error);
        case 2: return ExtractUTF8Char2_ (u_CodePoint, error);
        case 3: return ExtractUTF8Char3_ (u_CodePoint, error);
        case 4: return ExtractUTF8Char4_ (u_CodePoint, error);
    }

    error.SetIllegalByteSequence();

    return false;
}

bool StringIter::ParseUTF16Char (uintsys& u_CodePoint, ParseError& error)
{
    StringIter iter (*this);

    if (!iter.ExtractUint16 (u_CodePoint, error))
    {
        return false;
    }

    // "low" surrogate can not be first

    if ((u_CodePoint >= 0xDC00) && (u_CodePoint <= 0xDFFF))
    {
        error.SetUnicodeSurrogate();

        return false;
    }

    if ((u_CodePoint >= 0xD800) && (u_CodePoint <= 0xDBFF))
    {
        // "high" surrogate must be followed by a "low" surrogate

        uintsys u_High = (u_CodePoint & 0x03FF) << 10;
        uintsys u_Low  = 0;

        if (!iter.ExtractUint16 (u_Low, error))
        {
            error.SetUnicodeSurrogate();

            return false;
        }

        if ((u_Low < 0xDC00) || (u_Low > 0xDFFF))
        {
            error.SetUnicodeSurrogate();

            return false;
        }

        u_Low &= 0x03FF;

        u_CodePoint = (u_High | u_Low) + 0x010000;
    }

    Swap (iter);

    return true;
}

bool StringIter::ParseUTF32Char (uintsys& u_CodePoint, ParseError& error)
{
    StringIter iter (*this);

    if (!iter.ExtractUint32 (u_CodePoint, error))
    {
        return false;
    }

    if (u_CodePoint > MAX_UNICODE_CODE_POINT)
    {
        error.SetResultOutOfRange();

        return false;
    }

    Swap (iter);

    return true;
}

void StringIter::Truncate (uintsys u_MaxLength)
{
    uintsys u_Capacity = Capacity();

    if (u_MaxLength < u_Capacity)
    {
        p_End_ -= (u_Capacity - u_MaxLength);
    }
}

StringIter& StringIter::operator-= (uintsys u_NumBytes)
{
    if (IsValid() && ((uintsys)(p_Char_ - p_Start_) >= u_NumBytes))
    {
        p_Char_ -= u_NumBytes;
    }
    else
    {
        p_Char_ = p_End_;
    }

    return *this;
}

static inline uintsys MakeLowBitMask (uintsys u_NumBits)
{
    uint32 u_Mask = 0xFFFFFFFF;

    return u_Mask >> (32 - u_NumBits);
}

static inline bool ExtractBytes (StringIter& iter, uintsys u_NumBytes,
                                 uintsys& u, ParseError& error)
{
    u = 0;

    switch (u_NumBytes)
    {
        case 0: return true;
        case 1: return iter.ExtractUint8  (u, error);
        case 2: return iter.ExtractUint16 (u, error);
        case 3: return iter.ExtractUint24 (u, error);
        case 4: return iter.ExtractUint32 (u, error);
    }

    error.SetNumericOverflow();

    return false;
}

bool StringBitIter::ExtractBitsBigEndian_ (uintsys u_NumBits, uintsys& u,
                                           ParseError& error)
{
    u = 0;

    if (u_NumBits == 0)
    {
        return true;
    }

    uintsys u_BitsNeeded   = u_NumBits + u_BitOffset_;
    uintsys u_BytesNeeded  = (u_BitsNeeded + 7) / 8;
    uintsys u_NewBitOffset = u_BitsNeeded % 8;

    if (iter_.Capacity() < u_BytesNeeded)
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    StringIter temp (iter_);

    if (!ExtractBytes (temp, u_BytesNeeded, u, error))
    {
        return false;
    }

    uintsys u_Shift = (u_NewBitOffset == 0) ? 0 : (8 - u_NewBitOffset);

    u = (u >> u_Shift) & MakeLowBitMask (u_NumBits);

    u_BitOffset_ = u_NewBitOffset;

    if (u_NewBitOffset == 0)
    {
        iter_.Swap (temp);
    }
    else
    {
        iter_ += (u_BytesNeeded - 1);
    }

    return true;
}

static inline uintsys SwapEndianness (uintsys u_NumBytes, uintsys u)
{
    switch (u_NumBytes)
    {
        case 0: return 0;
        case 1: return u & 0xFF;
        case 2: return SwapEndian16Bit (u);
        case 3: return SwapEndian24Bit (u);
        case 4: return SwapEndian32Bit (u);
    }

    throw Exception ("SwapEndianness: can't swap more than 4 bytes");
}

bool StringBitIter::ExtractBitsLittleEndian_ (uintsys u_NumBits, uintsys& u,
                                              ParseError& error)
{
    u = 0;

    if (u_NumBits == 0)
    {
        return true;
    }

    uintsys u_BitsNeeded   = u_NumBits + u_BitOffset_;
    uintsys u_BytesNeeded  = (u_BitsNeeded + 7) / 8;
    uintsys u_NewBitOffset = u_BitsNeeded % 8;

    if (iter_.Capacity() < u_BytesNeeded)
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    StringIter temp (iter_);

    if (!ExtractBytes (temp, u_BytesNeeded, u, error))
    {
        return false;
    }

    u = SwapEndianness (u_BytesNeeded, u);

    u = (u >> u_BitOffset_) & MakeLowBitMask (u_NumBits);

    u_BitOffset_ = u_NewBitOffset;

    if (u_NewBitOffset == 0)
    {
        iter_.Swap (temp);
    }
    else
    {
        iter_ += (u_BytesNeeded - 1);
    }

    return true;
}

SubString& SubString::operator= (const String& str_Replace)
{
    uintsys u_ReferenceLength = str_Reference_.Length();
    uintsys u_ReplaceLength   = str_Replace.Length();

    if (u_Offset_ == u_ReferenceLength)
    {
        str_Reference_.Append (str_Replace);
    }
    else if (u_ReplaceLength == u_Length_)
    {
        uchar* ps_Dest = str_Reference_.mem_.EditInPlace() + u_Offset_;

        std::memcpy (ps_Dest, str_Replace.C(), u_ReplaceLength);
    }
    else if (u_ReplaceLength < u_Length_)
    {
        uchar* ps_Dest = str_Reference_.mem_.EditInPlace() + u_Offset_;

        std::memcpy  (ps_Dest, str_Replace.C(), u_ReplaceLength);
        std::memmove (ps_Dest + u_ReplaceLength, ps_Dest + u_Length_,
                      u_ReferenceLength - u_Offset_ - u_Length_);

        str_Reference_.mem_.EraseEnd (u_Length_ - u_ReplaceLength);
    }
    else
    {
        uintsys u_OriginalLength = u_ReferenceLength;
        uintsys u_ExtraChars     = u_ReplaceLength - u_Length_;

        uchar* ps_Dest = str_Reference_.mem_.Expand (0, u_ExtraChars)
                       + u_Offset_;

        std::memmove (ps_Dest + u_ReplaceLength, ps_Dest + u_Length_,
                      u_OriginalLength - u_Offset_ - u_Length_);
        std::memcpy  (ps_Dest, str_Replace.C(), u_ReplaceLength);
    }

    return *this;
}

template<typename INT>
static void SignedNumberToString (INT n, String& str)
{
    if (n == 0)
    {
        str.Append ('0');

        return;
    }

    str.Reserve (16);

    bool b_IsNegative = (n < 0);

    n = AbsoluteValue (n);

    while (n != 0)
    {
        char c = (n % 10) + '0';

        n /= 10;

        str.Append (c);
    }

    if (b_IsNegative)
    {
        str.Append ('-');
    }

    str.Reverse();
}

template<typename UINT>
static void NumberToString (UINT u, String& str)
{
    if (u == 0)
    {
        str.Append ('0');

        return;
    }

    str.Reserve (16);

    while (u != 0)
    {
        char c = (u % 10) + '0';

        u /= 10;

        str.Append (c);
    }

    str.Reverse();
}

String::String (int64 n)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    SignedNumberToString (n, *this);
}

String::String (int32 n)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    SignedNumberToString (n, *this);
}

String::String (int16 n)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    SignedNumberToString (n, *this);
}

String::String (uint64 u)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    NumberToString (u, *this);
}

String::String (uint32 u)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    NumberToString (u, *this);
}

String::String (uint16 u)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    NumberToString (u, *this);
}

String::String (const void* pv)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    char ac[32];

    ac[31] = 0;

    snprintf (ac, 31, "0x%p", pv);

    operator= (ac);
}

static inline bool IsInvalidBase64Char (uchar uc)
{
    return (uc >= 64);
}

const String String::Base64Decode () const
{
    String str_Result;

    uintsys u_Length = Length();
    uintsys u_Max    = ((u_Length + 3) / 4) * 3;

    str_Result.Reserve (u_Max);

    StringIter iter (*this);

    uintsys u_Current =  0;
    uintsys u_Shift   = 18;

    while (iter)
    {
        uchar uc = auc_Base64Decode[*iter++];

        if (IsInvalidBase64Char (uc))
        {
            continue;
        }

        if (u_Shift == 0)
        {
            u_Current |= uc;

            str_Result.AppendUint24 (u_Current);

            u_Current =  0;
            u_Shift   = 18;
        }
        else
        {
            u_Current |= (uc << u_Shift);

            u_Shift -= 6;
        }
    }

    if (u_Shift == 6)
    {
        str_Result.AppendUint8 (u_Current >> 16);
    }
    else if (u_Shift == 0)
    {
        str_Result.AppendUint16 (u_Current >> 8);
    }

    return str_Result;
}

static uintsys Base64ResultSize (uintsys u_BytesToEncode,
                                 uintsys& u_OutputLineLength,
                                 const char* pz_EOL)
{
    u_OutputLineLength &= (MAX_UINTSYS << 2); // must be multiple of 4 bytes

    u_OutputLineLength  = (u_OutputLineLength == 0) ? 76 : u_OutputLineLength;

    uintsys u_LengthEOL  = std::strlen (pz_EOL);
    uintsys u_FullGroups = u_BytesToEncode / 3;
    uintsys u_ExtraBytes = u_BytesToEncode - 3 * u_FullGroups;
    uintsys u_EncodeSize = u_FullGroups * 4 + (u_ExtraBytes ? 4 : 0);
    uintsys u_NumLines   = u_EncodeSize / u_OutputLineLength +
                         ((u_EncodeSize % u_OutputLineLength) ? 1 : 0);

    return u_EncodeSize + u_NumLines * u_LengthEOL;
}

static inline char GetBase64Char (uintsys u)
{
    return gpz_Base64[u & 0x3F];
}

static inline void AppendBase64Group24 (uintsys u_Group, String& str)
{
    uchar auc[4];

    auc[0] = GetBase64Char (u_Group >> 18);
    auc[1] = GetBase64Char (u_Group >> 12);
    auc[2] = GetBase64Char (u_Group >>  6);
    auc[3] = GetBase64Char (u_Group      );

    str.Append ((const uchar*)auc, 4);
}

static inline void AppendBase64Group16 (uintsys u_Group, String& str)
{
    uchar auc[4];

    auc[0] = GetBase64Char (u_Group >> 10);
    auc[1] = GetBase64Char (u_Group >>  4);
    auc[2] = GetBase64Char (u_Group <<  2);
    auc[3] = '=';

    str.Append ((const uchar*)auc, 4);
}

static inline void AppendBase64Group8 (uintsys u_Group, String& str)
{
    uchar auc[4];

    auc[0] = GetBase64Char (u_Group >> 2);
    auc[1] = GetBase64Char (u_Group << 4);
    auc[2] = '=';
    auc[3] = '=';

    str.Append ((const uchar*)auc, 4);
}

const String String::Base64Encode (uintsys u_LineLength,
                                   const char* pz_EOL) const
{
    if (IsEmpty())
    {
        return String(pz_EOL);
    }

    uintsys u_Size  = Base64ResultSize (Length(), u_LineLength, pz_EOL);
    uintsys u_Group = 0;
    uintsys u_Count = 0;

    String str_Result ((Preallocate (u_Size)));

    StringIter iter (*this);

    ParseError error;

    while (iter.ExtractUint24BE (u_Group, error))
    {
        AppendBase64Group24 (u_Group, str_Result);

        u_Count += 4;

        if (u_Count >= u_LineLength)
        {
            str_Result.Append (pz_EOL);

            u_Count = 0;
        }
    }

    if (iter.ExtractUint16BE (u_Group, error))
    {
        AppendBase64Group16 (u_Group, str_Result);

        u_Count += 4;
    }
    else if (iter.ExtractUint8 (u_Group, error))
    {
        AppendBase64Group8 (u_Group, str_Result);

        u_Count += 4;
    }

    if (u_Count != 0)
    {
        str_Result.Append (pz_EOL);
    }

    return str_Result;
}

intsys String::Compare (const String& str, bool b_CaseSensitive) const
{
    uintsys u_LengthThis = Length();
    uintsys u_LengthThat = str.Length();

    uintsys u_CharsToCompare = Minimum (u_LengthThis, u_LengthThat);

    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        b_CaseSensitive = false;
    }

    if (u_CharsToCompare != 0)
    {
        const uchar* p_This = PointerToFirstByte();
        const uchar* p_That = str.PointerToFirstByte();

        intsys n_Compare = StringCompare (p_This, p_That, u_CharsToCompare,
                                          b_CaseSensitive);

        if (n_Compare != 0)
        {
            return ((n_Compare > 0) ? 1 : -1);
        }
    }

    return ((u_LengthThis == u_LengthThat) ? 0 :
           ((u_LengthThis >  u_LengthThat) ? 1 : -1));
}

intsys String::Compare (char c, bool b_Case) const
{
    if (Length() == 0)
    {
        return -1;
    }

    if (b_IgnoreCase_)
    {
        b_Case = false;
    }

    uchar uc_First = *PointerToFirstByte();

    intsys n_Compare = mikestoolbox::ByteCompare (uc_First, (uchar)c, b_Case);

    if (n_Compare != 0)
    {
        return n_Compare;
    }

    return ((Length() == 1) ? 0 : 1);
}

void String::Complement ()
{
    uintsys u_Length = Length();

    if (u_Length > 0)
    {
        uchar* ps = PointerToFirstByteMod_();

        for (uintsys u=0; u<u_Length; ++u)
        {
            *ps++ ^= 0xFF;
        }
    }
}

uintsys String::CopyTo (char* ps_Buffer, uintsys u_Capacity) const
{
    if (u_Capacity <= 2)
    {
        ZeroMemory (ps_Buffer, u_Capacity);

        return 0;
    }

    u_Capacity -= 2;    // room for trailing NULLs

    uintsys u_Bytes = Length();

    if (u_Bytes > u_Capacity)
    {
        u_Bytes = u_Capacity;
    }

    std::memcpy (ps_Buffer, C(), u_Bytes);

    ps_Buffer[u_Bytes]   = '\0';
    ps_Buffer[u_Bytes+1] = '\0';

    return u_Bytes;
}

uintsys String::Count (uchar uc) const
{
    uintsys u_Count = 0;

    StringIter iter (*this);

    while (iter)
    {
        uchar uc2 = *iter++;

        if (uc2 == uc)
        {
            ++u_Count;
        }
    }

    return u_Count;
}

StringIter String::FindFirst (const String& str, const Index& offset,
                              bool b_Case) const
{
    uintsys u_Offset = 0;

    if (!offset.Calculate (Length(), u_Offset))
    {
        return StringIter();
    }

    uintsys u_SearchLength = str.Length();

    if (u_SearchLength == 0)
    {
        return StringIter (*this, u_Offset);
    }

    if (u_Offset + u_SearchLength > Length())
    {
        return StringIter();
    }

    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        b_Case = false;
    }

    uintsys u_NumTries = Length() - u_SearchLength - u_Offset + 1;

    const uchar* p_SearchString = str.PointerToFirstByte();
    const uchar* p_NextSearch   = PointerToFirstByte() + u_Offset;

    for (uintsys u=0; u<u_NumTries; ++u)
    {
        if (StringCompare (p_NextSearch++, p_SearchString, u_SearchLength,
                           b_Case) == 0)
        {
            return StringIter (*this, u_Offset + u);
        }
    }

    return StringIter();
}

StringIter String::FindFirst (char c, const Index& offset, bool b_Case) const
{
    uintsys u_Offset = 0;

    if (b_IgnoreCase_)
    {
        b_Case = false;
    }

    if (offset.Calculate (Length(), u_Offset))
    {
        const uchar* p_NextSearch = mem_.PointerToFirstByte() + u_Offset;

        uintsys u_Length = Length();

        for (uintsys u=u_Offset; u<u_Length; ++u)
        {
            if (mikestoolbox::ByteCompare (*p_NextSearch++,
                                           (uchar)c, b_Case) == 0)
            {
                return StringIter (*this, u);
            }
        }
    }

    return StringIter();
}

StringIter String::FindLast (const String& str, const Index& offset,
                             bool b_Case) const
{
    uintsys u_Offset = 0;

    if (!offset.Calculate (Length(), u_Offset))
    {
        return StringIter();
    }

    uintsys u_SearchLength = str.Length();

    if (u_SearchLength == 0)
    {
        return StringIter (*this, u_Offset);
    }

    if (u_SearchLength > Length())
    {
        return StringIter();
    }

    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        b_Case = false;
    }

    uintsys u_NumTries = u_Offset + 1;

    if (u_NumTries > Length() - u_SearchLength + 1)
    {
        u_NumTries = Length() - u_SearchLength + 1;
        u_Offset   = u_NumTries - 1;
    }

    const uchar* p_SearchString = str.PointerToFirstByte();
    const uchar* p_NextSearch   = PointerToFirstByte() + u_Offset;

    for (uintsys u=0; u<u_NumTries; ++u)
    {
        if (StringCompare (p_NextSearch--, p_SearchString, u_SearchLength,
                           b_Case) == 0)
        {
            return StringIter (*this, u_Offset - u);
        }
    }

    return StringIter();
}

StringIter String::FindLast (char c, const Index& offset, bool b_Case) const
{
    uintsys u_Offset = 0;

    if (b_IgnoreCase_)
    {
        b_Case = false;
    }

    if (offset.Calculate (Length(), u_Offset))
    {
        const uchar* p_NextSearch = mem_.PointerToFirstByte() + u_Offset;

        for (uintsys u=0; u<=u_Offset; ++u)
        {
            if (mikestoolbox::ByteCompare (*p_NextSearch--, (uchar)c,
                                           b_Case) == 0)
            {
                return StringIter (*this, u_Offset - u);
            }
        }
    }

    return StringIter();
}

void String::IncrementASCII ()
{
    uchar*  ps       = PointerToLastByteMod_();
    uintsys u_Length = Length();

    while (u_Length > 0)
    {
        uchar& uc = *ps;

        switch (uc)
        {
            case 'Z':   uc = 'A'; break;
            case 'z':   uc = 'a'; break;
            case '9':   uc = '0'; break;
            default:  ++uc;       return;
        }

        --u_Length;
        --ps;
    }
}

void String::IncrementBase10 ()
{
    uchar*  ps       = PointerToLastByteMod_();
    uintsys u_Length = Length();

    while (u_Length > 0)
    {
        uchar& uc = *ps;

        switch (uc)
        {
            case '9':   uc = '0'; break;
            default:  ++uc;       return;
        }

        --u_Length;
        --ps;
    }
}

void String::IncrementBase16 ()
{
    uchar*  ps       = PointerToLastByteMod_();
    uintsys u_Length = Length();

    while (u_Length > 0)
    {
        uchar& uc = *ps;

        switch (uc)
        {
            case 'F':
            case 'f':   uc = '0'; break;
            case '9':   uc = 'A'; return;
            default:  ++uc;       return;
        }

        --u_Length;
        --ps;
    }
}

void String::IncrementBase36 ()
{
    uchar*  ps       = PointerToLastByteMod_();
    uintsys u_Length = Length();

    while (u_Length > 0)
    {
        uchar& uc = *ps;

        switch (uc)
        {
            case 'Z':
            case 'z':   uc = '0'; break;
            case '9':   uc = 'A'; return;
            default:  ++uc;       return;
        }

        --u_Length;
        --ps;
    }
}

void String::IncrementBinary ()
{
    uchar*  ps       = PointerToLastByteMod_();
    uintsys u_Length = Length();

    while (u_Length > 0)
    {
        uchar& uc = *ps;

        ++uc;

        if (uc != 0)
        {
            return;
        }

        --u_Length;
        --ps;
    }
}

const StringList String::LineBreak () const
{
    StringList strl;

    if (IsEmpty())
    {
        return strl;
    }

    StringIter iter1 (*this);
    StringIter iter2 (iter1);

    while (iter1)
    {
        uchar uc (*iter1++);

        if (uc == '\n')
        {
            const uchar* p_Start = iter2.Pointer();
            const uchar* p_End   = iter1.Pointer();

            uintsys u_Length = p_End - p_Start;

            String str_Line (p_Start, u_Length);

            strl.Append (str_Line);

            iter2 = iter1;
        }
    }

    if (iter2)
    {
        String str_Line;

        iter2.Extract (str_Line);

        strl.Append (str_Line);
    }

    return strl;
}

bool String::Read (std::istream& is, uintsys u_MaxChars)
{
    Clear();

    if (u_MaxChars == 0)
    {
        u_MaxChars = ~u_MaxChars;
    }

    char c = 0;

    while (u_MaxChars--)
    {
        is.get (c);

        if (is.eof())
        {
            break;
        }

        if (!is.good())
        {
            return false;
        }

        Append (c);
    }

    return true;
}

bool String::ReadLine (std::istream& is, uintsys u_MaxChars)
{
    Clear();
    Reserve (60);

    if (u_MaxChars == 0)
    {
        u_MaxChars = ~u_MaxChars;
    }

    char c = 0;

    while (u_MaxChars--)
    {
        is.get (c);

        if (is.eof())
        {
            break;
        }

        if (!is.good())
        {
            return false;
        }

        Append (c);

        if (c == '\n')
        {
            break;
        }
    }

    return !IsEmpty();
}

String& String::Reverse ()
{
    uintsys u_Length = Length();

    if (u_Length > 1)
    {
        uchar* p1 = mem_.EditInPlace();
        uchar* p2 = p1 + u_Length - 1;

        uintsys u_LengthDiv2 = u_Length / 2;

        for (uintsys u=0; u<u_LengthDiv2; ++u)
        {
            uchar uc = *p1;

            *p1++ = *p2;

            *p2-- = uc;
        }
    }

    return *this;
}

static void AppendSanitary (uchar uc, String& str)
{
    if ((uc < 0x20) || (uc >= 0x7F))
    {
        str += "\\x";
        str.AppendHexByte (uc);

        return;
    }

    switch (uc)
    {
        case '\a':  str += "\\a";  break;
        case '\b':  str += "\\b";  break;
        case '\f':  str += "\\f";  break;
        case '\n':  str += "\\n";  break;
        case '\r':  str += "\\r";  break;
        case '\t':  str += "\\t";  break;
        case '\v':  str += "\\v";  break;
        case '\\':  str += "\\\\"; break;
        default:    str += uc;     break;
    }
}

const String String::Sanitize () const
{
    String str_Safe ((Preallocate (Length())));

    StringIter iter (*this);

    while (iter)
    {
        AppendSanitary (*iter++, str_Safe);
    }

    return str_Safe;
}

SubString String::Segment (const Index& offset, uintsys u_NumBytes)
{
    uintsys u_Length = Length();
    uintsys u_Offset = 0;

    if (offset.Calculate (u_Length, u_Offset))
    {
        uintsys u_End = u_Offset + u_NumBytes;

        if ((u_End < u_Offset) || (u_End > u_Length))
        {
            u_NumBytes = u_Length - u_Offset;
        }

        return SubString (*this, u_Offset, u_NumBytes);
    }

    if (!offset.IsNegative() && offset.Calculate (u_Length+1, u_Offset))
    {
        return SubString (*this, u_Length, 0);
    }

    throw Exception ("String::Segment: Index out of range");
}

const String String::Segment (const Index& offset, uintsys u_NumBytes) const
{
    String str;

    uintsys u_Length = Length();

    if (u_Length == 0)
    {
        return str;
    }

    uintsys u_Offset = 0;

    if (offset.Calculate (u_Length, u_Offset))
    {
        uintsys u_End = u_Offset + u_NumBytes;

        if ((u_End < u_Offset) || (u_End > u_Length))
        {
            u_NumBytes = u_Length - u_Offset;
        }

        if (u_NumBytes != 0)
        {
            uchar* ps_Dest = str.Allocate (u_NumBytes);

            std::memcpy (ps_Dest, C() + u_Offset, u_NumBytes);
        }
    }

    return str;
}

static const uchar ESCAPE_CHAR  = '\\';
static const uchar SINGLE_QUOTE = '\'';
static const uchar DOUBLE_QUOTE = '"';

const StringList String::ShellParse (ParseError& error) const
{
    StringList strl;
    String     str_Element;

    bool  b_Escaped = false;
    bool  b_InQuote = false;
    uchar uc_Quote  = DOUBLE_QUOTE;

    StringIter iter (*this);

    while (iter)
    {
        uchar uc = *iter++;

        if (b_Escaped)
        {
            b_Escaped = false;

            str_Element += uc;
        }
        else if (uc == ESCAPE_CHAR)
        {
            b_Escaped = true;
        }
        else if (b_InQuote)
        {
            if (uc == uc_Quote)
            {
                b_InQuote = false;
            }
            else
            {
                str_Element += uc;
            }
        }
        else if (std::isspace (uc))
        {
            strl.AppendNonEmpty (str_Element);

            str_Element.Clear();
        }
        else if ((uc == SINGLE_QUOTE) || (uc == DOUBLE_QUOTE))
        {
            b_InQuote = true;
            uc_Quote  = uc;
        }
        else
        {
            str_Element += uc;
        }
    }

    strl.AppendNonEmpty (str_Element);

    if (b_InQuote || b_Escaped)
    {
        strl.Clear();

        error.SetUnexpectedEndOfData();
    }

    return strl;
}

const StringList String::Split (const String& str_Pattern,
                                uintsys u_MaxStrings, bool b_Case) const
{
    if (str_Pattern.IsEmpty())
    {
        return Split (u_MaxStrings);
    }

    StringList strl;

    uintsys u_Length = Length();

    if (u_Length == 0)
    {
        return strl;
    }

    if (u_MaxStrings == 1)
    {
        strl.Append (*this);

        return strl;
    }

    if (u_MaxStrings == 0)
    {
        u_MaxStrings = ~u_MaxStrings;
    }

    intsys  n_Offset        =  0;
    intsys  n_MatchLocation = -1;
    uintsys u_PatternLength = str_Pattern.Length();

    while (strl.NumItems() < u_MaxStrings - 1)
    {
        StringIter iter = FindFirst (str_Pattern, n_Offset, b_Case);

        if (!iter)
        {
            break;
        }

        n_MatchLocation = iter.Offset();

        strl.Append (Segment (n_Offset, n_MatchLocation - n_Offset));

        n_Offset = n_MatchLocation + u_PatternLength;
    }

    if (n_Offset <= (intsys)u_Length)
    {
        strl.Append (Segment (n_Offset, u_Length - n_Offset));
    }

    return strl;
}

const StringList String::Split (uintsys u_MaxStrings) const
{
    if (u_MaxStrings == 1)
    {
        return StringList (*this);
    }

    if (u_MaxStrings == 0)
    {
        u_MaxStrings = ~u_MaxStrings;
    }

    StringList strl;

    uintsys u_Length = Length();
    uintsys u_Limit  = Minimum (u_MaxStrings - 1, u_Length);

    StringIter iter (*this);

    for (uintsys u=0; (iter && (u<u_Limit)); ++u)
    {
        String str (*iter++);

        strl.Append (str);
    }

    if (iter)
    {
        String str_Segment;

        iter.Extract (str_Segment);

        strl.Append (str_Segment);
    }

    return strl;
}

bool String::StartsWith (const String& str, bool b_Case) const
{
    uintsys u_PrefixLength = str.Length();

    if (Length() < u_PrefixLength)
    {
        return false;
    }

    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        b_Case = false;
    }

    const uchar* ps1 = PointerToFirstByte();
    const uchar* ps2 = str.PointerToFirstByte();

    return (StringCompare (ps1, ps2, u_PrefixLength, b_Case) == 0);
}

bool String::EndsWith (const String& str, bool b_Case) const
{
    uintsys u_ThisLength = Length();
    uintsys u_EndLength  = str.Length();

    if (u_ThisLength < u_EndLength)
    {
        return false;
    }

    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        b_Case = false;
    }

    const uchar* p_This = PointerToFirstByte();
    const uchar* p_End  = str.PointerToFirstByte();

    p_This += (u_ThisLength - u_EndLength);

    return (StringCompare (p_This, p_End, u_EndLength, b_Case) == 0);
}

static char ToEscapeChar (char c)
{
    switch (c)
    {
        case 'a':   return '\a';
        case 'b':   return '\b';
        case 'e':   return char(0x1B);
        case 'f':   return '\f';
        case 'n':   return '\n';
        case 'r':   return '\r';
        case 't':   return '\t';
        case 'v':   return '\v';
        case '0':   return '\0';
    }

    return c;
}

bool Interpolate (const String& str_Pattern, const StringList& strl_SubStrings,
                  String& str_Result)
{
    ParseError error;

    str_Result.Clear();

    uintsys u_MatchNum = 0;
    bool    b_Escaped  = false;

    StringIter iter (str_Pattern);

    while (iter)
    {
        uchar uc = *iter++;

        if (b_Escaped)
        {
            str_Result += ToEscapeChar (uc);

            b_Escaped = false;
        }
        else if (uc == '\\')
        {
            b_Escaped = true;
        }
        else if (uc == '$')
        {
            if (iter.SkipChar ('$', error))
            {
                str_Result += '$';
            }
            else if (iter.SkipChar ('&', error))
            {
                str_Result += strl_SubStrings[0];
            }
            else if (iter.ParseNumber (u_MatchNum, error) ||
                    (iter.SkipChar    ('{',        error) &&
                     iter.ParseNumber (u_MatchNum, error) &&
                     iter.SkipChar    ('}',        error)))
            {
                str_Result += strl_SubStrings[u_MatchNum];
            }
            else
            {
                str_Result.Clear();

                return false;
            }
        }
        else
        {
            str_Result += uc;
        }
    }

    if (b_Escaped)
    {
        str_Result += '\\';
    }

    return true;
}

intsys String::Replace (const PerlRegex& rex_Pattern,
                        const String& str_Replace,
                        StringList& strl_Substrings,
                        bool b_GlobalSearchReplace,
                        intsys n_Offset)
{
    strl_Substrings.Clear();

    if (!rex_Pattern)
    {
        return -1;
    }

    PerlRegexOptions options;
    PerlRegexMatches matches;

    options.StartingOffset (n_Offset);

    intsys n_MatchLoc = rex_Pattern.Match (*this, options, matches);

    strl_Substrings = matches.GetMatches();

    if (n_MatchLoc < 0)
    {
        return n_MatchLoc;
    }

    Backup<String> backup (*this);

    intsys n_MatchLength = matches.GetMatchLength (0);

    String str_Subst;

    if (!Interpolate (str_Replace, strl_Substrings, str_Subst))
    {
        return -1;
    }

    Segment (n_MatchLoc, n_MatchLength) = str_Subst;

    if (b_GlobalSearchReplace)
    {
        for (;;)
        {
            n_Offset = n_MatchLoc + str_Subst.Length();

            if (n_MatchLength == 0)
            {
                ++n_Offset;
            }

            options.StartingOffset (n_Offset);

            intsys n_NewMatchLoc = rex_Pattern.Match (*this, options, matches);

            if (n_NewMatchLoc < 0)
            {
                return n_MatchLoc;
            }
            else
            {
                strl_Substrings = matches.GetMatches();

                n_MatchLoc = n_NewMatchLoc;

                n_MatchLength = matches.GetMatchLength (0);

                if (!Interpolate (str_Replace, strl_Substrings, str_Subst))
                {
                    backup.Restore();

                    return -1;
                }

                Segment (n_MatchLoc, n_MatchLength) = str_Subst;
            }
        }
    }

    return n_MatchLoc;
}

const String String::Tail (uintsys u_NumChars) const
{
    uintsys u_Length = Length();

    if (u_NumChars > u_Length)
    {
        u_NumChars = u_Length;
    }

    String str;

    if (u_NumChars != 0)
    {
        str = Segment (u_Length - u_NumChars, u_NumChars);
    }

    return str;
}

String& String::operator= (int64 n)
{
    Clear();

    SignedNumberToString (n, *this);

    return *this;
}

String& String::operator= (int32 n)
{
    Clear();

    SignedNumberToString (n, *this);

    return *this;
}

String& String::operator= (uint64 u)
{
    Clear();

    NumberToString (u, *this);

    return *this;
}

String& String::operator= (uint32 u)
{
    Clear();

    NumberToString (u, *this);

    return *this;
}

} // namespace mikestoolbox

