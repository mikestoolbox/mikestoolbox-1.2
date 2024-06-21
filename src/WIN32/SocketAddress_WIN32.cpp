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
//  Synopsis:   Windows implementation of network address functions
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 46
#endif

extern "C" {

int inet_aton (const char* strptr, struct in_addr* addrptr)
{
    const uintsys u_MaxIPv4AddressLength = 15;

    uintsys u1 = 0;
    uintsys u2 = 0;
    uintsys u3 = 0;
    uintsys u4 = 0;

    mikestoolbox::String     str_Address (strptr);
    mikestoolbox::StringIter iter        (str_Address);

    if (str_Address.Length() > u_MaxIPv4AddressLength)
    {
        return 0;
    }

    ParseError error;

    if (!(iter.ParseNumber (u1,  error) &&
          iter.SkipChar    ('.', error) &&
          iter.ParseNumber (u2,  error) &&
          iter.SkipChar    ('.', error) &&
          iter.ParseNumber (u3,  error) &&
          iter.SkipChar    ('.', error) &&
          iter.ParseNumber (u4,  error)))
    {
        return 0;
    }

    if ((u1 > 255) || (u2 > 255) || (u3 > 255) || (u4 > 255))
    {
        return 0;
    }

    uintsys u_Address = (u1 << 24) | (u2 << 16) | (u3 << 8) | u4;

    addrptr->s_addr = htonl (u_Address);

    return 1;
}

int inet_pton (int family, const char* strptr, void* addrptr)
{
    if (family == AF_INET)
    {
        struct in_addr in_val;

        if (inet_aton (strptr, &in_val))
        {
            std::memcpy (addrptr, &in_val, sizeof (struct in_addr));

            return 1;
        }
    }

    errno = WSAEAFNOSUPPORT;

    return -1;
}

const char* inet_ntop (int family, const void* addrptr, char* strptr, size_t len)
{
    const unsigned char* p_Address = (const unsigned char*) addrptr;

    if (family == AF_INET)
    {
        mikestoolbox::String str_IP;

        str_IP += (uintsys) p_Address[0];
        str_IP += '.';
        str_IP += (uintsys) p_Address[1];
        str_IP += '.';
        str_IP += (uintsys) p_Address[2];
        str_IP += '.';
        str_IP += (uintsys) p_Address[3];

        if (str_IP.Length() >= len)
        {
            errno = ENOSPC;

            return 0;
        }

        std::strncpy (strptr, str_IP.C(), len);

        return strptr;
    }

    errno = WSAEAFNOSUPPORT;

    return 0;
}

}   // extern "C"

