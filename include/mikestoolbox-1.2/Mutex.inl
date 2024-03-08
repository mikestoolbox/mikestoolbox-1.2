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
//  File:       Mutex.inl
//
//  Synopsis:   Implementation of Mutex class for threaded applications
//----------------------------------------------------------------------------

namespace mikestoolbox {

//+---------------------------------------------------------------------------
//  Synopsis:   single-threaded implementation
//----------------------------------------------------------------------------

#ifdef SINGLE_THREADED

inline Mutex::Mutex ()
    : b_Recursive_ (false)
    , u_LockCount_ (internal::gn_SingleThreadedZero)
{
    // nothing
}

inline Mutex::Mutex (int)
    : b_Recursive_ (true)
    , u_LockCount_ (internal::gn_SingleThreadedZero)
{
    // nothing
}

inline Mutex::~Mutex ()
{
    // nothing
}

inline void Mutex::Lock_ () const
{
    if (b_Recursive_ || (u_LockCount_ == 0))
    {
        ++u_LockCount_;
    }
    else
    {
        throw Exception ("Mutex::Lock: Mutex is already locked");
    }
}

inline bool Mutex::TryLock_ () const
{
    if (b_Recursive_ || (u_LockCount_ == 0))
    {
        ++u_LockCount_;

        return true;
    }

    return false;
}

inline void Mutex::Unlock_ () const
{
    if (u_LockCount_)
    {
        --u_LockCount_;
    }
    else
    {
        throw Exception ("Mutex::Unlock: Mutex is not locked");
    }
}

#else

//+---------------------------------------------------------------------------
//  Synopsis:   UNIX implementation
//----------------------------------------------------------------------------

#ifdef PLATFORM_UNIX

inline Mutex::Mutex ()
    : mutex_       ()
    , attrs_       ()
    , b_Recursive_ (false)
{
    if (pthread_mutex_init (&mutex_, 0) != internal::gn_MultiThreadedZero)
    {
        throw Exception ("Mutex initialization failed");
    }
}

inline Mutex::Mutex (int)
    : mutex_       ()
    , attrs_       ()
    , b_Recursive_ (true)
{
    if (pthread_mutexattr_init (&attrs_) != internal::gn_MultiThreadedZero)
    {
        throw Exception ("RecursiveMutex initialization failed");
    }

    if (pthread_mutexattr_settype (&attrs_, PTHREAD_MUTEX_RECURSIVE) != 0)
    {
        pthread_mutexattr_destroy (&attrs_);

        throw Exception ("RecursiveMutex initialization failed");
    }

    if (pthread_mutex_init (&mutex_, &attrs_) != 0)
    {
        pthread_mutexattr_destroy (&attrs_);

        throw Exception ("RecursiveMutex initialization failed");
    }
}

inline Mutex::~Mutex ()
{
    pthread_mutex_destroy (&mutex_);

    if (b_Recursive_)
    {
        pthread_mutexattr_destroy (&attrs_);
    }
}

inline void Mutex::Lock_ () const
{
    if (pthread_mutex_lock (&mutex_) != 0)
    {
        throw Exception ("Mutex::Lock failed to acquire a lock");
    }
}

inline bool Mutex::TryLock_ () const
{
    return (pthread_mutex_trylock (&mutex_) == 0);
}

inline void Mutex::Unlock_ () const
{
    if (pthread_mutex_unlock (&mutex_) != 0)
    {
        throw Exception ("Mutex::Unlock failed to release a lock");
    }
}

#endif // PLATFORM_UNIX

//+---------------------------------------------------------------------------
//  Synopsis:   Windows implementation
//----------------------------------------------------------------------------

#ifdef PLATFORM_WINDOWS

inline Mutex::Mutex ()
    : mutex_ (0)
{
    mutex_ = CreateMutex (0, (BOOL)internal::gn_MultiThreadedZero, 0);

    if (mutex_ == 0)
    {
        throw Exception ("Mutex initialization failed");
    }
}

inline Mutex::Mutex (int)
    : mutex_ (0)
{
    mutex_ = CreateMutex (0, (BOOL)internal::gn_MultiThreadedZero, 0);

    if (mutex_ == 0)
    {
        throw Exception ("RecursiveMutex initialization failed");
    }
}

inline Mutex::~Mutex ()
{
    CloseHandle (mutex_);
}

inline void Mutex::Lock_ () const
{
    if (WaitForSingleObject (mutex_, INFINITE) != WAIT_OBJECT_0)
    {
        throw Exception ("Mutex::Lock failed to acquire a lock");
    }
}

inline bool Mutex::TryLock_ () const
{
    return (WaitForSingleObject (mutex_, 0) == WAIT_OBJECT_0) ? true : false;
}

inline void Mutex::Unlock_ () const
{
    if (!ReleaseMutex (mutex_))
    {
        throw Exception ("Mutex::Unlock failed to release a lock");
    }
}

#endif // PLATFORM_WINDOWS
#endif // SINGLE_THREADED

//+---------------------------------------------------------------------------
//  Synopsis:   platform-independent methods
//----------------------------------------------------------------------------

inline void Mutex::Lock () const
{
    Lock_();
}

inline bool Mutex::TryLock () const
{
    return TryLock_();
}

inline void Mutex::Unlock () const
{
    Unlock_();
}

inline RecursiveMutex::RecursiveMutex ()
    : Mutex (0)
{
    // nothing
}

inline MutexLocker::MutexLocker (const Mutex& mutex)
    : mutex_ (mutex)
{
    mutex_.Lock();
}

inline MutexLocker::~MutexLocker ()
{
    mutex_.Unlock();
}

inline MutexTryLocker::MutexTryLocker (const Mutex& mutex)
    : mutex_      (mutex)
    , b_IsLocked_ (mutex_.TryLock())
{
    // nothing
}

inline MutexTryLocker::~MutexTryLocker ()
{
    if (b_IsLocked_)
    {
        mutex_.Unlock();
    }
}

inline bool MutexTryLocker::IsLocked () const
{
    return b_IsLocked_;
}

} // namespace mikestoolbox

