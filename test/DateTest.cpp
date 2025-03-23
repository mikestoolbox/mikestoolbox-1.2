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

#include "Test.h"

bool TestDateAndParts ();
bool TestGeneralizedTime();
bool TestUTCTime();
bool TestRFC2822Time();

int main (int, char** argv)
{
    Tester check (argv[0]);

    Date ddd ((struct tm*)0);

    Date Jan_1_0001 (FourDigitYear (1), 1, 1);
    Date Jan_1_0002 (FourDigitYear (2), 1, 1);
    Date Jan_1_0003 (FourDigitYear (3), 1, 1);
    Date Jan_1_0004 (FourDigitYear (4), 1, 1);
    Date Jan_1_0005 (FourDigitYear (5), 1, 1);

    check (Jan_1_0001.Year()  == 1);
    check (Jan_1_0001.Month() == 1);
    check (Jan_1_0001.Day()   == 1);
    check (Jan_1_0001.IsSaturday());

    check (Jan_1_0002.Year()  == 2);
    check (Jan_1_0002.Month() == 1);
    check (Jan_1_0002.Day()   == 1);
    check (Jan_1_0002.IsSunday());

    check (Jan_1_0003.Year()  == 3);
    check (Jan_1_0003.Month() == 1);
    check (Jan_1_0003.Day()   == 1);
    check (Jan_1_0003.IsMonday());

    check (Jan_1_0004.Year()  == 4);
    check (Jan_1_0004.Month() == 1);
    check (Jan_1_0004.Day()   == 1);
    check (Jan_1_0004.IsTuesday());

    check (Jan_1_0005.Year()  == 5);
    check (Jan_1_0005.Month() == 1);
    check (Jan_1_0005.Day()   == 1);
    check (Jan_1_0005.IsThursday());

    Date Jan_1_1751  (FourDigitYear (1751),  1,  1);
    Date Dec_31_1751 (FourDigitYear (1751), 12, 31);
    Date Jan_1_1752  (FourDigitYear (1752),  1,  1);
    Date Sep_2_1752  (FourDigitYear (1752),  9,  2);
    Date Sep_14_1752 (FourDigitYear (1752),  9, 14);
    Date Dec_31_1752 (FourDigitYear (1752), 12, 31);
    Date Jan_1_1753  (FourDigitYear (1753),  1,  1);
    Date Dec_31_1753 (FourDigitYear (1753), 12, 31);
    Date Jan_1_1754  (FourDigitYear (1754),  1,  1);

    check (Jan_1_1751.Year()       == 1751);
    check (Jan_1_1751.Month()      ==    1);
    check (Jan_1_1751.Day()        ==    1);
    check (Jan_1_1751.IsTuesday());

    check (Dec_31_1751.Year()      == 1751);
    check (Dec_31_1751.Month()     ==   12);
    check (Dec_31_1751.Day()       ==   31);
    check (Dec_31_1751.IsTuesday());
    check (Dec_31_1751.DaysMoreThan (Jan_1_1751) == 364);

    check (Jan_1_1752.Year()       == 1752);
    check (Jan_1_1752.Month()      ==    1);
    check (Jan_1_1752.Day()        ==    1);
    check (Jan_1_1752.IsWednesday());
    check (Jan_1_1752.DaysMoreThan (Dec_31_1751) == 1);

    check (Sep_2_1752.Year()       == 1752);
    check (Sep_2_1752.Month()      ==    9);
    check (Sep_2_1752.Day()        ==    2);
    check (Sep_2_1752.IsWednesday());
    check (Sep_2_1752.DaysMoreThan (Jan_1_1752) == 245);

    check (Sep_14_1752.Year()      == 1752);
    check (Sep_14_1752.Month()     ==    9);
    check (Sep_14_1752.Day()       ==   14);
    check (Sep_14_1752.IsThursday());

    // weird leap year correction:
    check (Sep_14_1752.DaysMoreThan (Sep_2_1752) == 1);

    check (Dec_31_1752.Year()      == 1752);
    check (Dec_31_1752.Month()     ==   12);
    check (Dec_31_1752.Day()       ==   31);
    check (Dec_31_1752.IsSunday());
    check (Dec_31_1752.DaysMoreThan (Sep_2_1752)  == 109);
    check (Dec_31_1752.DaysMoreThan (Sep_14_1752) == 108);

    check (Jan_1_1753.Year()       == 1753);
    check (Jan_1_1753.Month()      ==    1);
    check (Jan_1_1753.Day()        ==    1);
    check (Jan_1_1753.IsMonday());
    check (Jan_1_1753.DaysMoreThan (Dec_31_1752) == 1);

    check (Dec_31_1753.Year()      == 1753);
    check (Dec_31_1753.Month()     ==   12);
    check (Dec_31_1753.Day()       ==   31);
    check (Dec_31_1753.IsMonday());
    check (Dec_31_1753.DaysMoreThan (Jan_1_1753) == 364);

    check (Jan_1_1754.Year()       == 1754);
    check (Jan_1_1754.Month()      ==    1);
    check (Jan_1_1754.Day()        ==    1);
    check (Jan_1_1754.IsTuesday());
    check (Jan_1_1754.DaysMoreThan (Dec_31_1753) == 1);

    Date Jan_1_1970 (FourDigitYear (1970), 1, 1);

    check (Jan_1_1970.Year()  == 1970);
    check (Jan_1_1970.Month() == 1);
    check (Jan_1_1970.Day()   == 1);
    check (Jan_1_1970.IsThursday());

    check (Jan_1_1970.DaysMoreThan (Jan_1_0001) == 719164);
    check (Jan_1_1970 == Date((time_t)0));

    Date Christmas_1984;    // a leap year

    check (Christmas_1984.FromHTTP ("Tue, 25 Dec 1984 06:30:45 GMT"));
    check (Christmas_1984.Year()   == 1984);
    check (Christmas_1984.Month()  ==   12);
    check (Christmas_1984.Day()    ==   25);
    check (Christmas_1984.IsTuesday());
    check (Christmas_1984.Hour()   ==    6);
    check (Christmas_1984.Minute() ==   30);
    check (Christmas_1984.Second() ==   45);

    check (Christmas_1984.FromHTTP ("Tuesday, 25-Dec-84 07:05:50 GMT"));
    check (Christmas_1984.Year()   == 1984);
    check (Christmas_1984.Month()  ==   12);
    check (Christmas_1984.Day()    ==   25);
    check (Christmas_1984.IsTuesday());
    check (Christmas_1984.Hour()   ==    7);
    check (Christmas_1984.Minute() ==    5);
    check (Christmas_1984.Second() ==   50);

    check (Christmas_1984.FromHTTP ("Tue Dec 25 15:15:15 1984"));
    check (Christmas_1984.Year()   == 1984);
    check (Christmas_1984.Month()  ==   12);
    check (Christmas_1984.Day()    ==   25);
    check (Christmas_1984.IsTuesday());
    check (Christmas_1984.Hour()   ==   15);
    check (Christmas_1984.Minute() ==   15);
    check (Christmas_1984.Second() ==   15);

    Date Christmas_1985;    // a non-leap year

    check (Christmas_1985.FromHTTP ("Wed, 25 Dec 1985 06:30:45 GMT"));
    check (Christmas_1985.Year()   == 1985);
    check (Christmas_1985.Month()  ==   12);
    check (Christmas_1985.Day()    ==   25);
    check (Christmas_1985.IsWednesday());
    check (Christmas_1985.Hour()   ==    6);
    check (Christmas_1985.Minute() ==   30);
    check (Christmas_1985.Second() ==   45);

    check (Christmas_1985.FromHTTP ("Wednesday, 25-Dec-85 07:05:50 GMT"));
    check (Christmas_1985.Year()   == 1985);
    check (Christmas_1985.Month()  ==   12);
    check (Christmas_1985.Day()    ==   25);
    check (Christmas_1985.IsWednesday());
    check (Christmas_1985.Hour()   ==    7);
    check (Christmas_1985.Minute() ==    5);
    check (Christmas_1985.Second() ==   50);

    check (Christmas_1985.FromHTTP ("Wed Dec 25 15:15:15 1985"));
    check (Christmas_1985.Year()   == 1985);
    check (Christmas_1985.Month()  ==   12);
    check (Christmas_1985.Day()    ==   25);
    check (Christmas_1985.IsWednesday());
    check (Christmas_1985.Hour()   ==   15);
    check (Christmas_1985.Minute() ==   15);
    check (Christmas_1985.Second() ==   15);

    check (TestDateAndParts());
    check (TestGeneralizedTime());
    check (TestUTCTime());
    check (TestRFC2822Time());

    //+++ check (TestLocalDate());

    check.Done();

    return 0;
}

void PrintDates (const Date& date1, const Date& date2)
{
    std::cout << date1.FormatGeneralizedTime() << std::endl;
    std::cout << date2.FormatGeneralizedTime() << std::endl;
}

bool TestDateAndParts ()
{
    Date date1 (FourDigitYear (1), 1, 1);
    Date date3 (FourDigitYear (9999), 12, 31);

    do
    {
        DateParts parts1 (date1);
        Date      date2  (parts1);
        DateParts parts2 (date2);

        if ((date1 != date2) || (parts1 != parts2))
        {
            PrintDates (date1, date2);

            return false;
        }

        date1.AddDays (1);
    }
    while (date1 != date3);

    return true;
}

bool TestGeneralizedTime ()
{
    Date date1 (FourDigitYear (1), 1, 1);
    Date date2;
    Date date3 (FourDigitYear (9999), 12, 31);

    do
    {
        date2.FromGeneralizedTime (date1.FormatGeneralizedTime());

        if (date1 != date2)
        {
            PrintDates (date1, date2);

            return false;
        }

        date1.AddDays (1);
    }
    while (date1 != date3);

    return true;
}

bool TestUTCTime ()
{
    Date date1 (FourDigitYear (1950),  1,  1);
    Date date2;
    Date date3 (FourDigitYear (2049), 12, 31);

    do
    {
        date2.FromUTCTime (date1.FormatUTCTime());

        if (date1 != date2)
        {
            PrintDates (date1, date2);

            return false;
        }

        date1.AddDays (1);
    }
    while (date1 != date3);

    return true;
}

bool TestRFC2822Time ()
{
    Date date1 (FourDigitYear (1000),  1,  1);
    Date date2;
    Date date3 (FourDigitYear (9999), 12, 31);

    do
    {
        date2.FromRFC2822 (date1.FormatRFC2822());

        if (date1 != date2)
        {
            PrintDates (date1, date2);

            return false;
        }

        date1.AddDays (1);
    }
    while (date1 != date3);

    return true;
}

