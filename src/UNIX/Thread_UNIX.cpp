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
//  File:       UNIX/ThreadUNIX.cpp
//
//  Synopsis:   UNIX implementation of Thread class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

#ifdef PLATFORM_UNIX

#ifndef SINGLE_THREADED

namespace mikestoolbox {

uintsys getThreadId()
{
    return (uintsys)pthread_self();
}

void* Thread::ThreadMain_ (void* p_Thread)
{
    Thread* p_This = (Thread*) p_Thread;

    try
    {
        pthread_detach (pthread_self());

        p_This->cond_Startup_.Wait();

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

    return 0;
}

Thread::~Thread ()
{
    // nothing
}

void Thread::Suspend ()
{
#if 0
    pthread_suspend (h_Thread_);
#endif
}

void Thread::Resume ()
{
#if 0
    pthread_continue (h_Thread_);
#endif
}

void Thread::SetPriority (intsys n_Priority)
{
    if (b_Running_)
    {
        //+++ change active priority
    }

    n_Priority_ = n_Priority;
}

uintsys Thread::GetId () const
{
    return (uintsys) h_Thread_;
}

} // namespace mikestoolbox

#endif // !SINGLE_THREADED

#endif // PLATFORM_UNIX

