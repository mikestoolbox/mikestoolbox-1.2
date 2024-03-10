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
//  File:       File.inl
//
//  Synopsis:   Implementation of File class
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline File::File (const String& str_Name)
    : str_Name_ (str_Name)
{
    // nothing
}

inline File& File::operator= (const String& str_Filename)
{
    str_Name_ = str_Filename;

    return *this;
}

inline String File::Name () const
{
    return str_Name_;
}

inline bool File::Append (const StringList& strl_Lines, int n_Flags)
{
    return Append (strl_Lines.Join(), n_Flags);
}

inline bool File::Read (StringList& strl_Lines) const
{
    strl_Lines.Clear();

    String str_Contents;

    if (!Read (str_Contents))
    {
        return false;
    }

    strl_Lines = str_Contents.LineBreak();

    return true;
}

inline bool File::Write (const StringList& strl_Lines, int n_Flags)
{
    return Write (strl_Lines.Join(), n_Flags);
}

inline File operator+ (const File& dir, const File& file)
{
    String str_File (file.Name());

    uchar uc0 = str_File[0];

    if ((uc0 == '/') || (uc0 == '\\'))
    {
        return file;
    }

#ifdef PLATFORM_WINDOWS
    uchar uc1 = str_File[1];
    uchar uc2 = str_File[2];


    if (std::isalpha (uc0) && (uc1 == ':') && ((uc2 == '/') || (uc2 == '\\')))
    {
        return file;
    }
#endif

    String str_Dir (dir.Name());

    uchar uc = str_Dir[-1];

    if ((uc != '/') && (uc != '\\'))
    {
        str_Dir += '/';
    }

    return str_Dir + str_File;
}

inline StringList ReadDirectory (const char* pz_Directory)
{
    String str_Directory (pz_Directory);

    return ReadDirectory (str_Directory);
}

} // namespace mikestoolbox

