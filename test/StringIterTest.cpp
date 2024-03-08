/*
  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.

  This source code is the property of Michael S. D'Errico and is
  protected under international copyright laws.
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

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

using namespace mikestoolbox;

bool TestExtractUint8  ();
bool TestExtractUint16 ();
bool TestExtractUint24 ();
bool TestExtractUint32 (uintsys u_TopByte);

int main (int, char** argv)
{
    Tester check (argv[0]);

    check (TestExtractUint8());
    check (TestExtractUint16());
    check (TestExtractUint24());
    check (TestExtractUint32 (0xA6));

    check.Done();

    return 0;
}

bool TestExtractUint8 ()
{
    bool b_Success = true;

    String str;

    for (uintsys u=0; u<256; ++u)
    {
        str.AppendUint8 (u);
    }

    StringIter iter (str);

    uintsys u_Expect = 0;
    uintsys u_Actual = 0;

    ParseError error;

    while (iter)
    {
        if (!iter.ExtractUint8 (u_Actual, error))
        {
            std::cout << "Extract failed" << std::endl;

            b_Success = false;

            break;
        }

        if (u_Actual != u_Expect)
        {
            std::cout << "Data Mismatch: expected " << u_Expect;
            std::cout << " actual " << u_Actual << std::endl;

            b_Success = false;
        }

        ++u_Expect;
    }

    return b_Success;
}

bool TestExtractUint16 ()
{
    bool b_Success = true;

    String str;

    str.Reserve (2<<16);

    for (uintsys u=0; u<(1<<16); ++u)
    {
        str.AppendUint16 (u);
    }

    StringIter iter (str);

    uintsys u_Expect = 0;
    uintsys u_Actual = 0;

    ParseError error;

    while (iter)
    {
        if (!iter.ExtractUint16 (u_Actual, error))
        {
            std::cout << "Extract failed" << std::endl;

            b_Success = false;

            break;
        }

        if (u_Actual != u_Expect)
        {
            std::cout << "Data Mismatch: expected " << u_Expect;
            std::cout << " actual " << u_Actual << std::endl;

            b_Success = false;
        }

        ++u_Expect;
    }

    return b_Success;
}

bool TestExtractUint24 ()
{
    bool b_Success = true;

    String str;

    str.Reserve (3<<24);

    for (uintsys u=0; u<(1<<24); ++u)
    {
        str.AppendUint24 (u);
    }

    StringIter iter (str);

    uintsys u_Expect = 0;
    uintsys u_Actual = 0;

    ParseError error;

    while (iter)
    {
        if (!iter.ExtractUint24 (u_Actual, error))
        {
            std::cout << "Extract failed" << std::endl;

            b_Success = false;

            break;
        }

        if (u_Actual != u_Expect)
        {
            std::cout << "Data Mismatch: expected " << u_Expect;
            std::cout << " actual " << u_Actual << std::endl;

            b_Success = false;
        }

        ++u_Expect;
    }

    return b_Success;
}

bool TestExtractUint32 (uintsys u_TopByte)
{
    bool b_Success = true;

    String str;

    str.Reserve (4<<24);

    for (uintsys u=0; u<(1<<24); ++u)
    {
        str.AppendUint8  (u_TopByte);
        str.AppendUint24 (u);
    }

    StringIter iter (str);

    uintsys u_Expect = u_TopByte << 24;
    uintsys u_Actual = 0;

    ParseError error;

    while (iter)
    {
        if (!iter.ExtractUint32 (u_Actual, error))
        {
            std::cout << "Extract failed" << std::endl;

            b_Success = false;

            break;
        }

        if (u_Actual != u_Expect)
        {
            std::cout << "Data Mismatch: expected " << u_Expect;
            std::cout << " actual " << u_Actual << std::endl;

            b_Success = false;

            break;
        }

        ++u_Expect;
    }

    return b_Success;
}

