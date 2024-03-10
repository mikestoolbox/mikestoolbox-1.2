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
//  File:       Statics.cpp
//
//  Synopsis:   Static objects
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

namespace internal {

// cause linking to fail unless all compiled with SINGLE_THREADED or not

#ifdef SINGLE_THREADED
const intsys gn_SingleThreadedZero = 0;
#else
const intsys gn_MultiThreadedZero  = 0;
#endif

} // namespace internal

int  DoubleFormat::n_DefaultPrecision_   = 8;
bool DoubleFormat::b_DefaultScientific_  = false;
bool DoubleFormat::b_DefaultDecimal_     = false;

const char* const gpz_Hex    = "0123456789abcdef";
const char* const gpz_HEX    = "0123456789ABCDEF";
const char* const gpz_Base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const uintsys MAX_UNICODE_CODE_POINT = 0x10FFFF;

template<>
SharedMemory<HeapMemory>::Data SharedMemory<HeapMemory>::data_Empty_ (0);

const StringList gstrl_EmptyStringListToInitializeEmptyListBaseClass;

double AutoPrintTimer::d_DefaultThreshold_ = 0.0;

#ifdef HAVE_AWFUL_DIR_FUNCTIONS
Mutex gmutex_ReadDirectory;
#endif

#ifdef HAVE_AWFUL_DATE_FUNCTIONS
Mutex Date::mutex_;
#endif

const uintsys   Date::u_SecondsPerDay_ = 24 * 60 * 60;
const Date      Date::date_UNIX_Epoch_ (1970, 1, 1);

const uintsys   DateParts::u_SecondsPerDay_ = 24 * 60 * 60;
const PerlRegex DateParts::regex_Http1_   ("(\\d+)[ -](\\w+)[ -](\\d+) (\\d+):(\\d+)(:(\\d+))?");
const PerlRegex DateParts::regex_Http2_   ("(\\w+) (\\d+) (\\d+):(\\d+)(:(\\d+))? (\\d+)");

const String  DateParts::str_Mon_ ("Mon");
const String  DateParts::str_Tue_ ("Tue");
const String  DateParts::str_Wed_ ("Wed");
const String  DateParts::str_Thu_ ("Thu");
const String  DateParts::str_Fri_ ("Fri");
const String  DateParts::str_Sat_ ("Sat");
const String  DateParts::str_Sun_ ("Sun");

const String  DateParts::str_Jan_ ("Jan");
const String  DateParts::str_Feb_ ("Feb");
const String  DateParts::str_Mar_ ("Mar");
const String  DateParts::str_Apr_ ("Apr");
const String  DateParts::str_May_ ("May");
const String  DateParts::str_Jun_ ("Jun");
const String  DateParts::str_Jul_ ("Jul");
const String  DateParts::str_Aug_ ("Aug");
const String  DateParts::str_Sep_ ("Sep");
const String  DateParts::str_Oct_ ("Oct");
const String  DateParts::str_Nov_ ("Nov");
const String  DateParts::str_Dec_ ("Dec");

const String  DateParts::str_GMT_ ("GMT");

const PerlRegex Network::regex_ConnectString_    ("^((([a-zA-Z0-9]+):)?//)?"   // protocol
                                                  "\\[?([a-zA-Z0-9.-]+)\\]?"   // host
                                                  "(:(\\d+))?/?$");            // port
const PerlRegex Network::regex_IPv4_             ("^\\[?(\\d+\\.\\d+\\.\\d+\\.\\d+)\\]?$");

} // namespace mikestoolbox

