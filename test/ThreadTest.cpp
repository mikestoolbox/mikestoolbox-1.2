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

#include "mikestoolbox-1.2.h"

using namespace mikestoolbox;

Mutex gmutex;

class ThreadA : public Thread
{
public:

    ThreadA (uintsys u_Count) : u_Count_ (u_Count) { }

private:

    uintsys u_Count_;

    intsys Main_ ();
};

intsys ThreadA::Main_ ()
{
    MutexLocker locker (gmutex);

    std::cout << "A: " << u_Count_ << std::endl;

    millisleep (3);

    return 0;
}

class ThreadB : public Thread
{
public:

    ThreadB (uintsys u_Count) : u_Count_ (u_Count) { }

private:

    uintsys u_Count_;

    intsys Main_ ();
};

intsys ThreadB::Main_ ()
{
    MutexLocker locker (gmutex);

    std::cout << "B: " << u_Count_ << std::endl;

    millisleep (3);

    return 0;
}

int main (int, char**)
{
    try
    {
        MutexLocker locker (gmutex);

        (new ThreadA (1))->Run();
        (new ThreadB (2))->Run();
        (new ThreadB (3))->Run();
        (new ThreadA (4))->Run();
        (new ThreadA (5))->Run();
        (new ThreadB (6))->Run();
    }
    catch (Exception& e)
    {
        std::cout << "Exception caught: " << e.Message() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception caught" << std::endl;
    }

    millisleep (1000);

    return 0;
}

