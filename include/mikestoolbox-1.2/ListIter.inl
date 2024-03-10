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
//  File:       ListIter.inl
//
//  Synopsis:   Methods of ListIter<T> and ListChangeIter<T>
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<typename T>
inline ListChangeIter<T>::ListChangeIter (List<T>& list, const Index& offset)
    : list_     (list)
    , p_Item_   (0)
    , u_Offset_ (0)
{
    Data* p_Data = list_.ModifyData();

    uintsys u_NumItems = p_Data->NumItems();

    if (offset.Calculate (u_NumItems, u_Offset_))
    {
        p_Item_ = p_Data->SeekToItem (u_Offset_);
    }
    else
    {
        u_Offset_ = u_NumItems;
    }
}

template<typename T>
inline ListIter<T>::ListIter (const List<T>& list, const Index& offset)
    : list_     (list)
    , p_Item_   (0)
    , u_Offset_ (0)
{
    const Data* p_Data = list_.ViewData();

    uintsys u_NumItems = p_Data->NumItems();

    if (offset.Calculate (u_NumItems, u_Offset_))
    {
        p_Item_ = p_Data->SeekToItem (u_Offset_);
    }
    else
    {
        u_Offset_ = u_NumItems;
    }
}

template<typename T>
inline void ListChangeIter<T>::MoveTo (const Index& offset)
{
    Data* p_Data = list_.ModifyData();

    uintsys u_NumItems = p_Data->NumItems();

    if (offset.Calculate (u_NumItems, u_Offset_))
    {
        p_Item_ = p_Data->SeekToItem (u_Offset_);
    }
    else
    {
        p_Item_   = 0;
        u_Offset_ = u_NumItems;
    }
}

template<typename T>
inline void ListIter<T>::MoveTo (const Index& offset)
{
    const Data* p_Data = list_.ViewData();

    uintsys u_NumItems = p_Data->NumItems();

    if (offset.Calculate (u_NumItems, u_Offset_))
    {
        p_Item_ = p_Data->SeekToItem (u_Offset_);
    }
    else
    {
        p_Item_   = 0;
        u_Offset_ = u_NumItems;
    }
}

template<typename T>
inline uintsys ListChangeIter<T>::Offset () const
{
    return u_Offset_;
}

template<typename T>
inline uintsys ListIter<T>::Offset () const
{
    return u_Offset_;
}

template<typename T>
inline ListChangeIter<T>::operator bool () const
{
    return p_Item_ != 0;
}

template<typename T>
inline ListIter<T>::operator bool () const
{
    return p_Item_ != 0;
}

template<typename T>
inline bool ListChangeIter<T>::operator== (const ChangeIter& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename T>
inline bool ListIter<T>::operator== (const Iter& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename T>
inline bool ListChangeIter<T>::operator!= (const ChangeIter& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename T>
inline bool ListIter<T>::operator!= (const Iter& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename T>
inline T& ListChangeIter<T>::operator* () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("ListChangeIter: Dereferencing invalid iterator");
    }

    return p_Item_->t_;
}

template<typename T>
inline const T& ListIter<T>::operator* () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("ListIter: Dereferencing invalid iterator");
    }

    return p_Item_->t_;
}

template<typename T>
inline T* ListChangeIter<T>::operator-> () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("ListChangeIter: Dereferencing invalid iterator");
    }

    return &p_Item_->t_;
}

template<typename T>
inline const T* ListIter<T>::operator-> () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("ListIter: Dereferencing invalid iterator");
    }

    return &p_Item_->t_;
}

template<typename T>
inline ListChangeIter<T>& ListChangeIter<T>::operator++ ()
{
    if (p_Item_ != 0)
    {
        uintsys u_NumItems = list_.NumItems();

        if (++u_Offset_ < u_NumItems)
        {
            p_Item_ = p_Item_->p_Next_;
        }
        else
        {
            p_Item_   = 0;
            u_Offset_ = u_NumItems;
        }
    }

    return *this;
}

template<typename T>
inline ListIter<T>& ListIter<T>::operator++ ()
{
    if (p_Item_ != 0)
    {
        uintsys u_NumItems = list_.NumItems();

        if (++u_Offset_ < u_NumItems)
        {
            p_Item_ = p_Item_->p_Next_;
        }
        else
        {
            p_Item_   = 0;
            u_Offset_ = u_NumItems;
        }
    }

    return *this;
}

template<typename T>
inline ListChangeIter<T>& ListChangeIter<T>::operator-- ()
{
    if (p_Item_ != 0)
    {
        if (u_Offset_ == 0)
        {
            p_Item_   = 0;
            u_Offset_ = list_.NumItems();
        }
        else
        {
            p_Item_ = p_Item_->p_Prev_;
            --u_Offset_;
        }
    }

    return *this;
}

template<typename T>
inline ListIter<T>& ListIter<T>::operator-- ()
{
    if (p_Item_ != 0)
    {
        if (u_Offset_ == 0)
        {
            p_Item_   = 0;
            u_Offset_ = list_.NumItems();
        }
        else
        {
            p_Item_ = p_Item_->p_Prev_;
            --u_Offset_;
        }
    }

    return *this;
}

template<typename T>
inline const ListChangeIter<T> ListChangeIter<T>::operator++ (int)
{
    ChangeIter iter (*this);

    operator++();

    return iter;
}

template<typename T>
inline const ListIter<T> ListIter<T>::operator++ (int)
{
    Iter iter (*this);

    operator++();

    return iter;
}

template<typename T>
inline const ListChangeIter<T> ListChangeIter<T>::operator-- (int)
{
    ChangeIter iter (*this);

    operator--();

    return iter;
}

template<typename T>
inline const ListIter<T> ListIter<T>::operator-- (int)
{
    Iter iter (*this);

    operator--();

    return iter;
}

template<typename T>
inline ListChangeIter<T>& ListChangeIter<T>::operator= (const ChangeIter& iter)
{
    if (list_.ViewData() != iter.list_.ViewData())
    {
        throw Exception ("ListChangeIter: Attempt to point into different List");
    }

    p_Item_   = iter.p_Item_;
    u_Offset_ = iter.u_Offset_;

    return *this;
}

template<typename T>
inline ListIter<T>& ListIter<T>::operator= (const ListType& list)
{
    Iter iter (list);

    return operator= (iter);
}

template<typename T>
inline ListIter<T>& ListIter<T>::operator= (const Iter& iter)
{
    if (list_.ViewData() != iter.list_.ViewData())
    {
        throw Exception ("ListIter: Attempt to point into different List");
    }

    p_Item_   = iter.p_Item_;
    u_Offset_ = iter.u_Offset_;

    return *this;
}

template<typename T>
inline ListIter<T>& ListIter<T>::operator= (const ChangeIter& iter)
{
    if (list_.ViewData() != iter.list_.ViewData())
    {
        throw Exception ("ListIter: Attempt to point into different List");
    }

    p_Item_   = iter.p_Item_;
    u_Offset_ = iter.u_Offset_;

    return *this;
}

} // namespace mikestoolbox

