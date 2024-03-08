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
//  File:       Memory.cpp
//
//  Synopsis:   Implementation of Memory classes
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

inline uintsys OptimumStringSize (uintsys u_MinCapacity)
{
    if (u_MinCapacity > (1UL<<30))
    {
        uintsys u_NewCapacity = ((u_MinCapacity & 0x0F) ? ((u_MinCapacity & (~0x0F)) + 0x10)
                                                        : u_MinCapacity);

        if (u_NewCapacity < u_MinCapacity)
        {
            throw Exception ("OptimumStringSize: Out of memory");
        }

        return u_NewCapacity;
    }

    uintsys u_Optimum = 32;

    while (u_Optimum < u_MinCapacity)
    {
        u_Optimum = u_Optimum << 1;
    }

    return u_Optimum;
}

uchar* HeapMemory::Allocate_ (uintsys u_Length, uintsys& u_Allocated)
{
    uintsys u_LengthWithNulls = u_Length + 2;  // add room for two NULL bytes

    if (u_LengthWithNulls < u_Length)
    {
        throw Exception ("HeapMemory::Allocate_: Integer overflow");
    }

    u_Allocated = OptimumStringSize (u_LengthWithNulls);

    uchar* p_Memory = reinterpret_cast<uchar*> (new(std::nothrow)
                          uintsys[u_Allocated/sizeof(uintsys)]);

    if (p_Memory == 0)
    {
        throw Exception ("HeapMemory::Allocate_: Out of memory");
    }

    u_Allocated -= 2;  // don't allow access to NULL bytes

    // zero the trailing NULL bytes

    uchar* p = p_Memory + u_Allocated;

    p[0] = 0;
    p[1] = 0;

    return p_Memory;
}

uchar* HeapMemory::Expand (uintsys u_NumCharsBefore, uintsys u_NumCharsAfter)
{
    uintsys u_ExtraChars = u_NumCharsBefore + u_NumCharsAfter;
    uintsys u_TotalChars = u_Length_ + u_ExtraChars;

    if ((u_ExtraChars < u_NumCharsBefore) || (u_TotalChars < u_Length_))
    {
        throw Exception ("HeapMemory::Expand: Out of memory");
    }

    if ((u_TotalChars > u_Capacity_) || (u_Capacity_ == 0))
    {
        uintsys u_NewCapacity = 0;

        uchar* p_NewMemory = Allocate_ (u_TotalChars, u_NewCapacity);

        if (p_Memory_ != 0)
        {
            std::memcpy (p_NewMemory + u_NumCharsBefore, PointerToFirstByte(), u_Length_);

            ZeroMemory (p_Memory_, u_Capacity_);    // destroy old memory

            delete [] p_Memory_;
        }

        p_Memory_   = p_NewMemory;
        u_Offset_   = 0;
        u_Length_   = u_TotalChars;
        u_Capacity_ = u_NewCapacity;

        NullTerminate_();

        return p_Memory_;
    }

    uintsys u_BeyondEnd = u_Offset_ + u_Length_;
    uintsys u_NewExtent = u_BeyondEnd + u_NumCharsAfter;

    if ((u_NumCharsBefore <= u_Offset_) && (u_NewExtent >= u_BeyondEnd) &&
        (u_NewExtent <= u_Capacity_))
    {
        u_Offset_ -= u_NumCharsBefore;
        u_Length_  = u_TotalChars;

        NullTerminate_();

        return p_Memory_ + u_Offset_;
    }

    std::memmove (p_Memory_ + u_NumCharsBefore, PointerToFirstByte(), u_Length_);

    u_Offset_ = 0;
    u_Length_ = u_TotalChars;

    NullTerminate_();

    return p_Memory_;
}

uchar* HeapMemory::Allocate (uintsys u_NumBytes)
{
    if ((u_NumBytes > u_Capacity_) || (u_Capacity_ == 0))
    {
        uintsys u_NewCapacity = 0;

        uchar* p_NewMemory = Allocate_ (u_NumBytes, u_NewCapacity);

        if (p_Memory_ != 0)
        {
            ZeroMemory (p_Memory_, u_Capacity_);    // destroy old memory

            delete [] p_Memory_;
        }

        p_Memory_   = p_NewMemory;
        u_Capacity_ = u_NewCapacity;
    }

    u_Offset_ = 0;
    u_Length_ = u_NumBytes;

    NullTerminate_();

    return p_Memory_;
}

} // namespace mikestoolbox

