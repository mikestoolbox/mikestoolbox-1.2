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

#include "mikestoolbox-1.2.h"
#include "Test.h"

//#define HOST "www.verisign.com"
//#define HOST "www.mikestoolbox.net"
#define PORT "443"
#define HOST "localhost"
//#define PORT "4433"

using namespace mikestoolbox;

int main (int, char** argv)
{
    Tester check (argv[0]);

    SocketAddress addr1 ("123.45.67.89");
    SocketAddress addr2 ("234.56.78.90:1024");
    SocketAddress addr3 (addr2);
    SocketAddress addr4 ("*:10025");

    check (addr1.GetAddress() == "123.45.67.89");
    check (addr2.GetAddress() == "234.56.78.90");
    check (addr3.GetAddress() == "234.56.78.90");
    check (addr4.GetAddress() == "0.0.0.0");

    check (addr1.GetPort() == 0);
    check (addr2.GetPort() == 1024);
    check (addr3.GetPort() == 1024);
    check (addr4.GetPort() == 10025);

    check (addr1 != addr3);
    check (addr2 == addr3);

    addr3 = "123.45.67.89:2048";

    check (addr1.GetAddress() == addr3.GetAddress());
    check (addr1.GetPort()    != addr3.GetPort());

    check (addr1 != addr3);

    addr1.SetPort (2048);

    check (addr1 == addr3);

    IpAddressRange range1 ("123.45.64.0/18");

    check (range1.Contains (addr1));
    check (!range1.Contains (addr2));

    Network network;

    TcpListener* p_Listener = network.TcpListen (addr4);

    check (p_Listener);

    if (p_Listener)
    {
        check (p_Listener->Listen (5));

        p_Listener->SetTimeout (20);

        TcpSocket* p_Client = network.TcpConnect ("127.0.0.1:10025");

        check (p_Client);

        TcpSocket* p_Server = p_Listener->Accept();

        check (p_Server);

        if (p_Server)
        {
            check (p_Server->SendLineNow ("Hello, world."));
            check (p_Server->Close());

            delete p_Server;
        }
        else
        {
            std::cout << "Network error: " << network.GetLastError() << std::endl;
        }

        if (p_Client)
        {
            String str_Line;

            check (p_Client->ReadLine (str_Line));
            check (str_Line == "Hello, world.\r\n");
            check (!p_Client->ReadLine (str_Line));
            check (p_Client->Close());

            delete p_Client;
        }
        else
        {
            std::cout << "Network error: connect failed" << std::endl;
        }

        check (p_Listener->Close());

        delete p_Listener;
    }
    else
    {
        std::cout << "Network error: " << network.GetLastError() << std::endl;
    }

    UdpSocket* p_UdpListener = network.UdpListen ("127.0.0.1:10025");

    check (p_UdpListener);

    if (p_UdpListener)
    {
        String str_Message ("Hello, world.");

        UdpSocket* p_Client = network.UdpConnect ("127.0.0.1:10025");

        check (p_Client);

        if (p_Client)
        {
            check (p_Client->SendData (str_Message));

            String str_Received;

            SocketAddress addr_Peer;

            //+++ doesn't work on Mac for some reason.....
            check (p_UdpListener->ReadData (str_Received, addr_Peer, 5.0));

            check (str_Received == str_Message);
        }
        else
        {
            std::cout << "UDP connect failed: " << network.GetLastError() << std::endl;
        }
    }
    else
    {
        std::cout << "Network error: " << network.GetLastError() << std::endl;
    }

    check.Done();

    return 0;
}

