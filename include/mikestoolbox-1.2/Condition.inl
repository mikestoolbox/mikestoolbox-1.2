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
//  File:       Condition.inl
//
//  Synopsis:   Implementation of Condition class
//----------------------------------------------------------------------------

namespace mikestoolbox {

//+---------------------------------------------------------------------------
//  Synopsis:   single-threaded implementation
//----------------------------------------------------------------------------

#ifdef SINGLE_THREADED

inline Condition::Condition ()
    : b_Signaled_ ((bool)internal::gn_SingleThreadedZero)
{
    // nothing
}

inline Condition::~Condition ()
{
    // nothing
}

inline void Condition::Signal ()
{
    b_Signaled_ = true;
}

inline void Condition::Wait ()
{
    if (b_Signaled_)
    {
        b_Signaled_ = false;
    }
    else
    {
        throw Exception ("Condition::Wait: Condition not signaled");
    }
}

#else

//+---------------------------------------------------------------------------
//  Synopsis:   UNIX implementation
//----------------------------------------------------------------------------

#ifdef PLATFORM_UNIX

inline Condition::Condition ()
    : mutex_      ()
    , condition_  ()
    , b_Signaled_ (false)
{
    if (pthread_cond_init (&condition_, 0) != internal::gn_MultiThreadedZero)
    {
        throw Exception ("Condition: failed to initialize condition");
    }
}

inline Condition::~Condition ()
{
    pthread_cond_destroy (&condition_);
}

inline bool Condition::Test_ ()
{
    return b_Signaled_;
}

inline void Condition::Set_ ()
{
    b_Signaled_ = true;
}

inline void Condition::Reset_ ()
{
    b_Signaled_ = false;
}

inline void Condition::Wait_ ()
{
    if (pthread_cond_wait (&condition_, &mutex_.mutex_) != 0)
    {
        throw Exception ("Condition::Wait failed to wait for condition");
    }
}

inline void Condition::Signal_ ()
{
    if (pthread_cond_signal (&condition_) != 0)
    {
        throw Exception ("Condition::Signal failed to signal condition");
    }
}

inline void Condition::Wait ()
{
    MutexLocker locker (mutex_);

    for (;;)
    {
        if (Test_())
        {
            Reset_();

            break;
        }

        Wait_();
    }
}

inline void Condition::Signal ()
{
    MutexLocker locker (mutex_);

    Set_();

    Signal_();
}

#endif // PLATFORM_UNIX

//+---------------------------------------------------------------------------
//  Synopsis:   Windows implementation
//----------------------------------------------------------------------------

#ifdef PLATFORM_WINDOWS

inline Condition::Condition ()
    : condition_ (0)
{
    condition_ = CreateSemaphore (0, internal::gn_MultiThreadedZero, 1, 0);

    if (condition_ == 0)
    {
        throw Exception ("Condition: failed to initialize condition");
    }
}

inline Condition::~Condition ()
{
    CloseHandle (condition_);
}

inline void Condition::Wait ()
{
    if (WaitForSingleObject (condition_, INFINITE) != WAIT_OBJECT_0)
    {
        throw Exception ("Condition::Wait failed to wait for condition");
    }
}

inline void Condition::Signal ()
{
    ReleaseSemaphore (condition_, 1, 0);
}

#endif // PLATFORM_WINDOWS
#endif // SINGLE_THREADED

} // namespace mikestoolbox

