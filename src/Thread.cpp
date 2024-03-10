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
//  File:       Thread.cpp
//
//  Synopsis:   Implementation of Thread class
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

#ifdef SINGLE_THREADED

uintsys getThreadId()
{
    return 0;
}

Thread::~Thread ()
{
    // nothing
}

void Thread::Suspend ()
{
    // nothing
}

void Thread::Resume ()
{
    // nothing
}

void Thread::SetPriority (intsys n_Priority)
{
    // nothing
}

uintsys Thread::GetId () const
{
    return 0;
}

void Thread::Run ()
{
    try
    {
        date_Start_ = Date();
        date_Stop_  = date_Start_;

        if (b_ShouldStop_)
        {
            n_ExitCode_ = -1;
        }
        else
        {
            b_Running_  = true;
            n_ExitCode_ = Main_();
        }
    }
    catch (StringException& e)
    {
        b_Exception_   = true;
        n_ExitCode_    = -1;
        str_Exception_ = e.StringMessage();
    }
    catch (Exception& e)
    {
        b_Exception_   = true;
        n_ExitCode_    = -1;
        str_Exception_ = e.Message();
    }
    catch (...)
    {
        b_Exception_  = true;
        n_ExitCode_   = -1;
    }

    date_Stop_ = Date();
    b_Running_ = false;
}

#else

void Thread::Run ()
{
    cond_Startup_.Signal();
}

#endif // SINGLE_THREADED

//+---------------------------------------------------------------------------
//  Platform-independent methods
//----------------------------------------------------------------------------

void Thread::Stop_ ()
{
    // nothing -- can be overridden
}

intsys SimpleThread::Main_ ()
{
    switch (type_)
    {
        case VOID_STAR_FUNCTION_WITH_ARG:
        {
            void* p_Return = func_VoidStarWithArg_ (p_Arg_);

            return (p_Return == 0) ? 0 : -1;
        }

        case VOID_FUNCTION_WITH_ARG:
        {
            func_VoidWithArg_ (p_Arg_);

            return 0;
        }

        case INT_FUNCTION_WITH_ARG:
        {
            return func_IntWithArg_ (p_Arg_);
        }

        case VOID_STAR_FUNCTION:
        {
            void* p_Return = func_VoidStar_ ();

            return (p_Return == 0) ? 0 : -1;
        }

        case VOID_FUNCTION:
        {
            func_Void_ ();

            return 0;
        }

        case INT_FUNCTION:
        {
            return func_Int_ ();
        }
    }

    return -1;
}

} // namespace mikestoolbox

