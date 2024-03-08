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
//  File:       WIN32/Thread.cpp
//
//  Synopsis:   Windows implementation of Thread class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifndef SINGLE_THREADED

namespace mikestoolbox {

uintsys getThreadId()
{
    return (uintsys) GetCurrentThreadId();
}

DWORD WINAPI Thread::ThreadMain_ (void* p_Thread)
{
    Thread* p_This = (Thread*) p_Thread;

    try
    {
        p_This->cond_Startup_.Wait();

        SetThreadPriority (GetCurrentThread(), p_This->n_Priority_);

        p_This->date_Start_ = Date();
        p_This->date_Stop_  = p_This->date_Start_;

        if (p_This->b_ShouldStop_)
        {
            p_This->n_ExitCode_ = -1;
        }
        else
        {
            p_This->b_Running_  = true;
            p_This->n_ExitCode_ = p_This->Main_();
        }
    }
    catch (StringException& e)
    {
        p_This->b_Exception_   = true;
        p_This->n_ExitCode_    = -1;
        p_This->str_Exception_ = e.StringMessage();
    }
    catch (Exception& e)
    {
        p_This->b_Exception_   = true;
        p_This->n_ExitCode_    = -1;
        p_This->str_Exception_ = e.Message();
    }
    catch (...)
    {
        p_This->b_Exception_  = true;
        p_This->n_ExitCode_   = -1;
    }

    p_This->date_Stop_ = Date();
    p_This->b_Running_ = false;

    //OpenSSL: ERR_remove_state (0);

    ExitThread (0);
}

Thread::~Thread ()
{
    CloseHandle (h_Thread_);
}

void Thread::Suspend ()
{
    SuspendThread (h_Thread_);
}

void Thread::Resume ()
{
    ResumeThread (h_Thread_);
}

void Thread::SetPriority (intsys n_Priority)
{
    switch (n_Priority)
    {
        case THREAD_PRIORITY_HIGHEST:
        case THREAD_PRIORITY_ABOVE_NORMAL:
        case THREAD_PRIORITY_NORMAL:
        case THREAD_PRIORITY_BELOW_NORMAL:
        case THREAD_PRIORITY_LOWEST:
        {
            n_Priority_ = n_Priority;

            if (b_Running_)
            {
                SetThreadPriority (h_Thread_, n_Priority_);
            }

            break;
        }
    }
}

uintsys Thread::GetId () const
{
    return (uintsys)dw_ThreadId_;
}

} // namespace mikestoolbox

#endif // !SINGLE_THREADED

