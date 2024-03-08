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
//  File:       Socket.cpp
//
//  Synopsis:   Implementation of Socket classes
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

static const uintsys SOCKET_RECV_CHUNK_SIZE =  512;
static const uintsys SOCKET_MAX_READ_AHEAD  = 1600;

Socket::Socket (SOCKET h_Socket)
    : socket_            (h_Socket)
    , addr_Local_        ()
    , addr_Peer_         ()
    , d_BytesSent_       (0.0)
    , d_BytesReceived_   (0.0)
    , u_PacketsSent_     (0)
    , u_PacketsReceived_ (0)
    , u_NumConnections_  (0)
    , d_Timeout_         (300)
    , u_State_           (SOCKET_STATE_NONE)
    , u_ErrorCode_       (ERROR_NO_ERROR)
    , b_Canceled_        (false)
{
    u_State_ = socket_ ? SOCKET_STATE_OPEN : SOCKET_STATE_NONE;
}

bool Socket::Close_ ()
{
    u_State_ = SOCKET_STATE_CLOSED;

    return socket_.Close();
}

static int CreateFD_SET (fd_set& set, const SocketList& list, bool& b_Canceled)
{
    if (b_Canceled)
    {
        return 0;
    }

    SOCKET max = 0;

    FD_ZERO (&set);

    ListIter<Socket*> iter (list);

    while (iter)
    {
        Socket* p_Socket = *iter++;

        if (p_Socket != 0)
        {
            if (p_Socket->IsCanceled())
            {
                b_Canceled = true;

                break;
            }

            SOCKET h = p_Socket->GetHandle();

            max = Maximum (h, max);

            FD_SET (h, &set);

            p_Socket->ResetTimeoutState();
        }
    }

    return (int) max;
}

void Socket::MarkAsTimedOut () const
{
    u_State_ |= SOCKET_STATE_TIMEDOUT;

    SetLastError_ (ERROR_SOCKET_TIMEDOUT);
}

static void RemoveEmptySockets (SocketList& list)
{
    ListChangeIter<Socket*> iter (list);

    while (iter)
    {
        Socket* p_Socket = *iter;

        if (p_Socket == 0)
        {
            list.Erase (iter);
        }
        else
        {
            ++iter;
        }
    }
}

static void CreateTimevalMax2Seconds (double d, struct timeval& tv)
{
    if ((d < 0.0) || (d >= 2.0))
    {
        tv.tv_sec  = 2;
        tv.tv_usec = 0;
    }
    else
    {
        uintsys u = d;

        tv.tv_sec  = u;
        tv.tv_usec = (uintsys) (1000000.0 * (d - u));
    }
}

static void MarkSocketsTimedOut (const SocketList& list)
{
    ListIter<Socket*> iter (list);

    while (iter)
    {
        Socket* p_Socket = *iter++;

        p_Socket->MarkAsTimedOut();
    }
}

static void RemoveUnsetSockets (const fd_set& set, SocketList& list)
{
    ListChangeIter<Socket*> iter (list);

    while (iter)
    {
        Socket* p_Socket = *iter;

        if (!FD_ISSET (p_Socket->GetHandle(), &set))
        {
            list.Erase (iter);
        }
        else
        {
            ++iter;
        }
    }
}

bool Socket::Select (SocketList& ReadSockets, SocketList& WriteSockets, double d_Timeout)
{
    int n_Return = -1;

    fd_set set_Read;
    fd_set set_Write;

    bool b_Canceled = false;
    bool b_TimedOut = true;
    bool b_Forever  = (d_Timeout < 0.0);

    struct timeval tv_Timeout;

    Date date_Now = Date::Now();
    Date date_Exp = date_Now;

    date_Exp.AddSeconds (d_Timeout);

    RemoveEmptySockets (ReadSockets);
    RemoveEmptySockets (WriteSockets);

    do
    {
        errno = 0;

        int n_Max = Maximum (CreateFD_SET (set_Read,  ReadSockets,  b_Canceled),
                             CreateFD_SET (set_Write, WriteSockets, b_Canceled));

        if (b_Canceled)
        {
            break;
        }

        CreateTimevalMax2Seconds (date_Exp.SecondsMoreThan (date_Now), tv_Timeout);

        n_Return = BerkeleySocket::Select (n_Max+1, &set_Read, &set_Write, 0, &tv_Timeout);

        if (n_Return != 0)
        {
            b_TimedOut = false;

            break;
        }

        date_Now = Date::Now();
    }
    while (b_Forever || (date_Now <= date_Exp));

    if (b_Canceled || b_TimedOut || (n_Return == 0) || (n_Return == -1))
    {
        if (b_TimedOut)
        {
            MarkSocketsTimedOut (ReadSockets);
            MarkSocketsTimedOut (WriteSockets);
        }

        ReadSockets.Clear();
        WriteSockets.Clear();

        return false;
    }

    if (n_Return < (int)(ReadSockets.NumItems() + WriteSockets.NumItems()))
    {
        RemoveUnsetSockets (set_Read,  ReadSockets);
        RemoveUnsetSockets (set_Write, WriteSockets);
    }

    return true;
}

bool Socket::CreateSocket_ (int n_Family, int n_Type, int n_Protocol)
{
    socket_.Close();

    if (socket_.CreateSocket (n_Family, n_Type, n_Protocol))
    {
        ClearError();

        u_State_ = SOCKET_STATE_OPEN;

        return true;
    }

    SetLastError_ (ERROR_SOCKET_CREATE_FAILED);

    return false;
}

bool Socket::CreateTCPSocket_ (int n_Family)
{
    socket_.Close();

    if (socket_.CreateTCPSocket (n_Family))
    {
        ClearError();

        u_State_ = SOCKET_STATE_OPEN;

        return true;
    }

    SetLastError_ (ERROR_SOCKET_CREATE_FAILED);

    return false;
}

bool Socket::CreateUDPSocket_ (int n_Family)
{
    socket_.Close();

    if (socket_.CreateUDPSocket (n_Family))
    {
        ClearError();

        u_State_ = SOCKET_STATE_OPEN;

        return true;
    }

    SetLastError_ (ERROR_SOCKET_CREATE_FAILED);

    return false;
}

bool Socket::Unblock_ ()
{
    return socket_.Unblock();
}

bool Socket::GetSockName_ ()
{
    if (!IsBound())
    {
        SetLastError_ (ERROR_SOCKET_NOT_BOUND);

        return false;
    }

    return socket_.GetSockName (addr_Local_);
}

bool Socket::GetPeerName_ ()
{
    if (!IsConnected())
    {
        SetLastError_ (ERROR_SOCKET_NOT_CONNECTED);

        return false;
    }

    return socket_.GetPeerName (addr_Peer_);
}

bool Socket::Bind_ (const SocketAddress& addr)
{
    if (!IsOpen())
    {
        SetLastError_ (ERROR_SOCKET_NOT_OPEN);

        return false;
    }

    if (IsBound())
    {
        SetLastError_ (ERROR_SOCKET_BOUND);

        return false;
    }

    if (socket_.Bind (addr))
    {
        u_State_ |= SOCKET_STATE_BOUND;

        ClearError();

        GetSockName_();

        return true;
    }

    SetLastError_ (ERROR_SOCKET_BIND_FAILED);

    return false;
}

bool Socket::Listen_ (uint u_Backlog)
{
    if (IsListening())
    {
        SetLastError_ (ERROR_SOCKET_LISTENING);

        return false;
    }

    if (!IsBound())
    {
        SetLastError_ (ERROR_SOCKET_NOT_BOUND);

        return false;
    }

    if (socket_.Listen (u_Backlog))
    {
        u_State_ |= SOCKET_STATE_LISTENING;

        ClearError();

        return true;
    }

    SetLastError_ (ERROR_SOCKET_LISTEN_FAILED);

    return false;
}

SOCKET Socket::Accept_ (SocketAddress& addr)
{
    if (!IsListening())
    {
        SetLastError_ (ERROR_SOCKET_NOT_LISTENING);

        return INVALID_SOCKET;
    }

    ClearError();

    SOCKET h_NewSocket = socket_.Accept (addr);

    if (h_NewSocket == INVALID_SOCKET)
    {
        SetLastError_ (ERROR_SOCKET_ACCEPT_FAILED);
    }
    else
    {
        ++u_NumConnections_;
    }

    return h_NewSocket;
}

bool Socket::Connect_ (const SocketAddress& addr)
{
    ClearError();

    if (!IsOpen())
    {
        SetLastError_ (ERROR_SOCKET_NOT_OPEN);

        return false;
    }

    if (IsConnecting())
    {
        SetLastError_ (ERROR_SOCKET_CONNECTING);

        return false;
    }

    if (socket_.Connect (addr))
    {
        GetSockName_();
        GetPeerName_();

        u_State_ |= SOCKET_STATE_CONNECTED |
                    SOCKET_STATE_READABLE  |
                    SOCKET_STATE_WRITABLE  |
                    SOCKET_STATE_BOUND;

        u_State_ &= ~SOCKET_STATE_CONNECTING;

        return true;
    }

#ifdef PLATFORM_WINDOWS
    if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
    if (errno == EINPROGRESS)
#endif
    {
        u_State_ |= SOCKET_STATE_CONNECTING;

        return false;
    }

    SetLastError_ (ERROR_SOCKET_CONNECT_FAILED);

    return false;
}

bool Socket::IsConnected (double d_Timeout) const
{
    ClearError();

    if (u_State_ & SOCKET_STATE_CONNECTED)
    {
        return true;
    }

    if (u_State_ & SOCKET_STATE_CONNECTING)
    {
        if (d_Timeout < 0.0)
        {
            d_Timeout = d_Timeout_;
        }

        fd_set ReadSet;
        fd_set WriteSet;

        struct timeval tv_Timeout;

        Date date_Now = Date::Now();
        Date date_Exp = date_Now;

        date_Exp.AddSeconds (d_Timeout);

        SOCKET sock = GetHandle();
        int    max  = sock + 1;

        ResetTimeoutState();

        do
        {
            if (IsCanceled())
            {
                SetLastError_ (ERROR_SOCKET_CANCELED);

                return false;
            }

            FD_ZERO (&ReadSet);
            FD_ZERO (&WriteSet);

            FD_SET (sock, &ReadSet);
            FD_SET (sock, &WriteSet);

            errno = 0;

            CreateTimevalMax2Seconds (date_Exp.SecondsMoreThan (date_Now), tv_Timeout);

            int n_Return = BerkeleySocket::Select (max, &ReadSet, &WriteSet, 0, &tv_Timeout);

            if (n_Return > 0)
            {
                int n_Error = GetPendingError_();

                u_State_ &= ~SOCKET_STATE_CONNECTING;

                if (n_Error == 0)
                {
                    u_State_ |= SOCKET_STATE_CONNECTED | SOCKET_STATE_BOUND |
                                SOCKET_STATE_READABLE  | SOCKET_STATE_WRITABLE;

                    return true;
                }

                u_ErrorCode_ = ERROR_SOCKET_CONNECT_FAILED;

                return false;
            }

            if ((n_Return < 0) && (errno != EINTR))
            {
                u_ErrorCode_ = ERROR_SOCKET_SELECT_FAILED;

                return false;
            }

            date_Now = Date::Now();
        }
        while (date_Now <= date_Exp);

        MarkAsTimedOut();
    }

    return false;
}

bool Socket::Shutdown_ (int n_How)
{
    ClearError();

    if (!IsConnected())
    {
        SetLastError_ (ERROR_SOCKET_NOT_CONNECTED);

        return false;
    }

    switch (n_How)
    {
        case SHUT_RD:
        {
            if (!IsReadable())
            {
                SetLastError_ (ERROR_SOCKET_NOT_READABLE);

                return false;
            }

            if (socket_.Shutdown (SHUT_RD))
            {
                u_State_ &= ~SOCKET_STATE_READABLE;

                return true;
            }

            break;
        }

        case SHUT_WR:
        {
            if (!IsWritable())
            {
                SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

                return false;
            }

            if (socket_.Shutdown (SHUT_WR))
            {
                u_State_ &= ~SOCKET_STATE_WRITABLE;

                return true;
            }

            break;
        }

        case SHUT_RDWR:
        {
            if (!IsOpen())
            {
                SetLastError_ (ERROR_SOCKET_NOT_OPEN);

                return false;
            }

            if (socket_.Shutdown (SHUT_RDWR))
            {
                u_State_ &= ~SOCKET_STATE_READABLE;
                u_State_ &= ~SOCKET_STATE_WRITABLE;

                return true;
            }

            break;
        }
    }

    SetLastError_ (ERROR_SOCKET_SHUTDOWN_FAILED);

    return false;
}

intsys Socket::Recv_ (void* p_Buffer, uintsys u_BufferSize, int n_Flags)
{
    if (!IsReadable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_READABLE);

        return -1;
    }

    intsys n_Return = socket_.Recv (p_Buffer, u_BufferSize, n_Flags);

    if (n_Return < 0)
    {
        u_State_ &= ~SOCKET_STATE_READABLE;

        SetLastError_ (ERROR_SOCKET_READ_FAILED);
    }
    else if (n_Return > 0)
    {
        ClearError();

        ++u_PacketsReceived_;
        d_BytesReceived_ += n_Return;
    }

    return n_Return;
}

intsys Socket::RecvFrom_ (void* p_Buffer, uintsys u_BufferSize, int n_Flags,
                          SocketAddress& addr_Peer)
{
    if (!IsReadable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_READABLE);

        return -1;
    }

    intsys n_Return = socket_.RecvFrom (p_Buffer, u_BufferSize, n_Flags, addr_Peer);

    if (n_Return < 0)
    {
        u_State_ &= ~SOCKET_STATE_READABLE;

        SetLastError_ (ERROR_SOCKET_READ_FAILED);
    }
    else if (n_Return > 0)
    {
        ClearError();

        ++u_PacketsReceived_;
        d_BytesReceived_ += n_Return;
    }

    return n_Return;
}

intsys Socket::Send_ (const void* p_Buffer, uintsys u_BufferSize, int n_Flags)
{
    if (!IsWritable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

        return -1;
    }

    intsys n_Return = socket_.Send (p_Buffer, u_BufferSize, n_Flags);

    if (n_Return < 0)
    {
        u_State_ &= ~SOCKET_STATE_WRITABLE;

        SetLastError_ (ERROR_SOCKET_SEND_FAILED);
    }
    else if (n_Return > 0)
    {
        ClearError();

        ++u_PacketsSent_;
        d_BytesSent_ += n_Return;
    }

    return n_Return;
}

intsys Socket::Send_ (const StringList& strl_Data, int n_Flags)
{
    if (!IsWritable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

        return -1;
    }

    intsys n_Return = socket_.Send (strl_Data, n_Flags);

    if (n_Return < 0)
    {
        u_State_ &= ~SOCKET_STATE_WRITABLE;

        SetLastError_ (ERROR_SOCKET_SEND_FAILED);
    }
    else if (n_Return > 0)
    {
        ClearError();

        ++u_PacketsSent_;
        d_BytesSent_ += n_Return;
    }

    return n_Return;
}

intsys Socket::SendTo_ (const void* p_Buffer, uintsys u_BufferSize,
                        int n_Flags, const SocketAddress& addr_Dest)
{
    if (!IsWritable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

        return -1;
    }

    intsys n_Return = socket_.SendTo (p_Buffer, u_BufferSize, n_Flags, addr_Dest);

    if (n_Return < 0)
    {
        u_State_ &= ~SOCKET_STATE_WRITABLE;

        SetLastError_ (ERROR_SOCKET_SEND_FAILED);
    }
    else if (n_Return > 0)
    {
        ClearError();

        ++u_PacketsSent_;
        d_BytesSent_ += n_Return;
    }

    return n_Return;
}

int Socket::GetPendingError_ () const
{
    return socket_.GetPendingError();
}

bool Socket::EnableBroadcast_ (bool b_Enable)
{
    return socket_.EnableBroadcast (b_Enable);
}

bool Socket::EnableKeepAlive_ (bool b_Enable)
{
    return socket_.EnableKeepAlive (b_Enable);
}

bool Socket::EnableLingerOption_ (bool b_Enable, int n_LingerTime)
{
    return socket_.EnableLingerOption (b_Enable, n_LingerTime);
}

bool Socket::SetRecvBufferSize_ (int n_Size)
{
    return socket_.SetRecvBufferSize (n_Size);
}

bool Socket::SetSendBufferSize_ (int n_Size)
{
    return socket_.SetSendBufferSize (n_Size);
}

bool Socket::ReuseAddress_ (bool b_Reuse)
{
    return socket_.ReuseAddress (b_Reuse);
}

bool Socket::ReusePort_ (bool b_Reuse)
{
    return socket_.ReusePort (b_Reuse);
}

int Socket::GetType_ () const
{
    return socket_.GetType();
}

int Socket::GetTcpMaxSegmentSize_ () const
{
    return socket_.GetTcpMaxSegmentSize();
}

bool Socket::SetTcpMaxSegmentSize_ (int n_Size)
{
    return socket_.SetTcpMaxSegmentSize (n_Size);
}

bool Socket::EnableTcpNoDelay_ (bool b_Enable)
{
    return socket_.EnableTcpNoDelay (b_Enable);
}

TcpListener::TcpListener (SOCKET h_Socket)
    : Socket               (h_Socket)
    , mutex_Accept_        ()
    , u_RecvChunkSize_     (SOCKET_RECV_CHUNK_SIZE)
    , u_MaxReadAhead_      (SOCKET_MAX_READ_AHEAD)
    , u_MaxLineLength_     (0)
    , u_MaxMessageSize_    (0)
    , b_KeepAlive_         (false)
    , b_Linger_            (false)
    , n_LingerTime_        (0)
    , b_NoDelay_           (false)
    , list_AllowedIps_     ()
    , list_BlockedIps_     ()
{
    // nothing
}

bool TcpListener::Socket_ (int n_Family)
{
    return Socket::CreateTCPSocket_ (n_Family);
}

bool TcpListener::SetTcpMaxSegmentSize (int n_Size)
{
    return Socket::SetTcpMaxSegmentSize_ (n_Size);
}

TcpSocket::TcpSocket (SOCKET h_Socket)
    : Socket                       (h_Socket)
    , str_PeerDomainName_          ()
    , strl_ReadBuffer_             ()
    , strl_SendBuffer_             ()
    , u_RecvChunkSize_             (SOCKET_RECV_CHUNK_SIZE)
    , u_MaxReadAhead_              (SOCKET_MAX_READ_AHEAD)
    , u_MaxLineLength_             (0)
    , u_MaxMessageSize_            (0)
    , p_Extra_                     (0)
{
    // nothing
}

static bool AddressInRange (const SocketAddress& address, const List<IpAddressRange>& list_Range)
{
    if (list_Range.IsEmpty())
    {
        return false;
    }

    ListIter<IpAddressRange> iter (list_Range);

    while (iter)
    {
        const IpAddressRange& range = *iter;

        if (range.Contains (address))
        {
            return true;
        }

        ++iter;
    }

    return false;
}

bool TcpListener::IsAddressBlocked (const SocketAddress& addr) const
{
    if (list_AllowedIps_.IsEmpty())
    {
        return AddressInRange (addr, list_BlockedIps_);
    }

    return !AddressInRange (addr, list_AllowedIps_) || AddressInRange (addr, list_BlockedIps_);
}

bool TcpListener::SetSocketOptions_ (TcpSocket* p_Socket)
{
    p_Socket->SetRecvChunkSize  (u_RecvChunkSize_);
    p_Socket->SetMaxReadAhead   (u_MaxReadAhead_);
    p_Socket->SetMaxLineLength  (u_MaxLineLength_);
    p_Socket->SetMaxMessageSize (u_MaxMessageSize_);

    return ((!b_KeepAlive_ || p_Socket->EnableKeepAlive())        &&
            (!b_NoDelay_   || p_Socket->EnableTcpNoDelayOption()) &&
            (!b_Linger_    || p_Socket->EnableLingerOption (b_Linger_, n_LingerTime_)));
}

TcpSocket* TcpListener::Accept ()
{
    if (!IsListening())
    {
        SetLastError_ (ERROR_SOCKET_NOT_LISTENING);

        return 0;
    }

    SocketAddress address;
    SOCKET        h_Socket = INVALID_SOCKET;

    {
        MutexLocker locker (mutex_Accept_);

        if (IsCanceled())
        {
            SetLastError_ (ERROR_SOCKET_CANCELED);

            return 0;
        }

        h_Socket = Socket::Accept_ (address);
    }

    if (h_Socket == INVALID_SOCKET)
    {
        return 0;
    }

    if (IsAddressBlocked (address))
    {
        BerkeleySocket::CloseSocket (h_Socket);

        SetLastError_ (ERROR_SOCKET_ADDRESS_BLOCKED);

        return 0;
    }

    TcpSocket* p_Socket = new(std::nothrow) TcpSocket (h_Socket);

    if (p_Socket == 0)
    {
        BerkeleySocket::CloseSocket (h_Socket);

        SetLastError_ (ERROR_SYSTEM_OUT_OF_MEMORY);

        return 0;
    }

    p_Socket->u_State_ = SOCKET_STATE_OPEN     | SOCKET_STATE_BOUND |
                         SOCKET_STATE_READABLE | SOCKET_STATE_WRITABLE |
                         SOCKET_STATE_CONNECTED;

    p_Socket->addr_Peer_ = address;

    if (!(p_Socket->GetSockName_() && p_Socket->Unblock_() && SetSocketOptions_ (p_Socket)))
    {
        SetLastError_ (p_Socket->GetLastError());

        delete p_Socket;

        return 0;
    }

    return p_Socket;
}

bool TcpSocket::Socket_ (int n_Family)
{
    return Socket::CreateTCPSocket_ (n_Family);
}

intsys TcpSocket::RecvData_ ()
{
    String str_Buffer;

    uintsys u_ChunkSize = Maximum (u_RecvChunkSize_, 128u);

    uchar* p_Buffer = str_Buffer.Allocate (u_ChunkSize);

    intsys n_BytesRead = Socket::Recv_ (p_Buffer, u_ChunkSize);

    if (n_BytesRead > 0)
    {
        str_Buffer.Truncate (n_BytesRead);

        strl_ReadBuffer_.Append (str_Buffer);
    }
    else if (n_BytesRead == 0)
    {
        Socket::SetLastError_ (ERROR_SOCKET_CLOSED);
    }

    return n_BytesRead;
}

bool TcpSocket::HaveDataToSend_ () const
{
    StringListIter iter (strl_SendBuffer_);

    while (iter)
    {
        const String& str (*iter);

        ++iter;

        if (!str.IsEmpty())
        {
            return true;
        }
    }

    return false;
}

intsys TcpSocket::ReadData_ (double d_Timeout)
{
    if (!IsReadable())
    {
        return -1;
    }

    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    bool b_SendError = false;

    SocketList list_ReadSockets (this);
    SocketList list_WriteSockets;

    for (;;)
    {
        if (!b_SendError && HaveDataToSend_())
        {
            list_WriteSockets.Append (this);
        }

        if (!Select (list_ReadSockets, list_WriteSockets, d_Timeout))
        {
            return 0;
        }

        if (!list_WriteSockets.IsEmpty())
        {
            intsys n_Sent = Socket::Send_ (strl_SendBuffer_);

            if (n_Sent > 0)
            {
                strl_SendBuffer_.EraseFrontBytes (n_Sent);
            }
            else
            {
                b_SendError = true;
            }

            list_WriteSockets.Clear();
        }

        if (list_ReadSockets.IsEmpty())
        {
            list_ReadSockets.Append (this);
        }
        else
        {
            return RecvData_();
        }
    }
}

bool TcpSocket::ReadData (uintsys u_NumBytes, String& str_Data, double d_Timeout)
{
    if (u_NumBytes == 0)
    {
        if (strl_ReadBuffer_.IsEmpty())
        {
            if (ReadData_ (d_Timeout) <= 0)
            {
                return false;
            }
        }

        str_Data = strl_ReadBuffer_.Join();

        strl_ReadBuffer_.Clear();

        return true;
    }

    uintsys u_Buffered = strl_ReadBuffer_.Size();

    while (u_Buffered < u_NumBytes)
    {
        intsys n_BytesRead = ReadData_ (d_Timeout);

        if (n_BytesRead <= 0)
        {
            return false;
        }

        u_Buffered += n_BytesRead;
    }

    StringListByteIter iter_Buffer (strl_ReadBuffer_);
    StringList strl_Remainder;

    ParseError error;

    if (iter_Buffer.Extract (u_NumBytes, str_Data, error) &&
        iter_Buffer.Extract (strl_Remainder))
    {
        strl_ReadBuffer_.Swap (strl_Remainder);

        return true;
    }

    return false;
}

bool TcpSocket::ReadLine (String& str_Line, double d_Timeout)
{
    ParseError error;

    for (;;)
    {
        StringListByteIter iter (strl_ReadBuffer_);

        if (iter.ExtractUpTo ('\n', str_Line, error))
        {
            StringList strl_Remainder;

            iter.Extract (strl_Remainder);

            strl_ReadBuffer_.Swap (strl_Remainder);

            return true;
        }

        uintsys u_Size = strl_ReadBuffer_.Size();

        if ((u_MaxLineLength_ > 0) && (u_Size >= u_MaxLineLength_))
        {
            SetLastError_ (ERROR_SOCKET_LINE_TOO_LONG);

            return false;
        }

        if (ReadData_ (d_Timeout) <= 0)
        {
            return false;
        }
    }
}

bool TcpSocket::ReadMultiLine (StringList& strl_Lines, double d_Timeout)
{
    strl_Lines.Clear();

    String str_EOM1 (".\r\n");
    String str_EOM2 (".\n");

    uintsys u_MessageSize = 0;

    for (;;)
    {
        String str_Line;

        if (!ReadLine (str_Line, d_Timeout))
        {
            return false;
        }

        if ((str_Line == str_EOM1) || (str_Line == str_EOM2))
        {
            return true;
        }

        if (str_Line[0] == '.')
        {
            str_Line.EraseFront();
        }

        strl_Lines.Append (str_Line);

        u_MessageSize += str_Line.Length();

        if (u_MaxMessageSize_ && (u_MessageSize > u_MaxMessageSize_))
        {
            SetLastError_ (ERROR_SOCKET_MESSAGE_TOO_BIG);

            return false;
        }
    }
}

bool TcpSocket::IsReadBufferFull_ () const
{
    return (u_MaxReadAhead_ == 0) || (strl_ReadBuffer_.Size() >= u_MaxReadAhead_);
}

bool TcpSocket::PartialFlush_ (uintsys u_MaxSends, double d_Timeout)
{
    if (!HaveDataToSend_())
    {
        return true;
    }

    SocketList list_ReadSockets;
    SocketList list_WriteSockets (this);

    uintsys u_NumSends  = 0;
    bool    b_ReadError = false;

    for (;;)
    {
        if (!(b_ReadError || IsReadBufferFull_()))
        {
            list_ReadSockets.Append (this);
        }

        if (!Select (list_ReadSockets, list_WriteSockets, d_Timeout))
        {
            return false;
        }

        if (!list_ReadSockets.IsEmpty())
        {
            b_ReadError = (RecvData_() <= 0);

            list_ReadSockets.Clear();
        }

        if (list_WriteSockets.IsEmpty())
        {
            list_WriteSockets.Append (this);
        }
        else
        {
            intsys n_Sent = Socket::Send_ (strl_SendBuffer_);

            if (n_Sent <= 0)
            {
                return false;
            }

            strl_SendBuffer_.EraseFrontBytes (n_Sent);

            if (++u_NumSends >= u_MaxSends)
            {
                return true;
            }

            if (!HaveDataToSend_())
            {
                return true;
            }
        }
    }
}

bool TcpSocket::FlushOutput (double d_Timeout)
{
    if (!IsWritable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

        return false;
    }

    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    return PartialFlush_ (MAX_UINTSYS, d_Timeout);
}

bool TcpSocket::SendData (const String& str_Data, double d_Timeout)
{
    strl_SendBuffer_.Append (str_Data);

    if (!IsWritable())
    {
        SetLastError_ (ERROR_SOCKET_NOT_WRITABLE);

        return false;
    }

    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    return (strl_SendBuffer_.Size() < 7500) || PartialFlush_ (1, d_Timeout);
}

bool TcpSocket::SendLine (const String& str_Line, double d_Timeout)
{
    String str_CRLF ("\r\n");

    if (str_Line.EndsWith (str_CRLF))
    {
        return SendData (str_Line, d_Timeout);
    }

    String str_LineToSend (str_Line);

    if (str_LineToSend.EndsWith ('\n'))
    {
        str_LineToSend.EraseEnd();

        str_LineToSend += str_CRLF;
    }
    else if (str_LineToSend.EndsWith ('\r'))
    {
        str_LineToSend += '\n';
    }
    else
    {
        str_LineToSend += str_CRLF;
    }

    return SendData (str_LineToSend, d_Timeout);
}

bool TcpSocket::SendMultiLine (const StringList& strl_Lines, double d_Timeout)
{
    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    String str_Dot ('.');

    StringListIter iter (strl_Lines);

    while (iter)
    {
        const String& str_Line (*iter);

        ++iter;

        if (str_Line.StartsWith ('.'))
        {
            SendData (str_Dot);
        }

        if (!SendLine (str_Line, d_Timeout))
        {
            if (d_Timeout != 0.0)
            {
                return false;
            }
        }
    }

    String str_EOM (".\r\n");

    return SendDataNow (str_EOM, d_Timeout);
}

bool TcpSocket::Close ()
{
    if (FlushOutput())
    {
        return Socket::Close_();
    }

    Socket::Close_();

    return false;
}

UdpSocket::UdpSocket (SOCKET h_Socket)
    : Socket (h_Socket)
{
    // nothing
}

bool UdpSocket::Socket_ (int n_Family)
{
    return Socket::CreateUDPSocket_ (n_Family);
}

bool UdpSocket::ReadData (String& str_Data, SocketAddress& addr_Peer, double d_Timeout)
{
    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    SocketList list (this);

    if (!SelectRead (list, d_Timeout))
    {
        return false;
    }

    const uintsys u_BufferSize = 2000;    // should be configurable

    uchar* p_Data = str_Data.Allocate (u_BufferSize);

    intsys n_Return = RecvFrom_ (p_Data, u_BufferSize, 0, addr_Peer);

    if (n_Return < 0)
    {
        return false;
    }

    str_Data.Truncate (n_Return);

    return true;
}

bool UdpSocket::SendData (const String& str_Data, double d_Timeout)
{
    if (!IsConnected())
    {
        SetLastError_ (ERROR_SOCKET_NOT_CONNECTED);

        return false;
    }

    if (d_Timeout < 0.0)
    {
        d_Timeout = d_Timeout_;
    }

    uintsys u_Length = str_Data.Length();

    if (u_Length == 0)
    {
        return true;
    }

    SocketList list (this);

    if (!SelectWrite (list, d_Timeout))
    {
        return false;
    }

    intsys n_Return = Socket::Send_ (str_Data);

    return (n_Return == (intsys)u_Length);
}

bool UdpSocket::SendDataTo (const String& str_Data, const SocketAddress& addr_Peer,
                            double d_Timeout)
{
    if (d_Timeout < 0)
    {
        d_Timeout = d_Timeout_;
    }

    SocketList list (this);

    if (!SelectWrite (list, d_Timeout))
    {
        return false;
    }

    intsys n_Return = Socket::SendTo_ ((const void*)str_Data.C(), str_Data.Length(), 0, addr_Peer);

    return (n_Return >= 0) && ((uintsys)n_Return == str_Data.Length());
}

} // namespace mikestoolbox

