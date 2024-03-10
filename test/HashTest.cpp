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

using namespace mikestoolbox;

int main (int, char** argv)
{
    Tester check (argv[0]);

    Hash<int,String> hash1;

    String  str_Empty;
    String  str_Foo   ("foo");
    String  str_Bar   ("bar");
    String  str_Baz   ("baz");
    String  str_Blurb ("blurb");
    String  str_Foe   ("foe");

    hash1(1) = str_Foo;
    hash1(2) = str_Bar;

    Hash<int,String> hash2 (hash1);

    check (hash2(1) == str_Foo);
    check (hash2(2) == str_Bar);

    hash1(3) = str_Baz;

    check (hash1(1) == str_Foo);
    check (hash1(2) == str_Bar);
    check (hash1(3) == str_Baz);

    check (hash2(1) == str_Foo);
    check (hash2(2) == str_Bar);
    check (hash2(3) != str_Baz);

    hash2(3) = hash1(3);

    check (hash2(3) == str_Baz);

    check (hash1 == hash2);

    hash2(4) = hash1(4);

    check (!hash1.Exists (4));
    check (hash2.Exists (4));
    check (hash2(4) == str_Empty);

    check (hash1 != hash2);

    hash1(4) = str_Empty;

    check (hash1 == hash2);

    List<String> list_Values (hash2.Values());

    check (list_Values.NumItems() == 4);
    check (list_Values.Contains (str_Foo));
    check (list_Values.Contains (str_Bar));
    check (list_Values.Contains (str_Baz));
    check (list_Values.Contains (str_Empty));
    check (!list_Values.Contains (str_Blurb));

    hash1(1)->Replace ("o$", "e");

    check (hash1(1) == str_Foe);

    check.Done();

    return 0;
}

