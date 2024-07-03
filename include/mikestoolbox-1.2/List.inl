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
//  File:       List.inl
//
//  Synopsis:   Methods of List<T>
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<typename T>
inline typename ListAllocatorType<T>::Type* List<T>::GetAllocator_ ()
{
    return ListData<T>::GetAllocator();
}

template<typename T>
inline List<T>::List ()
    : SharedResource (RESOURCE_COPY_ON_WRITE, Data::Undef())
{
    // nothing
}

template<typename T>
inline List<T>::List (Data* p_Data)
    : SharedResource (RESOURCE_COPY_ON_WRITE, p_Data)
{
    // nothing
}

template<typename T>
inline void List<T>::Swap (List<T>& list)
{
    SharedResource::Swap (list);
}

template<typename T>
inline void swap (List<T>& list1, List<T>& list2)
{
    list1.Swap (list2);
}

template<typename T>
template<typename CONTAINER>
inline List<T>& List<T>::operator= (const CONTAINER& c)
{
    ListType list (c);

    Swap (list);

    return *this;
}

template<typename T>
inline List<T>& List<T>::operator= (const List<T>& l)
{
    ListType list (l);

    Swap (list);

    return *this;
}

template<typename T>
inline List<T>& List<T>::operator= (const T& item)
{
    ListType list (item);

    Swap (list);

    return *this;
}

template<typename T>
inline bool List<T>::operator== (const List<T>& list) const
{
    const ListData<T>* p_This = ViewData();
    const ListData<T>* p_That = list.ViewData();

    if (p_This == p_That)
    {
        return true;
    }

    if (p_This->u_NumItems_ != p_That->u_NumItems_)
    {
        return false;
    }

    Iter iter1 (*this);
    Iter iter2 (list);

    while (iter1)
    {
        const T& t1 (*iter1);
        const T& t2 (*iter2);

        if (t1 != t2)
        {
            return false;
        }

        ++iter1;
        ++iter2;
    }

    return true;
}

template<typename T>
inline bool List<T>::operator!= (const List<T>& list) const
{
    return !operator== (list);
}

template<typename T>
inline const ListData<T>* List<T>::ViewData () const
{
    return (const ListData<T>*) SharedResource::ViewData();
}

template<typename T>
inline ListData<T>* List<T>::ModifyData ()
{
    return (ListData<T>*) SharedResource::ModifyData();
}

template<typename T>
inline ListData<T>*
    List<T>::MakeCopyOfSharedData_ (const SharedData* p_Copy) const
{
    const Data* p_Data = (const Data*) p_Copy;

    return GetAllocator_()->CreateData (p_Data);
}

template<typename T>
inline bool List<T>::Check () const
{
    return ViewData()->Check();
}

template<typename T>
inline List<T>& List<T>::Append (const T& item)
{
    ModifyData()->Append (item);

    return *this;
}

template<typename T>
template<typename CONTAINER>
inline List<T>& List<T>::Append (const CONTAINER& c)
{
    Append_ (c, typename ContainerType<CONTAINER>::Type());

    return *this;
}

template<typename T>
inline List<T>& List<T>::Clear ()
{
    ListType list;

    Swap (list);

    return *this;
}

template<typename T>
inline bool List<T>::Contains (const T& t) const
{
    Iter iter (*this);

    while (iter)
    {
        if (t == *iter)
        {
            return true;
        }

        ++iter;
    }

    return false;
}

template<typename T>
inline uintsys List<T>::Count (const T& t) const
{
    Iter iter (*this);

    uintsys u_Count = 0;

    while (iter)
    {
        if (t == *iter)
        {
            ++u_Count;
        }

        ++iter;
    }

    return u_Count;
}

template<typename T>
inline bool List<T>::IsEmpty () const
{
    return ViewData()->IsEmpty();
}

template<typename T>
inline uintsys List<T>::NumItems () const
{
    return ViewData()->NumItems();
}

template<typename T>
inline ListItem<T>* List<T>::StealItems_ ()
{
    return ModifyData()->StealItems();
}

template<typename T>
inline ListItem<T>* List<T>::CopyItems_ ()
{
    ListType list_Copy (*this);

    return list_Copy.StealItems_();
}

template<typename T>
inline const T List<T>::Pop ()
{
    return ModifyData()->Pop();
}

template<typename T>
inline List<T>& List<T>::Prepend (const T& item)
{
    ModifyData()->Prepend (item);

    return *this;
}

template<typename T>
inline List<T>& List<T>::Prepend (ListType list)
{
    ListType copy (list);

    ModifyData()->Prepend (copy.StealItems_(), list.NumItems());

    return *this;
}

template<typename T>
template<typename CONTAINER>
inline List<T>& List<T>::Prepend (const CONTAINER& c)
{
    ListType list (c);

    return Prepend (list);
}

template<typename T>
inline List<T>& List<T>::Prepend (const T& item, Repeat repeat)
{
    Data* p_Data = ModifyData();

    uintsys u = 0;

    try
    {
        for (uintsys count=0; count<repeat; ++count)
        {
            p_Data->Prepend (item); ++u;
        }
    }
    catch (...)
    {
        while (u != 0)
        {
            Shift();

            --u;
        }

        throw;
    }

    return *this;
}

template<typename T>
inline List<T>& List<T>::Reverse ()
{
    ModifyData()->Reverse();

    return *this;
}

template<typename T>
inline const T List<T>::Shift ()
{
    return ModifyData()->Shift();
}

template<typename T>
inline List<T>& List<T>::Truncate (uintsys u_Length)
{
    ModifyData()->Truncate (u_Length);

    return *this;
}

template<typename T>
inline const ListIter<T> List<T>::Begin () const
{
    return ListIter<T> (*this);
}

template<typename T>
inline const ListChangeIter<T> List<T>::Begin ()
{
    return ListChangeIter<T> (*this);
}

template<typename T>
inline const ListIter<T> List<T>::End () const
{
    return ListIter<T> (*this, -1);
}

template<typename T>
inline const ListChangeIter<T> List<T>::End ()
{
    return ListChangeIter<T> (*this, -1);
}

template<typename T>
inline List<T>& List<T>::Append (ListType list)
{
    ListType copy (list);

    ModifyData()->Append (copy.StealItems_(), list.NumItems());

    return *this;
}

template<typename T>
inline List<T>& List<T>::Append (const T& item, Repeat repeat)
{
    Data* p_Data = ModifyData();

    uintsys u = 0;

    try
    {
        for (uintsys count=0; count<repeat; ++count)
        {
            p_Data->Append (item); ++u;
        }
    }
    catch (...)
    {
        while (u != 0)
        {
            Pop();

            --u;
        }

        throw;
    }

    return *this;
}

template<typename T>
template<typename CONTAINER>
inline List<T>::List (const CONTAINER& c)
    : SharedResource (RESOURCE_COPY_ON_WRITE, Data::Undef())
{
    Append (c);
}

template<typename T>
inline List<T>::List (const T& t)
    : SharedResource (RESOURCE_COPY_ON_WRITE, Data::Undef())
{
    Append (t);
}

template<typename T>
inline List<T>::List (const T& t, Repeat repeat)
    : SharedResource (RESOURCE_COPY_ON_WRITE, Data::Undef())
{
    Append (t, repeat);
}

template<typename T>
template<typename CONTAINER>
inline void List<T>::Append_ (const CONTAINER& c, MikesToolboxSimpleContainer)
{
    if (!c.IsEmpty())
    {
        ListType list;

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            T item (*iter);

            list.Append (item);

            ++iter;
        }

        Append (list);
    }
}

template<typename T>
template<typename CONTAINER>
inline void
    List<T>::Append_ (const CONTAINER& c, MikesToolboxKeyValueContainer)
{
    if (!c.IsEmpty())
    {
        ListType list;

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            T key   (iter->Key());
            T value (iter->Value());

            list.Append (key);
            list.Append (value);

            ++iter;
        }

        Append (list);
    }
}

template<typename T>
template<typename CONTAINER>
inline void
    List<T>::Append_ (const CONTAINER& c, StandardCPlusPlusSimpleContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    ListType list;

    while (iter1 != iter2)
    {
        T item (*iter1);

        list.Append (item);

        ++iter1;
    }

    Append (list);
}

template<typename T>
template<typename CONTAINER>
inline void
    List<T>::Append_ (const CONTAINER& c, StandardCPlusPlusKeyValueContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    ListType list;

    while (iter1 != iter2)
    {
        T key   (iter1->first);
        T value (iter1->second);

        list.Append (key);
        list.Append (value);

        ++iter1;
    }

    Append (list);
}

template<typename T>
template<typename CONTAINER>
inline void List<T>::Append_ (const CONTAINER& c, UnknownTypeOfContainer)
{
    // try to Append it as a single item

    T item (c);

    Append (item);
}

template<typename T>
template<typename CONTAINER>
inline List<T>& List<T>::operator+= (const CONTAINER& c)
{
    return Append (c);
}

template<typename T>
inline List<T>& List<T>::operator+= (ListType list)
{
    return Append (list);
}

template<typename T>
inline List<T>& List<T>::operator+= (const T& item)
{
    return Append (item);
}

template<typename T>
template<typename CONTAINER>
inline const List<T> List<T>::operator+ (const CONTAINER& c) const
{
    ListType list (*this);

    list.Append (c);

    return list;
}

template<typename T>
inline const List<T> List<T>::operator+ (const List<T>& l) const
{
    ListType list (*this);

    list.Append (l);

    return list;
}

template<typename T>
inline const List<T> List<T>::operator+ (const T& item) const
{
    ListType list (*this);

    list.Append (item);

    return list;
}

template<typename T>
inline const T List<T>::operator[] (const Index& index) const
{
    uintsys u_Index = 0;

    if (index.Calculate (NumItems(), u_Index))
    {
        return ViewData()->SeekToItem (u_Index)->Value();
    }

    return T();
}

template<typename T>
T& List<T>::operator[] (const Index& index)
{
    if (index.Overflowed())
    {
        throw Exception ("List::operator[] index integer overflow");
    }

    Data* p_Data = ModifyData();

    uintsys u_NumItems = NumItems();
    uintsys u_Index    = 0;

    if (!index.Calculate (u_NumItems, u_Index))
    {
        uintsys u_Extra = index.Shortfall (u_NumItems);

        T item = T();

        ListType list (item, Repeat(u_Extra));

        if (index.IsNegative())
        {
            p_Data->Prepend (list.StealItems_(), u_Extra);

            u_Index = 0;
        }
        else
        {
            p_Data->Append (list.StealItems_(), u_Extra);

            u_Index = u_NumItems + u_Extra - 1;
        }
    }

    return p_Data->SeekToItem(u_Index)->Value();
}

template<typename T>
inline const T List<T>::operator() (const Index& index) const
{
    return operator[] (index);
}

template<typename T>
inline ListRef<T> List<T>::operator() (const Index& index)
{
    return Ref (*this, index);
}

template<typename T>
inline List<T>& List<T>::Erase (ListChangeIter<T>& iter)
{
    if (iter)
    {
        if (IsShared())
        {
            throw Exception ("List: Attempt to Erase shared element");
        }

        Data* p_Data = ModifyData();

        if (p_Data != iter.list_.ViewData())
        {
            throw Exception
                ("List: Attempt to Erase element using an invalid iterator");
        }

        Item* p_ToBeDeleted = iter.p_Item_;

        ++iter;
        --iter.u_Offset_;

        if (p_Data->p_Root_ == p_ToBeDeleted)
        {
            p_Data->RotateLeft();
        }

        if (--p_Data->u_NumItems_ == 0)
        {
            p_Data->p_Root_ = 0;
        }

        GetAllocator_()->DeleteItem (p_ToBeDeleted);
    }

    return *this;
}

template<typename T>
inline const List<T> List<T>::Split (const Index& index)
{
    uintsys u_Index = 0;

    if (index.Calculate (NumItems(), u_Index))
    {
        ListType list_New (ModifyData()->Split (u_Index));

        return list_New;
    }

    return ListType();
}

template<typename T>
inline const List<T> List<T>::Splice (const Index& index)
{
    return Split (index);
}

template<typename T>
inline const List<T> List<T>::Splice (const Index& index, uintsys u_Length)
{
    ListType list_Mid (Split (index));
    ListType list_End (list_Mid.Split (u_Length));

    Append (list_End);

    return list_Mid;
}

template<typename T>
inline const List<T> List<T>::Splice (const Index& index, uintsys u_Length,
                                      const List<T>& list_Replace)
{
    ListType list_Mid (Split (index));
    ListType list_End (list_Mid.Split (u_Length));

    Append (list_Replace);
    Append (list_End);

    return list_Mid;
}

template<typename T>
inline bool List<T>::Delete (const T& item)
{
    ChangeIter iter = Find (item);

    if (iter)
    {
        Erase (iter);

        return true;
    }

    return false;
}

template<typename T>
template<class CMP>
bool List<T>::IsSorted (const CMP& cmp) const
{
    if (NumItems() < 2)
    {
        return true;
    }

    Iter iter1 (*this);
    Iter iter2 (iter1);

    ++iter2;

    while (iter2)
    {
        if (cmp (*iter2, *iter1))
        {
            return false;
        }

        ++iter1;
        ++iter2;
    }

    return true;
}

template<typename T>
inline bool List<T>::IsSorted () const
{
    return IsSorted (Less<T>());
}

template<typename T>
template<class CMP>
inline List<T>& List<T>::Sort (const CMP& cmp)
{
    ModifyData()->Sort (cmp);

    return *this;
}

template<typename T>
inline List<T>& List<T>::Sort ()
{
    return Sort (Less<T>());
}

template<typename T>
inline List<T>& List<T>::Unique ()
{
    ChangeIter iter1 (*this);

    if (iter1)
    {
        ChangeIter iter2 (iter1);

        ++iter2;

        while (iter2)
        {
            if (*iter2 == *iter1)
            {
                Erase (iter2);
            }
            else
            {
                ++iter1;
                ++iter2;
            }
        }
    }

    return *this;
}

template<typename T>
inline const List<T> List<T>::Head (uintsys u_NumItems) const
{
    if (u_NumItems >= NumItems())
    {
        return *this;
    }

    ListType list (*this);

    list.Split (u_NumItems);

    return list;
}

template<typename T>
inline const List<T> List<T>::Tail (uintsys u_NumItems) const
{
    if (u_NumItems >= NumItems())
    {
        return *this;
    }

    ListType list (*this);

    return list.Split (NumItems() - u_NumItems);
}

template<typename T>
inline const ListIter<T> List<T>::Find (const T& item) const
{
    Iter iter (*this);

    while (iter && (item != *iter))
    {
        ++iter;
    }

    return iter;
}

template<typename T>
inline const ListChangeIter<T> List<T>::Find (const T& item)
{
    ChangeIter iter (*this);

    while (iter && (item != *iter))
    {
        ++iter;
    }

    return iter;
}

template<typename T>
inline const List<T>
    List<T>::SubList (const Index& offset, uintsys u_NumItems) const
{
    ListType list;

    Iter iter (*this, offset);

    while (iter && u_NumItems)
    {
        list.Append (*iter);

        ++iter;
        --u_NumItems;
    }

    return list;
}

template<typename T>
inline const T List<T>::Remove (const Index& index)
{
    ListType list (Split (index));

    if (list.IsEmpty())
    {
        return T();
    }

    T item (list.Shift());

    Append (list);

    return item;
}

template<typename T>
inline List<T>& List<T>::Insert (const Index& index, const T& item)
{
    ModifyData()->Insert (index, item);

    return *this;
}

template<typename T>
inline List<T>& List<T>::Insert (const Index& index, ListType list)
{
    Splice (index, 0, list);

    return *this;
}

template<typename T, typename T2>
List<T> MakeList (const T& t1, const T2& t2)
{
    List<T> list (t1);

    list.Append (t2);

    return list;
}

template<typename T, typename T2, typename T3>
List<T> MakeList (const T& t1, const T2& t2, const T3& t3)
{
    List<T> list (t1);

    list.Append (t2);
    list.Append (t3);

    return list;
}

template<typename T, typename T2, typename T3, typename T4>
List<T> MakeList (const T& t1, const T2& t2, const T3& t3, const T4& t4)
{
    List<T> list (t1);

    list.Append (t2);
    list.Append (t3);
    list.Append (t4);

    return list;
}

} // namespace mikestoolbox

