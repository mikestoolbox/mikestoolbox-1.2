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
//  Synopsis:   Windows-specific methods of the File class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

bool File::IsDirectory () const
{
    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad))
    {
        return (wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;
    }

    return false;
}

bool File::Delete ()
{
    if (IsDirectory())
    {
        return RemoveDirectory (str_Name_) ? true : false;
    }

    return DeleteFile (str_Name_) ? true : false;
}

bool File::Exists () const
{
    WIN32_FILE_ATTRIBUTE_DATA wfad;

    return GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad) ? true : false;
}

bool File::IsExecutable () const
{
    //+++ can we do better?

    return Exists();
}

bool File::IsReadable () const
{
    //+++ can we do better?

    return Exists();
}

bool File::IsRegular () const
{
    //+++ can we do better?

    return !IsDirectory();
}

bool File::IsWritable () const
{
    // can we do better?

    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad))
    {
        return (!(wfad.dwFileAttributes & FILE_ATTRIBUTE_READONLY));
    }

    return false;
}

Date File::DateModified () const
{
    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad))
    {
        return Date (&wfad.ftLastWriteTime);
    }

    return Date();
}

String File::BaseName () const
{
    String str_Base (str_Name_.UTF8());
    String str_DriveLetter;

    StringList strl_Matches;

    str_Base.Replace ("^([A-Za-z]:)", "", strl_Matches);

    if (!strl_Matches.IsEmpty())
    {
        str_DriveLetter = strl_Matches[1];
    }

    while (str_Base[-1] == '/')
    {
        str_Base.EraseEnd();
    }

    while (str_Base[-1] == '\\')
    {
        str_Base.EraseEnd();
    }

    StringIter iter (str_Base.FindLast ('/'));

    if (!iter)
    {
        iter = str_Base.FindLast ('\\');
    }

    if (iter)
    {
        str_Base.EraseFront (iter.Offset() + 1);
    }

    if (str_Base.IsEmpty())
    {
        str_Base = str_DriveLetter + '\\';
    }

    return str_Base;
}

String File::Directory () const
{
    String str_Directory (str_Name_.UTF8());
    String str_DriveLetter;

    StringList strl_Matches;

    str_Directory.Replace ("^([A-Za-z]:)", "", strl_Matches);

    if (!strl_Matches.IsEmpty())
    {
        str_DriveLetter = strl_Matches[1];
    }

    if (str_Directory[-1] == '/')
    {
        while (str_Directory[-2] == '/')
        {
            str_Directory.EraseEnd ();
        }

        str_Directory = str_DriveLetter + str_Directory;

        return str_Directory;
    }

    if (str_Directory[-1] == '\\')
    {
        while (str_Directory[-2] == '\\')
        {
            str_Directory.EraseEnd ();
        }

        str_Directory = str_DriveLetter + str_Directory;

        return str_Directory;
    }

    StringIter iter (str_Directory.FindLast ('/'));

    if (!iter)
    {
        iter = str_Directory.FindLast ('\\');
    }

    if (!iter)
    {
        return str_DriveLetter + "./";
    }

    str_Directory.EraseEnd (str_Directory.Length() - iter.Offset() - 1);

    while (str_Directory[-2] == '/')
    {
        str_Directory.EraseEnd ();
    }

    while (str_Directory[-2] == '\\')
    {
        str_Directory.EraseEnd ();
    }

    str_Directory = str_DriveLetter + str_Directory;

    return str_Directory;
}

bool File::CopyTo (File& file, int n_Flags) const
{
    BOOL b_FailIfExists = FALSE;

    if (!(n_Flags & FILE_REPLACE_EXISTING))
    {
        b_FailIfExists = TRUE;
    }

    return (CopyFile (str_Name_, file.str_Name_, b_FailIfExists) != 0);
}

bool File::Rename (const String& str_NewName, int n_Flags)
{
    DWORD dw_Flags = 0;

    if (n_Flags & FILE_REPLACE_EXISTING)
    {
        dw_Flags |= MOVEFILE_REPLACE_EXISTING;
    }

    if (n_Flags & FILE_COPY_ALLOWED)
    {
        dw_Flags |= MOVEFILE_COPY_ALLOWED;
    }

    WindowsString str_NewWindowsName (str_NewName);

    if (MoveFileEx (str_Name_, str_NewWindowsName, dw_Flags))
    {
        str_Name_ = str_NewWindowsName;

        return true;
    }

    return false;
}

uintsys File::Size () const
{
    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad))
    {
        uintsys u_SizeHigh = (uint32)wfad.nFileSizeHigh;

        u_SizeHigh = u_SizeHigh << 32;

        if ((u_SizeHigh >> 32) ^ (uint32)wfad.nFileSizeHigh)
        {
            throw StringException ("File size unrepresentable in uintsys: " + str_Name_);
        }

        return u_SizeHigh | (uint32)wfad.nFileSizeLow;
    }

    return 0;
}

String GetDirectory ()
{
    TCHAR ps_Buffer [4096];
    DWORD dw_Length = 4096;

    DWORD dw_Result = GetCurrentDirectory (dw_Length, ps_Buffer);

    if (dw_Result == 0)
    {
        return String();
    }

    if (dw_Result < dw_Length)
    {
#ifdef UNICODE
        String str_Unicode ((const char*)ps_Buffer, dw_Result*2);
        String str_Directory;

        str_Unicode.WindowsToUTF8 (str_Directory);

        return str_Directory;
#else
        return String (ps_Buffer, dw_Result);
#endif
    }

    String str_Directory;

    TCHAR* ps_NewBuffer = (TCHAR*) str_Directory.Allocate (dw_Result*sizeof(TCHAR));

    dw_Length = GetCurrentDirectory (dw_Result, ps_NewBuffer);

    str_Directory.Truncate (Minimum (dw_Length, dw_Result-1) * sizeof(TCHAR));

#ifdef UNICODE
    String str_UTF8;

    str_Directory.WindowsToUTF8 (str_UTF8);

    return str_UTF8;
#else
    return str_Directory;
#endif
}

bool File::Append (const StringI& str_Contents, int n_Flags)
{
    DWORD dw_CreateOptions;

    if (n_Flags & FILE_CREATE_OK)
    {
        dw_CreateOptions = OPEN_ALWAYS;
    }
    else
    {
        dw_CreateOptions = OPEN_EXISTING;
    }

    HANDLE handle = CreateFile (str_Name_, GENERIC_WRITE, 0, 0,
                                dw_CreateOptions, FILE_ATTRIBUTE_NORMAL, 0);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (str_Contents.Length() == 0)
    {
        CloseHandle (handle);

        return true;
    }

    DWORD dw_OriginalLength = SetFilePointer (handle, 0, 0, FILE_END);

    if (dw_OriginalLength == INVALID_SET_FILE_POINTER)
    {
        CloseHandle (handle);

        return false;
    }

    const char* ps_Contents = str_Contents.C();

    DWORD dw_NeedToWrite = str_Contents.Length();
    DWORD dw_Written     = 0;
    DWORD dw_Total       = 0;

    while (WriteFile (handle, (const void*) ps_Contents, dw_NeedToWrite, &dw_Written, 0))
    {
        ps_Contents    += dw_Written;
        dw_NeedToWrite -= dw_Written;
        dw_Total       += dw_Written;

        if (dw_NeedToWrite == 0)
        {
            break;
        }
    }

    if (dw_NeedToWrite == 0)
    {
        CloseHandle (handle);

        return true;
    }

    if (SetFilePointer (handle, dw_OriginalLength, 0, FILE_BEGIN) != INVALID_SET_FILE_POINTER)
    {
        SetEndOfFile (handle);
    }

    CloseHandle (handle);

    return false;
}

bool File::Read (StringI& str_Contents) const
{
    WIN32_FILE_ATTRIBUTE_DATA wfad;

    if (!GetFileAttributesEx (str_Name_, GetFileExInfoStandard, &wfad))
    {
        return false;
    }

    uintsys u_FileSize = wfad.nFileSizeLow;

    uchar* ps_Buffer = str_Contents.Allocate (u_FileSize);

    HANDLE handle = CreateFile (str_Name_, GENERIC_READ, FILE_SHARE_READ,
                                0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dw_NeedToRead = u_FileSize;
    DWORD dw_BytesRead  = 0;
    DWORD dw_TotalRead  = 0;

    while (ReadFile (handle, (void*) (ps_Buffer + dw_TotalRead), dw_NeedToRead, &dw_BytesRead, 0))
    {
        dw_NeedToRead -= dw_BytesRead;
        dw_TotalRead  += dw_BytesRead;

        if (dw_NeedToRead == 0)
        {
            break;
        }

        if (dw_BytesRead == 0)
        {
            break;
        }
    }

    CloseHandle (handle);

    str_Contents.Truncate (dw_TotalRead);

    return (dw_NeedToRead == 0);
}

bool File::Write (const StringI& str_Contents, int n_Flags)
{
    DWORD dw_CreateOptions;

    if (n_Flags & FILE_CREATE_OK)
    {
        if (n_Flags & FILE_REPLACE_EXISTING)
        {
            dw_CreateOptions = CREATE_ALWAYS;
        }
        else
        {
            dw_CreateOptions = CREATE_NEW;
        }
    }
    else
    {
        if (n_Flags & FILE_REPLACE_EXISTING)
        {
            dw_CreateOptions = TRUNCATE_EXISTING;
        }
        else
        {
            return false;
        }
    }

    HANDLE handle = CreateFile (str_Name_, GENERIC_WRITE, 0, 0,
                                dw_CreateOptions, FILE_ATTRIBUTE_NORMAL, 0);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (str_Contents.Length() == 0)
    {
        CloseHandle (handle);

        return true;
    }

    const char* ps_Contents = str_Contents.C();

    DWORD dw_NeedToWrite = str_Contents.Length();
    DWORD dw_Written     = 0;
    DWORD dw_Total       = 0;

    while (WriteFile (handle, (const void*) ps_Contents, dw_NeedToWrite, &dw_Written, 0))
    {
        ps_Contents    += dw_Written;
        dw_NeedToWrite -= dw_Written;
        dw_Total       += dw_Written;

        if (dw_NeedToWrite == 0)
        {
            break;
        }
    }

    CloseHandle (handle);

    if (dw_NeedToWrite == 0)
    {
        return true;
    }

    Delete();

    return false;
}

StringList ReadDirectory (const StringI& str_Directory)
{
    StringList strl_Files;

    WIN32_FIND_DATA find_data;

    char c_Separator = '/';

    if (str_Directory.FindFirst ('\\'))
    {
        c_Separator = '\\';
    }

    String str_FileSpec (str_Directory);

    str_FileSpec += c_Separator;
    str_FileSpec += '*';

    WindowsString str_WindowsFileSpec (str_FileSpec);

    HANDLE h_Find = FindFirstFile (str_WindowsFileSpec, &find_data);

    if (h_Find != INVALID_HANDLE_VALUE)
    {
        String str_DotDot ("..");

        for (;;)
        {
#ifdef UNICODE
            const TCHAR* p_FileName = find_data.cFileName;
            uintsys      u_Length   = lstrlen (p_FileName);

            String str_UnicodeFile ((const char*)p_FileName, u_Length*2);
            String str_File;

            str_UnicodeFile.WindowsToUTF8 (str_File);
#else
            String str_File (find_data.cFileName);
#endif

            if (str_File != '.' && str_File != str_DotDot)
            {
                strl_Files.Append (str_File);
            }

            if (!FindNextFile (h_Find, &find_data))
            {
                break;
            }
        }

        FindClose (h_Find);
    }

    return strl_Files;
}

bool MakeDirectory (const StringI& str_Dir)
{
    String str_Directory (str_Dir);
    String str_DriveLetter;

    StringList strl_Matches;

    str_Directory.Replace ("^([A-Za-z]:)", "", strl_Matches);

    if (!strl_Matches.IsEmpty())
    {
        str_DriveLetter = strl_Matches[1];
    }

    while (str_Directory[-1] == '/')
    {
        str_Directory.EraseEnd();

        if (str_Directory.IsEmpty())
        {
            return true;
        }
    }

    while (str_Directory[-1] == '\\')
    {
        str_Directory.EraseEnd();

        if (str_Directory.IsEmpty())
        {
            return true;
        }
    }

    if ((str_Directory == '.') || (str_Directory == ".."))
    {
        return true;
    }

    String str_FileName (str_DriveLetter + str_Directory);

    File file (str_FileName);

    if (file.IsDirectory())
    {
        return true;
    }

    if (file.Exists())
    {
        return false;
    }

    String str_SubDir (file.Directory());

    if (!MakeDirectory (str_SubDir))
    {
        return false;
    }

    WindowsString str_Name (str_FileName);

    return CreateDirectory (str_Name, 0) ? true : false;
}

inline bool SetDirectory (const StringI& str_Directory)
{
    WindowsString str (str_Directory);

    return SetCurrentDirectory (str) ? true : false;
}

} // namespace mikestoolbox

