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
//  File:       Thread.inl
//
//  Synopsis:   Implementation of inline Thread methods
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline bool Thread::IsRunning () const
{
    return b_Running_;
}

inline intsys Thread::ExitCode () const
{
    return n_ExitCode_;
}

inline bool Thread::ShouldStop () const
{
    return b_ShouldStop_;
}

inline bool Thread::ExceptionCaught () const
{
    return b_Exception_;
}

inline String Thread::ExceptionMessage () const
{
    return str_Exception_;
}

inline void Thread::Stop ()
{
    if (b_ShouldStop_)
    {
        return;
    }

    b_ShouldStop_ = true;

    cond_Startup_.Signal();

    Stop_();
}

inline double Thread::ElapsedTime () const
{
    if (IsRunning())
    {
        Date date_Now;

        return date_Now.SecondsMoreThan (date_Start_);
    }
    else
    {
        return date_Stop_.SecondsMoreThan (date_Start_);
    }
}

#ifdef SINGLE_THREADED

inline Thread::Thread ()
#ifdef PLATFORM_WINDOWS
    : dw_ThreadId_   (0)
    , b_Running_     (false)
#else
    : b_Running_     (false)
#endif
    , b_ShouldStop_  (false)
    , n_Priority_    (THREAD_PRIORITY_NORMAL)
    , n_ExitCode_    (internal::gn_SingleThreadedZero)
    , b_Exception_   (false)
    , str_Exception_ ()
    , cond_Startup_  ()
    , date_Start_    (time(0))
    , date_Stop_     (date_Start_)
{
    // nothing
}

#else

#ifdef PLATFORM_UNIX

inline Thread::Thread ()
    : h_Thread_      ()
    , b_Running_     (false)
    , b_ShouldStop_  (false)
    , n_Priority_    (THREAD_PRIORITY_NORMAL)
    , n_ExitCode_    (internal::gn_MultiThreadedZero)
    , b_Exception_   (false)
    , str_Exception_ ()
    , cond_Startup_  ()
    , date_Start_    (time(0))
    , date_Stop_     (date_Start_)
{
    if (pthread_create (&h_Thread_, 0, Thread::ThreadMain_, this) != 0)
    {
        throw Exception ("Thread: Failed to create thread");
    }
}

#endif // PLATFORM_UNIX

#ifdef PLATFORM_WINDOWS

inline Thread::Thread ()
    : h_Thread_      (0)
    , dw_ThreadId_   (0)
    , b_Running_     (false)
    , b_ShouldStop_  (false)
    , n_Priority_    (THREAD_PRIORITY_NORMAL)
    , n_ExitCode_    (internal::gn_MultiThreadedZero)
    , b_Exception_   (false)
    , str_Exception_ ()
    , cond_Startup_  ()
    , date_Start_    (time(0))
    , date_Stop_     (date_Start_)
{
    h_Thread_ = CreateThread (0, 0, Thread::ThreadMain_, (void*)this, 0,
                              &dw_ThreadId_);

    if (h_Thread_ == 0)
    {
        throw Exception ("Thread: Failed to create thread");
    }
}

#endif // PLATFORM_WINDOWS

#endif // SINGLE_THREADED

inline SimpleThread::SimpleThread (ThreadFunctionVoidStarWithArg func,
                                   void* p_Arg)
    : func_VoidStarWithArg_ (func)
    , func_VoidWithArg_     (0)
    , func_IntWithArg_      (0)
    , func_VoidStar_        (0)
    , func_Void_            (0)
    , func_Int_             (0)
    , type_                 (VOID_STAR_FUNCTION_WITH_ARG)
    , p_Arg_                (p_Arg)
{
    // nothing
}

inline SimpleThread::SimpleThread (ThreadFunctionVoidWithArg func, void* p_Arg)
    : func_VoidStarWithArg_ (0)
    , func_VoidWithArg_     (func)
    , func_IntWithArg_      (0)
    , func_VoidStar_        (0)
    , func_Void_            (0)
    , func_Int_             (0)
    , type_                 (VOID_FUNCTION_WITH_ARG)
    , p_Arg_                (p_Arg)
{
    // nothing
}

inline SimpleThread::SimpleThread (ThreadFunctionIntWithArg func, void* p_Arg)
    : func_VoidStarWithArg_ (0)
    , func_VoidWithArg_     (0)
    , func_IntWithArg_      (func)
    , func_VoidStar_        (0)
    , func_Void_            (0)
    , func_Int_             (0)
    , type_                 (INT_FUNCTION_WITH_ARG)
    , p_Arg_                (p_Arg)
{
    // nothing
}

inline SimpleThread::SimpleThread (ThreadFunctionVoidStar func)
    : func_VoidStarWithArg_ (0)
    , func_VoidWithArg_     (0)
    , func_IntWithArg_      (0)
    , func_VoidStar_        (func)
    , func_Void_            (0)
    , func_Int_             (0)
    , type_                 (VOID_STAR_FUNCTION)
    , p_Arg_                (0)
{
    // nothing
}

inline SimpleThread::SimpleThread (ThreadFunctionVoid func)
    : func_VoidStarWithArg_ (0)
    , func_VoidWithArg_     (0)
    , func_IntWithArg_      (0)
    , func_VoidStar_        (0)
    , func_Void_            (func)
    , func_Int_             (0)
    , type_                 (VOID_FUNCTION)
    , p_Arg_                (0)
{
    // nothing
}

inline SimpleThread::SimpleThread (ThreadFunctionInt func)
    : func_VoidStarWithArg_ (0)
    , func_VoidWithArg_     (0)
    , func_IntWithArg_      (0)
    , func_VoidStar_        (0)
    , func_Void_            (0)
    , func_Int_             (func)
    , type_                 (INT_FUNCTION)
    , p_Arg_                (0)
{
    // nothing
}

} // namespace mikestoolbox

