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

#include "mikestoolbox-1.2.h"

using namespace mikestoolbox;

class Tester
{
public:

    Tester (const String& str_Name);
    ~Tester ();

    void operator() (bool b);

    void Done ();

private:

    String  str_Name_;
    uintsys u_Count_;
    uintsys u_Correct_;
    bool    b_NormalExit_;
};

inline Tester::Tester (const String& str_Name)
    : str_Name_     (str_Name)
    , u_Count_      (0)
    , u_Correct_    (0)
    , b_NormalExit_ (false)
{
    str_Name_.Replace (".*?(\\w+)", "$1");  // basename
}

inline Tester::~Tester ()
{
    if (b_NormalExit_)
    {
        String str_Correct (u_Correct_);
        String str_Count   (u_Count_);

        str_Correct.PadFront (3);
        str_Count.PadFront   (3);

        std::cout << str_Correct << " of " << str_Count << " tests succeeded (";
        std::cout << str_Name_ << ")" << std::endl;
    }
    else
    {
        std::cout << "Abnormal exit after " << u_Count_ << " tests (";
        std::cout << str_Name_ << " - " << u_Correct_ << " correct)" << std::endl;
    }
}

inline void Tester::operator() (bool b)
{
    ++u_Count_;

    if (b)
    {
        ++u_Correct_;
    }
    else
    {
        std::cout << u_Count_ << " Failed (" << str_Name_ << ")" << std::endl;
    }
}

inline void Tester::Done ()
{
    b_NormalExit_ = true;
}

