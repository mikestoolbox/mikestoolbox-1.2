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
//  File:       UNIX/BerkeleySocketUNIX.cpp
//
//  Synopsis:   UNIX implementation of the BerkeleySocket class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#include <netdb.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#define SOCKET_OP_FAILED(x)  ((x) < 0)

#ifndef SOCKET_MESSAGE_SIZE
#define SOCKET_MESSAGE_SIZE 32
#endif

namespace mikestoolbox {

void StartupWindowsSockets ()
{
    // nothing, of course -- this is UNIX!
}

//+---------------------------------------------------------------------------
//  Class:      SocketMessage
//
//  Synopsis:   A class used for "gather write" to a Berkeley socket
//----------------------------------------------------------------------------

template<uintsys MAX_GATHER>
class SocketMessage
{
public:

    SocketMessage (const StringList& strl_Message);
    ~SocketMessage ();

    operator struct msghdr* ();

private:

    struct msghdr msg_;
    struct iovec  iov_[MAX_GATHER];

    SocketMessage (const SocketMessage&);
    SocketMessage& operator= (const SocketMessage&);
};

template<uintsys MAX_GATHER>
SocketMessage<MAX_GATHER>::SocketMessage (const StringList& strl_Message)
    : msg_ ()
    , iov_ ()
{
    ZeroStructure (msg_);

    msg_.msg_iov = iov_;

    StringListIter iter (strl_Message);

    uintsys u = 0;

    while (iter && u<MAX_GATHER)
    {
        const String& str (*iter);

        uintsys u_Length = str.Length();

        if (u_Length != 0)
        {
            iov_[u].iov_base = (void*)str.C();
            iov_[u].iov_len  = u_Length;

            ++msg_.msg_iovlen;
            ++u;
        }

        ++iter;
    }
}

template<uintsys MAX_GATHER>
SocketMessage<MAX_GATHER>::operator struct msghdr* ()
{
    return &msg_;
}

template<uintsys MAX_GATHER>
inline SocketMessage<MAX_GATHER>::~SocketMessage ()
{
    // nothing
}

bool BerkeleySocket::Unblock ()
{
    int n_Flags = fcntl (h_Socket_, F_GETFL, 0);

    if (n_Flags < 0)
    {
        return false;
    }

    n_Flags |= O_NONBLOCK;

    return (fcntl (h_Socket_, F_SETFL, n_Flags) >= 0);
}

intsys BerkeleySocket::Send (const StringList& strl_Data, int n_Flags)
{
    SocketMessage<SOCKET_MESSAGE_SIZE> message (strl_Data);

    intsys n_Return = sendmsg (h_Socket_, message, n_Flags);

    if (SOCKET_OP_FAILED(n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

int BerkeleySocket::GetTcpMaxSegmentSize () const
{
    int n_Size = 0;

    socklen_t n_Length = sizeof(n_Size);

    getsockopt (h_Socket_, IPPROTO_TCP, TCP_MAXSEG, (char*)&n_Size, &n_Length);

    return n_Size;
}

bool BerkeleySocket::SetTcpMaxSegmentSize (int n_Size)
{
    return setsockopt (h_Socket_, IPPROTO_TCP, TCP_MAXSEG, (const char*)&n_Size,
                        sizeof(n_Size)) == 0;
}

} // namespace mikestoolbox

