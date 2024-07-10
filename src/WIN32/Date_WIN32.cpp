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
//  Synopsis:   Windows implementation of Date and LocalDate objects
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_WINDOWS

namespace mikestoolbox {

Date Date::Now ()
{
    SYSTEMTIME time_Now;

    GetSystemTime (&time_Now);

    return Date (&time_Now);
}

Date::Date (const FILETIME* p_Time)
    : u_Days_   (1)
    , u_Millis_ (0)
{
    if (p_Time != 0)
    {
        SYSTEMTIME systime;

        if (FileTimeToSystemTime (p_Time, &systime) != 0)
        {
            Date date (&systime);

            operator= (date);
        }
    }
}

Date::Date (const LPSYSTEMTIME p_Time)
    : u_Days_   (1)
    , u_Millis_ (0)
{
    if (p_Time != 0)
    {
        Date date (p_Time->wYear,
                   p_Time->wMonth,
                   p_Time->wDay,
                   p_Time->wHour,
                   p_Time->wMinute,
                   p_Time->wSecond);

        operator= (date);

        u_Millis_ += p_Time->wMilliseconds;
    }
}

DateParts::DateParts (const FILETIME* p_Time)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    if (p_Time != 0)
    {
        SYSTEMTIME systime;

        if (FileTimeToSystemTime (p_Time, &systime) != 0)
        {
            Date date (&systime);

            operator= (date);
        }
    }
}

DateParts::DateParts (const LPSYSTEMTIME p_Time)
    : u_Year_     (1)
    , u_Month_    (1)
    , u_Day_      (1)
    , u_Hour_     (0)
    , u_Minute_   (0)
    , u_Second_   (0)
{
    if (p_Time != 0)
    {
        Date date (p_Time);

        operator= (date);
    }
}

LocalDate::LocalDate (const FILETIME* p_Time)
    : date_       (p_Time)
    , n_TZOffset_ (0)
{
    // nothing
}

LocalDate::LocalDate (const LPSYSTEMTIME p_Time)
    : date_       (p_Time)
    , n_TZOffset_ (0)
{
    // nothing
}

} // namespace mikestoolbox

#endif // PLATFORM_WINDOWS

