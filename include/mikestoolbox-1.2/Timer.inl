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
//  File:       Timer.inl
//
//  Synopsis:   Implementation of timer objects
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline Timer::operator double ()
{
    return Elapsed();
}

inline std::ostream& operator<< (std::ostream& os, Timer& timer)
{
    os << timer.Elapsed();

    return os;
}

inline void Timer::Print (std::ostream& os)
{
    os << Elapsed() << std::endl;
}

inline void Timer::Print ()
{
    Print (std::cout);
}

inline void Timer::Reset ()
{
    (void) Elapsed();
}

inline AutoPrintTimer::AutoPrintTimer (const String& str_Label, double d_Threshold)
    : timer_       ()
    , str_Label_   (str_Label)
    , d_Threshold_ (d_Threshold)
    , b_Enable_    (true)
{
    if (d_Threshold < 0.0)
    {
        d_Threshold_ = d_DefaultThreshold_;
    }
}

inline AutoPrintTimer::AutoPrintTimer ()
    : timer_       ()
    , str_Label_   ()
    , d_Threshold_ (d_DefaultThreshold_)
    , b_Enable_    (true)
{
    // nothing
}

inline AutoPrintTimer::~AutoPrintTimer ()
{
    if (b_Enable_)
    {
        double d_Elapsed = timer_.Elapsed();

        if (d_Elapsed >= d_Threshold_)
        {
            String str_Output (str_Label_);

            if (!str_Output.IsEmpty())
            {
                str_Output.Append (": ");
            }

            str_Output.Append (d_Elapsed);
            str_Output.Append ('\n');

            std::cout << str_Output << std::flush;
        }
    }
}

inline void AutoPrintTimer::SetDefaultThreshold (double d_Threshold)
{
    d_DefaultThreshold_ = d_Threshold;
}

inline void AutoPrintTimer::Reset ()
{
    timer_.Reset();
}

inline void AutoPrintTimer::Disable ()
{
    b_Enable_ = false;
}

} // namespace mikestoolbox

