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
//  File:       Date.cpp
//
//  Synopsis:   Implementation of objects that represent dates and times
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

#ifdef HAVE_AWFUL_DATE_FUNCTIONS
static Mutex gmutex_LocalTime;
#endif

intsys DecodeTimeZoneName (const String& str_TimeZone);

static const PerlRegex regex_RFC2822_Date ("(\\w+, )?(\\d+) (\\w+) (\\d+) "
                                           "(\\d+):(\\d+)(:(\\d+))?"
                                           "( *(([+-])(\\d+)|(\\w+)))?");

static bool IsLeapYear (uintsys u_Year)
{
    return ((u_Year > 0)        &&      // year is in the Common Era
            (u_Year % 4 == 0)   &&      // and is a multiple of 4,
                                        // and satisfies one of:
           ((u_Year <= 1752)    ||      //    1: is not after 1752, or
            (u_Year % 400 == 0) ||      //    2: is a multiple of 400, or
            (u_Year % 100)));           //    3: is not a multiple of 100
}

static uintsys DaysBeforeJan1 (uintsys u_Year)
{
    if (u_Year <= 1)
    {
        return 0;
    }

    --u_Year;

    uintsys u_NormalDays  = u_Year * 365;
    uintsys u_LeapDays    = u_Year / 4;
    uintsys u_Correction  = (u_Year < 1752) ? 0 : 11;
    uintsys u_Centuries   = (u_Year < 1800) ? 0 : (u_Year / 100 - 17);
    uintsys u_QuadCentury = (u_Year < 2000) ? 0 : (u_Year / 400 -  4);

    return (u_NormalDays + u_LeapDays + u_QuadCentury) -
           (u_Correction + u_Centuries);
}

#define MONTH_28(X) X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X
#define MONTH_29(X) MONTH_28(X),X
#define MONTH_30(X) MONTH_29(X),X
#define MONTH_31(X) MONTH_30(X),X

#define DAYS_28 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,\
                16,17,18,19,20,21,22,23,24,25,26,27,28
#define DAYS_29 DAYS_28,29
#define DAYS_30 DAYS_29,30
#define DAYS_31 DAYS_30,31

static const uintsys au_MonthNonLeap[365] = {
    MONTH_31(1),  MONTH_28(2), MONTH_31(3), MONTH_30(4), MONTH_31(5),
    MONTH_30(6),  MONTH_31(7), MONTH_31(8), MONTH_30(9), MONTH_31(10),
    MONTH_30(11), MONTH_31(12)
};

static const uintsys au_DayNonLeap[365] = {
    DAYS_31, DAYS_28, DAYS_31, DAYS_30, DAYS_31, DAYS_30,
    DAYS_31, DAYS_31, DAYS_30, DAYS_31, DAYS_30, DAYS_31
};

static const uintsys au_MonthLeap[366] = {
    MONTH_31(1),  MONTH_29(2), MONTH_31(3), MONTH_30(4), MONTH_31(5),
    MONTH_30(6),  MONTH_31(7), MONTH_31(8), MONTH_30(9), MONTH_31(10),
    MONTH_30(11), MONTH_31(12)
};

static const uintsys au_DayLeap[366] = {
    DAYS_31, DAYS_29, DAYS_31, DAYS_30, DAYS_31, DAYS_30,
    DAYS_31, DAYS_31, DAYS_30, DAYS_31, DAYS_30, DAYS_31
};

static const uintsys au_Month1752[355] = {
    MONTH_31(1), MONTH_29(2), MONTH_31(3), MONTH_30(4), MONTH_31(5),
    MONTH_30(6), MONTH_31(7), MONTH_31(8),
    // 19 days in September 1752
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    MONTH_31(10), MONTH_30(11), MONTH_31(12)
};

static const uintsys au_Day1752[355] = {
    DAYS_31, DAYS_29, DAYS_31, DAYS_30, DAYS_31, DAYS_30,
    DAYS_31, DAYS_31,
    // September 1752
    1, 2, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    DAYS_31, DAYS_30, DAYS_31
};

static const uintsys u_DaysBefore1752 = DaysBeforeJan1 (1752);
static const uintsys u_DaysBefore1753 = DaysBeforeJan1 (1753);
static const uintsys u_DaysBefore1800 = DaysBeforeJan1 (1800);
static const uintsys u_DaysBefore1801 = DaysBeforeJan1 (1801);
static const uintsys u_DaysBefore2000 = DaysBeforeJan1 (2000);
static const uintsys u_DaysBefore2001 = DaysBeforeJan1 (2001);

static const uintsys u_OneYear       = 365;
static const uintsys u_FourYears     =   4 * u_OneYear    + 1;
static const uintsys u_OneCentury    =  25 * u_FourYears  - 1;
static const uintsys u_FourCenturies =   4 * u_OneCentury + 1;

static void FindMonthAndDay (uintsys u_YearDay, uintsys u_Year,
                             uintsys& u_Month, uintsys& u_Day)
{
    if (u_Year == 1752)
    {
        u_Month = au_Month1752[u_YearDay];
        u_Day   = au_Day1752[u_YearDay];
    }
    else if (IsLeapYear (u_Year))
    {
        u_Month = au_MonthLeap[u_YearDay];
        u_Day   = au_DayLeap[u_YearDay];
    }
    else
    {
        u_Month = au_MonthNonLeap[u_YearDay];
        u_Day   = au_DayNonLeap[u_YearDay];
    }
}

template<uintsys A, uintsys B>
static inline void Foo (uintsys& u1, uintsys& u2)
{
    if (u1 >= A)
    {
        u1 -= A;
        u2 += B;
    }
}

template<uintsys A, uintsys B>
static inline void Bar (uintsys& u1, uintsys& u2)
{
    if (u1 >= A)
    {
        uintsys u = u1 / A;

        u1 -= u * A;
        u2 += u * B;
    }
}

static void DaysToCalendar (uintsys u_TotalDays, uintsys& u_Year,
                            uintsys& u_Month, uintsys& u_Day)
{
    if (u_TotalDays <= 1)
    {
        u_Year = u_Month = u_Day = 1;

        return;
    }

    --u_TotalDays;

    if (u_TotalDays < u_DaysBefore1752)
    {
        u_Year = 1;

        Bar<u_FourYears, 4> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
    }
    else if (u_TotalDays < u_DaysBefore1753)
    {
        u_Year = 1752;

        u_TotalDays -= u_DaysBefore1752;
    }
    else if (u_TotalDays < u_DaysBefore1800)
    {
        u_Year = 1753;

        u_TotalDays -= u_DaysBefore1753;

        Bar<u_FourYears, 4> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
        Foo<u_OneYear,   1> (u_TotalDays, u_Year);
    }
    else if (u_TotalDays < u_DaysBefore1801)
    {
        u_Year = 1800;

        u_TotalDays -= u_DaysBefore1800;
    }
    else if (u_TotalDays < u_DaysBefore2000)
    {
        u_Year = 1801;

        u_TotalDays -= u_DaysBefore1801;

        Foo<u_OneCentury, 100> (u_TotalDays, u_Year);
        Foo<u_OneCentury, 100> (u_TotalDays, u_Year);
        Bar<u_FourYears,    4> (u_TotalDays, u_Year);
        Foo<u_OneYear,      1> (u_TotalDays, u_Year);
        Foo<u_OneYear,      1> (u_TotalDays, u_Year);
        Foo<u_OneYear,      1> (u_TotalDays, u_Year);
    }
    else if (u_TotalDays < u_DaysBefore2001)
    {
        u_Year = 2000;

        u_TotalDays -= u_DaysBefore2000;
    }
    else
    {
        u_Year = 2001;

        u_TotalDays -= u_DaysBefore2001;

        Bar<u_FourCenturies, 400> (u_TotalDays, u_Year);
        Foo<u_OneCentury,    100> (u_TotalDays, u_Year);
        Foo<u_OneCentury,    100> (u_TotalDays, u_Year);
        Foo<u_OneCentury,    100> (u_TotalDays, u_Year);
        Bar<u_FourYears,       4> (u_TotalDays, u_Year);
        Foo<u_OneYear,         1> (u_TotalDays, u_Year);
        Foo<u_OneYear,         1> (u_TotalDays, u_Year);
        Foo<u_OneYear,         1> (u_TotalDays, u_Year);
    }

    FindMonthAndDay (u_TotalDays, u_Year, u_Month, u_Day);
}

static inline uintsys DayOfTheLeapYear (uintsys u_Month, uintsys u_Day)
{
    switch (u_Month)
    {
        case 1:     return u_Day;
        case 2:     return u_Day +  31;
        case 3:     return u_Day +  60;
        case 4:     return u_Day +  91;
        case 5:     return u_Day + 121;
        case 6:     return u_Day + 152;
        case 7:     return u_Day + 182;
        case 8:     return u_Day + 213;
        case 9:     return u_Day + 244;
        case 10:    return u_Day + 274;
        case 11:    return u_Day + 305;
        case 12:    return u_Day + 335;
    }

    return 0;
}

static inline uintsys DayOfTheNonLeapYear (uintsys u_Month, uintsys u_Day)
{
    switch (u_Month)
    {
        case 1:     return u_Day;
        case 2:     return u_Day +  31;
        case 3:     return u_Day +  59;
        case 4:     return u_Day +  90;
        case 5:     return u_Day + 120;
        case 6:     return u_Day + 151;
        case 7:     return u_Day + 181;
        case 8:     return u_Day + 212;
        case 9:     return u_Day + 243;
        case 10:    return u_Day + 273;
        case 11:    return u_Day + 304;
        case 12:    return u_Day + 334;
    }

    return 0;
}

static inline uintsys Year1752Correction (uintsys u_Year, uintsys u_Month,
                                          uintsys u_Day)
{
    return ((u_Year == 1752) && ((u_Month > 9) ||
           ((u_Month == 9) && (u_Day >= 14)))) ? 11 : 0;
}

static inline uintsys DaysSinceJesus (uintsys u_Year, uintsys u_Month,
                                      uintsys u_Day)
{
    return DaysBeforeJan1 (u_Year)
         + (IsLeapYear (u_Year) ? DayOfTheLeapYear    (u_Month, u_Day)
                                : DayOfTheNonLeapYear (u_Month, u_Day))
         - Year1752Correction (u_Year, u_Month, u_Day);
}

static inline uintsys Milliseconds (uintsys u_Hours, uintsys u_Minutes,
                                    uintsys u_Seconds)
{
    return ((u_Hours * 60 + u_Minutes) * 60 + u_Seconds) * 1000;
}

Date::Date (uintsys u_Year, uintsys u_Month, uintsys u_Day, uintsys u_Hour,
            uintsys u_Minute, uintsys u_Second)
    : u_Days_   (DaysSinceJesus (u_Year, u_Month,  u_Day))
    , u_Millis_ (Milliseconds   (u_Hour, u_Minute, u_Second))
{
    u_Days_   += u_Millis_ / (u_SecondsPerDay_ * 1000);
    u_Millis_ %= (u_SecondsPerDay_ * 1000);
}

Date::Date (const DateParts& parts)
    : u_Days_   (DaysSinceJesus (parts.Year(), parts.Month(),  parts.Day()))
    , u_Millis_ (Milliseconds   (parts.Hour(), parts.Minute(), parts.Second()))
{
    u_Days_   += u_Millis_ / (u_SecondsPerDay_ * 1000);
    u_Millis_ %= (u_SecondsPerDay_ * 1000);
}

Date::Date (time_t t)
    : u_Days_   (date_UNIX_Epoch_.u_Days_)
    , u_Millis_ (0)
{
    AddSeconds (t);
}

Date::Date (const struct tm* p_Time)
    : u_Days_   (1)
    , u_Millis_ (0)
{
    if (p_Time != 0)
    {
        Date date (p_Time->tm_year + 1900,
                   p_Time->tm_mon + 1,
                   p_Time->tm_mday,
                   p_Time->tm_hour,
                   p_Time->tm_min,
                   p_Time->tm_sec);

        operator= (date);

#ifdef HAVE_STRUCT_TM_GMTOFF
        AddSeconds (- p_Time->tm_gmtoff);
#endif
    }
}

Date::Date (const struct timeval* p_Time)
    : u_Days_   (date_UNIX_Epoch_.u_Days_)
    , u_Millis_ (0)
{
    if (p_Time)
    {
        u_Millis_ = (p_Time->tv_usec + 500) / 1000;

        AddSeconds (p_Time->tv_sec);
    }
}

Date::Date ()
    : u_Days_   (date_UNIX_Epoch_.u_Days_)
    , u_Millis_ (date_UNIX_Epoch_.u_Millis_)
{
    time_t t = time (0);

    AddSeconds (t);
}

Date Date::Now ()
{
    return Date (time(0));
}

intsys Date::DaysMoreThan (const Date& date) const
{
    if (operator< (date))
    {
        return -date.DaysMoreThan (*this);
    }

    // *this is later than date

    uintsys u_Days = u_Days_ - date.u_Days_;

    return (u_Millis_ >= date.u_Millis_) ? u_Days : u_Days - 1;
}

double Date::SecondsMoreThan (const Date& date) const
{
    if (operator< (date))
    {
        return -date.SecondsMoreThan (*this);
    }

    // *this is later than date

    double d_Seconds = u_Days_ - date.u_Days_;

    d_Seconds *= u_SecondsPerDay_;

    if (u_Millis_ >= date.u_Millis_)
    {
        return d_Seconds + (u_Millis_ - date.u_Millis_) / 1000.0;
    }
    else
    {
        return d_Seconds - (date.u_Millis_ - u_Millis_) / 1000.0;
    }
}

uintsys Date::DayOfWeek () const
{
    return (u_Days_ + 5) % 7;  // Jan 1, 1 A.D. is a Saturday
}

void Date::AddDays (intsys n_Days)
{
    if (n_Days < 0)
    {
        uintsys u_Subtract = -n_Days;

        if (u_Subtract >= u_Days_)
        {
            u_Days_   = 1;
            u_Millis_ = 0;
        }
        else
        {
            u_Days_ -= u_Subtract;
        }
    }
    else
    {
        u_Days_ += n_Days;
    }
}

void Date::AddSeconds (double d_Seconds)
{
    const uintsys u_MillisPerDay = 1000 * u_SecondsPerDay_;

    double  d_Millis = (1000. * fabs (d_Seconds)) + 0.5;
    uintsys u_Millis = d_Millis;
    uintsys u_Days   = u_Millis / u_MillisPerDay;

    u_Millis -= (u_Days * u_MillisPerDay);

    if (d_Seconds < 0)
    {
        if (u_Millis > u_Millis_)
        {
            u_Millis_ = u_MillisPerDay - (u_Millis - u_Millis_);
            ++u_Days;
        }
        else
        {
            u_Millis_ -= u_Millis;
        }

        AddDays (-u_Days);
    }
    else
    {
        u_Millis_ += u_Millis;

        if (u_Millis_ >= u_MillisPerDay)
        {
            u_Millis_ -= u_MillisPerDay;
            ++u_Days;
        }

        AddDays (u_Days);
    }
}

DateParts::DateParts (uintsys u_Year, uintsys u_Month,  uintsys u_Day,
                      uintsys u_Hour, uintsys u_Minute, uintsys u_Second)
    : u_Year_     (u_Year)
    , u_Month_    (u_Month)
    , u_Day_      (u_Day)
    , u_Hour_     (u_Hour)
    , u_Minute_   (u_Minute)
    , u_Second_   (u_Second)
{
    // normalize the values presented

    Date date (*this);

    operator= (date);
}

DateParts::DateParts (const Date& date)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    uintsys u = (date.u_Millis_ + 500) / 1000;

    u_Second_ = u % 60; u /= 60;
    u_Minute_ = u % 60; u /= 60;
    u_Hour_   = u % 24; u /= 24;

    DaysToCalendar (u + date.u_Days_, u_Year_, u_Month_, u_Day_);
}

DateParts::DateParts (time_t t)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    Date date (t);

    operator= (date);
}

DateParts::DateParts (const struct timeval* p_Time)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    Date date (p_Time);

    operator= (date);
}

DateParts::DateParts (const struct tm* p_Time)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    if (p_Time != 0)
    {
        DateParts parts (p_Time->tm_year + 1900,
                         p_Time->tm_mon + 1,
                         p_Time->tm_mday,
                         p_Time->tm_hour,
                         p_Time->tm_min,
                         p_Time->tm_sec);

#ifdef HAVE_STRUCT_TM_GMTOFF
        parts.AddSeconds (- p_Time->tm_gmtoff);
#endif

        operator= (parts);
    }
}

DateParts::DateParts ()
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    operator= (Date::Now());
}

void DateParts::SetDay (uintsys u_Day)
{
    if ((u_Day > 0) && (u_Day <= 31))
    {
        if (u_Day > u_Day_)
        {
            AddDays (u_Day - u_Day_);
        }
        else
        {
            u_Day_ = u_Day;
        }
    }
}

static inline void AppendOneDigit (String& str, uintsys u)
{
    str.Append (static_cast<char> ('0' + (u%10)));
}

static inline void AppendTwoDigit (String& str, uintsys u)
{
    AppendOneDigit (str, u/10);
    AppendOneDigit (str, u);
}

static inline void AppendTwoDigit (String& str, uintsys u, char c)
{
    AppendTwoDigit (str, u);

    str += c;
}

String Date::FormatGeneralizedTime () const
{
    DateParts parts (*this);

    return parts.FormatGeneralizedTime();
}

String Date::FormatUTCTime () const
{
    DateParts parts (*this);

    return parts.FormatUTCTime();
}

String Date::FormatHTTP () const
{
    DateParts parts (*this);

    return parts.FormatHTTP();
}

String Date::FormatRFC2822 () const
{
    DateParts parts (*this);

    return parts.FormatRFC2822();
}

bool Date::operator== (const Date& date) const
{
    return (DaysMoreThan (date) == 0) &&
            (AbsoluteValue (SecondsMoreThan (date)) < 0.5);
}

String DateParts::DayOfWeekText_ (uintsys u_DayOfWeek)
{
    switch (u_DayOfWeek)
    {
        case 0: return str_Sun_;
        case 1: return str_Mon_;
        case 2: return str_Tue_;
        case 3: return str_Wed_;
        case 4: return str_Thu_;
        case 5: return str_Fri_;
        case 6: return str_Sat_;
    }

    return String();
}

String DateParts::MonthText_ (uintsys u_Month)
{
    switch (u_Month)
    {
        case 1:  return str_Jan_;
        case 2:  return str_Feb_;
        case 3:  return str_Mar_;
        case 4:  return str_Apr_;
        case 5:  return str_May_;
        case 6:  return str_Jun_;
        case 7:  return str_Jul_;
        case 8:  return str_Aug_;
        case 9:  return str_Sep_;
        case 10: return str_Oct_;
        case 11: return str_Nov_;
        case 12: return str_Dec_;
    }

    return String();
}

String DateParts::FormatGeneralizedTime () const
{
    String str_Date (Preallocate(15));

    AppendTwoDigit (str_Date, u_Year_ / 100);
    AppendTwoDigit (str_Date, u_Year_ % 100);
    AppendTwoDigit (str_Date, u_Month_);
    AppendTwoDigit (str_Date, u_Day_);
    AppendTwoDigit (str_Date, u_Hour_);
    AppendTwoDigit (str_Date, u_Minute_);
    AppendTwoDigit (str_Date, u_Second_);

    str_Date.Append ('Z');

    return str_Date;
}

String DateParts::FormatHTTP () const
{
    // example: "Tue, 15 Nov 1994 12:45:26 GMT"

    String str_Date (Preallocate(29));

    str_Date.Append (DayOfWeekText_(DayOfWeek()));
    str_Date.Append (", ");
    AppendTwoDigit  (str_Date,   u_Day_,         ' ');
    str_Date.Append (MonthText_ (u_Month_));
    str_Date.Append (' ');
    AppendTwoDigit  (str_Date,   u_Year_ / 100);
    AppendTwoDigit  (str_Date,   u_Year_ % 100,  ' ');
    AppendTwoDigit  (str_Date,   u_Hour_,        ':');
    AppendTwoDigit  (str_Date,   u_Minute_,      ':');
    AppendTwoDigit  (str_Date,   u_Second_,      ' ');
    str_Date.Append (str_GMT_);

    return str_Date;
}

String DateParts::FormatRFC2822 () const
{
    // example: "Tue, 15 Nov 1994 04:45:26 -0800"

    String str_Date (Preallocate(31));

    str_Date.Append (DayOfWeekText_(DayOfWeek()));
    str_Date.Append (", ");
    AppendTwoDigit  (str_Date,   u_Day_,         ' ');
    str_Date.Append (MonthText_ (u_Month_));
    str_Date.Append (' ');
    AppendTwoDigit  (str_Date,   u_Year_ / 100);
    AppendTwoDigit  (str_Date,   u_Year_ % 100,  ' ');
    AppendTwoDigit  (str_Date,   u_Hour_,        ':');
    AppendTwoDigit  (str_Date,   u_Minute_,      ':');
    AppendTwoDigit  (str_Date,   u_Second_);

    str_Date.Append (" +0000");

    return str_Date;
}

String DateParts::FormatUTCTime () const
{
    String str_Date (Preallocate(13));

    AppendTwoDigit (str_Date, u_Year_ % 100);
    AppendTwoDigit (str_Date, u_Month_);
    AppendTwoDigit (str_Date, u_Day_);
    AppendTwoDigit (str_Date, u_Hour_);
    AppendTwoDigit (str_Date, u_Minute_);
    AppendTwoDigit (str_Date, u_Second_);

    str_Date.Append ('Z');

    return str_Date;
}

bool DateParts::FromGeneralizedTime (const String& str_Date)
{
    // GeneralizedTime format recognized:
    //
    //     YYYYMMDDhh(mm)?(ss)?(.f*)?(Z|[-+]hh(mm)?)

    DateParts parts;

    StringIter iter (str_Date);

    ParseError error;

    if (!(iter.ParseNumber (4, parts.u_Year_,   error) &&
          iter.ParseNumber (2, parts.u_Month_,  error) &&
          iter.ParseNumber (2, parts.u_Day_,    error) &&
          iter.ParseNumber (2, parts.u_Hour_,   error)))
    {
        return false;
    }

    iter.ParseNumber (2, parts.u_Minute_, error);     // optional minutes
    iter.ParseNumber (2, parts.u_Second_, error);     // optional seconds

    if (iter.SkipChar ('.', error))
    {
        uintsys u_Fraction = 0;

        iter.ParseNumber (u_Fraction, error);   // fractional part ignored
    }

    uintsys u_TZ = 0;

    if (!iter.SkipChar ('Z', error))
    {
        if (!(iter.SkipChar ('-', error) || iter.SkipChar ('+', error)))
        {
            return false;
        }

        if (!iter.ParseNumber (2, u_TZ, error)) // timezone offset (hours)
        {
            return false;
        }

        iter.ParseNumber (2, u_TZ, error); // timezone offset (optional minutes)
    }

    if (iter)
    {
        return false;
    }

    Swap (parts);

    return true;
}

bool DateParts::FromUTCTime (const String& str_Date)
{
    StringIter iter (str_Date);
    ParseError error;
    DateParts  parts;

    if (!(iter.ParseNumber (2, parts.u_Year_,   error) &&
          iter.ParseNumber (2, parts.u_Month_,  error) &&
          iter.ParseNumber (2, parts.u_Day_,    error) &&
          iter.ParseNumber (2, parts.u_Hour_,   error) &&
          iter.ParseNumber (2, parts.u_Minute_, error)))
    {
        return false;
    }

    iter.ParseNumber (2, parts.u_Second_, error);     // optional seconds

    // UTC Time can handle years from 1950 thru 2049

    parts.u_Year_ += (parts.u_Year_ >= 50) ? 1900 : 2000;

    if (iter.SkipChar ('Z', error) && !iter)
    {
        Swap (parts);

        return true;
    }

    uintsys u_TZ = 0;

    if (!(iter.SkipChar ('+', error) || iter.SkipChar ('-', error)))
    {
        return false;
    }

    if (!iter.ParseNumber (4, u_TZ, error) || iter)
    {
        return false;
    }

    Swap (parts);

    return true;
}

StringList DateParts::ParseHTTPDate_ (const String& str_Date)
{
    PerlRegexMatches matches;
    StringList       strl_Parts;

    if (str_Date.Match (regex_Http1_, matches))
    {
        strl_Parts.Append (matches.GetMatch (3));   // year
        strl_Parts.Append (matches.GetMatch (2));   // month
        strl_Parts.Append (matches.GetMatch (1));   // day
        strl_Parts.Append (matches.GetMatch (4));   // hour
        strl_Parts.Append (matches.GetMatch (5));   // minute
        strl_Parts.Append (matches.GetMatch (7));   // second
    }
    else if (str_Date.Match (regex_Http2_, matches))
    {
        strl_Parts.Append (matches.GetMatch (7));   // year
        strl_Parts.Append (matches.GetMatch (1));   // month
        strl_Parts.Append (matches.GetMatch (2));   // day
        strl_Parts.Append (matches.GetMatch (3));   // hour
        strl_Parts.Append (matches.GetMatch (4));   // minute
        strl_Parts.Append (matches.GetMatch (6));   // second
    }

    return strl_Parts;
}

bool DateParts::SetMonth_ (const String& str_Month)
{
    if (str_Month == str_Jan_) { u_Month_ =  1; return true; }
    if (str_Month == str_Feb_) { u_Month_ =  2; return true; }
    if (str_Month == str_Mar_) { u_Month_ =  3; return true; }
    if (str_Month == str_Apr_) { u_Month_ =  4; return true; }
    if (str_Month == str_May_) { u_Month_ =  5; return true; }
    if (str_Month == str_Jun_) { u_Month_ =  6; return true; }
    if (str_Month == str_Jul_) { u_Month_ =  7; return true; }
    if (str_Month == str_Aug_) { u_Month_ =  8; return true; }
    if (str_Month == str_Sep_) { u_Month_ =  9; return true; }
    if (str_Month == str_Oct_) { u_Month_ = 10; return true; }
    if (str_Month == str_Nov_) { u_Month_ = 11; return true; }
    if (str_Month == str_Dec_) { u_Month_ = 12; return true; }

    return false;
}

bool DateParts::FromHTTP (const String& str_Date)
{
    StringList strl_Parts (ParseHTTPDate_ (str_Date));

    if (strl_Parts.IsEmpty())
    {
        return false;
    }

    Backup<DateParts> backup (*this);

    String str_Month;

    u_Year_     = strl_Parts.Shift().AsInt();
    str_Month   = strl_Parts.Shift();
    u_Day_      = strl_Parts.Shift().AsInt();
    u_Hour_     = strl_Parts.Shift().AsInt();
    u_Minute_   = strl_Parts.Shift().AsInt();
    u_Second_   = strl_Parts.Shift().AsInt();

    u_Year_ = (u_Year_ < 50)   ? (u_Year_ + 2000) :
              (u_Year_ < 1000) ? (u_Year_ + 1900) : u_Year_;

    if (SetMonth_ (str_Month))
    {
        return true;
    }

    backup.Restore();

    return false;
}

static inline uintsys ToFourDigitYear (uintsys u_Year)
{
    return (u_Year <   50) ? (u_Year + 2000) :
           (u_Year < 1000) ? (u_Year + 1900) : u_Year;
}

bool DateParts::FromRFC2822 (const String& str_Date)
{
    PerlRegexMatches matches;

    if (!str_Date.Match (regex_RFC2822_Date, matches))
    {
        return false;
    }

    Backup<DateParts> backup (*this);

    String str_Month    (matches.GetMatch(3));
    String str_TimeZone (matches.GetMatch(10));

    u_Year_     = ToFourDigitYear (matches.GetMatch(4).AsInt());
    u_Day_      = matches.GetMatch(2).AsInt();
    u_Hour_     = matches.GetMatch(5).AsInt();
    u_Minute_   = matches.GetMatch(6).AsInt();
    u_Second_   = matches.GetMatch(8).AsInt();

    if (SetMonth_ (str_Month))
    {
        AddMinutes (-DecodeTimeZoneName (str_TimeZone));

        return true;
    }

    backup.Restore();

    return false;
}

void DateParts::AddYears (intsys n_Years)
{
    if (n_Years < 0)
    {
        uintsys u_Subtract = -n_Years;

        if (u_Subtract < u_Year_)
        {
            u_Year_ -= u_Subtract;
        }
        else
        {
            operator= (DateParts());
        }
    }
    else
    {
        u_Year_ += n_Years;

        if (u_Year_ > 9999)
        {
            operator= (DateParts(9999,12,31));
        }
    }
}

void DateParts::AddMonths (intsys n_Months)
{
    if (n_Months < 0)
    {
        uintsys u_Subtract = -n_Months;

        if (u_Subtract < u_Month_)
        {
            u_Month_ -= u_Subtract;
        }
        else
        {
            u_Subtract -= u_Month_;
            u_Month_    = 12;

            u_Month_  -= u_Subtract % 12;
            u_Subtract = u_Subtract / 12 + 1;

            if (u_Subtract >= u_Year_)
            {
                operator= (DateParts());
            }
            else
            {
                u_Year_ -= u_Subtract;
            }
        }
    }
    else
    {
        u_Month_ += n_Months;

        if (u_Month_ > 12)
        {
            --u_Month_;
            u_Year_  += u_Month_ / 12;
            u_Month_ %= 12;
            ++u_Month_;

            if (u_Year_ > 9999)
            {
                operator= (DateParts(9999,12,31));
            }
        }
    }
}

void DateParts::AddDays (intsys n_Days)
{
    Date date (*this);

    date.AddDays (n_Days);

    operator= (date);
}

void DateParts::AddHours (intsys n_Hours)
{
    Date date (*this);

    date.AddHours (n_Hours);

    operator= (date);
}

void DateParts::AddMinutes (intsys n_Minutes)
{
    Date date (*this);

    date.AddMinutes (n_Minutes);

    operator= (date);
}

void DateParts::AddSeconds (double d_Seconds)
{
    Date date (*this);

    date.AddSeconds (d_Seconds);

    operator= (date);
}

void DateParts::SetYear (uintsys u_Year)
{
    if (u_Year > 9999)
    {
        operator= (DateParts(9999,12,31));
    }
    else if (u_Year < 1)
    {
        operator= (DateParts());
    }
    else
    {
        u_Year_ = u_Year;
    }
}

void DateParts::SetMonth (uintsys u_Month)
{
    if (u_Month > 0)
    {
        u_Month_ = ((u_Month - 1) % 12) + 1;
    }
    else
    {
        u_Month_ = 12;
    }
}

void DateParts::SetHour (uintsys u_Hour)
{
    u_Hour_ = u_Hour % 24;
}

void DateParts::SetMinute (uintsys u_Minute)
{
    u_Minute_ = u_Minute % 60;
}

void DateParts::SetSecond (uintsys u_Second)
{
    u_Second_ = u_Second % 60;
}

bool DateParts::operator== (const DateParts& parts) const
{
    return (u_Year_     == parts.u_Year_)   &&
           (u_Month_    == parts.u_Month_)  &&
           (u_Day_      == parts.u_Day_)    &&
           (u_Hour_     == parts.u_Hour_)   &&
           (u_Minute_   == parts.u_Minute_) &&
           (u_Second_   == parts.u_Second_);
}

void DateParts::Swap (DateParts& parts)
{
    using std::swap;

    swap (u_Year_,      parts.u_Year_);
    swap (u_Month_,     parts.u_Month_);
    swap (u_Day_,       parts.u_Day_);
    swap (u_Hour_,      parts.u_Hour_);
    swap (u_Minute_,    parts.u_Minute_);
    swap (u_Second_,    parts.u_Second_);
}

LocalDate::LocalDate (uintsys u_Year, uintsys u_Month, uintsys u_Day,
                      uintsys u_Hour, uintsys u_Minute, uintsys u_Second,
                      intsys n_TZOffset)
    : date_       (u_Year, u_Month, u_Day, u_Hour, u_Minute, u_Second)
    , n_TZOffset_ (0)
{
    SetTimeZone (n_TZOffset);
}

static inline intsys TimeZoneOffset (const struct tm* p_Time)
{
#ifdef HAVE_STRUCT_TM_GMTOFF
    return p_Time ? (p_Time->tm_gmtoff / 60) : 0;
#else
    return 0;
#endif
}

static inline intsys LocalTimeZoneOffset (time_t t)
{
#ifdef HAVE_AWFUL_DATE_FUNCTIONS
    MutexLocker locker (gmutex_LocalTime);

    return TimeZoneOffset (localtime (&t));
#else
    struct tm tm_Local;

    return TimeZoneOffset (localtime_r (&t, &tm_Local));
#endif
}

LocalDate::LocalDate (time_t t)
    : date_       (t)
    , n_TZOffset_ (0)
{
    SetTimeZone (LocalTimeZoneOffset (t));
}

LocalDate::LocalDate (const struct tm* p_Time)
    : date_       (p_Time)
    , n_TZOffset_ (0)
{
    SetTimeZone (TimeZoneOffset (p_Time));
}

LocalDate::LocalDate (const struct timeval* p_Time)
    : date_       (p_Time)
    , n_TZOffset_ (0)
{
    if (p_Time)
    {
        SetTimeZone (LocalTimeZoneOffset (p_Time->tv_sec));
    }
}

LocalDate LocalDate::Now ()
{
    LocalDate date (time(0));

    return date;
}

void LocalDate::SetTimeZone (const String& str_TimeZoneName)
{
    SetTimeZone (DecodeTimeZoneName (str_TimeZoneName));
}

void LocalDate::SetTimeZone (intsys n_MinutesEastOfGMT)
{
    date_.AddMinutes (-n_TZOffset_);
    date_.AddMinutes (n_MinutesEastOfGMT);

    if (n_MinutesEastOfGMT >= 0)
    {
        n_TZOffset_ = n_MinutesEastOfGMT % (24 * 60);
    }
    else if (n_MinutesEastOfGMT < 0)
    {
        n_TZOffset_ = - ((-n_MinutesEastOfGMT) % (24 * 60));
    }
}

String LocalDate::FormatRFC2822 () const
{
    String str_RFC2822 (date_.FormatRFC2822());

    if (n_TZOffset_ == 0)
    {
        return str_RFC2822;
    }

    uintsys u_Offset  = AbsoluteValue (n_TZOffset_);
    uintsys u_Hours   = u_Offset / 60;
    uintsys u_Minutes = u_Offset % 60;

    str_RFC2822.EraseEnd (5);
    str_RFC2822.Append   ((n_TZOffset_ < 0) ? '-' : '+');

    AppendTwoDigit (str_RFC2822, u_Hours);
    AppendTwoDigit (str_RFC2822, u_Minutes);

    return str_RFC2822;
}

bool LocalDate::FromRFC2822 (const String& str_Date)
{
    if (date_.FromRFC2822 (str_Date))
    {
        n_TZOffset_ = 0;

        PerlRegexMatches matches;

        str_Date.Match (regex_RFC2822_Date, matches);

        String str_TimeZone (matches.GetMatch(10));

        SetTimeZone (str_TimeZone);

        return true;
    }

    return false;
}

#undef MONTH_28
#undef MONTH_29
#undef MONTH_30
#undef MONTH_31
#undef DAYS_28
#undef DAYS_29
#undef DAYS_30
#undef DAYS_31

} // namespace mikestoolbox

