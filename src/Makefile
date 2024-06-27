#  Makefile for Mike's Toolbox

#  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.
#
#  This source code is the property of Michael S. D'Errico and is
#  protected under international copyright laws.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of version 3 of the GNU General Public License as
#  published by the Free Software Foundation.
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

uname = $(shell uname)

ifeq ($(uname), Darwin)
  platform = osx
else
ifeq ($(uname), Linux)
  platform = unix
else
  platform = win32
endif
endif

library     = libmikestoolbox-1.2.a

CXX         = c++
LINK        = c++

OPTIMIZE    = -g -O2
STANDARD    = # -std=c++11
WARNINGS    = -Wall # -ansi -Weverything
ERRORS      = # -ferror-limit=5 -pedantic-errors
PROFILE     = # -fprofile-instr-generate

CXXFLAGS    = $(strip $(STANDARD) $(OPTIMIZE) $(WARNINGS) $(ERRORS) $(PROFILE))
LNFLAGS     = $(strip $(STANDARD) $(OPTIMIZE) $(PROFILE))

LIBTOOL     = libtool -static -o $(library) -
ARCHIVE     = ar rc  $(library)
RANLIB      = ranlib $(library)

headers     = $(wildcard ../include/mikestoolbox-1.2/*.*) ../include/mikestoolbox-1.2.h

library_cpp = $(wildcard *.cpp)
unix_cpp    = $(wildcard UNIX/*.cpp)
win32_cpp   = $(wildcard WIN32/*.cpp)

library_o   = $(patsubst %.cpp,%.o,$(library_cpp))
unix_o      = $(patsubst %.cpp,%.o,$(unix_cpp))
win32_o     = $(patsubst %.cpp,%.o,$(win32_cpp))

pch         = mikestoolbox-1.2.h.pch

.PHONY: all clean over o osx unix win32

all: $(platform)

clean:
	rm -f *.o UNIX/*.o WIN32/*.o $(pch) $(library)

over: clean
	make all

o: $(library_o) $($(patsubst %,%_o,$(platform)))

$(pch): $(headers) Makefile
	$(CXX) -x c++-header ../include/mikestoolbox-1.2.h -o $@

osx: $(library_o) $(unix_o)
	rm -f $(library)
	$(LIBTOOL) $(library_o) $(unix_o)

unix: $(library_o) $(unix_o)
	rm -f $(library)
	$(ARCHIVE) $(library_o) $(unix_o)
	$(RANLIB)

win32: $(library_o) $(win32_o)
	rm -f $(library)
	$(ARCHIVE) $(library_o) $(win32_o)
	$(RANLIB)

$(library_o) $(unix_o) $(win32_o): $(headers) Makefile

