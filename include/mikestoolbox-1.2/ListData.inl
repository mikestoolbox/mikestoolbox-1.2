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
//  File:       ListData.inl
//
//  Synopsis:   Methods of ListData<T>
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<typename T>
inline typename ListAllocatorType<T>::Type* ListData<T>::GetAllocator ()
{
    static Alloc alloc_;

    return &alloc_;
}

template<typename T>
inline ListData<T>* ListData<T>::Undef ()
{
    static Data undef_ ((AlwaysShared()));

    return &undef_;
}

template<typename T>
inline void ListData<T>::RotateLeft ()
{
    p_Root_ = p_Root_->p_Next_;
}

template<typename T>
inline void ListData<T>::RotateRight ()
{
    p_Root_ = p_Root_->p_Prev_;
}

template<typename T>
inline ListItem<T>* ListData<T>::StealItems ()
{
    Item* p_Items = p_Root_;

    p_Root_     = 0;
    u_NumItems_ = 0;

    return p_Items;
}

template<typename T>
inline void ListData<T>::Append (Item* p_Items, uintsys u_NumItems)
{
    if (u_NumItems != 0)
    {
        if (u_NumItems_ == 0)
        {
            p_Root_     = p_Items;
            u_NumItems_ = u_NumItems;
        }
        else
        {
            Item* p1 = p_Root_;
            Item* p2 = p_Root_->p_Prev_;
            Item* p3 = p_Items;
            Item* p4 = p_Items->p_Prev_;

            p2->Append (p3);
            p4->Append (p1);

            u_NumItems_ += u_NumItems;
        }
    }
}

template<typename T>
inline void ListData<T>::Append (Data* p_List)
{
    uintsys u_NumItems = p_List->u_NumItems_;

    Append (p_List->StealItems(), u_NumItems);
}

template<typename T>
inline void ListData<T>::Prepend (Item* p_Items, uintsys u_NumItems)
{
    if (u_NumItems != 0)
    {
        if (u_NumItems_ == 0)
        {
            p_Root_     = p_Items;
            u_NumItems_ = u_NumItems;
        }
        else
        {
            Item* p1 = p_Items;
            Item* p2 = p_Items->p_Prev_;
            Item* p3 = p_Root_;
            Item* p4 = p_Root_->p_Prev_;

            p2->Append (p3);
            p4->Append (p1);

            p_Root_      = p_Items;
            u_NumItems_ += u_NumItems;
        }
    }
}

template<typename T>
inline void ListData<T>::Prepend (Data* p_List)
{
    uintsys u_NumItems = p_List->u_NumItems_;

    Prepend (p_List->StealItems(), u_NumItems);
}

template<typename T>
inline ListData<T>::ListData (AlwaysShared share)
    : SharedData  (share)
    , p_Root_     (0)
    , u_NumItems_ (0)
{
    // nothing
}

template<typename T>
inline ListData<T>::ListData ()
    : SharedData  ()
    , p_Root_     (0)
    , u_NumItems_ (0)
{
    // nothing
}

template<typename T>
inline uintsys ListData<T>::NumItems () const
{
    return u_NumItems_;
}

template<typename T>
inline bool ListData<T>::IsEmpty () const
{
    return (u_NumItems_ == 0);
}

template<typename T>
bool ListData<T>::Check () const
{
    const Item* p_Item = p_Root_;

    if (p_Item == 0)
    {
        return (u_NumItems_ == 0);
    }

    for (uintsys u=0; u<u_NumItems_; ++u)
    {
        if (!p_Item->Check())
        {
            return false;
        }

        p_Item = p_Item->p_Next_;
    }

    return (p_Item == p_Root_);
}

template<typename T>
void ListData<T>::Clear ()
{
    if (u_NumItems_ != 0)
    {
        Alloc* alloc = GetAllocator();

        while (u_NumItems_ > 1)
        {
            alloc->DeleteItem (p_Root_->p_Prev_);

            --u_NumItems_;
        }

        alloc->DeleteItem (p_Root_);

        p_Root_     = 0;
        u_NumItems_ = 0;
    }
}

template<typename T>
inline ListData<T>::~ListData ()
{
    Clear();
}

template<typename T>
ListData<T>::ListData (const ListData<T>& copy)
    : SharedData  ()
    , p_Root_     (0)
    , u_NumItems_ (0)
{
    if (copy.u_NumItems_ != 0)
    {
        Alloc* alloc = GetAllocator();

        const Item* p_Other = copy.p_Root_;
              Item* p_New   = 0;

        try
        {
            p_Root_ = alloc->CreateItem (p_Other);

            ++u_NumItems_;

            for (uintsys u=1; u<copy.u_NumItems_; ++u)
            {
                p_Other = p_Other->p_Next_;
                p_New   = alloc->CreateItem (p_Other);

                p_Root_->p_Prev_->Append (p_New);

                p_New->Append (p_Root_);

                ++u_NumItems_;
            }
        }
        catch (...)
        {
            Clear();

            throw;
        }
    }
}

template<typename T>
inline void ListData<T>::Reverse ()
{
    using std::swap;

    if (u_NumItems_ > 1)
    {
        Item* p_Item = p_Root_;

        do
        {
            swap (p_Item->p_Next_, p_Item->p_Prev_);

            p_Item = p_Item->p_Prev_;   // actually going forward thru list
        }
        while (p_Item != p_Root_);

        p_Root_ = p_Root_->p_Next_;
    }
}

template<typename T>
inline const T ListData<T>::Shift ()
{
    if (u_NumItems_ == 0)
    {
        return T();
    }

    T t (p_Root_->Value());

    p_Root_ = p_Root_->p_Next_;

    GetAllocator()->DeleteItem (p_Root_->p_Prev_);

    if (--u_NumItems_ == 0)
    {
        p_Root_ = 0;
    }

    return t;
}

template<typename T>
inline const T ListData<T>::Pop ()
{
    if (u_NumItems_ > 1)
    {
        RotateRight();
    }

    return Shift();
}

template<typename T>
inline void ListData<T>::Append (const T& item)
{
    Item* p_Item = GetAllocator()->CreateItem (item);

    if (p_Root_ == 0)
    {
        p_Root_ = p_Item;
    }
    else
    {
        p_Root_->p_Prev_->Append (p_Item);

        p_Item->Append (p_Root_);
    }

    ++u_NumItems_;
}

template<typename T>
inline void ListData<T>::Append (const T& item, Repeat repeat)
{
    for (uintsys u=0; u<repeat; ++u)
    {
        Append (item);
    }
}

template<typename T>
inline void ListData<T>::Prepend (const T& item)
{
    Append (item);

    RotateRight();
}

template<typename T>
inline void ListData<T>::Prepend (const T& item, Repeat repeat)
{
    for (uintsys u=0; u<repeat; ++u)
    {
        Prepend (item);
    }
}

template<typename T>
inline void ListData<T>::Truncate (uintsys u_Length)
{
    if (u_NumItems_ > u_Length)
    {
        Alloc* alloc = GetAllocator();

        while (u_NumItems_ > u_Length)
        {
            alloc->DeleteItem (p_Root_->p_Prev_);

            --u_NumItems_;
        }
    }
}

template<typename T>
ListData<T>* ListData<T>::Split (uintsys u_Index)
{
    Data* p_Return = GetAllocator()->CreateData();

    if (u_Index == 0)
    {
        p_Return->p_Root_     = p_Root_;
        p_Return->u_NumItems_ = u_NumItems_;

        p_Root_     = 0;
        u_NumItems_ = 0;

        return p_Return;
    }

    Item* p_Item = SeekToItem (u_Index);
    Item* p_Prev = p_Item->p_Prev_;
    Item* p_Last = p_Root_->p_Prev_;

    p_Prev->Append (p_Root_);
    p_Last->Append (p_Item);

    p_Return->Append (p_Item, u_NumItems_ - u_Index);

    u_NumItems_ = u_Index;

    return p_Return;
}

template<typename T>
const ListItem<T>* ListData<T>::SeekToItem (uintsys u_Index) const
{
    const Item* p_Item = p_Root_;

    if (u_Index < u_NumItems_/2)
    {
        for (uintsys u=0; u<u_Index; ++u)
        {
            p_Item = p_Item->p_Next_;
        }
    }
    else
    {
        for (uintsys u=u_NumItems_; u>u_Index; --u)
        {
            p_Item = p_Item->p_Prev_;
        }
    }

    return p_Item;
}

template<typename T>
ListItem<T>* ListData<T>::SeekToItem (uintsys u_Index)
{
    Item* p_Item = p_Root_;

    if (u_Index < u_NumItems_/2)
    {
        for (uintsys u=0; u<u_Index; ++u)
        {
            p_Item = p_Item->p_Next_;
        }
    }
    else
    {
        for (uintsys u=u_NumItems_; u>u_Index; --u)
        {
            p_Item = p_Item->p_Prev_;
        }
    }

    return p_Item;
}

template<typename T>
void ListData<T>::Insert (const Index& index, const T& item)
{
    if (index.Overflowed())
    {
        throw Exception ("List::Insert: Index integer overflow");
    }

    uintsys u_Index = 0;

    if (!index.Calculate (u_NumItems_, u_Index))
    {
        T t = T();

        while (index.Shortfall (u_NumItems_) > 1)
        {
            if (index.IsNegative())
            {
                Prepend (t);
            }
            else
            {
                Append (t);
            }
        }

        if (index.IsNegative())
        {
            Prepend (item);
        }
        else
        {
            Append (item);
        }

        return;
    }

    if (u_Index == 0)
    {
        Prepend (item);

        return;
    }

    Item* p_NewItem   = GetAllocator()->CreateItem (item);
    Item* p_ItemAfter = SeekToItem (u_Index);

    p_ItemAfter->p_Prev_->Append (p_NewItem);
    p_NewItem->Append (p_ItemAfter);

    ++u_NumItems_;
}

} // namespace mikestoolbox

