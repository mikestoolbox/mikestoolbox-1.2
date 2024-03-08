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
//  File:       Index.inl
//
//  Synopsis:   Implementation of class that indexes into a container
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline Index::Index (uint64 u)
    : u_Index_    (u)
    , b_Negative_ (false)
    , b_Overflow_ (u_Index_ != u)
{
    // nothing
}

inline Index::Index (uint32 u)
    : u_Index_    (u)
    , b_Negative_ (false)
    , b_Overflow_ (u_Index_ != u)
{
    // nothing
}

inline Index::Index (uint16 u)
    : u_Index_    (u)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    // nothing
}

inline Index::Index (uint8 u)
    : u_Index_    (u)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    // nothing
}

inline Index::Index (int64 n)
    : u_Index_    (n)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    if (n < 0)
    {
        int64 n_Pos = -n;

        u_Index_    = n_Pos;
        b_Overflow_ = (u_Index_ != uintsys(n_Pos));
        b_Negative_ = true;
    }
    else
    {
        int64 n_Check = u_Index_;

        b_Overflow_ = (n_Check != n);
    }
}

inline Index::Index (int32 n)
    : u_Index_    (n)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    if (n < 0)
    {
        int32 n_Pos = -n;

        u_Index_    = n_Pos;
        b_Overflow_ = (u_Index_ != uintsys(n_Pos));
        b_Negative_ = true;
    }
    else
    {
        int32 n_Check = u_Index_;

        b_Overflow_ = (n_Check != n);
    }
}

inline Index::Index (int16 n)
    : u_Index_    (n)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    if (n < 0)
    {
        u_Index_    = -n;
        b_Negative_ = true;
    }
}

inline Index::Index (int8 n)
    : u_Index_    (n)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    if (n < 0)
    {
        u_Index_    = -n;
        b_Negative_ = true;
    }
}

inline Index::Index ()
    : u_Index_    (0)
    , b_Negative_ (false)
    , b_Overflow_ (false)
{
    // nothing
}

inline bool Index::IsNegative () const
{
    return b_Negative_;
}

inline bool Index::IsZero () const
{
    return u_Index_ == 0;
}

inline bool Index::Overflowed () const
{
    return b_Overflow_;
}

inline uintsys Index::Shortfall (uintsys u_NumItems) const
{
    if (b_Negative_)
    {
        return (u_Index_ > u_NumItems) ? (u_Index_ - u_NumItems) : 0;
    }
    else
    {
        return (u_Index_ >= u_NumItems) ? (u_Index_ - u_NumItems + 1) : 0;
    }
}

inline bool Index::Calculate (uintsys u_NumItems, uintsys& u_Index) const
{
    if (b_Overflow_)
    {
        return false;
    }

    if (b_Negative_)
    {
        if (u_Index_ <= u_NumItems)
        {
            u_Index = u_NumItems - u_Index_;

            return true;
        }
    }
    else
    {
        if (u_Index_ < u_NumItems)
        {
            u_Index = u_Index_;

            return true;
        }
    }

    return false;
}

inline Index& Index::operator= (uint64 u)
{
    Index index (u);

    return operator= (index);
}

inline Index& Index::operator= (uint32 u)
{
    Index index (u);

    return operator= (index);
}

inline Index& Index::operator= (uint16 u)
{
    Index index (u);

    return operator= (index);
}

inline Index& Index::operator= (uint8 u)
{
    Index index (u);

    return operator= (index);
}

inline Index& Index::operator= (int64 n)
{
    Index index (n);

    return operator= (index);
}

inline Index& Index::operator= (int32 n)
{
    Index index (n);

    return operator= (index);
}

inline Index& Index::operator= (int16 n)
{
    Index index (n);

    return operator= (index);
}

inline Index& Index::operator= (int8 n)
{
    Index index (n);

    return operator= (index);
}

} // namespace mikestoolbox

