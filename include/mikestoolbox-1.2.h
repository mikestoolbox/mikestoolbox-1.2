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

#ifndef MIKESTOOLBOX_1_2_H
#define MIKESTOOLBOX_1_2_H

#include "mikestoolbox-1.2/Base.def"

#include "mikestoolbox-1.2/Error.const"

#include "mikestoolbox-1.2/Backup.class"
#include "mikestoolbox-1.2/Exception.class"
#include "mikestoolbox-1.2/Mutex.class"
#include "mikestoolbox-1.2/Condition.class"
#include "mikestoolbox-1.2/RefCount.class"
#include "mikestoolbox-1.2/Unsigned.class"
#include "mikestoolbox-1.2/Repeat.class"
#include "mikestoolbox-1.2/Preallocate.class"
#include "mikestoolbox-1.2/Index.class"
#include "mikestoolbox-1.2/PointerHolder.class"
#include "mikestoolbox-1.2/DoubleFormat.class"
#include "mikestoolbox-1.2/SmartNumber.class"
#include "mikestoolbox-1.2/Shared.class"
#include "mikestoolbox-1.2/ContainerType.class"
#include "mikestoolbox-1.2/ImportOptions.class"
#include "mikestoolbox-1.2/List.class"
#include "mikestoolbox-1.2/Map.class"
#include "mikestoolbox-1.2/MapIter.class"
#include "mikestoolbox-1.2/Hash.class"
#include "mikestoolbox-1.2/CharRef.class"
#include "mikestoolbox-1.2/PerlRegex.class"
#include "mikestoolbox-1.2/Memory.class"
#include "mikestoolbox-1.2/ParseError.class"
#include "mikestoolbox-1.2/String.class"
#include "mikestoolbox-1.2/StringIter.class"
#include "mikestoolbox-1.2/WindowsString.class"
#include "mikestoolbox-1.2/Timer.class"
#include "mikestoolbox-1.2/StringException.class"
#include "mikestoolbox-1.2/StringList.class"
#include "mikestoolbox-1.2/Date.class"
#include "mikestoolbox-1.2/DateParts.class"
#include "mikestoolbox-1.2/LocalDate.class"
#include "mikestoolbox-1.2/File.class"
#include "mikestoolbox-1.2/Thread.class"
#include "mikestoolbox-1.2/SimpleThread.class"
#include "mikestoolbox-1.2/SocketAddress.class"
#include "mikestoolbox-1.2/BerkeleySocket.class"
#include "mikestoolbox-1.2/Socket.class"
#include "mikestoolbox-1.2/Network.class"
#include "mikestoolbox-1.2/Typename.class"

#include "mikestoolbox-1.2/Backup.inl"
#include "mikestoolbox-1.2/ParseError.inl"
#include "mikestoolbox-1.2/Exception.inl"
#include "mikestoolbox-1.2/Timer.inl"
#include "mikestoolbox-1.2/Mutex.inl"
#include "mikestoolbox-1.2/Condition.inl"
#include "mikestoolbox-1.2/RefCount.inl"
#include "mikestoolbox-1.2/PointerHolder.inl"
#include "mikestoolbox-1.2/Unsigned.inl"
#include "mikestoolbox-1.2/Repeat.inl"
#include "mikestoolbox-1.2/Preallocate.inl"
#include "mikestoolbox-1.2/Index.inl"
#include "mikestoolbox-1.2/DoubleFormat.inl"
#include "mikestoolbox-1.2/Container.inl"
#include "mikestoolbox-1.2/SmartNumber.inl"
#include "mikestoolbox-1.2/Shared.inl"
#include "mikestoolbox-1.2/Hash.inl"
#include "mikestoolbox-1.2/ListAlloc.inl"
#include "mikestoolbox-1.2/ListItem.inl"
#include "mikestoolbox-1.2/ListRef.inl"
#include "mikestoolbox-1.2/ListData.inl"
#include "mikestoolbox-1.2/ListIter.inl"
#include "mikestoolbox-1.2/List.inl"
#include "mikestoolbox-1.2/ListSort.inl"
#include "mikestoolbox-1.2/Map.inl"
#include "mikestoolbox-1.2/Memory.inl"
#include "mikestoolbox-1.2/PerlRegex.inl"
#include "mikestoolbox-1.2/CharRef.inl"
#include "mikestoolbox-1.2/String.inl"
#include "mikestoolbox-1.2/WindowsString.inl"
#include "mikestoolbox-1.2/StringException.inl"
#include "mikestoolbox-1.2/StringList.inl"
#include "mikestoolbox-1.2/File.inl"
#include "mikestoolbox-1.2/Date.inl"
#include "mikestoolbox-1.2/Thread.inl"
#include "mikestoolbox-1.2/SocketAddress.inl"
#include "mikestoolbox-1.2/BerkeleySocket.inl"
#include "mikestoolbox-1.2/Socket.inl"
#include "mikestoolbox-1.2/Network.inl"
#include "mikestoolbox-1.2/Typename.inl"

#endif // MIKESTOOLBOX_1_2_H

