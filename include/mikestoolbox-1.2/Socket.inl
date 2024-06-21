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
//  File:       Socket.inl
//
//  Synopsis:   Inline methods for Socket-based classes
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline Socket::~Socket ()
{
    Close_();
}

inline SOCKET Socket::GetHandle () const
{
    return socket_.GetHandle();
}

inline bool Socket::IsOpen () const
{
    return u_State_ & SOCKET_STATE_OPEN;
}

inline bool Socket::IsBound () const
{
    return u_State_ & SOCKET_STATE_BOUND;
}

inline bool Socket::IsConnecting () const
{
    return u_State_ & SOCKET_STATE_CONNECTING;
}

inline bool Socket::IsReadable () const
{
    return u_State_ & SOCKET_STATE_READABLE;
}

inline bool Socket::IsWritable () const
{
    return u_State_ & SOCKET_STATE_WRITABLE;
}

inline bool Socket::IsListening () const
{
    return u_State_ & SOCKET_STATE_LISTENING;
}

inline bool Socket::IsClosed () const
{
    return u_State_ & SOCKET_STATE_CLOSED;
}

inline bool Socket::TimedOut () const
{
    return u_State_ & SOCKET_STATE_TIMEDOUT;
}

inline void Socket::ResetTimeoutState () const
{
    u_State_ &= ~SOCKET_STATE_TIMEDOUT;
}

inline uintsys Socket::GetLastError () const
{
    return u_ErrorCode_;
}

inline void Socket::SetLastError_ (uintsys u_ErrorCode) const
{
    u_ErrorCode_ = u_ErrorCode;
}

inline void Socket::ClearError () const
{
    u_ErrorCode_ = ERROR_NO_ERROR;
}

inline intsys Socket::Send_ (const String& str_Data, intsys n_Flags)
{
    return Socket::Send_ ((const void*)str_Data.PointerToFirstChar(),
                          str_Data.Length(), n_Flags);
}

inline intsys Socket::SendTo_ (const String& str_Data, intsys n_Flags,
                               const SocketAddress& addr_Dest)
{
    return Socket::SendTo_ ((const void*)str_Data.PointerToFirstChar(),
                            str_Data.Length(), n_Flags, addr_Dest);
}

inline SocketAddress Socket::LocalAddress () const
{
    return addr_Local_;
}

inline SocketAddress Socket::PeerAddress () const
{
    return addr_Peer_;
}

inline void Socket::SetTimeout (double d_Timeout) const
{
    if (d_Timeout < 0.0)
    {
        d_Timeout = 0.0;
    }

    d_Timeout_ = d_Timeout;
}

inline void Socket::Cancel () const
{
    b_Canceled_ = true;
}

inline bool Socket::IsCanceled () const
{
    return b_Canceled_;
}

inline void Socket::ResetCancelState () const
{
    b_Canceled_ = false;
}

inline double Socket::BytesSent () const
{
    return d_BytesSent_;
}

inline double Socket::BytesReceived () const
{
    return d_BytesReceived_;
}

inline uintsys Socket::NumConnections () const
{
    return u_NumConnections_;
}

inline uintsys Socket::PacketsSent () const
{
    return u_PacketsSent_;
}

inline uintsys Socket::PacketsReceived () const
{
    return u_PacketsReceived_;
}

inline bool Socket::SelectRead (SocketList& list_ReadSockets, double d_Timeout)
{
    SocketList list_WriteSockets;

    return Select (list_ReadSockets, list_WriteSockets, d_Timeout);
}

inline bool Socket::SelectWrite (SocketList& list_WriteSockets, double d_Timeout)
{
    SocketList list_ReadSockets;

    return Select (list_ReadSockets, list_WriteSockets, d_Timeout);
}

inline bool TcpListener::Close ()
{
    return Socket::Close_();
}

inline TcpListener::~TcpListener ()
{
    Close();
}

inline void TcpListener::AllowIpAddresses_ (const List<IpAddressRange>& list)
{
    list_AllowedIps_.Append (list);
}

inline void TcpListener::BlockIpAddresses_ (const List<IpAddressRange>& list)
{
    list_BlockedIps_.Append (list);
}

inline bool TcpListener::Unblock ()
{
    return Socket::Unblock_();
}

inline void TcpListener::SetRecvChunkSize (uintsys u_Bytes)
{
    u_RecvChunkSize_ = u_Bytes;
}

inline void TcpListener::SetMaxReadAhead (uintsys u_Bytes)
{
    u_MaxReadAhead_ = u_Bytes;
}

inline void TcpListener::SetMaxLineLength (uintsys u_Length)
{
    u_MaxLineLength_ = u_Length;
}

inline void TcpListener::SetMaxMessageSize (uintsys u_Size)
{
    u_MaxMessageSize_ = u_Size;
}

inline void TcpListener::EnableKeepAlive (bool b_Enable)
{
    b_KeepAlive_ = b_Enable;
}

inline void TcpListener::EnableLingerOption (bool b_Enable, intsys n_LingerTime)
{
    b_Linger_     = b_Enable;
    n_LingerTime_ = n_LingerTime;
}

inline bool TcpListener::SetTcpRecvBufferSize (intsys n_Size)
{
    return Socket::SetRecvBufferSize_ (n_Size);
}

inline bool TcpListener::SetTcpSendBufferSize (intsys n_Size)
{
    return Socket::SetSendBufferSize_ (n_Size);
}

inline void TcpListener::EnableTcpNoDelayOption (bool b_Enable)
{
    b_NoDelay_ = b_Enable;
}

inline bool TcpListener::SetTcpMaxSegmentSize (intsys n_Size)
{
    return Socket::SetTcpMaxSegmentSize_ (n_Size);
}

inline bool TcpListener::Listen (intsys n_Backlog)
{
    if (IsListening())
    {
        SetLastError_ (ERROR_SOCKET_LISTENING);

        return false;
    }

    return Socket::Listen_ (n_Backlog);
}

inline TcpSocket::~TcpSocket ()
{
    Close();
}

inline void TcpSocket::SetServerDomainName_ (const String& str_DomainName)
{
    str_PeerDomainName_ = str_DomainName;
}

inline uintsys TcpSocket::GetLastError () const
{
    return Socket::GetLastError();
}

inline bool TcpSocket::ReadData (String& str_Data, double d_Timeout)
{
    return ReadData (0, str_Data, d_Timeout);
}

inline bool TcpSocket::SendDataNow (const String& str_Data, double d_Timeout)
{
    return SendData (str_Data, d_Timeout) && FlushOutput (d_Timeout);
}

inline bool TcpSocket::SendLineNow (const String& str_Line, double d_Timeout)
{
    return SendLine (str_Line, d_Timeout) && FlushOutput (d_Timeout);
}

inline void TcpSocket::SetRecvChunkSize (uintsys u_Bytes)
{
    u_RecvChunkSize_ = Maximum (128u, Minimum (4096u, u_Bytes));
}

inline void TcpSocket::SetMaxReadAhead (uintsys u_Bytes)
{
    u_MaxReadAhead_ = u_Bytes;
}

inline void TcpSocket::SetMaxLineLength (uintsys u_Length)
{
    u_MaxLineLength_ = u_Length;
}

inline void TcpSocket::SetMaxMessageSize (uintsys u_Size)
{
    u_MaxMessageSize_ = u_Size;
}

inline bool TcpSocket::EnableKeepAlive (bool b_Enable)
{
    return Socket::EnableKeepAlive_ (b_Enable);
}

inline bool TcpSocket::EnableLingerOption (bool b_Enable, intsys n_LingerTime)
{
    return Socket::EnableLingerOption_ (b_Enable, n_LingerTime);
}

inline bool TcpSocket::SetTcpRecvBufferSize (intsys n_Size)
{
    return Socket::SetRecvBufferSize_ (n_Size);
}

inline bool TcpSocket::SetTcpSendBufferSize (intsys n_Size)
{
    return Socket::SetSendBufferSize_ (n_Size);
}

inline bool TcpSocket::SetTcpMaxSegmentSize (intsys n_Size)
{
    return Socket::SetTcpMaxSegmentSize_ (n_Size);
}

inline intsys TcpSocket::GetTcpMaxSegmentSize () const
{
    return Socket::GetTcpMaxSegmentSize_();
}

inline bool TcpSocket::EnableTcpNoDelayOption (bool b_Enable)
{
    return Socket::EnableTcpNoDelay_ (b_Enable);
}

inline bool UdpSocket::Connect (const SocketAddress& addr_Peer)
{
    return Socket::Connect_ (addr_Peer);
}

inline bool UdpSocket::Close ()
{
    return Socket::Close_();
}

} // namespace mikestoolbox

