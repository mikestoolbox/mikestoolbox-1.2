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
//  File:       File.cpp
//
//  Synopsis:   Platform-independent methods of the File class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

bool File::DeleteRecursive ()
{
    if (IsDirectory())
    {
        StringList strl_Files (ReadDirectory (Name()));

        char c_Separator = '/';

        if (!Name().FindFirst('/') && Name().FindFirst('\\'))
        {
            c_Separator = '\\';
        }

        while (!strl_Files.IsEmpty())
        {
            String str_Name ((Name() + c_Separator) + strl_Files.Shift());

            File file (str_Name);

            file.DeleteRecursive();
        }
    }

    return Delete();
}

StringList File::Head (uintsys u_NumLines) const
{
    StringList strl_Lines;
    String     str_Line;

    std::ifstream ifs (str_Name_.C(), std::ios::binary);

    if (ifs.is_open())
    {
        while (ifs.good() && !ifs.eof() && (strl_Lines.NumItems() < u_NumLines) &&
               str_Line.ReadLine (ifs))
        {
            strl_Lines.Append (str_Line);
        }

        ifs.close();
    }

    return strl_Lines;
}

StringList File::Tail (uintsys u_NumLines) const
{
    StringList strl_Lines;
    String     str_Line;

    if (u_NumLines > 0)
    {
        std::ifstream ifs (str_Name_.C(), std::ios::binary);

        if (ifs.is_open())
        {
            while (ifs.good() && !ifs.eof() && str_Line.ReadLine (ifs))
            {
                strl_Lines.Append (str_Line);

                if (strl_Lines.NumItems() > u_NumLines)
                {
                    strl_Lines.Shift();
                }
            }
        }

        ifs.close();
    }

    return strl_Lines;
}

} // namespace mikestoolbox

