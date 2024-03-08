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
//  File:       CharRef.inl
//
//  Synopsis:   Implementation of objects that reference chars within Strings
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline CharRef::CharRef (char* p)
    : p_Address_     (p)
    , c_PlaceHolder_ ('\0')
{
    if (p == 0)
    {
        p_Address_ = &c_PlaceHolder_;
    }
}

inline CharRef::CharRef (const CharRef& ref)
    : p_Address_     (ref.p_Address_)
    , c_PlaceHolder_ (ref.c_PlaceHolder_)
{
    if (p_Address_ == &ref.c_PlaceHolder_)
    {
        p_Address_ = &c_PlaceHolder_;
    }
}

inline char CharRef::Value () const
{
    return *p_Address_;
}

inline void CharRef::Set (char c)
{
    *p_Address_ = c;
}

inline CharRef::operator char () const
{
    return Value();
}

inline CharRef& CharRef::operator= (char c)
{
    Set (c);

    return *this;
}

inline CharRef& CharRef::operator= (const CharRef& ref)
{
    Set (ref.Value());

    return *this;
}

inline ByteRef::ByteRef (uchar* p)
    : p_Address_      (p)
    , uc_PlaceHolder_ (0)
{
    if (p == 0)
    {
        p_Address_ = &uc_PlaceHolder_;
    }
}

inline ByteRef::ByteRef (const ByteRef& ref)
    : p_Address_      (ref.p_Address_)
    , uc_PlaceHolder_ (ref.uc_PlaceHolder_)
{
    if (p_Address_ == &ref.uc_PlaceHolder_)
    {
        p_Address_ = &uc_PlaceHolder_;
    }
}

inline uchar ByteRef::Value () const
{
    return *p_Address_;
}

inline void ByteRef::Set (uchar uc)
{
    *p_Address_ = uc;
}

inline ByteRef::operator uchar () const
{
    return Value();
}

inline ByteRef& ByteRef::operator= (uchar uc)
{
    Set (uc);

    return *this;
}

inline ByteRef& ByteRef::operator= (const ByteRef& ref)
{
    Set (ref.Value());

    return *this;
}

} // namespace mikestoolbox

