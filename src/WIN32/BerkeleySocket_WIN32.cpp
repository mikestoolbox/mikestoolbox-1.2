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

#define SOCKET_OP_FAILED(x)  ((x) == SOCKET_ERROR)

#ifndef SOCKET_MESSAGE_SIZE
#define SOCKET_MESSAGE_SIZE 32
#endif

namespace mikestoolbox {

class SocketStartup
{
public:

    SocketStartup ();
    ~SocketStartup ();
};

inline SocketStartup::SocketStartup ()
{
    WORD    wVersion = MAKEWORD (2, 0);
    WSADATA WSAData;

    if (WSAStartup (wVersion, &WSAData) != 0)
    {
        throw Exception ("WSAStartup failed");
    }
}

inline SocketStartup::~SocketStartup ()
{
    WSACleanup();
}

void StartupWindowsSockets ()
{
    static SocketStartup startup;
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

    LPWSABUF    GetBuffers  ();
    DWORD       NumBuffers  () const;

private:

    WSABUF buffers_[MAX_GATHER];
    DWORD  dw_NumBuffers_;

    SocketMessage (const SocketMessage&);
    SocketMessage& operator= (const SocketMessage&);
};

template<uintsys MAX_GATHER>
inline SocketMessage<MAX_GATHER>::SocketMessage (const StringList& strl)
    : buffers_       ()
    , dw_NumBuffers_ (0)
{
    StringListConstIter iter (strl_Message);

    uintsys u = 0;

    while (iter && u<MAX_GATHER)
    {
        const String& str (*iter);

        uintsys u_Length = str.Length();

        if (u_Length != 0)
        {
            buffers_[u].buf = (char*)str.C();
            buffers_[u].len = u_Length;

            ++dw_NumBuffers_;
            ++u;
        }

        ++iter;
    }
}

template<uintsys MAX_GATHER>
inline LPWSABUF SocketMessage<MAX_GATHER>::GetBuffers ()
{
    return buffers_;
}

template<uintsys MAX_GATHER>
inline DWORD SocketMessage<MAX_GATHER>::NumBuffers () const
{
    return dw_NumBuffers_;
}

template<uintsys MAX_GATHER>
inline SocketMessage<MAX_GATHER>::~SocketMessage ()
{
    // nothing
}

bool BerkeleySocket::Unblock ()
{
    u_long arg = 1;

    return ioctlsocket (h_Socket_, FIONBIO, &arg) == 0;
}

intsys BerkeleySocket::Send (const StringList& strl_Data, int n_Flags)
{
    SocketMessage<SOCKET_MESSAGE_SIZE> message (strl_Data);

    DWORD dw_BytesSent = 0;

    int n_Return = WSASend (h_Socket_, message.GetBuffers(), message.NumBuffers(),
                            &dw_BytesSent, n_Flags, 0, 0);

    if (n_Return != 0)
    {
        return -1;
    }

    return dw_BytesSent;
}

} // namespace mikestoolbox

