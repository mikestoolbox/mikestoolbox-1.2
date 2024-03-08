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

#include <vector>

#include "mikestoolbox-1.2.h"
#include "Test.h"

using namespace mikestoolbox;

bool TestStandardVector ();

int main (int, char** argv)
{
    try
    {
        Tester check (argv[0]);

        Map<String,String> map1;

        map1("foo") = "foo";
        map1("bar") = "bar";

        Map<String,String> map2 (map1);

        check (map2("foo") == "foo");
        check (map2("bar") == "bar");

        map1("baz") = "baz";

        check (map1("foo") == "foo");
        check (map1("bar") == "bar");
        check (map1("baz") == "baz");

        check (map2("foo") == "foo");
        check (map2("bar") == "bar");
        check (map2("baz") != "baz");

        map2("baz") = map1("baz");

        check (map1 == map2);

        map2("foo2") = map1("foo2");

        check (!map1.Exists ("foo2"));
        check (map2.Exists ("foo2"));
        check (map2("foo2") == "");

        MapIter<String,String> iter (map1);

        check (iter);
        check (iter->Key()   == "bar");
        check (iter->Value() == "bar");
        check (++iter);
        check (iter->Key()   == "baz");
        check (iter->Value() == "baz");
        check (++iter);
        check (iter->Key()   == "foo");
        check (iter->Value() == "foo");
        check (!++iter);

        map1("foo")->Replace ("o$", "e");

        check (map1("foo") != "foo");
        check (map1("foo") == "foe");

        check (TestStandardVector());

        check.Done();
    }
    catch (Exception& e)
    {
        std::cout << "Exception caught: " << e.Message() << std::endl;
    }

    return 0;
}

bool TestStandardVector ()
{
    Map<String,String> map;

    std::vector<String> v;

    v.push_back (String("foo"));
    v.push_back (String("foo"));
    v.push_back (String("bar"));
    v.push_back (String("bar"));

    map.Import (v);

    return (map.NumItems() == 2)    &&
           (map["foo"] == "foo")    &&
           (map["bar"] == "bar")    &&
           (map["baz"] != "baz");
}

