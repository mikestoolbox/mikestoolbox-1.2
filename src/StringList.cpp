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
//  File:       StringList.cpp
//
//  Synopsis:   Implementation of StringList methods
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

StringList::StringList (const char* const * ppz)
    : Base ()
{
    if (ppz == 0)
    {
        return;
    }

    while (const char* pz = *ppz++)
    {
        String str (pz);

        Append (str);
    }
}

StringList::StringList (int argc, const char* const * argv)
    : Base ()
{
    if (argv != 0)
    {
        for (int n=0; n<argc; ++n)
        {
            const char* pz = *argv++;

            String str (pz);

            Append (str);
        }
    }
}

uintsys StringList::Size () const
{
    StringListIter iter (*this);

    uintsys u_Size1 = 0;
    uintsys u_Size2 = 0;

    while (iter)
    {
        u_Size1 += iter->Length();

        if (u_Size1 < u_Size2)
        {
            throw Exception ("StringList::Size: Integer overflow");
        }

        u_Size2 = u_Size1;

        ++iter;
    }

    return u_Size1;
}

void StringList::EraseFrontBytes (uintsys u_NumBytes)
{
    uintsys u_ToBeDeleted = 0;

    StringListChangeIter iter (*this);

    while (u_NumBytes && iter)
    {
        String& str (*iter);

        uintsys u_Length = str.Length();

        if (u_NumBytes < u_Length)
        {
            str.EraseFront (u_NumBytes);

            break;
        }
        else
        {
            u_NumBytes -= u_Length;

            ++u_ToBeDeleted;
        }

        ++iter;
    }

    for (uintsys u=0; u<u_ToBeDeleted; ++u)
    {
        Shift();
    }
}

const StringList StringList::Grep (const PerlRegex& rex_Pattern) const
{
    StringList strl_Return;

    if (!rex_Pattern)
    {
        return strl_Return;
    }

    StringListIter iter (*this);

    while (iter)
    {
        const String& str (*iter);

        ++iter;

        if (str.Match (rex_Pattern))
        {
            strl_Return.Append (str);
        }
    }

    return strl_Return;
}

StringListByteIter::StringListByteIter (const StringList& strl)
    : iter_Strings_ (strl)
    , iter_Bytes_   (0, 0)
{
    while (iter_Strings_)
    {
        iter_Bytes_ = *iter_Strings_;

        if (iter_Bytes_)
        {
            break;
        }

        ++iter_Strings_;
    }
}

uintsys StringListByteIter::Capacity () const
{
    uintsys u_Capacity = 0;

    if (IsValid())
    {
        u_Capacity = iter_Bytes_.Capacity();

        StringListIter iter (iter_Strings_);

        while (++iter)
        {
            u_Capacity += iter->Length();
        }
    }

    return u_Capacity;
}

StringListByteIter& StringListByteIter::operator++ ()
{
    if (IsValid())
    {
        if (!(++iter_Bytes_))
        {
            AdvanceString_();
        }
    }

    return *this;
}

bool StringListByteIter::ExtractUint8 (uintsys& u_Value, ParseError& error)
{
    if (!IsValid())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    u_Value = *iter_Bytes_;

    if (!(++iter_Bytes_))
    {
        AdvanceString_();
    }

    return true;
}

bool StringListByteIter::ExtractUint16 (uintsys& u_Value, ParseError& error)
{
    if (!IsValid())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    if (iter_Bytes_.ExtractUint16 (u_Value, error))
    {
        if (!iter_Bytes_)
        {
            AdvanceString_();
        }

        return true;
    }

    StringListByteIter iter (*this);

    uintsys u1 = 0;
    uintsys u2 = 0;

    if (!(iter.ExtractUint8 (u1, error) && iter.ExtractUint8 (u2, error)))
    {
        return false;
    }

    u_Value = (u1 << 8) | u2;

    Swap (iter);

    return true;
}

bool StringListByteIter::ExtractUint24 (uintsys& u_Value, ParseError& error)
{
    if (!IsValid())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    if (iter_Bytes_.ExtractUint24 (u_Value, error))
    {
        if (!iter_Bytes_)
        {
            AdvanceString_();
        }

        return true;
    }

    StringListByteIter iter (*this);

    uintsys u1 = 0;
    uintsys u2 = 0;

    if (!(iter.ExtractUint16 (u1, error) && iter.ExtractUint8 (u2, error)))
    {
        return false;
    }

    u_Value = (u1 << 8) | u2;

    Swap (iter);

    return true;
}

bool StringListByteIter::ExtractUint32 (uintsys& u_Value, ParseError& error)
{
    if (!IsValid())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    if (iter_Bytes_.ExtractUint32 (u_Value, error))
    {
        if (!iter_Bytes_)
        {
            AdvanceString_();
        }

        return true;
    }

    StringListByteIter iter (*this);

    uintsys u1 = 0;
    uintsys u2 = 0;

    if (!(iter.ExtractUint24 (u1, error) && iter.ExtractUint8 (u2, error)))
    {
        return false;
    }

    u_Value = (u1 << 8) | u2;

    Swap (iter);

    return true;
}

bool StringListByteIter::Extract (StringList& strl_Rest)
{
    strl_Rest.Clear();

    if (IsValid())
    {
        String str_First;

        iter_Bytes_.Extract (str_First);

        strl_Rest.Append (str_First);

        while (++iter_Strings_)
        {
            strl_Rest.Append (*iter_Strings_);
        }
    }

    return true;
}

bool StringListByteIter::Extract (uintsys u_NumBytes, StringList& strl_Content, ParseError& error)
{
    strl_Content.Clear();

    if (u_NumBytes == 0)
    {
        return true;
    }

    if (u_NumBytes > Capacity())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    uintsys u_Capacity = iter_Bytes_.Capacity();

    String str_Content;

    if (u_Capacity > u_NumBytes)
    {
        iter_Bytes_.Extract (u_NumBytes, str_Content, error);

        strl_Content.Append (str_Content);

        return true;
    }

    iter_Bytes_.Extract (str_Content);

    strl_Content.Append (str_Content);

    AdvanceString_();

    u_NumBytes -= u_Capacity;

    while (u_NumBytes != 0)
    {
        const String& str (*iter_Strings_);

        uintsys u_Length = str.Length();

        if (u_Length <= u_NumBytes)
        {
            strl_Content.Append (str);

            u_NumBytes -= u_Length;

            AdvanceString_();

            continue;
        }

        iter_Bytes_.Extract (u_NumBytes, str_Content, error);

        strl_Content.Append (str_Content);

        break;
    }

    return true;
}

bool StringListByteIter::Extract (uintsys u_Length, String& str_Content, ParseError& error)
{
    str_Content.Clear();

    if (u_Length > Capacity())
    {
        error.SetUnexpectedEndOfData();

        return false;
    }

    if (u_Length == 0)
    {
        return true;
    }

    str_Content.Reserve (u_Length);

    if (iter_Bytes_.Extract (u_Length, str_Content, error))
    {
        if (!iter_Bytes_)
        {
            AdvanceString_();
        }

        return true;
    }

    for (;;)
    {
        uintsys u_Capacity = iter_Bytes_.Capacity();

        if (u_Capacity < u_Length)
        {
            str_Content.Append (iter_Bytes_.Pointer(), u_Capacity);

            iter_Bytes_ += u_Capacity;  // make it invalid
            u_Length    -= u_Capacity;

            AdvanceString_();
        }
        else
        {
            str_Content.Append (iter_Bytes_.Pointer(), u_Length);

            iter_Bytes_ += u_Length;

            if (!iter_Bytes_)
            {
                AdvanceString_();
            }

            break;
        }
    }

    return true;
}

bool StringListByteIter::ExtractUpTo (uchar uc_End, String& str_Content, ParseError& error)
{
    Backup<StringListByteIter> backup (*this);

    str_Content.Clear();

    StringList strl_Content;

    ParseError ignore;

    while (iter_Strings_)
    {
        StringIter iter (iter_Bytes_);

        uintsys u_Length = 0;

        uchar uc = ~uc_End;  // initial value must not match uc_End

        while (iter)
        {
            uc = *iter++;

            ++u_Length;

            if (uc == uc_End)
            {
                break;
            }
        }

        String str;

        iter_Bytes_.Extract (u_Length, str, error);

        strl_Content.Append (str);

        if (uc == uc_End)
        {
            str_Content = strl_Content.Join();

            return true;
        }

        AdvanceString_();
    }

    error.SetUnexpectedEndOfData();

    backup.Restore();

    return false;
}

const String StringList::Join (const String& str_Separator) const
{
    String str_Result;

    uintsys u_NumItems = NumItems();

    if (u_NumItems == 0)
    {
        return str_Result;
    }

    uintsys u_Size = Size() + str_Separator.Length() * (u_NumItems - 1);

    str_Result.Reserve (u_Size);

    StringListIter iter (*this);

    str_Result.Append (*iter);

    ++iter;

    while (iter)
    {
        const String& str (*iter);

        str_Result.Append (str_Separator, str);

        ++iter;
    }

    return str_Result;
}

const String StringList::Join () const
{
    String str_Result;

    uintsys u_Size = Size();

    if (u_Size == 0)
    {
        return str_Result;
    }

    str_Result.Reserve (u_Size);

    StringListIter iter (*this);

    while (iter)
    {
        const String& str (*iter);

        str_Result.Append (str);

        ++iter;
    }

    return str_Result;
}

static bool ReadUpToChar (StringIter& iter, char c_Sentinel, String& str)
{
    const char ESCAPE_CHAR = '\\';

    bool b_Escaped = false;

    str.Clear();

    while (iter)
    {
        char c = *iter++;

        if (b_Escaped)
        {
            b_Escaped = false;
        }
        else if (c == c_Sentinel)
        {
            return true;
        }
        else if (c == ESCAPE_CHAR)
        {
            b_Escaped = true;
        }

        str += c;
    }

    return false;
}

bool StringList::ParseGrepMatch_ (const String& str_Specification, String& str_Pattern,
                                  bool& b_IgnoreCase, bool& b_InvertMatch)
{
    b_IgnoreCase  = false;
    b_InvertMatch = false;

    StringIter iter (str_Specification);

    char c_Separator = 0;

    while (iter && !c_Separator)
    {
        char c = *iter++;

        switch (c)
        {
            case ' ':   break;

            case '!':   b_InvertMatch = !b_InvertMatch; break;

            case '/':   c_Separator = '/'; break;

            case 'm':   if (iter)
                        {
                            c_Separator = *iter++;
                        }
                        else
                        {
                            return false;
                        }
                        break;

            default:    return false;
        }
    }

    if (c_Separator == 0)
    {
        return false;
    }

    if (!ReadUpToChar (iter, c_Separator, str_Pattern))
    {
        return false;
    }

    while (iter)
    {
        char c = *iter++;

        if (c == 'i')
        {
            b_IgnoreCase = true;

            break;
        }
    }

    return true;
}

bool StringList::ParseGrepSubst_ (const String& str_Specification, String& str_Pattern,
                                  bool& b_IgnoreCase, String& str_Replace, bool& b_GlobalReplace,
                                  bool& b_InvertMatch)
{
    b_IgnoreCase    = false;
    b_InvertMatch   = false;
    b_GlobalReplace = false;

    StringIter iter (str_Specification);

    char c_Separator = 0;

    while (iter && !c_Separator)
    {
        char c = *iter++;

        switch (c)
        {
            case ' ':   break;

            case '!':   b_InvertMatch = !b_InvertMatch; break;

            case 's':   if (iter)
                        {
                            c_Separator = *iter++;
                        }
                        else
                        {
                            return false;
                        }
                        break;

            default:    return false;
        }
    }

    if (c_Separator == 0)
    {
        return false;
    }

    if (!(ReadUpToChar (iter, c_Separator, str_Pattern) &&
          ReadUpToChar (iter, c_Separator, str_Replace)))
    {
        return false;
    }

    while (iter)
    {
        char c = *iter++;

        if (c == 'i')
        {
            b_IgnoreCase = true;
        }
        else if (c == 'g')
        {
            b_GlobalReplace = true;
        }
    }

    return true;
}

bool StringList::ParseGrepFiles_ (const String& str_Specification, char& c_FileType,
                                  bool& b_InvertMatch)
{
    b_InvertMatch = false;
    c_FileType    = 0;

    StringIter iter (str_Specification);

    while (iter && !c_FileType)
    {
        char c = *iter++;

        switch (c)
        {
            case ' ':   break;

            case '!':   b_InvertMatch = !b_InvertMatch; break;

            case '-':   if (iter)
                        {
                            c_FileType = *iter++;
                        }
                        else
                        {
                            return false;
                        }
                        break;

            default:    return false;
        }
    }

    switch (c_FileType)
    {
        case 'd':   // directory
        case 'e':   // exists
        case 'f':   // regular file
        case 'r':   // readable
        case 'w':   // writable
        case 'x':   // executable
        case 'z':   // zero-length file
        {
            return true;
        }
    }

    return false;
}

static bool CheckFileType (const String& str_File, char c_Type)
{
    File file (str_File);

    switch (c_Type)
    {
        case 'd':   return file.IsDirectory();
        case 'e':   return file.Exists();
        case 'f':   return file.IsRegular();
        case 'r':   return file.IsReadable();
        case 'w':   return file.IsWritable();
        case 'x':   return file.IsExecutable();
        case 'z':   return (file.Size() == 0);
    }

    return false;
}

const StringList StringList::GrepFiles_ (char c_FileType, bool b_InvertMatch) const
{
    StringListIter iter (*this);

    StringList strl_Matches;

    while (iter)
    {
        String str_File (*iter++);

        if (b_InvertMatch ^ CheckFileType (str_File, c_FileType))
        {
            strl_Matches.Append (str_File);
        }
    }

    return strl_Matches;
}

const StringList StringList::GrepMatch_ (const String& str_Pattern, bool b_IgnoreCase,
                                         bool b_InvertMatch) const
{
    StringListIter iter (*this);

    PerlRegex rex_Pattern (str_Pattern.C(), PerlRegexOptions().CaseSensitive(!b_IgnoreCase));

    StringList strl_Matches;

    while (iter)
    {
        String str (*iter++);

        if (b_InvertMatch ^ str.Match (rex_Pattern))
        {
            strl_Matches.Append (str);
        }
    }

    return strl_Matches;
}

const StringList StringList::GrepSubst_ (const String& str_Pattern, bool b_IgnoreCase,
                                         const String& str_Replace, bool b_GlobalReplace,
                                         bool b_InvertMatch)
{
    StringListChangeIter iter (*this);

    PerlRegex rex_Pattern (str_Pattern.C(), PerlRegexOptions().CaseSensitive(!b_IgnoreCase));

    StringList strl_Matches;

    while (iter)
    {
        String& str (*iter); ++iter;

        if (b_InvertMatch ^ (str.Replace (rex_Pattern, str_Replace, b_GlobalReplace) >= 0))
        {
            strl_Matches.Append (str);
        }
    }

    return strl_Matches;
}

const StringList StringList::Grep (const String& str_Specification)
{
    String str_Pattern;             // regex to match on each string
    String str_Replace;             // replacement for the matching pattern

    bool b_InvertMatch   = false;   // return non-matching Strings
    bool b_IgnoreCase    = false;   // was /i specified?
    bool b_GlobalReplace = false;   // was /g specified?
    char c_FileType      = 'e';     // check if file exists by default

    if (ParseGrepFiles_ (str_Specification, c_FileType, b_InvertMatch))
    {
        return GrepFiles_ (c_FileType, b_InvertMatch);
    }
    else if (ParseGrepMatch_ (str_Specification, str_Pattern,
                              b_IgnoreCase, b_InvertMatch))
    {
        return GrepMatch_ (str_Pattern, b_IgnoreCase, b_InvertMatch);
    }
    else if (ParseGrepSubst_ (str_Specification, str_Pattern, b_IgnoreCase,
                              str_Replace, b_GlobalReplace, b_InvertMatch))
    {
        return GrepSubst_ (str_Pattern, b_IgnoreCase, str_Replace,
                           b_GlobalReplace, b_InvertMatch);
    }

    return StringList();
}

} // namespace mikestoolbox

