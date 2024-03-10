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
//  File:       DoubleFormat.inl
//
//  Synopsis:   Implementation of DoubleFormat class
//----------------------------------------------------------------------------

#ifdef MSVC
#define snprintf _snprintf
#endif

namespace mikestoolbox {

inline DoubleFormat::DoubleFormat ()
    : n_Precision_  (n_DefaultPrecision_)
    , b_Scientific_ (b_DefaultScientific_)
    , b_Decimal_    (b_DefaultDecimal_)
{
    // nothing
}

inline void DoubleFormat::SetPrecision (int n_Precision)
{
    n_Precision_ = n_Precision;
}

inline void DoubleFormat::UseScientificNotation ()
{
    b_Scientific_ = true;
    b_Decimal_    = false;
}

inline void DoubleFormat::UseDecimalNotation ()
{
    b_Scientific_ = false;
    b_Decimal_    = true;
}

inline void DoubleFormat::AutoSelectNotation ()
{
    b_Scientific_ = false;
    b_Decimal_    = false;
}

inline void DoubleFormat::SetDefaultPrecision (int n_Precision)
{
    n_DefaultPrecision_ = n_Precision;
}

inline void DoubleFormat::SetDefaultScientific ()
{
    b_DefaultScientific_ = true;
    b_DefaultDecimal_    = false;
}

inline void DoubleFormat::SetDefaultDecimal ()
{
    b_DefaultScientific_ = false;
    b_DefaultDecimal_    = true;
}

inline void DoubleFormat::SetDefaultAuto ()
{
    b_DefaultScientific_ = false;
    b_DefaultDecimal_    = false;
}

inline intsys DoubleFormat::Print (double d, char* ps_Buffer, uintsys u_Capacity) const
{
    const char* pz_Format = "%1.*g";

    if (b_Scientific_)
    {
        pz_Format = "%1.*e";
    }
    else if (b_Decimal_)
    {
        pz_Format = "%1.*f";
    }

    return snprintf (ps_Buffer, u_Capacity, pz_Format, n_Precision_, d);
}

} // namespace mikestoolbox

