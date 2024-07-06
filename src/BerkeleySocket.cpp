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
//  File:       BerkeleySocket.cpp
//
//  Synopsis:   Implementation of the BerkeleySocket class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_UNIX
#define closesocket close
#define ioctlsocket ioctl
#endif

#ifdef PLATFORM_WINDOWS
#define SOCKET_OP_FAILED(x)  ((x) == SOCKET_ERROR)
#else
#define SOCKET_OP_FAILED(x)  ((x) < 0)
#endif

namespace mikestoolbox {

void StartupWindowsSockets ();

bool BerkeleySocket::CreateSocket (intsys n_Family, intsys n_Type,
                                   intsys n_Protocol)
{
    StartupWindowsSockets();

    Close();

    h_Socket_ = socket (n_Family, n_Type, n_Protocol);

    return (h_Socket_ != INVALID_SOCKET);
}

bool BerkeleySocket::CreateTCPSocket (intsys n_Family)
{
    return CreateSocket (n_Family, SOCK_STREAM, IPPROTO_TCP);
}

bool BerkeleySocket::CreateUDPSocket (intsys n_Family)
{
    return CreateSocket (n_Family, SOCK_DGRAM, IPPROTO_UDP);
}

void BerkeleySocket::CloseSocket (SOCKET sock)
{
    if (sock != INVALID_SOCKET)
    {
        closesocket (sock);
    }
}

bool BerkeleySocket::Close ()
{
    if (h_Socket_ != INVALID_SOCKET)
    {
        closesocket (h_Socket_);

        h_Socket_ = INVALID_SOCKET;
    }

    return true;
}

bool BerkeleySocket::GetSockName (SocketAddress& addr) const
{
    socklen_t n_Length = addr.Capacity();

    return (getsockname (h_Socket_, addr, &n_Length) == 0);
}

bool BerkeleySocket::GetPeerName (SocketAddress& addr) const
{
    socklen_t n_Length = addr.Capacity();

    return (getpeername (h_Socket_, addr, &n_Length) == 0);
}

bool BerkeleySocket::Bind (const SocketAddress& addr)
{
    return (bind (h_Socket_, addr, addr.Length()) == 0);
}

bool BerkeleySocket::Listen (uintsys u_Backlog)
{
    return (listen (h_Socket_, u_Backlog) == 0);
}

SOCKET BerkeleySocket::Accept (SocketAddress& addr)
{
    socklen_t n_Length = addr.Capacity();

    return accept (h_Socket_, addr, &n_Length);
}

bool BerkeleySocket::Connect (const SocketAddress& addr)
{
    return (connect (h_Socket_, addr, addr.Length()) == 0);
}

bool BerkeleySocket::Shutdown (intsys n_How)
{
    return (shutdown (h_Socket_, n_How) == 0);
}

intsys BerkeleySocket::Recv (void* p_Buffer, uintsys u_BufferSize,
                             intsys n_Flags)
{
    intsys n_Return = recv (h_Socket_, reinterpret_cast<char*>(p_Buffer),
                            u_BufferSize, n_Flags);

    if (SOCKET_OP_FAILED(n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

intsys BerkeleySocket::RecvFrom (void* p_Buffer, uintsys u_BufferSize,
                                 intsys n_Flags, SocketAddress& addr_Peer)
{
    socklen_t n_Length = addr_Peer.Capacity();

    intsys n_Return = recvfrom (h_Socket_, reinterpret_cast<char*>(p_Buffer),
                                u_BufferSize, n_Flags, addr_Peer, &n_Length);

    if (SOCKET_OP_FAILED(n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

intsys BerkeleySocket::Send (const void* p_Buffer, uintsys u_BufferSize,
                             intsys n_Flags)
{
    intsys n_Return = send (h_Socket_, reinterpret_cast<const char*>(p_Buffer),
                            u_BufferSize, n_Flags);

    if (SOCKET_OP_FAILED(n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

intsys BerkeleySocket::SendTo (const void* p_Buffer, uintsys u_BufferSize,
                               intsys n_Flags, const SocketAddress& addr_Dest)
{
    intsys n_Return = sendto (h_Socket_,
                              reinterpret_cast<const char*>(p_Buffer),
                              u_BufferSize, n_Flags, addr_Dest,
                              addr_Dest.Length());

    if (SOCKET_OP_FAILED(n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

intsys BerkeleySocket::GetPendingError () const
{
    intsys n_Error = 0;

    socklen_t n_Length = sizeof(n_Error);

    intsys n_Return = getsockopt (h_Socket_, SOL_SOCKET, SO_ERROR,
                               reinterpret_cast<char*>(&n_Error), &n_Length);

    if (n_Return < 0)
    {
        n_Error = errno;
    }

    return n_Error;
}

bool BerkeleySocket::EnableBroadcast (bool b_Enable)
{
    intsys yes = b_Enable ? 1 : 0;

    return (setsockopt (h_Socket_, SOL_SOCKET, SO_BROADCAST,
                        reinterpret_cast<const char*>(&yes),
                        sizeof(yes)) == 0);
}

bool BerkeleySocket::EnableKeepAlive (bool b_Enable)
{
    intsys yes = b_Enable ? 1 : 0;

    return (setsockopt (h_Socket_, SOL_SOCKET, SO_KEEPALIVE,
                        reinterpret_cast<const char*>(&yes),
                        sizeof(yes)) == 0);
}

bool BerkeleySocket::EnableLingerOption (bool b_Enable, intsys n_LingerTime)
{
    struct linger Linger;

    Linger.l_onoff  = b_Enable ? 1 : 0;
    Linger.l_linger = n_LingerTime;

    return (setsockopt (h_Socket_, SOL_SOCKET, SO_LINGER,
                        reinterpret_cast<const char*>(&Linger),
                        sizeof(Linger)) == 0);
}

bool BerkeleySocket::SetRecvBufferSize (intsys n_Size)
{
    return (setsockopt (h_Socket_, SOL_SOCKET, SO_RCVBUF,
                        reinterpret_cast<const char*>(&n_Size),
                        sizeof(n_Size)) == 0);
}

bool BerkeleySocket::SetSendBufferSize (intsys n_Size)
{
    return (setsockopt (h_Socket_, SOL_SOCKET, SO_SNDBUF,
                        reinterpret_cast<const char*>(&n_Size),
                        sizeof(n_Size)) == 0);
}

bool BerkeleySocket::ReuseAddress (bool b_Reuse)
{
    intsys yes = b_Reuse ? 1 : 0;

    return (setsockopt (h_Socket_, SOL_SOCKET, SO_REUSEADDR,
                        reinterpret_cast<const char*>(&yes),
                        sizeof(yes)) == 0);
}

bool BerkeleySocket::ReusePort (bool b_Reuse)
{
#ifdef SO_REUSEPORT
    intsys yes = b_Reuse ? 1 : 0;

    return (setsockopt (h_Socket_, SOL_SOCKET, SO_REUSEPORT,
                        reinterpret_cast<const char*>(&yes),
                        sizeof(yes)) == 0);
#else
    return false;
#endif
}

intsys BerkeleySocket::GetType () const
{
    intsys n_Type = 0;

    socklen_t n_Length = sizeof(n_Type);

    getsockopt (h_Socket_, SOL_SOCKET, SO_TYPE,
                reinterpret_cast<char*>(&n_Type), &n_Length);

    return n_Type;
}

bool BerkeleySocket::EnableTcpNoDelay (bool b_Enable)
{
    intsys yes = b_Enable ? 1 : 0;

    return setsockopt (h_Socket_, IPPROTO_TCP, TCP_NODELAY,
                       reinterpret_cast<const char*>(&yes),
                       sizeof(yes)) == 0;
}

intsys BerkeleySocket::Select (intsys n_Max, fd_set* set_Read,
                               fd_set* set_Write, fd_set* set_Error,
                               struct timeval* tv_Timeout)
{
    intsys n_Return = select (n_Max, set_Read, set_Write, set_Error,
                              tv_Timeout);

    if (SOCKET_OP_FAILED (n_Return))
    {
        n_Return = -1;
    }

    return n_Return;
}

} // namespace mikestoolbox

