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

//+---------------------------------------------------------------------------
//  File:       FileTest.cpp
//
//  Synopsis:   Test program for File class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"
#include "Test.h"

using namespace mikestoolbox;

int main (int, char** argv)
{
    Tester check (argv[0]);

    File file ("TestFile");

    file.Delete();

    check (!file.Exists());

    check (file.Name()      == "TestFile");
    check (file.BaseName()  == "TestFile");
    check (file.Directory() == "./");

    String str_LineA ('A', Repeat(1023));
    String str_LineB ('B', Repeat(1023));
    String str_LineC ('C', Repeat(1023));

    str_LineA += '\n';
    str_LineB += '\n';
    str_LineC += '\n';

    check (file.Write (str_LineA));
    check (file.Size() == 1024);
    check (file.IsRegular());
    check (!file.IsDirectory());

    String     str_Contents;
    StringList strl_Contents;

    PerlRegexMatches matches;

    check (file.Read (str_Contents));

    check (str_Contents.Length() == 1024);
    check (str_Contents.Match ("^A+\n$", matches));

    String str_NewFile ("TestFileNew");

    File file_New (str_NewFile);

    check (file.CopyTo (file_New));

    String str_Contents2;

    check (file_New.Read (str_Contents2));

    check (str_Contents == str_Contents2);

    check (file.Append (str_LineB));
    check (file.Size() == 2048);
    check (file.Append (str_LineC));
    check (file.Size() == 3072);

    check (file.Read (str_Contents));
    check (file.Read (strl_Contents));
    check (str_Contents.Match ("^A+\nB+\nC+\n$", matches));

    check (strl_Contents.Shift() == str_LineA);
    check (strl_Contents.Shift() == str_LineB);
    check (strl_Contents.Shift() == str_LineC);
    check (strl_Contents.IsEmpty());

    check (!file.Rename (file_New.Name()));
    check (file.Name() == "TestFile");

    check (file.Rename (file_New.Name(), FILE_REPLACE_EXISTING));
    check (file.Name() == "TestFileNew");

    StringList strl_Lines (file.Head (2));

    check (strl_Lines.NumItems() == 2);
    check (strl_Lines.Size() == 2048);

    strl_Lines = file.Tail (2);

    check (strl_Lines.NumItems() == 2);
    check (strl_Lines.Size() == 2048);
    check (strl_Lines.Join().Match ("^B+\nC+\n$", matches));

    check (file.Delete());

    check.Done();

    return 0;
}

