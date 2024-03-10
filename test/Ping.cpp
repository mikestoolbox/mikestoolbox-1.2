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

using namespace mikestoolbox;

int main (int, char**)
{
    signal (SIGPIPE, SIG_IGN);

    String  str_Host;
    uintsys u_Port = 80;

    std::cout << "> ";
    std::cin  >> str_Host;

    for (;;)
    {
        Network network;

        try
        {
            PointerHolder<TcpSocket> sock (network.TcpConnect (str_Host, u_Port));

            Date date;

            if (sock.IsNull())
            {
                std::cout << "Retrying at " << date.FormatRFC2822() << std::endl;

                millisleep (10000);
            }
            else
            {
                sock = 0;

                std::cout << "Connection made at " << Date::Now().FormatRFC2822() << std::endl;

                millisleep (30000);
            }
        }
        catch (Exception& e)
        {
            std::cout << "Exception caught: " << e.Message() << std::endl;
        }
    }

    return 0;
}

