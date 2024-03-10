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
//  File:       Unsigned.inl
//
//  Synopsis:   Implementation of class that holds a number used for repetition
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline Unsigned::Unsigned (uint64 u)
    : u_ (u)
{
    uint64 u_Check = u_;

    if (u_Check != u)
    {
        throw Exception ("Unsigned: Integer overflow");
    }
}

inline Unsigned::Unsigned (uint32 u)
    : u_ (u)
{
    // nothing
}

inline Unsigned::Unsigned (uint16 u)
    : u_ (u)
{
    // nothing
}

inline Unsigned::Unsigned (uint8 u)
    : u_ (u)
{
    // nothing
}

inline Unsigned::Unsigned (int64 n)
    : u_ (0)
{
    if (n > 0)
    {
        u_ = n;

        int64 n_Check = u_;

        if (n_Check != n)
        {
            throw Exception ("Unsigned: Integer overflow");
        }
    }
}

inline Unsigned::Unsigned (int32 n)
    : u_ ((n < 0) ? 0 : n)
{
    // nothing
}

inline Unsigned::Unsigned (int16 n)
    : u_ ((n < 0) ? 0 : n)
{
    // nothing
}

inline Unsigned::Unsigned (int8 n)
    : u_ ((n < 0) ? 0 : n)
{
    // nothing
}

inline Unsigned::Unsigned ()
    : u_ (0)
{
    // nothing
}

inline Unsigned::operator uintsys () const
{
    return u_;
}

} // namespace mikestoolbox

