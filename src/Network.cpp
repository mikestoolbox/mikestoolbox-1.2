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
//  File:       Network.cpp
//
//  Synopsis:   Implementation of Network class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

TcpListener* Network::TcpListen (const SocketAddress& address)
{
    TcpListener* p_Socket = new(std::nothrow) TcpListener;

    if (p_Socket == 0)
    {
        SetLastError_ (ERROR_SYSTEM_OUT_OF_MEMORY);

        return 0;
    }

    if (p_Socket->Socket_       (address.Family()) &&
        p_Socket->ReuseAddress_ ()                 &&
        p_Socket->Bind_         (address))
    {
        p_Socket->AllowIpAddresses_ (list_AllowedIps_);
        p_Socket->BlockIpAddresses_ (list_BlockedIps_);

        return p_Socket;
    }

    SetLastError_ (p_Socket->GetLastError());

    p_Socket->Close();

    delete p_Socket;

    return 0;
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

bool Network::IsAddressBlocked (const SocketAddress& addr) const
{
    if (list_AllowedIps_.IsEmpty())
    {
        return AddressInRange (addr, list_BlockedIps_);
    }

    return !AddressInRange (addr, list_AllowedIps_) || AddressInRange (addr, list_BlockedIps_);
}

class AddressInfo
{
public:

    AddressInfo (int n_Family, int n_Type);
    ~AddressInfo ();

    void                SetNumeric      ();

    void                Clear           ();

    bool                Lookup          (const String& str_Host, const String& str_Port);

    void                Next            ();

    struct sockaddr*    Address         () const;
    int                 Family          () const;

                        operator bool   () const;

private:

    struct addrinfo  info_;
    struct addrinfo* p_Orig_;
    struct addrinfo* p_Current_;

    AddressInfo (const AddressInfo&);
    AddressInfo& operator= (const AddressInfo&);
};

inline AddressInfo::AddressInfo (int n_Family, int n_Type)
    : info_      ()
    , p_Orig_    (0)
    , p_Current_ (0)
{
    ZeroStructure (info_);

    info_.ai_family   = n_Family;
    info_.ai_socktype = n_Type;
}

inline void AddressInfo::Clear()
{
    if (p_Orig_)
    {
        freeaddrinfo (p_Orig_);

        p_Orig_    = 0;
        p_Current_ = 0;
    }
}

inline AddressInfo::~AddressInfo ()
{
    Clear();
}

inline void AddressInfo::SetNumeric ()
{
    info_.ai_flags = AI_NUMERICHOST;
}

inline bool AddressInfo::Lookup (const String& str_Host, const String& str_Port)
{
    Clear();

    if (getaddrinfo (str_Host.C(), str_Port.C(), &info_, &p_Orig_) == 0)
    {
        p_Current_ = p_Orig_;

        return true;
    }

    return false;
}

inline void AddressInfo::Next ()
{
    if (p_Current_)
    {
        p_Current_ = p_Current_->ai_next;
    }
}

inline AddressInfo::operator bool () const
{
    return p_Current_;
}

inline struct sockaddr* AddressInfo::Address () const
{
    return p_Current_ ? p_Current_->ai_addr : 0;
}

inline int AddressInfo::Family () const
{
    return p_Current_ ? p_Current_->ai_family : AF_UNSPEC;
}

bool Network::PrepareSocket_ (AddressInfo& hints, Socket* p_Socket)
{
    while (hints)
    {
        SocketAddress address (hints.Address());

        if (IsAddressBlocked (address))
        {
            SetLastError_ (ERROR_NETWORK_ADDRESS_BLOCKED);

            break;
        }
        else
        {
            if (p_Socket->Socket_  (hints.Family()) &&
                p_Socket->Connect_ (address)        &&
                p_Socket->Unblock_ ())
            {
                ClearError();

                p_Socket->ClearError();

                return true;
            }

            SetLastError_ (p_Socket->GetLastError());

            p_Socket->Close_();
        }

        hints.Next();
    }

    return false;
}

TcpSocket* Network::TcpConnect (const String& str_Destination)
{
    PerlRegexMatches matches;

    if (!str_Destination.Match (regex_ConnectString_, matches))
    {
        SetLastError_ (ERROR_NETWORK_BAD_CONNECT_STRING);

        return 0;
    }

    String str_Service (matches.GetMatch (3));
    String str_Host    (matches.GetMatch (4));
    String str_Port    (matches.GetMatch (6));

    if (str_Port.IsEmpty())
    {
        SetLastError_ (ERROR_NETWORK_PORT_UNSPECIFIED);

        return 0;
    }

    AddressInfo hints (AF_UNSPEC, SOCK_STREAM);

    if (str_Host.Replace (regex_IPv4_, "$1") >= 0)
    {
        hints.SetNumeric();
    }

    if (!hints.Lookup (str_Host, str_Port))
    {
        SetLastError_ (ERROR_NETWORK_LOOKUP_FAILED);

        return 0;
    }

    TcpSocket* p_Socket = new(std::nothrow) TcpSocket;

    if (p_Socket == 0)
    {
        SetLastError_ (ERROR_SYSTEM_OUT_OF_MEMORY);

        return 0;
    }

    if (PrepareSocket_ (hints, p_Socket))
    {
        p_Socket->SetServerDomainName_ (str_Host);

        return p_Socket;
    }

    delete p_Socket;

    return 0;
}

TcpSocket* Network::TcpConnect (const String& str_Peer, uint16 u_Port)
{
    String str_Destination (str_Peer);
    String str_PortSpec    (':');

    str_PortSpec += u_Port;

    str_Destination.Replace ("(:\\d+)?/?$", str_PortSpec);

    return TcpConnect (str_Destination);
}

UdpSocket* Network::UdpListen (const SocketAddress& address)
{
    UdpSocket* p_Socket = new(std::nothrow) UdpSocket;

    if (p_Socket == 0)
    {
        SetLastError_ (ERROR_SYSTEM_OUT_OF_MEMORY);

        return 0;
    }

    if (p_Socket->Socket_  (address.Family()) &&
        p_Socket->Unblock_ ()                 &&
        p_Socket->Bind_    (address))
    {
        p_Socket->u_State_ |= SOCKET_STATE_LISTENING
                           |  SOCKET_STATE_READABLE
                           |  SOCKET_STATE_WRITABLE;

        return p_Socket;
    }

    SetLastError_ (p_Socket->GetLastError());

    p_Socket->Close();

    delete p_Socket;

    return 0;
}

UdpSocket* Network::UdpConnect (const String& str_Destination)
{
    PerlRegexMatches matches;

    if (!str_Destination.Match (regex_ConnectString_, matches))
    {
        SetLastError_ (ERROR_NETWORK_BAD_CONNECT_STRING);

        return 0;
    }

    String str_Service (matches.GetMatch (3));
    String str_Host    (matches.GetMatch (4));
    String str_Port    (matches.GetMatch (6));

    if (str_Port.IsEmpty())
    {
        SetLastError_ (ERROR_NETWORK_PORT_UNSPECIFIED);

        return 0;
    }

    AddressInfo hints (AF_UNSPEC, SOCK_DGRAM);

    if (str_Host.Replace (regex_IPv4_, "$1") >= 0)
    {
        hints.SetNumeric();
    }

    if (!hints.Lookup (str_Host, str_Port))
    {
        SetLastError_ (ERROR_NETWORK_LOOKUP_FAILED);

        return 0;
    }

    UdpSocket* p_Socket = new(std::nothrow) UdpSocket;

    if (p_Socket == 0)
    {
        SetLastError_ (ERROR_SYSTEM_OUT_OF_MEMORY);

        return 0;
    }

    if (PrepareSocket_ (hints, p_Socket))
    {
        return p_Socket;
    }

    delete p_Socket;

    return 0;
}

UdpSocket* Network::UdpConnect (const String& str_Peer, uint16 u_Port)
{
    String str_Destination (str_Peer);
    String str_PortSpec    (':');

    str_PortSpec += u_Port;

    str_Destination.Replace ("(:\\d+)?/?$", str_PortSpec);

    return UdpConnect (str_Destination);
}

} // namespace mikestoolbox

