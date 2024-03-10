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
//  File:       SocketAddress.cpp
//
//  Synopsis:   Implementation of classes that represent socket addresses
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_UNIX
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

namespace mikestoolbox {

SocketAddress::SocketAddress ()
    : ss_Address_ ()
{
    ZeroStructure (ss_Address_);

    ss_Address_.ss_family = AF_UNSPEC;
}

SocketAddress::SocketAddress (const struct sockaddr* p_Address)
    : ss_Address_ ()
{
    ZeroStructure (ss_Address_);

    switch (p_Address->sa_family)
    {
        case AF_INET:
        {
            std::memmove (&ss_Address_, p_Address, sizeof(struct sockaddr_in));

            break;
        }

        case AF_INET6:
        {
            std::memmove (&ss_Address_, p_Address, sizeof(struct sockaddr_in6));

            break;
        }

        default:
        {
            ss_Address_.ss_family = AF_UNSPEC;

            break;
        }
    }
}

SocketAddress::SocketAddress (const SocketAddress& addr)
    : ss_Address_ ()
{
    std::memmove (&ss_Address_, &addr.ss_Address_, sizeof(ss_Address_));
}

uint16 SocketAddress::GetPort () const
{
    switch (ss_Address_.ss_family)
    {
        case AF_INET:
        {
            const struct sockaddr_in* psin = (const struct sockaddr_in*) &ss_Address_;

            return ntohs (psin->sin_port);
        }

        case AF_INET6:
        {
            const struct sockaddr_in6* psin6 = (const struct sockaddr_in6*) &ss_Address_;

            return ntohs (psin6->sin6_port);
        }
    }

    return ntohs (0);
}

bool SocketAddress::SetPort (uint16 u_Port)
{
    switch (ss_Address_.ss_family)
    {
        case AF_INET:
        {
            struct sockaddr_in* psin = (struct sockaddr_in*) &ss_Address_;

            psin->sin_port = htons (u_Port);

            return true;
        }

        case AF_INET6:
        {
            struct sockaddr_in6* psin6 = (struct sockaddr_in6*) &ss_Address_;

            psin6->sin6_port = htons (u_Port);

            return true;
        }
    }

    return false;
}

bool SocketAddress::SetAddress (const String& str_Address)
{
    uint16 u_Port = GetPort();

    ZeroStructure (ss_Address_);

    ss_Address_.ss_family = AF_INET;

    if (str_Address == '*' || str_Address == "*:*")
    {
        return true;
    }

    if (str_Address.StartsWith ("[::]"))
    {
        ss_Address_.ss_family = AF_INET6;

        PerlRegexMatches matches;

        if (str_Address.Match ("^\\[::\\]:(\\d+)$", matches))
        {
            String str_Port (matches.GetMatch (1));

            SetPort (str_Port.AsUint());
        }

        return true;
    }

    PerlRegexMatches matches;

    if (str_Address.Match ("^\\*:(\\d+)$", matches))
    {
        String str_Port (matches.GetMatch (1));

        SetPort (str_Port.AsUint());

        return true;
    }

    const char* pz_Address = str_Address.C();

    String str_IP;

    if (str_Address.Match ("^(\\d{1,3}(\\.\\d{1,3}){3})[.:](\\d+)$", matches))
    {
        String str_Port (matches.GetMatch (3));

        str_IP = matches.GetMatch (1);

        pz_Address = str_IP.C();
        u_Port     = str_Port.AsUint();
    }

    struct in_addr  addrV4;
    struct in6_addr addrV6;

    if (inet_pton (AF_INET, pz_Address, &addrV4) > 0)
    {
        struct sockaddr_in* psin = (struct sockaddr_in*) &ss_Address_;

        psin->sin_addr.s_addr = addrV4.s_addr;
    }
    else if (inet_pton (AF_INET6, pz_Address, &addrV6) > 0)
    {
        struct sockaddr_in6* psin6 = (struct sockaddr_in6*) &ss_Address_;

        psin6->sin6_family = AF_INET6;

        std::memmove (&psin6->sin6_addr, &addrV6, sizeof(struct in6_addr));
    }
    else
    {
        return false;
    }

    SetPort (u_Port);

    return true;
}

SocketAddress::SocketAddress (const String& str_Address, uint16 u_Port)
    : ss_Address_ ()
{
    SetAddress (str_Address);
    SetPort    (u_Port);
}

SocketAddress::SocketAddress (const char* pz_Address, uint16 u_Port)
    : ss_Address_ ()
{
    SetAddress (pz_Address);
    SetPort    (u_Port);
}

SocketAddress::SocketAddress (const String& str_Address)
    : ss_Address_ ()
{
    SetAddress (str_Address);
}

SocketAddress::SocketAddress (const char* pz_Address)
    : ss_Address_ ()
{
    SetAddress (pz_Address);
}

SocketAddress::operator const struct sockaddr* () const
{
    return (const struct sockaddr*) &ss_Address_;
}

SocketAddress::operator struct sockaddr* ()
{
    return (struct sockaddr*) &ss_Address_;
}

bool SocketAddress::GetAddress (String& str_Address) const
{
    str_Address.Clear();

    switch (ss_Address_.ss_family)
    {
        case AF_INET:
        {
            char ac_Address [INET_ADDRSTRLEN];

            const char* pz_Address = 0;

            const struct sockaddr_in* psin = (const struct sockaddr_in*) &ss_Address_;

            pz_Address = inet_ntop (AF_INET, &psin->sin_addr, ac_Address, INET_ADDRSTRLEN);

            if (pz_Address)
            {
                str_Address = pz_Address;
            }

            break;
        }

        case AF_INET6:
        {
            char ac_Address [INET6_ADDRSTRLEN];

            const char* pz_Address = 0;

            const struct sockaddr_in6* psin6 = (const struct sockaddr_in6*) &ss_Address_;

            pz_Address = inet_ntop (AF_INET6, &psin6->sin6_addr, ac_Address, INET6_ADDRSTRLEN);

            if (pz_Address)
            {
                str_Address = pz_Address;
            }

            break;
        }

        default:
        {
            str_Address = "UnknAddrFamily";

            return false;
        }
    }

    return !str_Address.IsEmpty();
}

int SocketAddress::Family () const
{
    return ss_Address_.ss_family;
}

bool SocketAddress::IsValid () const
{
    switch (ss_Address_.ss_family)
    {
        case AF_INET:
        case AF_INET6:  return true;
    }

    return false;
}

int SocketAddress::Length () const
{
    switch (ss_Address_.ss_family)
    {
        case AF_INET:   return sizeof (struct sockaddr_in);
        case AF_INET6:  return sizeof (struct sockaddr_in6);
    }

    return 0;
}

uintsys SocketAddress::Capacity () const
{
    return sizeof (ss_Address_);
}

SocketAddress& SocketAddress::operator= (const SocketAddress& addr)
{
    std::memmove (&ss_Address_, &addr.ss_Address_, sizeof(ss_Address_));

    return *this;
}

bool SocketAddress::operator== (const SocketAddress& addr) const
{
    uint16 u_ThisFamily = ss_Address_.ss_family;
    uint16 u_ThatFamily = addr.ss_Address_.ss_family;

    if (u_ThisFamily != u_ThatFamily)
    {
        return false;
    }

    switch (u_ThisFamily)
    {
        case AF_INET:
        {
            return (std::memcmp (&ss_Address_, &addr.ss_Address_,
                                 sizeof(struct sockaddr_in)) == 0);
        }

        case AF_INET6:
        {
            return (std::memcmp (&ss_Address_, &addr.ss_Address_,
                                 sizeof(struct sockaddr_in6)) == 0);
        }
    }

    return false;
}

IpAddressRange::IpAddressRange (const String& str_Address, uint u_NumBits)
    : addr_      (str_Address)
    , u_NumBits_ (u_NumBits)
    , u_Bitmask_ (0)
{
    if (addr_.Family() != AF_INET)
    {
        addr_      = "*";
        u_NumBits_ = 32;
    }

    if (u_NumBits_ > 32)
    {
        u_NumBits_ = 32;
    }

    u_Bitmask_ = (~u_Bitmask_) << (32 - u_NumBits_);
}

IpAddressRange::IpAddressRange (const char* pz_Address, uint u_NumBits)
    : addr_      (pz_Address)
    , u_NumBits_ (u_NumBits)
    , u_Bitmask_ (0)
{
    if (addr_.Family() != AF_INET)
    {
        addr_      = "*";
        u_NumBits_ = 32;
    }

    if (u_NumBits_ > 32)
    {
        u_NumBits_ = 32;
    }

    u_Bitmask_ = (~u_Bitmask_) << (32 - u_NumBits_);
}

IpAddressRange::IpAddressRange (const String& str_RangeSpec)
    : addr_      ("*")
    , u_NumBits_ (32)
    , u_Bitmask_ (0)
{
    PerlRegexMatches matches;

    u_Bitmask_ = ~u_Bitmask_;

    if (str_RangeSpec.Match ("^(\\d+(\\.\\d+){3})(/(\\d+))?$", matches))
    {
        String str_Address (matches.GetMatch (1));
        String str_NumBits (matches.GetMatch (4));

        addr_ = str_Address;

        if (str_NumBits.Length())
        {
            u_NumBits_ = Minimum (32U, str_NumBits.AsUint());
            u_Bitmask_ = u_Bitmask_ << (32U - u_NumBits_);
        }
    }
}

IpAddressRange::IpAddressRange (const char* pz_RangeSpec)
    : addr_      ("*")
    , u_NumBits_ (32)
    , u_Bitmask_ (0)
{
    PerlRegexMatches matches;

    String str_RangeSpec (pz_RangeSpec);

    u_Bitmask_ = ~u_Bitmask_;

    if (str_RangeSpec.Match ("^(\\d+(\\.\\d+){3})(/(\\d+))?$", matches))
    {
        String str_Address (matches.GetMatch (1));
        String str_NumBits (matches.GetMatch (4));

        addr_ = str_Address;

        if (str_NumBits.Length())
        {
            u_NumBits_ = Minimum (32U, str_NumBits.AsUint());
            u_Bitmask_ = u_Bitmask_ << (32U - u_NumBits_);
        }
    }
}

IpAddressRange::IpAddressRange (const SocketAddress& addr, uint u_NumBits)
    : addr_      (addr)
    , u_NumBits_ (u_NumBits)
    , u_Bitmask_ (0)
{
    if (addr_.Family() != AF_INET)
    {
        addr_      = "*";
        u_NumBits_ = 32;
    }

    if (u_NumBits_ > 32)
    {
        u_NumBits_ = 32;
    }

    u_Bitmask_ = (~u_Bitmask_) << (32 - u_NumBits_);
}

bool IpAddressRange::Contains (const SocketAddress& addr) const
{
    const struct sockaddr* p_Address = addr;
    const struct sockaddr* p_IpRange = addr_;

    if (p_Address->sa_family != AF_INET)
    {
        return false;
    }

    const struct sockaddr_in* p_Address_in = (const struct sockaddr_in*) p_Address;
    const struct sockaddr_in* p_IpRange_in = (const struct sockaddr_in*) p_IpRange;

    uintsys u_Address_in = ntohl (p_Address_in->sin_addr.s_addr);
    uintsys u_IpRange_in = ntohl (p_IpRange_in->sin_addr.s_addr);

    return ((u_Address_in & u_Bitmask_) == (u_IpRange_in & u_Bitmask_));
}

} // namespace mikestoolbox

