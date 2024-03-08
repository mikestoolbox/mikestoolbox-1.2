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
//  File:       SocketAddress.inl
//
//  Synopsis:   Implementation of classes that represent socket addresses
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline bool SocketAddress::SetAddress (const char* pz_Address)
{
    String str_Address (pz_Address);

    return SetAddress (str_Address);
}

inline String SocketAddress::GetAddress () const
{
    String str_Address;

    GetAddress (str_Address);

    return str_Address;
}

inline SocketAddress& SocketAddress::operator= (const String& str_Address)
{
    SetAddress (str_Address);

    return *this;
}

inline SocketAddress& SocketAddress::operator= (const char* pz_Address)
{
    String str_Address (pz_Address);

    return operator= (str_Address);
}

inline bool SocketAddress::operator!= (const SocketAddress& addr) const
{
    return !operator== (addr);
}

inline IpAddressRange::IpAddressRange ()
    : addr_      ("*")
    , u_NumBits_ (32)
    , u_Bitmask_ (0)
{
    u_Bitmask_ = ~u_Bitmask_;
}

} // namespace mikestoolbox

