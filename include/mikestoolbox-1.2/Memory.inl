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
//  File:       Memory.inl
//
//  Synopsis:   Implementation of Memory classes
//----------------------------------------------------------------------------

namespace mikestoolbox {

//+---------------------------------------------------------------------------
//  Class:      HeapMemory
//----------------------------------------------------------------------------

inline uintsys HeapMemory::Length () const
{
    return u_Length_;
}

inline const char* HeapMemory::PointerToFirstChar () const
{
    if (p_Memory_ == 0)
    {
        return "\0\0";
    }

    return (const char*) (p_Memory_ + u_Offset_);
}

inline const uchar* HeapMemory::PointerToFirstByte () const
{
    return (const uchar*) PointerToFirstChar();
}

inline HeapMemory::HeapMemory (const HeapMemory& mem)
    : p_Memory_   (0)
    , u_Offset_   (0)
    , u_Length_   (0)
    , u_Capacity_ (0)
{
    uintsys u_Length = mem.Length();

    if (u_Length != 0)
    {
        std::memcpy (Allocate(u_Length), mem.PointerToFirstByte(), u_Length);
    }
}

template<class MEMORY>
inline HeapMemory::HeapMemory (const MEMORY& mem)
    : p_Memory_   (0)
    , u_Offset_   (0)
    , u_Length_   (0)
    , u_Capacity_ (0)
{
    uintsys u_Length = mem.Length();

    if (u_Length != 0)
    {
        std::memcpy (Allocate(u_Length), mem.PointerToFirstByte(), u_Length);
    }
}

inline HeapMemory::HeapMemory (const char* pz)
    : p_Memory_   (0)
    , u_Offset_   (0)
    , u_Length_   (0)
    , u_Capacity_ (0)
{
    if (pz != 0)
    {
        uintsys u_Length = std::strlen (pz);

        if (u_Length != 0)
        {
            std::memcpy (Allocate(u_Length), pz, u_Length);
        }
    }
}

inline HeapMemory::HeapMemory (Preallocate amount)
    : p_Memory_   (0)
    , u_Offset_   (0)
    , u_Length_   (0)
    , u_Capacity_ (0)
{
    Reserve (amount);
}

inline HeapMemory::HeapMemory ()
    : p_Memory_   (0)
    , u_Offset_   (0)
    , u_Length_   (0)
    , u_Capacity_ (0)
{
    // nothing
}

inline HeapMemory::~HeapMemory ()
{
    if (p_Memory_ != 0)
    {
        // destroy string contents

        ZeroMemory (p_Memory_, u_Capacity_);

        delete [] p_Memory_;
    }
}

inline void HeapMemory::Append (const uchar* ps_Data, uintsys u_AppendLength)
{
    if (u_AppendLength != 0)
    {
        uintsys u_OriginalLength = Length();

        uchar* ps_Dest = Expand (0, u_AppendLength);

        std::memcpy (ps_Dest + u_OriginalLength, ps_Data, u_AppendLength);
    }
}

inline void HeapMemory::Append (const HeapMemory& mem)
{
    uintsys u_AppendLength = mem.Length();

    if (u_AppendLength != 0)
    {
        uintsys u_OriginalLength = Length();

        uchar* ps_Dest = Expand (0, u_AppendLength);

        std::memcpy (ps_Dest + u_OriginalLength, mem.PointerToFirstByte(),
                     u_AppendLength);
    }
}

template<class MEMORY>
inline void HeapMemory::Append (const MEMORY& mem)
{
    Append (mem.PointerToFirstByte(), mem.Length());
}

inline void HeapMemory::Clear ()
{
    if (p_Memory_ != 0)
    {
        ZeroMemory (p_Memory_, u_Offset_ + u_Length_);

        u_Offset_ = 0;
        u_Length_ = 0;
    }
}

inline void HeapMemory::Destroy ()
{
    if (p_Memory_ != 0)
    {
        ZeroMemory (p_Memory_, u_Capacity_);

        u_Offset_ = 0;
        u_Length_ = 0;
    }
}

inline uchar* HeapMemory::EditInPlace ()
{
    if (p_Memory_ == 0)
    {
        return Allocate (0);
    }

    return p_Memory_ + u_Offset_;
}

inline void HeapMemory::NullTerminate_ ()
{
    if (p_Memory_ != 0)
    {
        uchar* puc = &p_Memory_[u_Offset_ + u_Length_];

        *puc++ = 0;
        *puc   = 0;
    }
}

inline void HeapMemory::Erase (const Index& index, uintsys u_NumBytes)
{
    uintsys u_Index = 0;

    if ((u_NumBytes != 0) && index.Calculate (u_Length_, u_Index))
    {
        if (u_Index == 0)
        {
            if (u_NumBytes >= u_Length_)
            {
                u_Offset_ = 0;
                u_Length_ = 0;

                NullTerminate_();
            }
            else
            {
                u_Offset_ += u_NumBytes;
                u_Length_ -= u_NumBytes;
            }
        }
        else
        {
            uintsys u_BeyondEnd = u_Index + u_NumBytes;

            if ((u_BeyondEnd < u_Index) || (u_BeyondEnd >= u_Length_))
            {
                u_Length_ = u_Index;
            }
            else
            {
                std::memmove (&p_Memory_[u_Offset_ + u_Index],
                              &p_Memory_[u_Offset_ + u_BeyondEnd],
                              u_Length_ - u_BeyondEnd);

                u_Length_ -= u_NumBytes;
            }

            NullTerminate_();
        }
    }
}

inline void HeapMemory::EraseEnd (uintsys u_NumBytes)
{
    if (p_Memory_ != 0)
    {
        if (u_NumBytes < u_Length_)
        {
            u_Length_ -= u_NumBytes;
        }
        else
        {
            u_Offset_ = 0;
            u_Length_ = 0;
        }

        NullTerminate_();
    }
}

inline void HeapMemory::EraseFront (uintsys u_NumBytes)
{
    if (p_Memory_ != 0)
    {
        if (u_NumBytes < u_Length_)
        {
            u_Offset_ += u_NumBytes;
            u_Length_ -= u_NumBytes;
        }
        else
        {
            u_Offset_ = 0;
            u_Length_ = 0;

            NullTerminate_();
        }
    }
}

inline void HeapMemory::Prepend (const HeapMemory& mem)
{
    if (this == &mem)
    {
        Append (mem);

        return;
    }

    uintsys u_PrependLength = mem.Length();

    if (u_PrependLength != 0)
    {
        uchar* ps_Dest = Expand (u_PrependLength, 0);

        std::memcpy (ps_Dest, mem.PointerToFirstByte(), u_PrependLength);
    }
}

template<class MEMORY>
inline void HeapMemory::Prepend (const MEMORY& mem)
{
    uintsys u_PrependLength = mem.Length();

    if (u_PrependLength != 0)
    {
        uchar* ps_Dest = Expand (u_PrependLength, 0);

        std::memcpy (ps_Dest, mem.PointerToFirstByte(), u_PrependLength);
    }
}

inline void HeapMemory::Reserve (uintsys u_NumBytes)
{
    uintsys u_CurrentLength = Length();

    if (u_NumBytes > u_CurrentLength)
    {
        Expand (0, u_NumBytes - u_CurrentLength);

        u_Length_ = u_CurrentLength;

        NullTerminate_();
    }
}

inline void HeapMemory::Swap (HeapMemory& mem)
{
    uchar*  p_Memory   = p_Memory_;
    uintsys u_Offset   = u_Offset_;
    uintsys u_Length   = u_Length_;
    uintsys u_Capacity = u_Capacity_;

    p_Memory_   = mem.p_Memory_;
    u_Offset_   = mem.u_Offset_;
    u_Length_   = mem.u_Length_;
    u_Capacity_ = mem.u_Capacity_;

    mem.p_Memory_   = p_Memory;
    mem.u_Offset_   = u_Offset;
    mem.u_Length_   = u_Length;
    mem.u_Capacity_ = u_Capacity;
}

inline HeapMemory& HeapMemory::operator= (const HeapMemory& mem)
{
    if (this == &mem)
    {
        return *this;
    }

    uintsys u_NewLength = mem.Length();

    if (u_NewLength == 0)
    {
        Clear();
    }
    else
    {
        std::memcpy (Allocate (u_NewLength), mem.PointerToFirstByte(),
                     u_NewLength);
    }

    return *this;
}

template<class MEMORY>
inline HeapMemory& HeapMemory::operator= (const MEMORY& mem)
{
    uintsys u_NewLength = mem.Length();

    if (u_NewLength == 0)
    {
        Clear();
    }
    else
    {
        std::memcpy (Allocate (u_NewLength), mem.PointerToFirstByte(),
                     u_NewLength);
    }

    return *this;
}

inline HeapMemory& HeapMemory::operator= (const char* pz)
{
    if (pz == 0)
    {
        Clear();
    }
    else
    {
        uintsys u_NewLength = std::strlen (pz);

        if (u_NewLength == 0)
        {
            Clear();
        }
        else
        {
            std::memcpy (Allocate (u_NewLength), pz, u_NewLength);
        }
    }

    return *this;
}

inline bool HeapMemory::operator== (const HeapMemory& mem) const
{
    if (this == &mem)
    {
        return true;
    }

    if (u_Length_ != mem.u_Length_)
    {
        return false;
    }

    if (u_Length_ == 0)
    {
        return true;
    }

    return std::memcmp (PointerToFirstByte(), mem.PointerToFirstByte(),
                        u_Length_) == 0;
}

template<class MEMORY>
inline bool HeapMemory::operator== (const MEMORY& mem) const
{
    if (u_Length_ != mem.Length())
    {
        return false;
    }

    if (u_Length_ == 0)
    {
        return true;
    }

    return std::memcmp (PointerToFirstByte(), mem.PointerToFirstByte(),
                        u_Length_) == 0;
}

inline bool HeapMemory::operator< (const HeapMemory& mem) const
{
    if (this == &mem)
    {
        return false;
    }

    uintsys u_MinLength = Minimum (u_Length_, mem.Length());

    if (u_MinLength == 0)
    {
        return (u_Length_ < mem.Length());
    }

    int n_Compare = std::memcmp (PointerToFirstByte(),
                                 mem.PointerToFirstByte(),
                                 u_MinLength);

    if (n_Compare == 0)
    {
        return (u_Length_ < mem.u_Length_);
    }

    return (n_Compare < 0);
}

template<class MEMORY>
inline bool HeapMemory::operator< (const MEMORY& mem) const
{
    uintsys u_MinLength = Minimum (u_Length_, mem.Length());

    if (u_MinLength == 0)
    {
        return (u_Length_ < mem.Length());
    }

    int n_Compare = std::memcmp (PointerToFirstByte(),
                                 mem.PointerToFirstByte(),
                                 u_MinLength);

    if (n_Compare == 0)
    {
        return (u_Length_ < mem.Length());
    }

    return (n_Compare < 0);
}

//+---------------------------------------------------------------------------
//  Class:      SharedMemory
//----------------------------------------------------------------------------

template<class MEMORY>
inline SharedMemory<MEMORY>::Data::Data (const Data& data)
    : mem_         (data.mem_)
    , refs_        (1)
    , b_CountRefs_ (true)
{
    // nothing
}

template<class MEMORY>
inline SharedMemory<MEMORY>::Data::Data ()
    : mem_         ()
    , refs_        (1)
    , b_CountRefs_ (true)
{
    // nothing
}

template<class MEMORY>
inline SharedMemory<MEMORY>::Data::Data (int n)
    : mem_         ()
    , refs_        (2)
    , b_CountRefs_ (false)
{
    // nothing
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::Data::IncrementRefs ()
{
    return (b_CountRefs_ ? (uintsys)(++refs_) : 2U);
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::Data::DecrementRefs ()
{
    return (b_CountRefs_ ? (uintsys)(--refs_) : 2U);
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::IncrementRefs_ () const
{
    return p_Data_->IncrementRefs();
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::DecrementRefs_ () const
{
    return p_Data_->DecrementRefs();
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::References_ () const
{
    return p_Data_->refs_;
}

template<class MEMORY>
inline SharedMemory<MEMORY>::SharedMemory (const SharedMemory& mem)
    : p_Data_ (mem.p_Data_)
{
    IncrementRefs_();
}

template<class MEMORY>
template<class MEMORY2>
inline SharedMemory<MEMORY>::SharedMemory (const MEMORY2& mem)
    : p_Data_ (new(std::nothrow) Data)
{
    if (p_Data_ == 0)
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    p_Data_->mem_ = mem;
}

template<class MEMORY>
inline SharedMemory<MEMORY>::SharedMemory (const char* pz)
    : p_Data_ (new(std::nothrow) Data)
{
    if (p_Data_ == 0)
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    p_Data_->mem_ = pz;
}

template<class MEMORY>
inline SharedMemory<MEMORY>::SharedMemory (Preallocate amount)
    : p_Data_ (&data_Empty_)
{
    Reserve (amount);
}

template<class MEMORY>
inline SharedMemory<MEMORY>::SharedMemory ()
    : p_Data_ (&data_Empty_)
{
    // nothing
}

template<class MEMORY>
inline SharedMemory<MEMORY>::~SharedMemory ()
{
    if ((References_() == 1) || (DecrementRefs_() == 0))
    {
        delete p_Data_;
    }
}

template<class MEMORY>
inline uintsys SharedMemory<MEMORY>::Length () const
{
    return p_Data_->mem_.Length();
}

template<class MEMORY>
inline const char* SharedMemory<MEMORY>::PointerToFirstChar () const
{
    return p_Data_->mem_.PointerToFirstChar();
}

template<class MEMORY>
inline const uchar* SharedMemory<MEMORY>::PointerToFirstByte () const
{
    return p_Data_->mem_.PointerToFirstByte();
}

template<class MEMORY>
inline typename SharedMemory<MEMORY>::Data* SharedMemory<MEMORY>::ModifyData_ ()
{
    if (References_() != 1)
    {
        Data* p_NewData = new(std::nothrow) Data (*p_Data_);

        if (p_NewData == 0)
        {
            throw Exception ("SharedMemory: Out of memory");
        }

        Data* p_ToDelete = 0;

        if (DecrementRefs_() == 0)
        {
            p_ToDelete = p_Data_;
        }

        p_Data_ = p_NewData;

        delete p_ToDelete;
    }

    return p_Data_;
}

template<class MEMORY>
inline uchar* SharedMemory<MEMORY>::EditInPlace ()
{
    return ModifyData_()->mem_.EditInPlace();
}

template<class MEMORY>
inline void
    SharedMemory<MEMORY>::Append (const uchar* ps_Data, uintsys u_AppendLength)
{
    uintsys u_OriginalLength = Length();
    uchar*  ps_Dest          = Expand (0, u_AppendLength) + u_OriginalLength;

    std::memcpy (ps_Dest, ps_Data, u_AppendLength);
}

template<class MEMORY>
template<class MEMORY2>
inline void SharedMemory<MEMORY>::Append (const MEMORY2& mem)
{
    uintsys      u_OrigLength   = Length();
    uintsys      u_AppendLength = mem.Length();
    uchar*       p_Dest         = Expand (0, u_AppendLength) + u_OrigLength;
    const uchar* p_Source       = mem.PointerToFirstByte();

    std::memcpy (p_Dest, p_Source, u_AppendLength);
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::Clear ()
{
    if (References_() == 1)
    {
        p_Data_->mem_.Clear();
    }
    else if (DecrementRefs_() == 0)
    {
        IncrementRefs_();

        p_Data_->mem_.Clear();
    }
    else
    {
        p_Data_ = &data_Empty_;
    }
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::Destroy ()
{
    if (References_() == 1)
    {
        p_Data_->mem_.Destroy();
    }
    else if (DecrementRefs_() == 0)
    {
        IncrementRefs_();

        p_Data_->mem_.Destroy();
    }
    else
    {
        p_Data_ = &data_Empty_;
    }
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::Erase (const Index& offset,
                                         uintsys u_NumBytes)
{
    uintsys u_Offset = 0;

    if ((u_NumBytes != 0) && offset.Calculate (Length(), u_Offset))
    {
        ModifyData_()->mem_.Erase (offset, u_NumBytes);
    }
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::EraseEnd (uintsys u_NumBytes)
{
    if (u_NumBytes != 0)
    {
        ModifyData_()->mem_.EraseEnd (u_NumBytes);
    }
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::EraseFront (uintsys u_NumBytes)
{
    if (u_NumBytes != 0)
    {
        ModifyData_()->mem_.EraseFront (u_NumBytes);
    }
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::Prepend (const SharedMemory& mem)
{
    if (this == &mem)
    {
        Append (mem);   // handles case where appending self
    }
    else
    {
        uintsys      u_Length = mem.Length();
        uchar*       p_Dest   = Expand (u_Length, 0);
        const uchar* p_Source = mem.PointerToFirstByte();

        std::memcpy (p_Dest, p_Source, u_Length);
    }
}

template<class MEMORY>
template<class MEMORY2>
inline void SharedMemory<MEMORY>::Prepend (const MEMORY2& mem)
{
    uintsys      u_Length = mem.Length();
    uchar*       p_Dest   = Expand (u_Length, 0);
    const uchar* p_Source = mem.PointerToFirstByte();

    std::memcpy (p_Dest, p_Source, u_Length);
}

template<class MEMORY>
inline void SharedMemory<MEMORY>::Swap (SharedMemory& mem)
{
    Data* p_Temp = p_Data_;

    p_Data_ = mem.p_Data_;

    mem.p_Data_ = p_Temp;
}

template<class MEMORY>
inline SharedMemory<MEMORY>&
    SharedMemory<MEMORY>::operator= (const SharedMemory<MEMORY>& mem)
{
    SharedMemory<MEMORY> mem_New (mem);

    Swap (mem_New);

    return *this;
}

template<class MEMORY>
template<class MEMORY2>
inline SharedMemory<MEMORY>&
    SharedMemory<MEMORY>::operator= (const MEMORY2& mem)
{
    SharedMemory<MEMORY> mem_New (mem);

    Swap (mem_New);

    return *this;
}

template<class MEMORY>
inline SharedMemory<MEMORY>& SharedMemory<MEMORY>::operator= (const char* pz)
{
    SharedMemory<MEMORY> mem_New (pz);

    Swap (mem_New);

    return *this;
}

template<class MEMORY>
inline bool
    SharedMemory<MEMORY>::operator== (const SharedMemory<MEMORY>& mem) const
{
    const Data* p_This = p_Data_;
    const Data* p_That = mem.p_Data_;

    return (p_This == p_That) || (p_This->mem_ == p_That->mem_);
}

template<class MEMORY>
template<class MEMORY2>
inline bool SharedMemory<MEMORY>::operator== (const MEMORY2& mem) const
{
    return (p_Data_->mem_ == mem);
}

template<class MEMORY>
inline bool SharedMemory<MEMORY>::operator< (const SharedMemory& mem) const
{
    const Data* p_This = p_Data_;
    const Data* p_That = mem.p_Data_;

    return (p_This != p_That) && (p_This->mem_ < p_That->mem_);
}

template<class MEMORY>
template<class MEMORY2>
inline bool SharedMemory<MEMORY>::operator< (const MEMORY2& mem) const
{
    return (p_Data_->mem_ < mem);
}

template<class MEMORY>
void SharedMemory<MEMORY>::Append (const SharedMemory<MEMORY>& mem)
{
    uintsys u_Length = Length();

    if (this == &mem)   // appending this to this
    {
        if (u_Length == 0)
        {
            return;
        }

        if (References_() == 1)
        {
            uchar* p_String = p_Data_->mem_.Expand (0, u_Length);

            std::memcpy (p_String+u_Length, p_String, u_Length);
        }
        else
        {
            if (IsTopBitSet (u_Length))
            {
                throw Exception ("SharedMemory::Append: Integer overflow");
            }

            SharedMemory<MEMORY> mem_New;

            uchar*       p_NewString = mem_New.Allocate (2*u_Length);
            const uchar* p_Source    = mem.PointerToFirstByte();

            std::memcpy (p_NewString,          p_Source, u_Length);
            std::memcpy (p_NewString+u_Length, p_Source, u_Length);

            Swap (mem_New);
        }
    }
    else
    {
        uintsys u_AppendLength = mem.Length();

        if (u_AppendLength != 0)
        {
            uchar*       p_Dest   = Expand (0, u_AppendLength);
            const uchar* p_Source = mem.PointerToFirstByte();

            std::memcpy (p_Dest+u_Length, p_Source, u_AppendLength);
        }
    }
}

template<class MEMORY>
void SharedMemory<MEMORY>::Reserve (uintsys u_NumBytes)
{
    if (References_() == 1)
    {
        p_Data_->mem_.Reserve (u_NumBytes);

        return;
    }

    SharedMemory<MEMORY> mem_New;    // originally points to data_Empty_

    Data* p_NewData = new(std::nothrow) Data;

    if (p_NewData == 0)
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    mem_New.p_Data_ = p_NewData;

    p_NewData->mem_.Reserve (u_NumBytes);

    uintsys u_Length = Length();

    uchar* p_Dest = p_NewData->mem_.Allocate (u_Length);

    std::memcpy (p_Dest, PointerToFirstByte(), u_Length);

    Swap (mem_New);
}

template<class MEMORY>
uchar* SharedMemory<MEMORY>::Expand (uintsys u_NumCharsBefore,
                                     uintsys u_NumCharsAfter)
{
    if (References_() == 1)
    {
        return p_Data_->mem_.Expand (u_NumCharsBefore, u_NumCharsAfter);
    }

    SharedMemory<MEMORY> mem_New;    // originally points to data_Empty_

    Data* p_NewData = new(std::nothrow) Data;

    if (p_NewData == 0)
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    mem_New.p_Data_ = p_NewData;

    uintsys u_OriginalLength = Length();
    uintsys u_ExtraChars     = u_NumCharsBefore + u_NumCharsAfter;
    uintsys u_TotalChars     = u_OriginalLength + u_ExtraChars;

    if ((u_ExtraChars < u_NumCharsBefore) ||
        (u_TotalChars < u_OriginalLength))
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    uchar* p_Start = p_NewData->mem_.Allocate (u_TotalChars);

    std::memcpy (p_Start+u_NumCharsBefore, PointerToFirstByte(),
                 u_OriginalLength);

    Swap (mem_New);

    return p_Start;
}

template<class MEMORY>
uchar* SharedMemory<MEMORY>::Allocate (uintsys u_NumBytes)
{
    if (References_() == 1)
    {
        return p_Data_->mem_.Allocate (u_NumBytes);
    }

    SharedMemory<MEMORY> mem_New;    // originally points to data_Empty_

    Data* p_NewData = new(std::nothrow) Data;

    if (p_NewData == 0)
    {
        throw Exception ("SharedMemory: Out of memory");
    }

    mem_New.p_Data_ = p_NewData;

    uchar* p_Buffer = p_NewData->mem_.Allocate (u_NumBytes);

    Swap (mem_New);

    return p_Buffer;
}

} // namespace mikestoolbox

