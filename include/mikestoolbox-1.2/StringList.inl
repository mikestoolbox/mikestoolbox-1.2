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
//  File:       StringList.inl
//
//  Synopsis:   Inline methods of StringList
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline bool StringList::Check () const
{
    return Base::Check();
}

template<class CONTAINER>
inline void StringList::Append (const CONTAINER& c)
{
    Base::Append (c);
}

inline void StringList::Append (const StringList& strl)
{
    Base::Append (strl);
}

inline void StringList::Append (const String& str)
{
    Base::Append (str);
}

inline void StringList::AppendNonEmpty (const String& str)
{
    if (!str.IsEmpty())
    {
        Append (str);
    }
}

inline void StringList::Append (const String& str1, const String& str2)
{
    uintsys u_OrigNumItems = NumItems();

    try
    {
        Base::Append (str1);
        Base::Append (str2);
    }
    catch (...)
    {
        Truncate (u_OrigNumItems);

        throw;
    }
}

inline void StringList::Append (const String& str1, const String& str2, const String& str3)
{
    uintsys u_OrigNumItems = NumItems();

    try
    {
        Base::Append (str1);
        Base::Append (str2);
        Base::Append (str3);
    }
    catch (...)
    {
        Truncate (u_OrigNumItems);

        throw;
    }
}

inline void StringList::Append (const String& str1, const String& str2,
                                const String& str3, const String& str4)
{
    uintsys u_OrigNumItems = NumItems();

    try
    {
        Base::Append (str1);
        Base::Append (str2);
        Base::Append (str3);
        Base::Append (str4);
    }
    catch (...)
    {
        Truncate (u_OrigNumItems);

        throw;
    }
}

inline void StringList::Append (const String& str, Repeat repeat)
{
    uintsys u_OrigNumItems = NumItems();

    try
    {
        for (uintsys u=0; u<repeat; ++u)
        {
            Base::Append (str);
        }
    }
    catch (...)
    {
        Truncate (u_OrigNumItems);

        throw;
    }
}

inline void StringList::Append (const char* pz)
{
    Base::Append (String(pz));
}

inline void StringList::Append (char c)
{
    Base::Append (String(c));
}

template<typename CONTAINER>
inline StringList::StringList (const CONTAINER& c)
    : Base (c)
{
    // nothing
}

inline StringList::StringList (const String& str, Repeat repeat)
    : Base (str, repeat)
{
    // nothing
}

inline StringList::StringList (const String& str)
    : Base (str)
{
    // nothing
}

inline StringList::StringList (const char* pz)
    : Base (String(pz))
{
    // nothing
}

inline StringList::StringList ()
    : Base ()
{
    // nothing
}

template<typename CONTAINER>
inline StringList& StringList::operator= (const CONTAINER& c)
{
    StringList strl (c);

    Swap (strl);

    return *this;
}

inline StringList& StringList::operator= (const StringList& strl)
{
    StringList strl2 (strl);

    Swap (strl2);

    return *this;
}

inline StringList& StringList::operator= (const String& str)
{
    StringList strl (str);

    Swap (strl);

    return *this;
}

inline StringList StringList::operator+ (const StringList& strl) const
{
    StringList strl_Sum (*this);

    strl_Sum.Append (strl);

    return strl_Sum;
}

inline StringList StringList::operator+ (const String& str) const
{
    StringList strl_Sum (*this);

    strl_Sum.Append (str);

    return strl_Sum;
}

inline const StringList StringList::Grep (const char* pz_Pattern)
{
    String str_Pattern (pz_Pattern);

    return Grep (str_Pattern);
}

inline const String StringList::Join (const char* pz_Separator) const
{
    return Join (String(pz_Separator));
}

inline const String StringList::Join (char c_Separator) const
{
    return Join (String(c_Separator));
}

inline const String StringList::Base64Decode () const
{
    return Join().Base64Decode();
}

inline const String StringList::Base64Encode (uintsys u_LineLength) const
{
    return Join().Base64Encode (u_LineLength);
}

inline StringListRef::StringListRef (StringList& strl, const Index& index)
    : strl_  (strl)
    , index_ (index)
{
    // nothing
}

inline StringListRef::operator const String () const
{
    const StringList& strl (strl_);

    return strl[index_];
}

inline String* StringListRef::operator-> ()
{
    return &strl_[index_];
}

inline StringListRef& StringListRef::operator= (const String& str)
{
    strl_[index_] = str;

    return *this;
}

inline StringListRef& StringListRef::operator= (const StringListRef& ref)
{
    strl_[index_] = ref;

    return *this;
}

template<typename U>
inline bool StringListRef::operator== (const U& u) const
{
    const StringList& strl (strl_);

    return (strl[index_] == String(u));
}

template<typename U>
inline bool StringListRef::operator!= (const U& u) const
{
    return !operator== (u);
}

inline void StringList::Append (std::istream& is)
{
    String str;

    while (!is.eof())
    {
        is >> str;

        if (str.IsEmpty())
        {
            break;
        }

        Append (str);
    }
}

inline void StringList::Read (std::istream& is)
{
    Clear();

    Append (is);
}

inline std::ostream& operator<< (std::ostream& os, const StringList& strl)
{
    StringListIter iter (strl);

    while (iter)
    {
        os << *iter++;
    }

    return os;
}

inline const StringListByteIter StringListByteIter::operator++ (int)
{
    StringListByteIter iter (*this);

    operator++();

    return iter;
}

inline bool StringListByteIter::IsValid () const
{
    return iter_Strings_;
}

inline StringListByteIter::operator bool () const
{
    return IsValid();
}

inline void StringListByteIter::Swap (StringListByteIter& iter)
{
    using std::swap;

    swap (iter_Strings_, iter.iter_Strings_);
    swap (iter_Bytes_,   iter.iter_Bytes_);
}

inline void StringListByteIter::AdvanceString_ ()
{
    while (++iter_Strings_)
    {
        iter_Bytes_ = *iter_Strings_;

        if  (iter_Bytes_)
        {
            return;
        }
    }

    iter_Bytes_ = StringIter(); // invalid
}

} // namespace mikestoolbox

namespace std {

template<>
inline void swap<mikestoolbox::StringList> (mikestoolbox::StringList& strl1,
                                            mikestoolbox::StringList& strl2)
{
    strl1.Swap (strl2);
}

} // namespace std

