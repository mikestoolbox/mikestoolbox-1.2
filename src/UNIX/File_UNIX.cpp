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
//  File:       UNIX/FileUNIX.cpp
//
//  Synopsis:   UNIX implementation of the File class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_UNIX

namespace mikestoolbox {

#ifdef HAVE_AWFUL_DIR_FUNCTIONS
static Mutex gmutex_ReadDirectory;
#endif

class UnixFileHandle
{
public:

    UnixFileHandle  (int handle);
    ~UnixFileHandle ();

    bool    IsOpen  () const;

    operator int    () const;

private:

    int handle_;

    UnixFileHandle (const UnixFileHandle&);
    UnixFileHandle& operator= (const UnixFileHandle&);
};

inline UnixFileHandle::UnixFileHandle (int handle)
    : handle_   (handle)
{
    // nothing
}

inline UnixFileHandle::~UnixFileHandle ()
{
    if (handle_ >= 0)
    {
        close (handle_);
    }
}

inline bool UnixFileHandle::IsOpen () const
{
    return (handle_ >= 0);
}

inline UnixFileHandle::operator int () const
{
    return handle_;
}

bool SetDirectory (const String& str_Directory)
{
    return (chdir (str_Directory.C()) == 0);
}

bool File::IsDirectory () const
{
    struct stat stat_Buf;

    if (stat (str_Name_.C(), &stat_Buf) == 0)
    {
        return S_ISDIR(stat_Buf.st_mode);
    }

    return false;
}

bool File::Delete ()
{
    return (unlink (str_Name_.C()) == 0);
}

bool File::Exists () const
{
    return (access (str_Name_.C(), F_OK) == 0);
}

bool File::IsExecutable () const
{
    return (access (str_Name_.C(), X_OK) == 0);
}

bool File::IsReadable () const
{
    return (access (str_Name_.C(), R_OK) == 0);
}

bool File::IsRegular () const
{
    struct stat stat_Buf;

    if (stat (str_Name_.C(), &stat_Buf) == 0)
    {
        return S_ISREG(stat_Buf.st_mode);
    }

    return false;
}

bool File::IsWritable () const
{
    return (access (str_Name_.C(), W_OK) == 0);
}

Date File::DateModified () const
{
    struct stat stat_Buf;

    if (stat (str_Name_.C(), &stat_Buf) == 0)
    {
        return Date (stat_Buf.st_mtime);
    }

    return Date (1, 1, 1);
}

String File::BaseName () const
{
    String str_Base (str_Name_);

    while (str_Base[-1] == '/')
    {
        str_Base.EraseEnd();
    }

    StringIter iter (str_Base.FindLast ('/'));

    if (iter)
    {
        str_Base.EraseFront (iter.Offset() + 1);
    }

    if (str_Base.IsEmpty())
    {
        str_Base = '/';
    }

    return str_Base;
}

String File::Directory () const
{
    String str_Directory (str_Name_);

    if (str_Directory[-1] == '/')
    {
        while (str_Directory[-2] == '/')
        {
            str_Directory.EraseEnd ();
        }

        return str_Directory;
    }

    StringIter iter (str_Directory.FindLast ('/'));

    if (!iter)
    {
        return "./";
    }

    str_Directory.EraseEnd (str_Directory.Length() - iter.Offset() - 1);

    while (str_Directory[-2] == '/')
    {
        str_Directory.EraseEnd ();
    }

    return str_Directory;
}

static bool WriteBytesToFile (int h_Dest, const char* p_Bytes, uintsys u_Length)
{
    while (u_Length > 0)
    {
        ssize_t n_Write = write (h_Dest, p_Bytes, u_Length);

        if (n_Write <= 0)
        {
            return false;
        }

        u_Length -= n_Write;
    }

    return true;
}

static inline bool WriteStringToFile (int h_Dest, const String& str)
{
    return WriteBytesToFile (h_Dest, str.C(), str.Length());
}

static bool CopyFileContents (int h_Dest, int h_Source)
{
    const uintsys u_BufferSize = 8192;

    char ac_Buffer[u_BufferSize];

    ssize_t n_Read = 0;

    for (;;)
    {
        n_Read = read (h_Source, ac_Buffer, u_BufferSize);

        if (n_Read <= 0)
        {
            break;
        }

        if (!WriteBytesToFile (h_Dest, ac_Buffer, n_Read))
        {
            break;
        }
    }

    return (n_Read == 0);
}

bool File::CopyTo (File& file, int n_Flags) const
{
    int n_OpenFlags = O_WRONLY | O_CREAT | O_TRUNC;

    if (!(n_Flags & FILE_REPLACE_EXISTING))
    {
        n_OpenFlags |= O_EXCL;
    }

    UnixFileHandle h_OldFile (open (str_Name_.C(), O_RDONLY));

    if (h_OldFile.IsOpen())
    {
        UnixFileHandle h_NewFile (open (file.Name().C(),
                                        n_OpenFlags,
                                        S_IRUSR|S_IWUSR));

        if (h_NewFile.IsOpen())
        {
            if (CopyFileContents (h_NewFile, h_OldFile))
            {
                return true;
            }

            file.Delete();
        }
    }

    return false;
}

bool File::Rename (const String& str_NewName, int n_Flags)
{
    if (!(n_Flags & FILE_REPLACE_EXISTING))
    {
        File f (str_NewName);

        if (f.Exists())
        {
            return false;
        }
    }

    if (rename (str_Name_.C(), str_NewName.C()) == 0)
    {
        str_Name_ = str_NewName;

        return true;
    }

    if (n_Flags & FILE_COPY_ALLOWED)
    {
        File file_New (str_NewName);

        if (CopyTo (file_New, n_Flags))
        {
            Delete();
            str_Name_ = str_NewName;

            return true;
        }
    }

    return false;
}

uint64 File::Size () const
{
    struct stat stat_Buf;

    if (stat (str_Name_.C(), &stat_Buf) == 0)
    {
        return stat_Buf.st_size;
    }

    return 0;
}

String GetDirectory ()
{
    String str_Dir;

    char* ps_Buffer = (char*) str_Dir.Allocate (MAXPATHLEN);

    if (getcwd (ps_Buffer, MAXPATHLEN) == 0)
    {
        str_Dir.Clear();
    }
    else
    {
        str_Dir.Truncate (std::strlen (ps_Buffer));
    }

    return str_Dir;
}

bool File::Append (const String& str_Contents, int n_Flags)
{
    int n_OpenFlags = O_WRONLY;

    if (n_Flags & FILE_CREATE_OK)
    {
        n_OpenFlags |= O_CREAT;
    }

    UnixFileHandle handle (open (str_Name_.C(), n_OpenFlags, S_IRUSR|S_IWUSR));

    if (handle.IsOpen())
    {
        off_t n_OriginalLength = lseek (handle, 0, SEEK_END);

        if (n_OriginalLength >= 0)
        {
            if (WriteStringToFile (handle, str_Contents))
            {
                return true;
            }

            if (ftruncate (handle, n_OriginalLength) < 0)
            {
                return false;
            }
        }
    }

    return false;
}

bool File::Read (String& str_Contents) const
{
    UnixFileHandle handle (open (str_Name_.C(), O_RDONLY, S_IRUSR));

    if (handle.IsOpen())
    {
        struct stat stat_Buf;

        if (fstat (handle, &stat_Buf) != 0)
        {
            return false;
        }

        uintsys u_FileSize = stat_Buf.st_size;
        uchar*  ps_Buffer  = str_Contents.Allocate (u_FileSize);
        uintsys u_Needed   = u_FileSize;

        while (u_Needed > 0)
        {
            ssize_t n_Retval = read (handle, ps_Buffer, u_Needed);

            if (n_Retval < 0)
            {
                str_Contents.Clear();

                return false;
            }

            if (n_Retval == 0)
            {
                u_FileSize -= u_Needed;

                break;
            }

            u_Needed -= n_Retval;
        }

        str_Contents.Truncate (u_FileSize);
    }

    return true;
}

bool File::Write (const String& str_Contents, int n_Flags)
{
    int n_OpenFlags = O_WRONLY | O_TRUNC;

    if (n_Flags & FILE_CREATE_OK)
    {
        n_OpenFlags |= O_CREAT;
    }

    if (!(n_Flags & FILE_REPLACE_EXISTING))
    {
        n_OpenFlags |= O_EXCL;
    }

    UnixFileHandle handle (open (str_Name_.C(), n_OpenFlags, S_IRUSR|S_IWUSR));

    if (handle.IsOpen())
    {
        if (WriteStringToFile (handle, str_Contents))
        {
            return true;
        }

        Delete();
    }

    return false;
}

StringList ReadDirectory (const String& str_Directory)
{
    StringList strl_Files;

#ifdef HAVE_AWFUL_DIR_FUNCTIONS
    MutexLocker lock (gmutex_ReadDirectory);
#else
    struct dirent dir_entry;
#endif

    struct dirent* p_Dirent = 0;

    String str_DotDot ("..");

    DIR* dir = opendir (str_Directory.C());

    if (dir)
    {
        for (;;)
        {
            #ifdef HAVE_AWFUL_DIR_FUNCTIONS

                p_Dirent = readdir (dir);

            #else

                if (readdir_r (dir, &dir_entry, &p_Dirent) != 0)
                {
                    break;
                }

            #endif

            if (p_Dirent == 0)
            {
                break;
            }

            String str_File (p_Dirent->d_name);

            if (str_File == '.' || str_File == str_DotDot)
            {
                continue;
            }

            strl_Files.Append (str_File);
        }

        closedir (dir);
    }

    return strl_Files;
}

bool MakeDirectory (const String& str_Dir)
{
    String str_Directory (str_Dir);

    while (str_Directory[-1] == '/')
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

    File file (str_Directory);

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

    return (mkdir (str_Directory.C(), S_IRWXU|S_IRWXG) == 0);
}

} // namespace mikestoolbox

#endif // PLATFORM_UNIX

