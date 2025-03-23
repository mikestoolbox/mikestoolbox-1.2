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
//  File:       Date.inl
//
//  Synopsis:   Implementation of objects that represent dates and times
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline uintsys ToFourDigitYear (uintsys u_Year)
{
    return (u_Year <   50) ? (u_Year + 2000) :
           (u_Year < 1000) ? (u_Year + 1900) : u_Year;
}

inline TwoDigitYear::TwoDigitYear (uintsys u_Year)
    : year_ (ToFourDigitYear (u_Year))
{
    // nothing
}

inline TwoDigitYear::operator uintsys () const
{
    return year_;
}

inline FourDigitYear::FourDigitYear (uintsys u_Year)
    : year_ (u_Year)
{
    if (year_ == 0)
    {
        year_ = 1;
    }
    else if (year_ > 9999)
    {
        year_ = 9999;
    }
}

inline FourDigitYear::operator uintsys () const
{
    return year_;
}

inline uintsys Date::Year () const
{
    DateParts parts (*this);

    return parts.Year();
}

inline uintsys Date::Month () const
{
    DateParts parts (*this);

    return parts.Month();
}

inline uintsys Date::Day () const
{
    DateParts parts (*this);

    return parts.Day();
}

inline uintsys Date::Hour () const
{
    DateParts parts (*this);

    return parts.Hour();
}

inline uintsys Date::Minute () const
{
    DateParts parts (*this);

    return parts.Minute();
}

inline uintsys Date::Second () const
{
    DateParts parts (*this);

    return parts.Second();
}

inline void Date::SetYear (uintsys u_Year)
{
    DateParts parts (*this);

    parts.SetYear (u_Year);

    operator= (parts);
}

inline void Date::SetMonth (uintsys u_Month)
{
    DateParts parts (*this);

    parts.SetMonth (u_Month);

    operator= (parts);
}

inline void Date::SetDay (uintsys u_Day)
{
    DateParts parts (*this);

    parts.SetDay (u_Day);

    operator= (parts);
}

inline void Date::SetHour (uintsys u_Hour)
{
    DateParts parts (*this);

    parts.SetHour (u_Hour);

    operator= (parts);
}

inline void Date::SetMinute (uintsys u_Minute)
{
    DateParts parts (*this);

    parts.SetMinute (u_Minute);

    operator= (parts);
}

inline void Date::SetSecond (uintsys u_Second)
{
    DateParts parts (*this);

    parts.SetSecond (u_Second);

    operator= (parts);
}

inline void Date::AddYears (intsys n_Years)
{
    DateParts parts (*this);

    parts.AddYears (n_Years);

    operator= (parts);
}

inline void Date::AddMonths (intsys n_Months)
{
    DateParts parts (*this);

    parts.AddMonths (n_Months);

    operator= (parts);
}

inline void Date::AddMinutes (intsys n_Minutes)
{
    AddSeconds (60 * n_Minutes);
}

inline void Date::AddHours (intsys n_Hours)
{
    AddMinutes (60 * n_Hours);
}

inline bool Date::IsSunday () const
{
    return DayOfWeek() == 0;
}

inline bool Date::IsMonday () const
{
    return DayOfWeek() == 1;
}

inline bool Date::IsTuesday () const
{
    return DayOfWeek() == 2;
}

inline bool Date::IsWednesday () const
{
    return DayOfWeek() == 3;
}

inline bool Date::IsThursday () const
{
    return DayOfWeek() == 4;
}

inline bool Date::IsFriday () const
{
    return DayOfWeek() == 5;
}

inline bool Date::IsSaturday () const
{
    return DayOfWeek() == 6;
}

inline bool Date::FromGeneralizedTime (const String& str_Date)
{
    DateParts parts;

    if (parts.FromGeneralizedTime (str_Date))
    {
        operator= (parts);

        return true;
    }

    return false;
}

inline bool Date::FromHTTP (const String& str_Date)
{
    DateParts parts;

    if (parts.FromHTTP (str_Date))
    {
        operator= (parts);

        return true;
    }

    return false;
}

inline bool Date::FromRFC2822 (const String& str_Date)
{
    DateParts parts;

    if (parts.FromRFC2822 (str_Date))
    {
        operator= (parts);

        return true;
    }

    return false;
}

inline bool Date::FromUTCTime (const String& str_Date)
{
    DateParts parts;

    if (parts.FromUTCTime (str_Date))
    {
        operator= (parts);

        return true;
    }

    return false;
}

inline void Date::Swap (Date& date)
{
    using std::swap;

    swap (u_Days_,   date.u_Days_);
    swap (u_Millis_, date.u_Millis_);
}

inline Date& Date::operator= (const DateParts& parts)
{
    Date date (parts);

    return operator= (date);
}

inline bool Date::operator!= (const Date& date) const
{
    return !operator== (date);
}

inline bool Date::operator< (const Date& date) const
{
    return  (u_Days_   <  date.u_Days_) ||
           ((u_Days_   == date.u_Days_) &&
            (u_Millis_ <  date.u_Millis_));
}

inline bool Date::operator>= (const Date& date) const
{
    return !operator< (date);
}

inline bool Date::operator> (const Date& date) const
{
    return date.operator< (*this);
}

inline bool Date::operator<= (const Date& date) const
{
    return !date.operator< (*this);
}

inline uintsys DateParts::Year () const
{
    return u_Year_;
}

inline uintsys DateParts::Month () const
{
    return u_Month_;
}

inline uintsys DateParts::Day () const
{
    return u_Day_;
}

inline uintsys DateParts::Hour () const
{
    return u_Hour_;
}

inline uintsys DateParts::Minute () const
{
    return u_Minute_;
}

inline uintsys DateParts::Second () const
{
    return u_Second_;
}

inline uintsys DateParts::DayOfWeek () const
{
    Date date (*this);

    return date.DayOfWeek();
}

inline bool DateParts::IsSunday () const
{
    return DayOfWeek() == 0;
}

inline bool DateParts::IsMonday () const
{
    return DayOfWeek() == 1;
}

inline bool DateParts::IsTuesday () const
{
    return DayOfWeek() == 2;
}

inline bool DateParts::IsWednesday () const
{
    return DayOfWeek() == 3;
}

inline bool DateParts::IsThursday () const
{
    return DayOfWeek() == 4;
}

inline bool DateParts::IsFriday () const
{
    return DayOfWeek() == 5;
}

inline bool DateParts::IsSaturday () const
{
    return DayOfWeek() == 6;
}

inline DateParts& DateParts::operator= (const Date& date)
{
    DateParts parts (date);

    return operator= (parts);
}

inline bool DateParts::operator!= (const DateParts& parts) const
{
    return !operator== (parts);
}

inline bool DateParts::operator< (const DateParts& parts) const
{
    Date date1 (*this);
    Date date2 (parts);

    return date1 < date2;
}

inline bool DateParts::operator<= (const DateParts& parts) const
{
    Date date1 (*this);
    Date date2 (parts);

    return date1 <= date2;
}

inline bool DateParts::operator> (const DateParts& parts) const
{
    return !(parts <= *this);
}

inline bool DateParts::operator>= (const DateParts& parts) const
{
    return !(parts < *this);
}

inline LocalDate::LocalDate (const DateParts& parts)
    : date_       (parts)
    , n_TZOffset_ (0)
{
    // nothing
}

inline LocalDate::LocalDate (const Date& date)
    : date_       (date)
    , n_TZOffset_ (0)
{
    // nothing
}

inline LocalDate::LocalDate ()
    : date_       ()
    , n_TZOffset_ (0)
{
    // nothing
}

inline uintsys LocalDate::Year () const
{
    return date_.Year();
}

inline uintsys LocalDate::Month () const
{
    return date_.Month();
}

inline uintsys LocalDate::Day () const
{
    return date_.Day();
}

inline uintsys LocalDate::Hour () const
{
    return date_.Hour();
}

inline uintsys LocalDate::Minute () const
{
    return date_.Minute();
}

inline uintsys LocalDate::Second () const
{
    return date_.Second();
}

inline uintsys LocalDate::DayOfWeek () const
{
    return date_.DayOfWeek();
}

inline bool LocalDate::IsSunday () const
{
    return date_.IsSunday();
}

inline bool LocalDate::IsMonday () const
{
    return date_.IsMonday();
}

inline bool LocalDate::IsTuesday () const
{
    return date_.IsTuesday();
}

inline bool LocalDate::IsWednesday () const
{
    return date_.IsWednesday();
}

inline bool LocalDate::IsThursday () const
{
    return date_.IsThursday();
}

inline bool LocalDate::IsFriday () const
{
    return date_.IsFriday();
}

inline bool LocalDate::IsSaturday () const
{
    return date_.IsSaturday();
}

inline void LocalDate::AddYears (intsys n_Years)
{
    date_.AddYears (n_Years);
}

inline void LocalDate::AddMonths (intsys n_Months)
{
    date_.AddMonths (n_Months);
}

inline void LocalDate::AddDays (intsys n_Days)
{
    date_.AddDays (n_Days);
}

inline void LocalDate::AddHours (intsys n_Hours)
{
    date_.AddHours (n_Hours);
}

inline void LocalDate::AddMinutes (intsys n_Minutes)
{
    date_.AddMinutes (n_Minutes);
}

inline void LocalDate::AddSeconds (double d_Seconds)
{
    date_.AddSeconds (d_Seconds);
}

inline void LocalDate::SetYear (uintsys u_Year)
{
    date_.SetYear (u_Year);
}

inline void LocalDate::SetMonth (uintsys u_Month)
{
    date_.SetMonth (u_Month);
}

inline void LocalDate::SetDay (uintsys u_Day)
{
    date_.SetDay (u_Day);
}

inline void LocalDate::SetHour (uintsys u_Hour)
{
    date_.SetHour (u_Hour);
}

inline void LocalDate::SetMinute (uintsys u_Minute)
{
    date_.SetMinute (u_Minute);
}

inline void LocalDate::SetSecond (uintsys u_Second)
{
    date_.SetSecond (u_Second);
}

inline Date LocalDate::GreenwichMeanTime () const
{
    LocalDate date (*this);

    date.SetTimeZone (0);

    return date.date_;
}

inline intsys LocalDate::DaysMoreThan (const LocalDate& date_Other) const
{
    return GreenwichMeanTime().DaysMoreThan (date_Other.GreenwichMeanTime());
}

inline double LocalDate::SecondsMoreThan (const LocalDate& date_Other) const
{
    return GreenwichMeanTime().SecondsMoreThan (date_Other.GreenwichMeanTime());
}

inline String LocalDate::FormatGeneralizedTime () const
{
    return GreenwichMeanTime().FormatGeneralizedTime();
}

inline String LocalDate::FormatHTTP () const
{
    return GreenwichMeanTime().FormatHTTP();
}

inline String LocalDate::FormatUTCTime () const
{
    return GreenwichMeanTime().FormatUTCTime();
}

inline bool LocalDate::FromGeneralizedTime (const String& str_Date)
{
    if (date_.FromGeneralizedTime (str_Date))
    {
        date_.AddMinutes (n_TZOffset_);

        return true;
    }

    return false;
}

inline bool LocalDate::FromHTTP (const String& str_Date)
{
    if (date_.FromHTTP (str_Date))
    {
        date_.AddMinutes (n_TZOffset_);

        return true;
    }

    return false;
}

inline bool LocalDate::FromUTCTime (const String& str_Date)
{
    if (date_.FromUTCTime (str_Date))
    {
        date_.AddMinutes (n_TZOffset_);

        return true;
    }

    return false;
}

inline void LocalDate::Swap (LocalDate& date)
{
    date_.Swap (date.date_);

    std::swap (n_TZOffset_, date.n_TZOffset_);
}

inline bool LocalDate::operator== (const LocalDate& date) const
{
    return GreenwichMeanTime() == date.GreenwichMeanTime();
}

inline bool LocalDate::operator!= (const LocalDate& date) const
{
    return !operator== (date);
}

inline bool LocalDate::operator< (const LocalDate& date) const
{
    return GreenwichMeanTime() < date.GreenwichMeanTime();
}

inline bool LocalDate::operator>= (const LocalDate& date) const
{
    return !operator< (date);
}

inline bool LocalDate::operator> (const LocalDate& date) const
{
    return date.GreenwichMeanTime() < GreenwichMeanTime();
}

inline bool LocalDate::operator<= (const LocalDate& date) const
{
    return !operator> (date);
}

inline LocalDate& LocalDate::operator= (const DateParts& parts)
{
    date_ = parts;

    date_.AddMinutes (n_TZOffset_);

    return *this;
}

inline LocalDate& LocalDate::operator= (const Date& date)
{
    date_ = date;

    date_.AddMinutes (n_TZOffset_);

    return *this;
}

} // namespace mikestoolbox

namespace std {

template<>
inline void swap<mikestoolbox::Date> (mikestoolbox::Date& date1,
                                      mikestoolbox::Date& date2)
{
    date1.Swap (date2);
}

template<>
inline void swap<mikestoolbox::DateParts> (mikestoolbox::DateParts& parts1,
                                           mikestoolbox::DateParts& parts2)
{
    parts1.Swap (parts2);
}

template<>
inline void swap<mikestoolbox::LocalDate> (mikestoolbox::LocalDate& date1,
                                           mikestoolbox::LocalDate& date2)
{
    date1.Swap (date2);
}

} // namespace std

