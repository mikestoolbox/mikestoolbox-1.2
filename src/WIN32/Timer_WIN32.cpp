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
//  File:       WIN32/Timer.cpp
//
//  Synopsis:   Windows implementation of system timer objects
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_WINDOWS

namespace mikestoolbox {

static const double d_Scale = 65536.0 * 65536.0;    // 2^32

Timer::Timer ()
    : d_Frequency_ (0.0)
    , d_Count_     (0.0)
{
    LARGE_INTEGER li;

    if (QueryPerformanceFrequency (&li))
    {
        double d = li.HighPart * d_Scale;

        d += li.LowPart;

        d_Frequency_ = d;
    }

    if (QueryPerformanceCounter (&li))
    {
        double d = li.HighPart * d_Scale;

        d += li.LowPart;

        d_Count_ = d;
    }
}

double Timer::Elapsed ()
{
    double d_Elapsed = 0.0;

    if (d_Frequency_ != 0.0)
    {
        LARGE_INTEGER li;

        if (QueryPerformanceCounter (&li))
        {
            double d = li.HighPart * d_Scale;

            d += li.LowPart;

            double d_Diff = d - d_Count_;

            d_Count_ = d;

            d_Elapsed = d_Diff / d_Frequency_;
        }
    }

    return d_Elapsed;
}

} // namespace mikestoolbox

#endif // PLATFORM_WINDOWS

