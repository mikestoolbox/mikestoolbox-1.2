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

void Print (const StringList& strl)
{
    StringListIter iter (strl);

    uintsys u = 0;

    while (iter)
    {
        std::cout << ++u << " " << *iter++ << std::endl;
    }

    std::cout << std::endl;
}

int main (int, char** argv)
{
    Tester check (argv[0]);

    try
    {
        StringList strl;

        strl.Append ("foo");
        strl.Append ("bar");
        strl.Append ("baz");

        strl.EraseFrontBytes (1);

        check (strl.NumItems() == 3);
        check (strl[0] == "oo");

        strl.EraseFrontBytes (6);

        check (strl.NumItems() == 1);
        check (strl[0] == "az");

        String str;

        str.AppendUint16 (0x8000);

        strl.Clear();
        strl.Append (str, str);

        StringListByteIter iter (strl);

        uintsys u = 0;

        ParseError error;

        check (iter.ExtractUint32 (u, error));
        check (u == 0x80008000U);

        strl.Clear();

        for (uintsys u=0; u<10; ++u)
        {
            check (strl.Check());

            strl.Append (String(u));
        }

        check (strl.Check());

        StringList strl2 (strl);

        check (strl.Check());
        check (strl2.Check());

        check (strl2.Pop().AsInt() == 9);

        check (strl.NumItems()  == 10);
        check (strl2.NumItems() == 9);

        check (strl.Check());
        check (strl2.Check());

        strl2.Append (String(9));

        check (strl2.Check());
        check (strl == strl2);

        StringList strl3 (strl);

        check (strl.Check());
        check (strl3.Check());

        check (strl3.Shift().AsInt() == 0);

        check (strl.NumItems()  == 10);
        check (strl3.NumItems() == 9);

        check (strl.Check());
        check (strl3.Check());

        strl3.Prepend (String(0));

        check (strl3.Check());
        check (strl == strl3);

        check (strl3.IsSorted());

        strl3.Reverse();

        check (strl3.Check());
        check (strl3.IsSorted (Greater<String>()));

        for (uintsys u=0; u<10; ++u)
        {
            check (strl[u] == strl2[u]);
            check (strl[u] == strl3[9-u]);
        }

        StringList strl4 (strl);

        strl4.Append (strl4);

        check (strl.NumItems()  == 10);
        check (strl.Check());
        check (strl4.NumItems() == 20);
        check (strl4.Check());
        check (strl4.Count (String(3)) == 2);
        check (strl4.Count (String(10)) == 0);
        check (strl4.Contains (String(0)));
        check (!strl4.Contains (String(20)));

        strl4.Remove (13);

        check (strl4.NumItems() == 19);
        check (strl4.Check());
        check (strl4.Count (String(3)) == 1);

        StringListChangeIter iter3 (strl4, 3);

        check (iter3 && *iter3 == String(3));

        strl4.Erase (iter3);

        check (iter3 && *iter3 == String(4));
        check (strl4.NumItems() == 18);
        check (strl4.Check());
        check (strl4.Count (String(3)) == 0);

        strl4.Insert (3, String(3));

        check (strl4.NumItems() == 19);
        check (strl4.Check());
        check (strl4.Count (String(3)) == 1);

        strl4.Sort();

        check (strl4.NumItems() == 19);
        check (strl4.Check());

        strl4.Unique();

        check (strl4.Check());
        check (strl4.NumItems() == 10);

        check (strl == strl4);

        check (strl4.Remove (3).AsInt() == 3);
        check (strl4.NumItems() == 9);
        check (strl4.Check());

        check (!strl4.Delete (String(3)));
        check (strl4.Delete (String(4)));
        check (strl4.NumItems() == 8);
        check (strl4.Check());

        check (strl.Find (String(3)));
        check (!strl4.Find (String(3)));

        strl.Clear();
        strl.Append ("foo");
        strl.Append ("bar");
        strl.Append ("baz");

        check (strl.Grep ("/ba/").NumItems()        == 2);
        check (strl.Grep (" ! /ba/").NumItems()     == 1);
        check (strl.Grep ("!!/ba/").NumItems()      == 2);
        check (strl.Grep ("/BA/").NumItems()        == 0);
        check (strl.Grep ("/BA/i").NumItems()       == 2);
        check (strl.Grep ("m\\ba\\").NumItems()     == 2);
        check (strl.Grep ("!s#[oa]#i#g").NumItems() == 0);
        check (strl.Shift() == "fii");
        check (strl.Shift() == "bir");
        check (strl.Shift() == "biz");
    }
    catch (Exception& e)
    {
        std::cout << "Exception caught: " << e.Message() << std::endl;
    }

    check.Done();

    return 0;
}

