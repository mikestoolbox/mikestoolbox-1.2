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
//  File:       String.inl
//
//  Synopsis:   Inline function definitions for the String class
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline intsys ByteCompareCase (uchar uc1, uchar uc2)
{
    return ((uc1 == uc2) ?  0 :
           ((uc1 <  uc2) ? -1 : 1));
}

inline intsys ByteCompareNoCase (uchar uc1, uchar uc2)
{
    return ByteCompareCase (ByteToLower(uc1), ByteToLower(uc2));
}

inline intsys ByteCompare (uchar uc1, uchar uc2, bool b_CaseSensitive)
{
    return (b_CaseSensitive ? ByteCompareCase   (uc1, uc2)
                            : ByteCompareNoCase (uc1, uc2));
}

inline bool String::ByteCompareEqual (const String& str_Other) const
{
    uintsys u_Matches    = 0;
    uintsys u_Mismatches = 0;

    return ByteCompare (str_Other, u_Matches, u_Mismatches);
}

inline intsys StringCompare (const uchar* ps1, const uchar* ps2, uintsys u_NumChars, bool b_CaseSensitive)
{
    intsys n_Return = 0;

    if (b_CaseSensitive)
    {
        for (uintsys u=0; u<u_NumChars; ++u)
        {
            n_Return = ByteCompareCase (*ps1++, *ps2++);

            if (n_Return == 0)
            {
                continue;
            }

            break;
        }
    }
    else
    {
        for (uintsys u=0; u<u_NumChars; ++u)
        {
            n_Return = ByteCompareNoCase (*ps1++, *ps2++);

            if (n_Return == 0)
            {
                continue;
            }

            break;
        }
    }

    return n_Return;
}

inline intsys StringCompare (const char* ps1, const char* ps2, uintsys u_NumChars, bool b_CaseSensitive)
{
    return StringCompare ((const uchar*)ps1, (const uchar*)ps2, u_NumChars, b_CaseSensitive);
}

inline SubString::SubString (String& str_Reference, uintsys u_Offset, uintsys u_Length)
    : str_Reference_ (str_Reference)
    , u_Offset_      (u_Offset)
    , u_Length_      (u_Length)
{
    // bounds check and integer overflow check

    uintsys u_End = u_Offset + u_Length;

    if ((u_End < u_Offset) || (u_End > str_Reference.Length()))
    {
        throw Exception ("SubString: Index out of range");
    }
}

inline const char* SubString::Start () const
{
    const char* ps_Return = str_Reference_.C();

    ps_Return += u_Offset_;

    return ps_Return;
}

inline uintsys SubString::Length () const
{
    return u_Length_;
}

inline bool SubString::operator== (const String& str) const
{
    if (u_Length_ != str.Length())
    {
        return false;
    }

    if (u_Length_ == 0)
    {
        return true;
    }

    const char* ps1 = str.C();
    const char* ps2 = Start();

    bool b_CaseSensitive = (str_Reference_.IsCaseSensitive() && str.IsCaseSensitive());

    return (StringCompare (ps1, ps2, u_Length_, b_CaseSensitive) == 0);
}

inline bool SubString::operator== (const char* pz) const
{
    String str (pz);

    return operator== (str);
}

inline bool SubString::operator!= (const String& str) const
{
    return !operator== (str);
}

inline bool SubString::operator!= (const char* pz) const
{
    String str (pz);

    return !operator== (str);
}

inline SubString& SubString::operator= (const SubString& substr)
{
    String str_Replace (substr);

    return operator= (str_Replace);
}

inline SubString& SubString::operator= (const char* pz_Replace)
{
    String str_Replace (pz_Replace);

    return operator= (str_Replace);
}

inline String::String (const String& str)
    : mem_          (str.mem_)
    , b_IgnoreCase_ (str.b_IgnoreCase_)
{
    // nothing
}

inline String::String (const String& str1, const String& str2)
    : mem_          ()
    , b_IgnoreCase_ (str1.b_IgnoreCase_ || str2.b_IgnoreCase_)
{
    Append (str1, str2);
}

inline String::String (const String& str1, const String& str2, const String& str3)
    : mem_          ()
    , b_IgnoreCase_ (str1.b_IgnoreCase_ || str2.b_IgnoreCase_ || str3.b_IgnoreCase_)
{
    Append (str1, str2, str3);
}

inline String::String (const String& str1, const String& str2,
                       const String& str3, const String& str4)
    : mem_          ()
    , b_IgnoreCase_ (str1.b_IgnoreCase_ || str2.b_IgnoreCase_ || str3.b_IgnoreCase_ ||
                     str4.b_IgnoreCase_)
{
    Append (str1, str2, str3, str4);
}

inline String::String (const SubString& substr)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uintsys u_Length = substr.Length();

    std::memcpy (mem_.Allocate (u_Length), substr.Start(), u_Length);
}

inline String::String (const std::string& s)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uintsys u_Length = s.length();

    std::memcpy (mem_.Allocate (u_Length), s.data(), u_Length);
}

inline String::String (const StringIter& iter)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uintsys u_Length = iter.Capacity();

    if (u_Length > 0)
    {
        std::memcpy (mem_.Allocate (u_Length), iter.Pointer(), u_Length);
    }
}

inline String::String (const uchar* ps_Source, uintsys u_NumBytes)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (u_NumBytes);

    if (ps_Source != 0)
    {
        std::memcpy (ps_Dest, ps_Source, u_NumBytes);
    }
    else
    {
        ZeroMemory (ps_Dest, u_NumBytes);
    }
}

inline String::String (const char* ps_Source, uintsys u_NumBytes)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (u_NumBytes);

    if (ps_Source != 0)
    {
        std::memcpy (ps_Dest, ps_Source, u_NumBytes);
    }
    else
    {
        ZeroMemory (ps_Dest, u_NumBytes);
    }
}

inline String::String (const char* pz)
    : mem_          (pz)
    , b_IgnoreCase_ (false)
{
    // nothing
}

inline String::String (uchar uc, Repeat repeat)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    if (repeat > 0)
    {
        uchar* ps_Dest = mem_.Allocate (repeat);

        std::memset (ps_Dest, uc, repeat);
    }
}

inline String::String (uchar uc)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (1);

    *ps_Dest = uc;
}

inline String::String (uchar uc1, uchar uc2)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (2);

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
}

inline String::String (uchar uc1, uchar uc2, uchar uc3)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (3);

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
    ps_Dest[2] = uc3;
}

inline String::String (uchar uc1, uchar uc2, uchar uc3, uchar uc4)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (4);

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
    ps_Dest[2] = uc3;
    ps_Dest[3] = uc4;
}

inline String::String (char c, Repeat repeat)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    if (repeat > 0)
    {
        uchar* ps_Dest = mem_.Allocate (repeat);

        std::memset (ps_Dest, c, repeat);
    }
}

inline String::String (char c)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (1);

    ps_Dest[0] = (uchar)c;
}

inline String::String (char c1, char c2)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (2);

    ps_Dest[0] = (uchar)c1;
    ps_Dest[1] = (uchar)c2;
}

inline String::String (char c1, char c2, char c3)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (3);

    ps_Dest[0] = (uchar)c1;
    ps_Dest[1] = (uchar)c2;
    ps_Dest[2] = (uchar)c3;
}

inline String::String (char c1, char c2, char c3, char c4)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    uchar* ps_Dest = mem_.Allocate (4);

    ps_Dest[0] = (uchar)c1;
    ps_Dest[1] = (uchar)c2;
    ps_Dest[2] = (uchar)c3;
    ps_Dest[3] = (uchar)c4;
}

inline String::String (double d, DoubleFormat format)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    char ac[65];

    ac[64] = 0;

    format.Print (d, ac, 64);

    operator= (ac);
}

inline String::String (float f, DoubleFormat format)
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    char ac[65];

    ac[64] = 0;

    format.Print (f, ac, 64);

    operator= (ac);
}

inline String::String (Preallocate amount)
    : mem_          (amount)
    , b_IgnoreCase_ (false)
{
    // nothing
}

inline String::String ()
    : mem_          ()
    , b_IgnoreCase_ (false)
{
    // nothing
}

inline const char* String::PointerToFirstChar () const
{
    const char* p_Base = mem_.PointerToFirstChar();

    return p_Base ? p_Base : "\0\0";
}

inline const char* String::C () const
{
    return PointerToFirstChar();
}

inline const uchar* String::PointerToFirstByte () const
{
    return (const uchar*)PointerToFirstChar();
}

inline uintsys String::Length () const
{
    return mem_.Length();
}

inline bool String::IsEmpty () const
{
    return (mem_.Length() == 0);
}

inline uchar* String::Allocate (uintsys u_Size)
{
    return mem_.Allocate (u_Size);
}

inline void String::Append (const String& str)
{
    mem_.Append (str.mem_);
}

inline void String::Append (const String& str1, const String& str2)
{
    Reserve (Length() + str1.Length() + str2.Length());

    Append (str1);
    Append (str2);
}

inline void String::Append (const String& str1, const String& str2, const String& str3)
{
    Reserve (Length() + str1.Length() + str2.Length() + str3.Length());

    Append (str1);
    Append (str2);
    Append (str3);
}

inline void String::Append (const String& str1, const String& str2, const String& str3,
                            const String& str4)
{
    Reserve (Length() + str1.Length() + str2.Length() + str3.Length() + str4.Length());

    Append (str1);
    Append (str2);
    Append (str3);
    Append (str4);
}

inline void String::Append (const StringIter& iter)
{
    String str (iter);

    Append (str);
}

inline void String::Append (const StringList& strl)
{
    uintsys u_ListSize = strl.Size();

    if (u_ListSize == 0)
    {
        return;
    }

    Reserve (Length() + u_ListSize);

    StringListIter iter (strl);

    while (iter)
    {
        Append (*iter++);
    }
}

inline void String::Append (const char* pz)
{
    String str (pz);

    Append (str);
}

inline void String::Append (const void* p)
{
    String str (p);

    Append (str);
}

inline void String::Append (const uchar* puc, uintsys u_Length)
{
    if (u_Length != 0)
    {
        uintsys u_OriginalLength = Length();

        uchar* ps_Dest = mem_.Expand (0, u_Length);

        std::memcpy (ps_Dest + u_OriginalLength, puc, u_Length);
    }
}

inline void String::Append (const std::string& s)
{
    const char* ps_Source = s.data();
    uintsys     u_Length  = s.length();

    Append (ps_Source, u_Length);
}

inline void String::Append (const char* ps, uintsys u_Length)
{
    Append ((const uchar*)ps, u_Length);
}

inline void String::Append (uchar uc)
{
    Append (&uc, 1);
}

inline void String::Append (uchar uc, Repeat repeat)
{
    if (repeat != 0)
    {
        uintsys u_OriginalLength = Length();

        uchar* ps_Dest = mem_.Expand (0, repeat);

        std::memset (ps_Dest + u_OriginalLength, uc, repeat);
    }
}

inline void String::Append (uchar uc1, uchar uc2)
{
    uintsys u_OriginalLength = Length();

    uchar* ps_Dest = mem_.Expand (0, 2) + u_OriginalLength;

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
}

inline void String::Append (uchar uc1, uchar uc2, uchar uc3)
{
    uintsys u_OriginalLength = Length();

    uchar* ps_Dest = mem_.Expand (0, 3) + u_OriginalLength;

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
    ps_Dest[2] = uc3;
}

inline void String::Append (uchar uc1, uchar uc2, uchar uc3, uchar uc4)
{
    uintsys u_OriginalLength = Length();

    uchar* ps_Dest = mem_.Expand (0, 4) + u_OriginalLength;

    ps_Dest[0] = uc1;
    ps_Dest[1] = uc2;
    ps_Dest[2] = uc3;
    ps_Dest[3] = uc4;
}

inline void String::Append (char c)
{
    Append ((uchar)c);
}

inline void String::Append (char c, Repeat repeat)
{
    Append ((uchar)c, repeat);
}

inline void String::Append (char c1, char c2)
{
    Append ((uchar)c1, (uchar)c2);
}

inline void String::Append (char c1, char c2, char c3)
{
    Append ((uchar)c1, (uchar)c2, (uchar)c3);
}

inline void String::Append (char c1, char c2, char c3, char c4)
{
    Append ((uchar)c1, (uchar)c2, (uchar)c3, (uchar)c4);
}

inline void String::Append (uint16 u)
{
    Append (String (u));
}

inline void String::Append (uint32 u)
{
    Append (String (u));
}

inline void String::Append (uint64 u)
{
    Append (String (u));
}

inline void String::Append (int16 n)
{
    Append (String (n));
}

inline void String::Append (int32 n)
{
    Append (String (n));
}

inline void String::Append (int64 n)
{
    Append (String (n));
}

inline void String::AppendUint32 (uintsys u)
{
    uchar uc1 = (u >> 24) & 0xFF;
    uchar uc2 = (u >> 16) & 0xFF;
    uchar uc3 = (u >>  8) & 0xFF;
    uchar uc4 =  u        & 0xFF;

    Append (uc1, uc2, uc3, uc4);
}

inline void String::AppendUint24 (uintsys u)
{
    uchar uc1 = (u >> 16) & 0xFF;
    uchar uc2 = (u >>  8) & 0xFF;
    uchar uc3 =  u        & 0xFF;

    Append (uc1, uc2, uc3);
}

inline void String::AppendUint16 (uintsys u)
{
    uchar uc1 = (u >> 8) & 0xFF;
    uchar uc2 =  u       & 0xFF;

    Append (uc1, uc2);
}

inline void String::AppendUint8 (uintsys u)
{
    Append ((uchar) (u & 0xFF));
}

inline void String::AppendUTF8 (uintsys u)
{
    if (u > MAX_UNICODE_CODE_POINT)
    {
        throw Exception ("String::AppendUTF8: illegal character");
    }

    if (u < 0x80)
    {
        AppendUint8 (u);
    }
    else if (u < 0x0800)
    {
        AppendUint16 (((u << 2) & 0x1F00) |
                      ( u       & 0x003F) |
                                  0xC080);
    }
    else if (u < 0x010000)
    {
        AppendUint24 (((u << 4) & 0x0F0000) |
                      ((u << 2) & 0x003F00) |
                      ( u       & 0x00003F) |
                                  0xE08080);
    }
    else
    {
        AppendUint32 (((u << 6) & 0x07000000) |
                      ((u << 4) & 0x003F0000) |
                      ((u << 2) & 0x00003F00) |
                      ( u       & 0x0000003F) |
                                  0xF0808080);
    }
}

inline void String::AppendUTF16 (uintsys u)
{
    if (u > MAX_UNICODE_CODE_POINT)
    {
        throw Exception ("String::AppendUTF16: illegal character");
    }

    if (u <= 0xFFFF)
    {
        AppendUint16 (u);
    }
    else
    {
        // output two Unicode surrogates

        u -= 0x010000;

        uintsys u_Hi = 0xD800 | (u >> 10);
        uintsys u_Lo = 0xDC00 | (u & 0x3FF);

        AppendUint16 (u_Hi);
        AppendUint16 (u_Lo);
    }
}

inline void String::AppendUTF32 (uintsys u)
{
    if (u > MAX_UNICODE_CODE_POINT)
    {
        throw Exception ("String::AppendUTF32: illegal character");
    }

    AppendUint32 (u);
}

inline void String::AppendHexByte (uchar uc)
{
    Append (gpz_HEX[(uc >> 4) & 0x0F],
            gpz_HEX[ uc       & 0x0F]);
}

inline void String::AppendHexBytes (uchar uc1, uchar uc2)
{
    Append (gpz_HEX[(uc1 >> 4) & 0x0F],
            gpz_HEX[ uc1       & 0x0F],
            gpz_HEX[(uc2 >> 4) & 0x0F],
            gpz_HEX[ uc2       & 0x0F]);
}

inline double String::AsDouble () const
{
    if (IsEmpty())
    {
        return 0.0;
    }

    return std::strtod (PointerToFirstChar(), 0);
}

inline float String::AsFloat () const
{
    return (float)AsDouble();
}

inline intsys String::AsInt () const
{
    if (IsEmpty())
    {
        return 0;
    }

#ifdef HAVE_STD_STRTOLL
    return std::strtoll (PointerToFirstChar(), 0, 10);
#else
    return std::strtol (PointerToFirstChar(), 0, 10);
#endif
}

inline uintsys String::AsUint () const
{
    if (IsEmpty())
    {
        return 0UL;
    }

#ifdef HAVE_STD_STRTOULL
    return std::strtoull (PointerToFirstChar(), 0, 10);
#else
    return std::strtoul (PointerToFirstChar(), 0, 10);
#endif
}

inline const String String::Base64Encode () const
{
    return Base64Encode (MAX_UINTSYS, "");
}

inline const StringIter String::Begin () const
{
    return StringIter (*this);
}

inline char String::CharAt (const Index& index) const
{
    uintsys u_Index = 0;

    if (index.Calculate (Length(), u_Index))
    {
        return *(PointerToFirstChar() + u_Index);
    }

    return 0;
}

inline CharRef String::CharAt (const Index& index)
{
    uintsys u_Index = 0;

    if (index.Calculate (Length(), u_Index))
    {
        return (char*) (mem_.EditInPlace() + u_Index);
    }

    return 0;
}

inline uchar* String::PointerToFirstByteMod_ ()
{
    return mem_.EditInPlace();
}

inline uchar* String::PointerToLastByteMod_ ()
{
    uintsys u_Length = Length();

    u_Length = (u_Length > 0) ? (u_Length - 1) : 0;

    return PointerToFirstByteMod_() + u_Length;
}

inline void String::Clear ()
{
    mem_.Clear();
}

inline intsys String::Compare (const char* pz, bool b_CaseSensitive) const
{
    String str (pz);

    return Compare (str, b_CaseSensitive);
}

inline bool String::Contains (const String& str, const Index& offset, bool b_Case) const
{
    return (FindFirst (str, offset, b_Case));
}

inline bool String::Contains (const char* pz, const Index& offset, bool b_Case) const
{
    String str (pz);

    return Contains (str, offset, b_Case);
}

inline bool String::Contains (char c, const Index& offset, bool b_Case) const
{
    return (FindFirst (c, offset, b_Case));
}

inline uintsys String::Count (char c) const
{
    return Count ((uchar)c);
}

inline void String::Destroy ()
{
    mem_.Destroy();
}

inline const StringIter String::End () const
{
    return StringIter (*this, -1);
}

inline void String::Erase (const Index& offset, uintsys u_NumChars)
{
    mem_.Erase (offset, u_NumChars);
}

inline void String::EraseEnd (uintsys u_NumChars)
{
    mem_.EraseEnd (u_NumChars);
}

inline void String::EraseFront (uintsys u_NumChars)
{
    mem_.EraseFront (u_NumChars);
}

inline StringIter String::FindFirst (const char* pz, const Index& offset, bool b_Case) const
{
    String str (pz);

    return FindFirst (str, offset, b_Case);
}

inline StringIter String::FindLast (const char* pz, const Index& offset, bool b_Case) const
{
    String str (pz);

    return FindLast (str, offset, b_Case);
}

inline SubString String::Head (uintsys u_NumChars)
{
    if (u_NumChars > Length())
    {
        u_NumChars = Length();
    }

    return Segment (0, u_NumChars);
}

inline const String String::Head (uintsys u_NumChars) const
{
    if (u_NumChars > Length())
    {
        u_NumChars = Length();
    }

    return Segment (0, u_NumChars);
}

inline void String::IgnoreCase (bool b_Ignore)
{
    b_IgnoreCase_ = b_Ignore;
}

inline void String::Insert (const String& str, const Index& offset)
{
    Segment (offset, 0) = str;
}

inline void String::Insert (const char* pz, const Index& offset)
{
    String str (pz);

    Insert (str, offset);
}

inline void String::Insert (char c, const Index& offset)
{
    char pz[2];

    pz[0] = c;
    pz[1] = 0;

    String str (pz);

    Insert (str, offset);
}

inline bool String::IsCaseSensitive () const
{
    return !b_IgnoreCase_;
}

inline bool String::IsValidUTF8 () const
{
    ParseError error;
    uintsys    u = 0;

    StringIter iter (*this);

    while (iter.ParseUTF8Char (u, error))
    {
        // nothing
    }

    return !iter;
}

inline bool String::IsValidUTF16 () const
{
    ParseError error;
    uintsys    u = 0;

    StringIter iter (*this);

    while (iter.ParseUTF16Char (u, error))
    {
        // nothing
    }

    return !iter;
}

inline bool String::IsValidUTF32 () const
{
    ParseError error;
    uintsys    u = 0;

    StringIter iter (*this);

    while (iter.ParseUTF32Char (u, error))
    {
        // nothing
    }

    return !iter;
}

inline uchar String::FirstByte () const
{
    return *PointerToFirstByte();
}

inline char String::FirstChar () const
{
    return *PointerToFirstChar();
}

inline uchar String::LastByte () const
{
    uintsys u_Length = Length();

    if (u_Length > 0)
    {
        return *(PointerToFirstByte() + u_Length - 1);
    }

    return 0;
}

inline char String::LastChar () const
{
    return (char) LastByte();
}

inline const StringIter String::Match (const PerlRegex& rex_Pattern,
                                       const PerlRegexOptions& options,
                                       PerlRegexMatches& matches) const
{
    intsys n_Match = rex_Pattern.Match (*this, options, matches);

    if (n_Match >= 0)
    {
        return StringIter (*this, n_Match);
    }

    return StringIter();
}

inline const StringIter String::Match (const PerlRegex& rex_Pattern,
                                       const PerlRegexOptions& options) const
{
    PerlRegexMatches matches;

    intsys n_Match = rex_Pattern.Match (*this, options, matches);

    if (n_Match >= 0)
    {
        return StringIter (*this, n_Match);
    }

    return StringIter();
}

inline const StringIter String::Match (const PerlRegex& rex_Pattern,
                                       PerlRegexMatches& matches) const
{
    PerlRegexOptions options;

    intsys n_Match = rex_Pattern.Match (*this, options, matches);

    if (n_Match >= 0)
    {
        return StringIter (*this, n_Match);
    }

    return StringIter();
}

inline const StringIter String::Match (const PerlRegex& rex_Pattern) const
{
    PerlRegexOptions options;
    PerlRegexMatches matches;

    intsys n_Match = rex_Pattern.Match (*this, options, matches);

    if (n_Match >= 0)
    {
        return StringIter (*this, n_Match);
    }

    return StringIter();
}

inline const StringIter String::Match (const String& str_Pattern) const
{
    PerlRegex rex_Pattern (str_Pattern);

    return Match (rex_Pattern);
}

inline const StringIter String::Match (const char* pz_Pattern) const
{
    PerlRegex rex_Pattern (pz_Pattern);

    return Match (rex_Pattern);
}

inline void String::NOT ()
{
    Complement();
}

inline void String::NAND (const String& str)
{
    AND (str);
    NOT ();
}

inline void String::NOR (const String& str)
{
    OR (str);
    NOT();
}

inline void String::XNOR (const String& str)
{
    XOR (str);
    NOT ();
}

inline void String::PadEnd (uintsys u_Length, char c)
{
    uintsys u_CurrentLength = Length();

    if (u_Length > u_CurrentLength)
    {
        Append (c, Repeat (u_Length - u_CurrentLength));
    }
}

inline void String::PadFront (uintsys u_Length, char c)
{
    uintsys u_CurrentLength = Length();

    if (u_Length > u_CurrentLength)
    {
        Prepend (c, Repeat (u_Length - u_CurrentLength));
    }
}

inline void String::Prepend (const String& str)
{
    if (C() == str.C())
    {
        Append (str);

        return;
    }

    uintsys u_PrependLength = str.Length();

    if (u_PrependLength != 0)
    {
        uchar* ps_Dest = mem_.Expand (u_PrependLength, 0);

        std::memcpy (ps_Dest, str.C(), u_PrependLength);
    }
}

inline void String::Prepend (const char* pz)
{
    String str (pz);

    Prepend (str);
}

inline void String::Prepend (char c)
{
    uchar* ps_Dest = mem_.Expand (1, 0);

    *ps_Dest = (uchar)c;
}

inline void String::Prepend (char c, Repeat repeat)
{
    if (repeat != 0)
    {
        uchar* ps_Dest = mem_.Expand (repeat, 0);

        std::memset (ps_Dest, c, repeat);
    }
}

inline void String::Reserve (uintsys u_NumChars)
{
    mem_.Reserve (u_NumChars);
}

inline const StringList String::Split (const char* pz_Pattern, uintsys u_MaxStrings, bool b_Case) const
{
    String str_Pattern (pz_Pattern);

    return Split (str_Pattern, u_MaxStrings, b_Case);
}

inline const StringList String::Split (char c_SplitChar, uintsys u_MaxStrings, bool b_Case) const
{
    String str_Pattern (c_SplitChar);

    return Split (str_Pattern, u_MaxStrings, b_Case);
}

inline bool String::StartsWith (const char* pz, bool b_Case) const
{
    String str (pz);

    return StartsWith (str, b_Case);
}

inline bool String::StartsWith (char c, bool b_Case) const
{
    String str (c);

    return StartsWith (str, b_Case);
}

inline bool String::EndsWith (const char* pz, bool b_Case) const
{
    String str (pz);

    return EndsWith (str, b_Case);
}

inline bool String::EndsWith (char c, bool b_Case) const
{
    String str (c);

    return EndsWith (str, b_Case);
}

inline intsys String::Replace (const PerlRegex& rex_Pattern,
                                     const String& str_Replace,
                                     bool b_GlobalSearchReplace, intsys n_Offset)
{
    StringList strl_Matches;

    return Replace (rex_Pattern, str_Replace, strl_Matches,
                    b_GlobalSearchReplace, n_Offset);
}

inline intsys String::Replace (const PerlRegex& rex_Pattern, const char* pz_Replace,
                                     bool b_GlobalSearchReplace, intsys n_Offset)
{
    String str_Replace (pz_Replace);

    return Replace (rex_Pattern, str_Replace, b_GlobalSearchReplace, n_Offset);
}

inline intsys String::Replace (const PerlRegex& rex_Pattern, const char* pz_Replace,
                                     StringList& strl_Substrings, bool b_GlobalSearchReplace,
                                     intsys n_Offset)
{
    String str_Replace (pz_Replace);

    return Replace (rex_Pattern, str_Replace, strl_Substrings, b_GlobalSearchReplace, n_Offset);
}

inline intsys String::Replace (const String& str_Pattern, const String& str_Replace,
                               StringList& strl_Substrings, bool b_GlobalSearchReplace,
                               intsys n_Offset, bool b_Case)
{
    if (b_IgnoreCase_ || !str_Pattern.IsCaseSensitive())
    {
        b_Case = false;
    }

    PerlRegexOptions options;

    options.CaseSensitive (b_Case);

    PerlRegex rex_Pattern (str_Pattern.C(), options);

    return Replace (rex_Pattern, str_Replace, strl_Substrings, b_GlobalSearchReplace, n_Offset);
}

inline intsys String::Replace (const String& str_Pattern,
                                     const String& str_Replace,
                                     bool b_GlobalSearchReplace, intsys n_Offset, bool b_Case)
{
    StringList strl_Matches;

    return Replace (str_Pattern, str_Replace, strl_Matches, b_GlobalSearchReplace, n_Offset, b_Case);
}

inline intsys String::Replace (const char* pz_Pattern, const String& str_Replace,
                                     bool b_GlobalSearchReplace, intsys n_Offset,
                                     bool b_Case)
{
    String str_Pattern (pz_Pattern);

    return Replace (str_Pattern, str_Replace, b_GlobalSearchReplace, n_Offset, b_Case);
}

inline intsys String::Replace (const char* pz_Pattern, const String& str_Replace,
                                     StringList& strl_Substrings, bool b_GlobalSearchReplace,
                                     intsys n_Offset, bool b_Case)
{
    String str_Pattern (pz_Pattern);

    return Replace (str_Pattern, str_Replace, strl_Substrings, b_GlobalSearchReplace, n_Offset);
}

inline intsys String::Replace (const char* pz_Pattern, const char* pz_Replace,
                                     bool b_GlobalSearchReplace, intsys n_Offset,
                                     bool b_Case)
{
    String str_Pattern (pz_Pattern);
    String str_Replace (pz_Replace);

    return Replace (str_Pattern, str_Replace, b_GlobalSearchReplace, n_Offset, b_Case);
}

inline intsys String::Replace (const char* pz_Pattern, const char* pz_Replace,
                                     StringList& strl_Substrings, bool b_GlobalSearchReplace,
                                     intsys n_Offset, bool b_Case)
{
    String str_Pattern (pz_Pattern);
    String str_Replace (pz_Replace);

    return Replace (str_Pattern, str_Replace, strl_Substrings, b_GlobalSearchReplace, n_Offset);
}

inline void String::StripLeadingZeros ()
{
    while ((!IsEmpty()) && (FirstByte() == 0))
    {
        EraseFront();
    }
}

inline void String::Swap (String& str)
{
    mem_.Swap (str.mem_);

    bool b = b_IgnoreCase_;

    b_IgnoreCase_ = str.b_IgnoreCase_;

    str.b_IgnoreCase_ = b;
}

inline SubString String::Tail (uintsys u_NumChars)
{
    uintsys u_Length = Length();

    if (u_NumChars > u_Length)
    {
        u_NumChars = u_Length;
    }

    return Segment (u_Length - u_NumChars, u_NumChars);
}

inline void String::Truncate (uintsys u_MaxChars)
{
    uintsys u_Length = Length();

    if (u_Length > u_MaxChars)
    {
        EraseEnd (u_Length - u_MaxChars);
    }
}

inline ByteRef String::ByteAt (const Index& offset)
{
    // no implicit expansion

    uintsys u_Offset = 0;

    if (offset.Calculate (mem_.Length(), u_Offset))
    {
        return mem_.EditInPlace() + u_Offset;
    }

    return 0;
}

inline uchar String::ByteAt (const Index& offset) const
{
    uintsys u_Offset = 0;

    if (offset.Calculate (mem_.Length(), u_Offset))
    {
        return *(PointerToFirstByte() + u_Offset);
    }

    return 0;
}

inline bool String::Write (std::ostream& os, bool b_FlushOutput) const
{
    uintsys u_Length = Length();

    if (u_Length != 0)
    {
        os.write (PointerToFirstChar(), u_Length);

        if (!os.good())
        {
            return false;
        }

        if (b_FlushOutput)
        {
            os << std::flush;
        }
    }

    return true;
}

inline String& String::operator= (const String& str)
{
    mem_ = str.mem_;

    return *this;
}

inline String& String::operator= (const StringIter& iter)
{
    String str (iter);

    return operator= (str);
}

inline String& String::operator= (const SubString& substr)
{
    uintsys u_Length = substr.Length();
    uchar*  ps_Dest  = mem_.Allocate (u_Length);

    std::memcpy (ps_Dest, substr.Start(), u_Length);

    return *this;
}

inline String& String::operator= (const std::string& s)
{
    uintsys u_Length = s.length();
    uchar*  ps_Dest  = mem_.Allocate (u_Length);

    std::memcpy (ps_Dest, s.data(), u_Length);

    return *this;
}

inline String& String::operator= (const char* pz)
{
    mem_ = pz;

    return *this;
}

inline String& String::operator= (uchar uc)
{
    uchar* ps_Dest = mem_.Allocate (1);

    *ps_Dest = uc;

    return *this;
}

inline String& String::operator= (char c)
{
    return operator= ((uchar)c);
}

inline String& String::operator= (double d)
{
    DoubleFormat format;

    char ac[65];

    ac[64] = 0;

    format.Print (d, ac, 64);

    return operator= (ac);
}

inline String& String::operator= (float f)
{
    return operator= ((double)f);
}

inline String& String::operator= (int16 s)
{
    return operator= ((int32)s);
}

inline String& String::operator= (uint16 us)
{
    return operator= ((uint32)us);
}

inline String& String::operator+= (const String& str)
{
    Append (str);

    return *this;
}

inline String& String::operator+= (const StringIter& iter)
{
    Append (iter);

    return *this;
}

inline String& String::operator+= (const StringList& strl)
{
    Append (strl);

    return *this;
}

inline String& String::operator+= (const std::string& s)
{
    uintsys u_OriginalLength = mem_.Length();
    uintsys u_AppendLength   = s.length();

    if (u_AppendLength != 0)
    {
        uchar* ps_Dest = mem_.Expand (0, u_AppendLength);

        std::memcpy (ps_Dest + u_OriginalLength, s.data(), u_AppendLength);
    }

    return *this;
}

inline String& String::operator+= (const char* pz)
{
    Append (pz);

    return *this;
}

inline String& String::operator+= (const void* p)
{
    Append (p);

    return *this;
}

inline String& String::operator+= (uchar uc)
{
    Append (uc);

    return *this;
}

inline String& String::operator+= (char c)
{
    Append (c);

    return *this;
}

inline void String::Append (double d, DoubleFormat format)
{
    char ac[65];

    ac[64] = 0;

    format.Print (d, ac, 64);

    operator+= (ac);
}

inline void String::Append (float f, DoubleFormat format)
{
    Append ((double)f, format);
}

inline String& String::operator+= (double d)
{
    DoubleFormat format;

    Append (d, format);

    return *this;
}

inline String& String::operator+= (float f)
{
    return operator+= ((double)f);
}

inline String& String::operator+= (int64 n)
{
    String str (n);

    return operator+= (str);
}

inline String& String::operator+= (int32 n)
{
    String str (n);

    return operator+= (str);
}

inline String& String::operator+= (int16 s)
{
    return operator+= ((int32)s);
}

inline String& String::operator+= (uint64 u)
{
    String str (u);

    return operator+= (str);
}

inline String& String::operator+= (uint32 u)
{
    String str (u);

    return operator+= (str);
}

inline String& String::operator+= (uint16 u)
{
    return operator+= ((uint32)u);
}

inline const String String::operator+ (const String& str) const
{
    String str_Sum (*this, str);

    return str_Sum;
}

inline const String String::operator+ (const StringIter& iter) const
{
    String str_Sum (*this);

    str_Sum += iter;

    return str_Sum;
}

inline const String String::operator+ (const std::string& s) const
{
    String str_Sum (*this);

    str_Sum += s;

    return str_Sum;
}

inline const String String::operator+ (const char* pz) const
{
    String str_Sum (*this);

    str_Sum += pz;

    return str_Sum;
}

inline const String String::operator+ (const void* p) const
{
    String str_Sum (*this);

    str_Sum += p;

    return str_Sum;
}

inline const String String::operator+ (uchar uc) const
{
    String str_Sum (*this);

    str_Sum += uc;

    return str_Sum;
}

inline const String String::operator+ (char c) const
{
    String str_Sum (*this);

    str_Sum += c;

    return str_Sum;
}

inline const String String::operator+ (double d) const
{
    String str_Sum (*this);

    str_Sum += d;

    return str_Sum;
}

inline const String String::operator+ (float f) const
{
    String str_Sum (*this);

    str_Sum += f;

    return str_Sum;
}

inline const String String::operator+ (int64 n) const
{
    String str_Sum (*this);

    str_Sum += n;

    return str_Sum;
}

inline const String String::operator+ (int32 n) const
{
    String str_Sum (*this);

    str_Sum += n;

    return str_Sum;
}

inline const String String::operator+ (int16 n) const
{
    String str_Sum (*this);

    str_Sum += n;

    return str_Sum;
}

inline const String String::operator+ (uint64 u) const
{
    String str_Sum (*this);

    str_Sum += u;

    return str_Sum;
}

inline const String String::operator+ (uint32 u) const
{
    String str_Sum (*this);

    str_Sum += u;

    return str_Sum;
}

inline const String String::operator+ (uint16 u) const
{
    String str_Sum (*this);

    str_Sum += u;

    return str_Sum;
}

inline char String::operator[] (const Index& index) const
{
    return CharAt (index);
}

inline CharRef String::operator[] (const Index& index)
{
    return CharAt (index);
}

inline bool String::operator== (const String& str) const
{
    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        return (Compare (str) == 0);
    }

    return mem_ == str.mem_;
}

inline bool String::operator!= (const String& str) const
{
    return !operator== (str);
}

inline bool String::operator< (const String& str) const
{
    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        return (Compare (str) < 0);
    }

    return mem_ < str.mem_;
}

inline bool String::operator<= (const String& str) const
{
    return !(str < *this);
}

inline bool String::operator> (const String& str) const
{
    if (b_IgnoreCase_ || !str.IsCaseSensitive())
    {
        return (Compare (str) > 0);
    }

    return str.mem_ < mem_;
}

inline bool String::operator>= (const String& str) const
{
    return !operator< (str);
}

inline bool String::operator== (const char* pz) const
{
    return (Compare (pz) == 0);
}

inline bool String::operator!= (const char* pz) const
{
    return (Compare (pz) != 0);
}

inline bool String::operator< (const char* pz) const
{
    return (Compare (pz) < 0);
}

inline bool String::operator<= (const char* pz) const
{
    return (Compare (pz) <= 0);
}

inline bool String::operator> (const char* pz) const
{
    return (Compare (pz) > 0);
}

inline bool String::operator>= (const char* pz) const
{
    return (Compare (pz) >= 0);
}

inline bool String::operator== (char c) const
{
    return (Compare (c) == 0);
}

inline bool String::operator!= (char c) const
{
    return (Compare (c) != 0);
}

inline bool String::operator< (char c) const
{
    return (Compare (c) < 0);
}

inline bool String::operator<= (char c) const
{
    return (Compare (c) <= 0);
}

inline bool String::operator> (char c) const
{
    return (Compare (c) > 0);
}

inline bool String::operator>= (char c) const
{
    return (Compare (c) >= 0);
}

inline StringIter::StringIter (const String& str, const Index& offset)
    : str_               (str)
    , p_Start_           (str_.PointerToFirstByte())
    , p_End_             (p_Start_ + str_.Length())
    , p_Char_            (p_End_)
    , pmf_ExtractUint16_ (&StringIter::ExtractUint16BE)
    , pmf_ExtractUint24_ (&StringIter::ExtractUint24BE)
    , pmf_ExtractUint32_ (&StringIter::ExtractUint32BE)
{
    uintsys u_Offset = 0;

    if (offset.Calculate (str.Length(), u_Offset))
    {
        p_Char_ = p_Start_ + u_Offset;
    }
}

inline StringIter::StringIter (const String& str)
    : str_     (str)
    , p_Start_ (str_.PointerToFirstByte())
    , p_End_   (p_Start_ + str_.Length())
    , p_Char_  (p_Start_)
    , pmf_ExtractUint16_ (&StringIter::ExtractUint16BE)
    , pmf_ExtractUint24_ (&StringIter::ExtractUint24BE)
    , pmf_ExtractUint32_ (&StringIter::ExtractUint32BE)
{
    // nothing
}

inline StringIter::StringIter (const StringIter& iter, const uchar* ps, uintsys u_Length)
    : str_     (iter.str_)
    , p_Start_ (ps)
    , p_End_   (ps+u_Length)
    , p_Char_  (ps)
    , pmf_ExtractUint16_ (iter.pmf_ExtractUint16_)
    , pmf_ExtractUint24_ (iter.pmf_ExtractUint24_)
    , pmf_ExtractUint32_ (iter.pmf_ExtractUint32_)
{
    // nothing
}

inline StringIter::StringIter ()
    : str_     ()
    , p_Start_ (0)
    , p_End_   (0)
    , p_Char_  (0)
    , pmf_ExtractUint16_ (&StringIter::ExtractUint16BE)
    , pmf_ExtractUint24_ (&StringIter::ExtractUint24BE)
    , pmf_ExtractUint32_ (&StringIter::ExtractUint32BE)
{
    // nothing
}

inline void StringIter::Swap (StringIter& iter)
{
    using std::swap;

    swap (str_,               iter.str_);
    swap (p_Start_,           iter.p_Start_);
    swap (p_End_,             iter.p_End_);
    swap (p_Char_,            iter.p_Char_);
    swap (pmf_ExtractUint16_, iter.pmf_ExtractUint16_);
    swap (pmf_ExtractUint24_, iter.pmf_ExtractUint24_);
    swap (pmf_ExtractUint32_, iter.pmf_ExtractUint32_);
}

inline void StringIter::SetBigEndian ()
{
    pmf_ExtractUint16_ = &StringIter::ExtractUint16BE;
    pmf_ExtractUint24_ = &StringIter::ExtractUint24BE;
    pmf_ExtractUint32_ = &StringIter::ExtractUint32BE;
}

inline void StringIter::SetLittleEndian ()
{
    pmf_ExtractUint16_ = &StringIter::ExtractUint16LE;
    pmf_ExtractUint24_ = &StringIter::ExtractUint24LE;
    pmf_ExtractUint32_ = &StringIter::ExtractUint32LE;
}

inline bool StringIter::IsValid () const
{
    return ((p_Char_ < p_End_) && (p_Char_ >= p_Start_));
}

inline StringIter::operator bool () const
{
    return IsValid();
}

inline uintsys StringIter::Capacity () const
{
    return (IsValid() ? (p_End_ - p_Char_) : 0);
}

inline uintsys StringIter::Offset () const
{
    if (IsValid())
    {
        return p_Char_ - p_Start_;
    }

    throw Exception ("StringIter::Offset: pointing outside of data");
}

inline uintsys StringIter::Length () const
{
    return (IsValid() ? 1 : 0);
}

inline const uchar* StringIter::Pointer () const
{
    return (IsValid() ? p_Char_ : p_End_);
}

inline StringIter::operator String () const
{
    if (IsValid())
    {
        return String (p_Char_, p_End_ - p_Char_);
    }

    return String();
}

inline intsys StringIter::Match (const PerlRegex& regex, PerlRegexMatches& matches) const
{
    if (IsValid())
    {
        PerlRegexOptions options;

        options.AnchoredMatching();

        return regex.Match (p_Char_, Capacity(), options, matches);
    }

    return -1;
}

inline bool StringIter::ExtractByte (uchar& uc, ParseError& error)
{
    if (IsValid())
    {
        uc = *p_Char_++;

        return true;
    }

    error.SetUnexpectedEndOfData();

    return false;
}

inline bool StringIter::ExtractUint8 (uintsys& u, ParseError& error)
{
    if (IsValid())
    {
        u = *p_Char_++;

        return true;
    }

    error.SetUnexpectedEndOfData();

    return false;
}

inline bool StringIter::ExtractUint16BE (uintsys& u, ParseError& error)
{
    if (Capacity() < 2)
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    u  = (*p_Char_++) << 8;
    u |=  *p_Char_++;

    return true;
}

inline bool StringIter::ExtractUint24BE (uintsys& u, ParseError& error)
{
    if (Capacity() < 3)
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    u  = (*p_Char_++) << 16;
    u |= (*p_Char_++) <<  8;
    u |=  *p_Char_++;

    return true;
}

inline bool StringIter::ExtractUint32BE (uintsys& u, ParseError& error)
{
    if (Capacity() < 4)
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    uint32 u32 = 0;

    u32  = (*p_Char_++) << 24;
    u32 |= (*p_Char_++) << 16;
    u32 |= (*p_Char_++) <<  8;
    u32 |=  *p_Char_++;

    u = u32;

    return true;
}

inline bool StringIter::ExtractUint16LE (uintsys& u, ParseError& error)
{
    if (!ExtractUint16BE (u, error))
    {
        return false;
    }

    u = SwapEndian16Bit (u);

    return true;
}

inline bool StringIter::ExtractUint24LE (uintsys& u, ParseError& error)
{
    if (!ExtractUint24BE (u, error))
    {
        return false;
    }

    u = SwapEndian24Bit (u);

    return true;
}

inline bool StringIter::ExtractUint32LE (uintsys& u, ParseError& error)
{
    if (!ExtractUint32BE (u, error))
    {
        return false;
    }

    u = SwapEndian32Bit (u);

    return true;
}

inline bool StringIter::ExtractUint16 (uintsys& u, ParseError& error)
{
    return (this->*pmf_ExtractUint16_) (u, error);
}

inline bool StringIter::ExtractUint24 (uintsys& u, ParseError& error)
{
    return (this->*pmf_ExtractUint24_) (u, error);
}

inline bool StringIter::ExtractUint32 (uintsys& u, ParseError& error)
{
    return (this->*pmf_ExtractUint32_) (u, error);
}

inline StringIter& StringIter::operator++ ()
{
    p_Char_ += (IsValid() ? 1 : 0);

    return *this;
}

inline const StringIter StringIter::operator++ (int)
{
    StringIter iter (*this);

    operator++();

    return iter;
}

inline StringIter& StringIter::operator+= (uintsys u_NumBytes)
{
    if (u_NumBytes >= Capacity())
    {
        p_Char_ = p_End_;
    }
    else
    {
        p_Char_ += u_NumBytes;
    }

    return *this;
}

inline StringIter& StringIter::operator-- ()
{
    if ((p_Char_ > p_Start_) && (p_Char_ < p_End_))
    {
        --p_Char_;
    }

    return *this;
}

inline const StringIter StringIter::operator-- (int)
{
    StringIter iter (*this);

    operator--();

    return iter;
}

inline uchar StringIter::operator* () const
{
    return (IsValid() ? *p_Char_ : 0);
}

inline uchar StringIter::operator[] (uintsys u_Index) const
{
    if (u_Index >= Capacity())
    {
        throw Exception ("StringIter::operator[]: index out of range");
    }

    return p_Char_[u_Index];
}

inline StringIter& StringIter::operator= (const String& str)
{
    StringIter iter (str);

    return operator= (iter);
}

inline bool StringIter::operator== (const StringIter& iter) const
{
    return (IsValid() && iter.IsValid() && (p_Char_ == iter.p_Char_));
}

inline bool StringIter::operator!= (const StringIter& iter) const
{
    return !operator== (iter);
}

inline bool StringIter::operator== (const String& str) const
{
    uintsys u_Length = Capacity();

    if (u_Length != str.Length())
    {
        return false;
    }

    bool b_Case = str.IsCaseSensitive();

    return (StringCompare (p_Char_, str.PointerToFirstByte(), u_Length, b_Case) == 0);
}

inline bool StringIter::operator!= (const String& str) const
{
    return !operator== (str);
}

inline std::ostream& operator<< (std::ostream& os, const String& str)
{
    str.Write (os, false);

    return os;
}

inline std::ostream& operator<< (std::ostream& os, const StringIter& iter)
{
    if (iter)
    {
        os.write ((const char*)iter.Pointer(), iter.Capacity());
    }

    return os;
}

inline std::ostream& operator<< (std::ostream& os, const SubString& substr)
{
    uintsys u_Length = substr.Length();

    if (u_Length != 0)
    {
        os.write (substr.Start(), u_Length);
    }

    return os;
}

inline std::istream& operator>> (std::istream& is, String& str)
{
    char c;

    str.Clear();

    while (is.get (c))
    {
        if (std::isspace (c))
        {
            if (!str.IsEmpty())
            {
                break;
            }
        }
        else
        {
            str.Append (c);
        }
    }

    return is;
}

inline const String operator+ (const std::string& s, const String& str)
{
    String str_Sum (s);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (const uchar* pz, const String& str)
{
    String str_Sum (pz);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (const char* pz, const String& str)
{
    String str_Sum (pz);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (uchar uc, const String& str)
{
    String str_Sum (uc);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (char c, const String& str)
{
    String str_Sum (c);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (double d, const String& str)
{
    String str_Sum (d);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (float f, const String& str)
{
    String str_Sum (f);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (int64 n, const String& str)
{
    String str_Sum (n);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (int32 n, const String& str)
{
    String str_Sum (n);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (int16 n, const String& str)
{
    String str_Sum (n);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (uint64 u, const String& str)
{
    String str_Sum (u);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (uint32 u, const String& str)
{
    String str_Sum (u);

    str_Sum += str;

    return str_Sum;
}

inline const String operator+ (uint16 u, const String& str)
{
    String str_Sum (u);

    str_Sum += str;

    return str_Sum;
}

inline StringBitIter::StringBitIter (const String& str)
    : iter_            (str)
    , u_BitOffset_     (0)
    , pmf_ExtractBits_ (&StringBitIter::ExtractBitsBigEndian_)
{
    // nothing
}

inline void StringBitIter::SetBigEndian ()
{
    pmf_ExtractBits_ = &StringBitIter::ExtractBitsBigEndian_;
}

inline void StringBitIter::SetLittleEndian ()
{
    pmf_ExtractBits_ = &StringBitIter::ExtractBitsLittleEndian_;
}

inline bool StringBitIter::ExtractBits (uintsys u_NumBits, uintsys& u, ParseError& error)
{
    return (this->*pmf_ExtractBits_) (u_NumBits, u, error);
}

inline StringBitIter::operator bool () const
{
    return iter_.IsValid();
}

} // namespace mikestoolbox

namespace std {

template<>
inline void swap<mikestoolbox::String> (mikestoolbox::String& str1, mikestoolbox::String& str2)
{
    str1.Swap (str2);
}

} // namespace std

