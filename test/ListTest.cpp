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

Tester check ("ListTest");

void TestEnds ()
{
    List<uintsys> list1;
    List<uintsys> list2;

    list1.Append (2);

    check (list1.Check());

    list2 = list1;

    list1.Append (3);

    check (list1.Check());
    check (list2.Check());

    list2 = list1;

    list1.Prepend (1);

    check (list1.Check());
    check (list2.Check());

    list2 = list1;

    list1.Prepend (0);

    check (list1.Check());
    check (list2.Check());

    List<uintsys> list3 (MakeList (0, 1, 2, 3));

    check (list3.Check());
    check (list3 == list1);
    check (list3 != list2);

    List<uintsys> list4 (list3);

    check (list3.Pop() == 3);
    check (list3.Check());
    check (list4.Check());
    check (list4 != list3);

    check (list3.NumItems() == 3);
    check (list4.NumItems() == 4);

    list3 = list4;

    check (list3.Shift() == 0);
    check (list3.Check());
    check (list4.Check());
    check (list4 != list3);

    check (list3.NumItems() == 3);
    check (list4.NumItems() == 4);

    list3.Prepend (0);

    check (list3.Check());
    check (list4.Check());
    check (list4 == list3);

    check (list3.NumItems() == 4);
    check (list4.NumItems() == 4);

    list4 = list3;

    list4.Append (list4);

    check (list3.Check());
    check (list4.Check());
    check (list3.NumItems() == 4);
    check (list4.NumItems() == 8);

    check (list3.Contains (0));
    check (list4.Contains (0));

    check (!list3.Contains (20));
    check (!list4.Contains (20));

    check (list3.Count (3) == 1);
    check (list4.Count (3) == 2);

    check (list3.Count (10) == 0);
    check (list4.Count (10) == 0);
}

class Div10
{
public:

    bool operator () (uintsys u1, uintsys u2) const { return (u1/10 < u2/10); }
};

void PrintList (const List<uintsys>& list)
{
    ListIter<uintsys> iter (list);

    while (iter)
    {
        std::cout << *iter++ << ' ';
    }

    std::cout << std::endl;
}

void TestSort ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));

    check (list1.IsSorted());

    List<uintsys> list2 (list1);

    list2.Reverse();

    check (list1.Check());
    check (list2.Check());

    check (list2.IsSorted (Greater<uintsys>()));

    check (list1[0] == list2[3]);
    check (list1[1] == list2[2]);
    check (list1[2] == list2[1]);
    check (list1[3] == list2[0]);

    list1.Append (list2);

    check (list1.Check());
    check (list2.Check());

    list1.Sort();

    check (list1.Check());
    check (list1.NumItems() == 8);
    check (list1.IsSorted());

    list1.Unique();

    check (list1.Check());
    check (list1.NumItems() == 4);
    check (list1.IsSorted());

    check (list1[0] == 0);
    check (list1[1] == 1);
    check (list1[2] == 2);
    check (list1[3] == 3);

    List<uintsys> list3 (MakeList (44, 13, 50, 37));

    list3.Append (MakeList (48, 10, 35, 43));

    List<uintsys> list4 (MakeList (13, 10, 37, 35));

    list4.Append (MakeList (44, 48, 43, 50));

    Div10 div10;

    list3.Sort (div10);

    check (list3.Check());
    check (list3 == list4);
}

void TestDelete ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));
    List<uintsys> list2 (list1);

    check (list1.Delete (0));
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (list1.Delete (1));
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (list1.Delete (3));
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (!list1.Delete (4));
    check (list1.Check());
    check (list1.NumItems() == 4);
}

void TestFind ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));

    check (list1.Find (0));
    check (list1.Find (1));
    check (list1.Find (2));
    check (list1.Find (3));
    check (!list1.Find (4));
}

void TestExpand ()
{
    List<uintsys> list1;

    list1[0] = 1;

    check (list1.Check());
    check (list1.NumItems() == 1);
    check (list1[0] == 1);
    check (list1[-1] == 1);
    check (list1.NumItems() == 1);

    list1.Clear();

    list1[1] = 1;

    check (list1.Check());
    check (list1.NumItems() == 2);
    check (list1[1] == 1);
    check (list1[-2] == 0);
    check (list1.NumItems() == 2);

    list1.Clear();

    list1[-2] = 1;

    check (list1.Check());
    check (list1.NumItems() == 2);
    check (list1[0] == 1);
    check (list1[-1] == 0);
    check (list1.NumItems() == 2);
}

void TestHead ()
{
    List<uintsys> list1;
    List<uintsys> list2;
    List<uintsys> list3;

    list1.Append (MakeList (0, 1, 2, 3));
    list1.Append (MakeList (4, 5, 6, 7));

    list2 = list1.Head (3);
    list3 = list1.Tail (5);

    check (list2.Check());
    check (list2.NumItems() == 3);
    check (list3.Check());
    check (list3.NumItems() == 5);

    list2.Append (list3);

    check (list2.Check());
    check (list2.NumItems() == 8);
    check (list3.Check());
    check (list3.NumItems() == 5);

    check (list1 == list2);
}

void TestContainers ()
{
    Hash<uintsys,uintsys> hash;
    Map<uintsys,uintsys>  map;

    hash(1) = 1;
    hash(2) = 2;
    hash(3) = 4;

    map(1) = 1;
    map(2) = 2;
    map(3) = 4;

    List<uintsys> list1 (hash);
    List<uintsys> list2 (map);
    List<uintsys> list3 (MakeList (1, 1, 2, 2));

    list3.Append (MakeList (3, 4));

    check (list1.Check());
    check (list2.Check());
    check (list3.Check());
    check (list3.NumItems() == 6);

    list1.Sort();   // Hash is unordered

    check (list1 == list2);
    check (list2 == list3);
}

void TestTruncate ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));
    List<uintsys> list2 (list1);

    check (list1.Check());
    check (list2.Check());
    check (list1.NumItems() == 4);
    check (list2.NumItems() == 4);

    list2.Truncate (3);

    check (list1.Check());
    check (list1.NumItems() == 4);
    check (list2.Check());
    check (list2.NumItems() == 3);
    check (list2[2] == 2);
}

void TestInsertErase ()
{
    List<uintsys> list1 (MakeList (1, 3));
    List<uintsys> list2 (MakeList (0, 1, 2, 3));

    list1.Insert (1, 2u);

    check (list1.Check());
    check (list1.NumItems() == 3);

    list1.Insert (0, 0);

    check (list1.Check());
    check (list1.NumItems() == 4);
    check (list1 == list2);

    list1.Remove (1);
    list2.Remove (-3);

    check (list1.Check());
    check (list2.Check());
    check (list1.NumItems() == 3);
    check (list2.NumItems() == 3);
    check (list1 == list2);

    ListChangeIter<uintsys> iter1 (list1, 1);

    list1.Erase (iter1);

    check (list1.Check());
    check (list1.NumItems() == 2);
    check (list1[0] == 0);
    check (list1[1] == 3);
    check (iter1);
    check (iter1 && iter1.Offset() == 1);

    ListChangeIter<uintsys> iter2 (list2, -3);

    list2.Erase (iter2);

    check (list2.Check());
    check (list2.NumItems() == 2);
    check (list2[0] == 2);
    check (list2[1] == 3);
    check (iter2);
    check (iter2 && iter2.Offset() == 0);

    while (iter1)
    {
        list1.Erase (iter1);
    }

    check (list1.Check());
    check (list1.NumItems() == 1);

    while (iter2)
    {
        list2.Erase (iter2);
    }

    check (list2.Check());
    check (list2.IsEmpty());
}

void TestRemove ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));
    List<uintsys> list2 (list1);

    check (list1.Remove (0) == 0);
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (list1.Remove (1) == 1);
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (list1.Remove (-2) == 2);
    check (list1.Check());
    check (list1.NumItems() == 3);

    list1 = list2;

    check (list1.Remove (-1) == 3);
    check (list1.Check());
    check (list1.NumItems() == 3);
}

void TestSplice ()
{
    List<uintsys> list1;
    List<uintsys> list2;
    List<uintsys> list3;

    list1.Append (MakeList (0, 1, 2, 3));
    list1.Append (MakeList (4, 5, 6, 7));

    list3 = list1;

    check (list1.Check());
    check (list1.NumItems() == 8);

    list2 = list1.Splice (5);

    check (list1.Check());
    check (list1.NumItems() == 5);

    check (list2.Check());
    check (list2.NumItems() == 3);

    list1.Splice (5, 0, list2);

    check (list1.Check());
    check (list2.Check());
    check (list1 == list3);

    list2 = list1.Splice (0, 2);

    check (list1.Check());
    check (list1.NumItems() == 6);

    check (list2.Check());
    check (list2.NumItems() == 2);

    list1.Splice (0, 0, list2);

    check (list1.Check());
    check (list2.Check());
    check (list1 == list3);

    list2 = list1.Splice (-2, 2);

    check (list1.Check());
    check (list1.NumItems() == 6);

    check (list2.Check());
    check (list2.NumItems() == 2);

    list1.Splice (list1.NumItems(), 0, list2);

    check (list1.Check());
    check (list2.Check());
    check (list1 == list3);

    list2 = list1.Splice (2, 2);

    check (list1.Check());
    check (list1.NumItems() == 6);

    check (list2.Check());
    check (list2.NumItems() == 2);

    list1.Splice (2, 0, list2);

    check (list1.Check());
    check (list2.Check());
    check (list1 == list3);
}

void TestSplit ()
{
    List<uintsys> list1 (MakeList (0, 1, 2, 3));
    List<uintsys> list2 (list1.Split(5));
    List<uintsys> list3 (list1.Split(3));
    List<uintsys> list4 (list2.Split(0));

    check (list1.Check());
    check (list2.Check());
    check (list3.Check());
    check (list4.Check());

    check (list1.NumItems() == 3);
    check (list2.NumItems() == 0);
    check (list3.NumItems() == 1);
    check (list4.NumItems() == 0);

    check (list1[0] == 0);
    check (list1[1] == 1);
    check (list1[2] == 2);
    check (list3[0] == 3);

    List<uintsys> list5 (list3.Split(0));

    check (list3.Check());
    check (list5.Check());

    check (list3.NumItems() == 0);
    check (list5.NumItems() == 1);

    check (list5[0] == 3);
}

int main ()
{
    TestEnds ();
    TestSort();
    TestDelete();
    TestFind();
    TestExpand();
    TestHead();
    TestContainers();
    TestTruncate();
    TestInsertErase();
    TestRemove();
    TestSplice();
    TestSplit();

    check.Done();

    return 0;
}

