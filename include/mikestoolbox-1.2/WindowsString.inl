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
//  File:       WindowsString.inl
//
//  Synopsis:   Inline function definitions for the WindowsString class
//----------------------------------------------------------------------------

#ifdef PLATFORM_WINDOWS

namespace mikestoolbox {

inline String UTF8ToWindows (const String& str_UTF8)
{
    StringIter iter (str_UTF8);
    String     result;
    uintsys    uc;
    ParseError error;

    result.Reserve (str_UTF8.Length() * 2);

    while (iter)
    {
        if (iter.ParseUTF8Char (uc, error))
        {
            result.AppendUTF16LE (uc);
        }
        else
        {
            throw Exception ("UTF8ToWindows: Bad UTF8 character");
        }
    }

    return result;
}

inline String WindowsToUTF8 (const String& str_Windows)
{
    StringIter iter (str_Windows);
    String     result;
    uintsys    uc;
    ParseError error;

    iter.SetLittleEndian();

    result.Reserve (str_Windows.Length());

    while (iter)
    {
        if (iter.ParseUTF16Char (uc, error))
        {
            result.AppendUTF8 (uc);
        }
        else
        {
            throw Exception ("UTF8ToWindows: Bad UTF16 character");
        }
    }

    return result;
}

inline WindowsString::WindowsString (const String& str)
    : str_UTF8_    (str)
    , str_Windows_ (UTF8ToWindows (str_UTF8_))
{
    // nothing
}

inline WindowsString::WindowsString (const WCHAR* pz, uintsys u_Length)
    : str_UTF8_    ()
    , str_Windows_ ()
{
    str_Windows_ = String ((const char*)pz, u_Length * sizeof (WCHAR));
    str_UTF8_    = WindowsToUTF8 (str_Windows_);
}

inline WindowsString::WindowsString (const char* pz, uintsys u_Length)
    : str_UTF8_    (pz, u_Length)
    , str_Windows_ (UTF8ToWindows (str_UTF8_))
{
    // nothing
}

inline WindowsString::WindowsString (const WCHAR* pz)
    : str_UTF8_    ()
    , str_Windows_ ()
{
    if (pz != 0)
    {
        uintsys u_Length = lstrlenW (pz);

        str_Windows_ = String ((const char*)pz, u_Length * sizeof (WCHAR));
        str_UTF8_    = WindowsToUTF8 (str_Windows_);
    }
}

inline WindowsString::WindowsString (const char* pz)
    : str_UTF8_    (pz)
    , str_Windows_ (UTF8ToWindows (str_UTF8_))
{
    // nothing
}

inline WindowsString::WindowsString ()
    : str_UTF8_    ()
    , str_Windows_ ()
{
    // nothing
}

inline String WindowsString::UTF8 () const
{
    return str_UTF8_;
}

inline String WindowsString::Windows () const
{
    return str_Windows_;
}

inline const char* WindowsString::C () const
{
    return str_UTF8_.C();
}

inline void WindowsString::Clear ()
{
    str_UTF8_.Clear();
    str_Windows_.Clear();
}

inline bool WindowsString::IsEmpty () const
{
    return str_UTF8_.IsEmpty();
}

inline uintsys WindowsString::LengthUTF8 () const
{
    return str_UTF8_.Length();
}

inline uintsys WindowsString::LengthWindows () const
{
    return str_Windows_.Length() / 2;
}

inline uintsys WindowsString::Length () const
{
#ifdef UNICODE
    return LengthWindows();
#else
    return LengthUTF8();
#endif
}

inline void WindowsString::Swap (WindowsString& str)
{
    str_UTF8_.Swap    (str.str_UTF8_);
    str_Windows_.Swap (str.str_Windows_);
}

inline WindowsString::operator const char* () const
{
    return str_UTF8_.C();
}

inline WindowsString::operator const WCHAR* () const
{
    return (const WCHAR*)str_Windows_.C();
}

inline WindowsString::operator const String& () const
{
    return str_UTF8_;
}

inline WindowsString& WindowsString::operator= (const String& str)
{
    str_UTF8_    = str;
    str_Windows_ = UTF8ToWindows (str_UTF8_);

    return *this;
}

inline WindowsString& WindowsString::operator= (const char* pz)
{
    String str (pz);

    return operator= (str);
}

inline WindowsString& WindowsString::operator= (const WCHAR* pz)
{
    if (pz == 0)
    {
        Clear();
    }
    else
    {
        uintsys u_Length = lstrlenW (pz);

        if (u_Length == 0)
        {
            Clear();
        }
        else
        {
            str_Windows_ = String ((const char*)pz, u_Length * sizeof(WCHAR));
            str_UTF8_    = WindowsToUTF8 (str_Windows_);
        }
    }

    return *this;
}

inline bool WindowsString::operator== (const WindowsString& str) const
{
    return str_UTF8_ == str.str_UTF8_;
}

inline bool WindowsString::operator== (const String& str) const
{
    return str_UTF8_ == str;
}

inline bool WindowsString::operator== (const char* pz) const
{
    return str_UTF8_ == pz;
}

inline bool WindowsString::operator== (const WCHAR* pz) const
{
    WindowsString str (pz);

    return operator== (str);
}

inline bool WindowsString::operator!= (const WindowsString& str) const
{
    return !operator== (str);
}

inline bool WindowsString::operator!= (const String& str) const
{
    return !operator== (str);
}

inline bool WindowsString::operator!= (const char* pz) const
{
    return !operator== (pz);
}

inline bool WindowsString::operator!= (const WCHAR* pz) const
{
    return !operator== (pz);
}

} // namespace mikestoolbox

namespace std {

template<>
inline void swap<mikestoolbox::WindowsString> (mikestoolbox::WindowsString& str1,
                                               mikestoolbox::WindowsString& str2)
{
    str1.Swap (str2);
}

} // namespace std

#endif // PLATFORM_WINDOWS

