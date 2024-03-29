#  Makefile for Mike's Toolbox tests

#  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.
#
#  This source code is the property of Michael S. D'Errico and is
#  protected under international copyright laws.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
#  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#  Options for Contacting the Author:
#
#    email name:   mikestoolbox
#    email domain: pobox.com
#    X/Twitter:    @mikestoolbox
#    mail:         Michael D'Errico
#                  10161 Park Run Drive, Suite 150
#                  Las Vegas, NV 89145

CXX         = c++
LINK        = c++

OPTIMIZE    = -O2
WARNINGS    = -Wall -ansi # -Weverything
ERRORS      = # -ferror-limit=5 -pedantic-errors
PROFILE     = # -fprofile-instr-generate

CXXFLAGS    = $(strip $(OPTIMIZE) $(WARNINGS) $(ERRORS) $(PROFILE))
LNFLAGS     = $(strip $(OPTIMIZE) $(PROFILE))

MAIN_LIBS   = -lmikestoolbox-1.2 -lpcre
MACOSX_LIBS = $(MAIN_LIBS)
LINUX_LIBS  = $(MAIN_LIBS) -lpthread -ldl
WIN32_LIBS  = $(MAIN_LIBS) -lws2_32 -lgdi32 -lcomdlg32 -lgmon

LIBS        = $(MACOSX_LIBS)

tests       = DateTest          \
              FileTest          \
              HashTest          \
              ListTest          \
              MapTest           \
              SocketTest        \
              StringIterTest    \
              StringListTest    \
              StringTest

other   =     Ping              \
              ThreadTest        \
              Typename

targets = $(tests) $(other)

test_o  = $(patsubst %,%.o,$(tests))
other_o = $(patsubst %,%.o,$(other))

command = $(patsubst %,./%;,$(tests))

objects = $(test_o) $(other_o)

.PHONY: all clean over o test

all: $(objects) $(targets)

clean:
	rm -f *.o $(targets)

over: clean
	make

o: $(objects)

test: $(tests)
	$(command)

$(objects): Makefile
$(test_o):  Test.h

%: %.o ../src/libmikestoolbox-1.2.a
	$(LINK) $(LNFLAGS) -o $@ $< $(LIBS)

