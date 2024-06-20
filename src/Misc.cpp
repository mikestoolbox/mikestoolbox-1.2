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

namespace mikestoolbox {

String Latin1ToUTF8 (const String& str)
{
    String str_Result;

    str_Result.Reserve (str.Length());

    StringIter iter (str);

    while (iter)
    {
        str_Result.AppendUTF8 (*iter++);
    }

    return str_Result;
}

String UTF16ToUTF8 (const String& str)
{
    StringIter iter (str);

    String str_Result (Preallocate (str.Length()));

    ParseError error;

    uintsys u = 0;

    while (iter.ParseUTF16Char (u, error))
    {
        str_Result.AppendUTF8 (u);
    }

    if (error.IsOK() && !iter)
    {
        return str_Result;
    }

    throw Exception ("UTF16ToUTF8: Bad UTF-16 encoding");
}

String UTF32ToUTF8 (const String& str)
{
    StringIter iter (str);

    String str_Result (Preallocate (str.Length()));

    ParseError error;

    uintsys u = 0;

    while (iter.ParseUTF32Char (u, error))
    {
        str_Result.AppendUTF8 (u);
    }

    if (error.IsOK() && !iter)
    {
        return str_Result;
    }

    throw Exception ("UTF32ToUTF8: Bad UTF-32 encoding");
}

} // namespace mikestoolbox

