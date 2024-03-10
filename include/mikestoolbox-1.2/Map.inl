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
//  File:       Map.inl
//
//  Synopsis:   Implementation of Map and related classes
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<typename K, typename V, class CMP>
const MapItem<K,V,CMP>* MapItem<K,V,CMP>::Next () const
{
    const Item* p_Item = this;

    if (p_Item->p_Right_ == 0)
    {
        // there are no children to our right, so
        // go back up the chain until we hit a node
        // that is a left child, then choose the
        // parent of this left child

        while ((p_Item != 0) && !p_Item->b_Left_)
        {
            p_Item = p_Item->p_Parent_;
        }

        if (p_Item != 0)
        {
            p_Item = p_Item->p_Parent_;
        }
    }
    else
    {
        // since there are children to our right,
        // the next node is found by traversing to
        // the right child and then following down
        // the path to its leftmost descendant

        p_Item = p_Item->p_Right_;

        while (p_Item->p_Left_ != 0)
        {
            p_Item = p_Item->p_Left_;
        }
    }

    return p_Item;
}

template<typename K, typename V, class CMP>
MapItem<K,V,CMP>* MapItem<K,V,CMP>::Next ()
{
    Item* p_Item = this;

    if (p_Item->p_Right_ == 0)
    {
        // there are no children to our right, so
        // go back up the chain until we hit a node
        // that is a left child, then choose the
        // parent of this left child

        while ((p_Item != 0) && !p_Item->b_Left_)
        {
            p_Item = p_Item->p_Parent_;
        }

        if (p_Item != 0)
        {
            p_Item = p_Item->p_Parent_;
        }
    }
    else
    {
        // since there are children to our right,
        // the next node is found by traversing to
        // the right child and then following down
        // the path to its leftmost descendant

        p_Item = p_Item->p_Right_;

        while (p_Item->p_Left_ != 0)
        {
            p_Item = p_Item->p_Left_;
        }
    }

    return p_Item;
}

template<typename K, typename V, class CMP>
const MapItem<K,V,CMP>* MapItem<K,V,CMP>::Prev () const
{
    const Item* p_Item = this;

    if (p_Item->p_Left_ == 0)
    {
        // there are no children to our left, so
        // go back up the chain until we hit a node
        // that is a right child, then choose the
        // parent of this right child

        while ((p_Item != 0) && p_Item->b_Left_)
        {
            p_Item = p_Item->p_Parent_;
        }

        if (p_Item != 0)
        {
            p_Item = p_Item->p_Parent_;
        }
    }
    else
    {
        // since there are children to our left,
        // the previous node is found by traversing
        // down to the left child and then following
        // down the path to its rightmost descendant

        p_Item = p_Item->p_Left_;

        while (p_Item->p_Right_ != 0)
        {
            p_Item = p_Item->p_Right_;
        }
    }

    return p_Item;
}

template<typename K, typename V, class CMP>
MapItem<K,V,CMP>* MapItem<K,V,CMP>::Prev ()
{
    Item* p_Item = this;

    if (p_Item->p_Left_ == 0)
    {
        // there are no children to our left, so
        // go back up the chain until we hit a node
        // that is a right child, then choose the
        // parent of this right child

        while ((p_Item != 0) && p_Item->b_Left_)
        {
            p_Item = p_Item->p_Parent_;
        }

        if (p_Item != 0)
        {
            p_Item = p_Item->p_Parent_;
        }
    }
    else
    {
        // since there are children to our left,
        // the previous node is found by traversing
        // down to the left child and then following
        // down the path to its rightmost descendant

        p_Item = p_Item->p_Left_;

        while (p_Item->p_Right_ != 0)
        {
            p_Item = p_Item->p_Right_;
        }
    }

    return p_Item;
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::IsRed () const
{
    return b_Red_;
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::IsBlack () const
{
    return !b_Red_;
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::IsLeft () const
{
    return b_Left_;
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::IsRight () const
{
    return !b_Left_;
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* MapItem<K,V,CMP>::Left () const
{
    return p_Left_;
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* MapItem<K,V,CMP>::Right () const
{
    return p_Right_;
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* MapItem<K,V,CMP>::Parent () const
{
    return p_Parent_;
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>* MapItem<K,V,CMP>::Left ()
{
    return p_Left_;
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>* MapItem<K,V,CMP>::Right ()
{
    return p_Right_;
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>* MapItem<K,V,CMP>::Parent ()
{
    return p_Parent_;
}

template<typename K, typename V, class CMP>
inline void MapItem<K,V,CMP>::Redden ()
{
    b_Red_ = true;
}

template<typename K, typename V, class CMP>
inline void MapItem<K,V,CMP>::Blacken ()
{
    b_Red_ = false;
}

template<typename K, typename V, class CMP>
inline MapChangeIter<K,V,CMP>::MapChangeIter (Map<K,V,CMP>& map, const K& key)
    : map_    (map)
    , p_Item_ (map_.FindItem_ (key))
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapChangeIter<K,V,CMP>::MapChangeIter (Map<K,V,CMP>& map)
    : map_    (map)
    , p_Item_ (map_.RootMod_())
{
    if (p_Item_ != 0)
    {
        while (p_Item_->p_Left_ != 0)
        {
            p_Item_ = p_Item_->p_Left_;
        }
    }
}

template<typename K, typename V, class CMP>
inline MapChangeIter<K,V,CMP>::MapChangeIter (Map<K,V,CMP>& map, MapItem<K,V,CMP>* p_Item)
    : map_     (map)
    , p_Item_  (p_Item)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline void MapChangeIter<K,V,CMP>::MoveTo (const K& key)
{
    p_Item_ = map_.FindItem_ (key);
}

template<typename K, typename V, class CMP>
inline MapChangeIter<K,V,CMP>::operator bool () const
{
    return (p_Item_ != 0);
}

template<typename K, typename V, class CMP>
inline const K& MapChangeIter<K,V,CMP>::Key () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("MapChangeIter: Dereferencing invalid iterator");
    }

    return p_Item_->Key();
}

template<typename K, typename V, class CMP>
inline V& MapChangeIter<K,V,CMP>::Value () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("MapChangeIter: Dereferencing invalid iterator");
    }

    return p_Item_->Value();
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>* MapChangeIter<K,V,CMP>::operator-> () const
{
    if (p_Item_ == 0)
    {
        throw Exception ("MapChangeIter: Dereferencing invalid iterator");
    }

    return p_Item_;
}

template<typename K, typename V, class CMP>
MapChangeIter<K,V,CMP>& MapChangeIter<K,V,CMP>::operator++ ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->Next();
    }

    return *this;
}

template<typename K, typename V, class CMP>
inline const MapChangeIter<K,V,CMP> MapChangeIter<K,V,CMP>::operator++ (int)
{
    ChangeIter iter (*this);

    operator++ ();

    return iter;
}

template<typename K, typename V, class CMP>
MapChangeIter<K,V,CMP>& MapChangeIter<K,V,CMP>::operator-- ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->Prev();
    }

    return *this;
}

template<typename K, typename V, class CMP>
inline const MapChangeIter<K,V,CMP> MapChangeIter<K,V,CMP>::operator-- (int)
{
    ChangeIter iter (*this);

    operator-- ();

    return iter;
}

template<typename K, typename V, class CMP>
inline MapChangeIter<K,V,CMP>& MapChangeIter<K,V,CMP>::operator= (const MapChangeIter<K,V,CMP>& iter)
{
    p_Item_ = (map_.ViewData() == iter.map_.ViewData()) ? iter.p_Item_ : 0;

    return *this;
}

template<typename K, typename V, class CMP>
inline bool MapChangeIter<K,V,CMP>::operator== (const MapChangeIter<K,V,CMP>& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapChangeIter<K,V,CMP>::operator!= (const MapChangeIter<K,V,CMP>& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapChangeIter<K,V,CMP>::operator== (const MapIter<K,V,CMP>& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapChangeIter<K,V,CMP>::operator!= (const MapIter<K,V,CMP>& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>::MapIter (const Map<K,V,CMP>& map, const K& key)
    : map_    (map)
    , p_Item_ (map_.FindItem_ (key))
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>::MapIter (const Map<K,V,CMP>& map)
    : map_    (map)
    , p_Item_ (map_.Root_())
{
    if (p_Item_ != 0)
    {
        while (p_Item_->p_Left_ != 0)
        {
            p_Item_ = p_Item_->p_Left_;
        }
    }
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>::MapIter (const Map<K,V,CMP>& map, const MapItem<K,V,CMP>* p_Item)
    : map_    (map)
    , p_Item_ (p_Item)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>::MapIter (const MapChangeIter<K,V,CMP>& iter)
    : map_    (iter.map_)
    , p_Item_ (iter.p_Item_)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline void MapIter<K,V,CMP>::MoveTo (const K& key)
{
    p_Item_ = map_.FindItem_ (key);
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>::operator bool () const
{
    return p_Item_ != 0;
}

template<typename K, typename V, class CMP>
inline const K& MapIter<K,V,CMP>::Key () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Key();
    }

    throw Exception ("MapIter: Dereferencing invalid iterator");
}

template<typename K, typename V, class CMP>
inline const V& MapIter<K,V,CMP>::Value () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Value();
    }

    throw Exception ("MapIter: Dereferencing invalid iterator");
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* MapIter<K,V,CMP>::operator-> () const
{
    if (p_Item_ != 0)
    {
        return p_Item_;
    }

    throw Exception ("MapIter: Dereferencing invalid iterator");
}

template<typename K, typename V, class CMP>
MapIter<K,V,CMP>& MapIter<K,V,CMP>::operator++ ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->Next();
    }

    return *this;
}

template<typename K, typename V, class CMP>
inline const MapIter<K,V,CMP> MapIter<K,V,CMP>::operator++ (int)
{
    Iter iter (*this);

    operator++ ();

    return iter;
}

template<typename K, typename V, class CMP>
MapIter<K,V,CMP>& MapIter<K,V,CMP>::operator-- ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->Prev();
    }

    return *this;
}

template<typename K, typename V, class CMP>
inline const MapIter<K,V,CMP> MapIter<K,V,CMP>::operator-- (int)
{
    Iter iter (*this);

    operator-- ();

    return iter;
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>& MapIter<K,V,CMP>::operator= (const MapChangeIter<K,V,CMP>& iter)
{
    p_Item_ = (map_.ViewData() == iter.map_.ViewData()) ? iter.p_Item_ : 0;

    return *this;
}

template<typename K, typename V, class CMP>
inline MapIter<K,V,CMP>& MapIter<K,V,CMP>::operator= (const MapIter<K,V,CMP>& iter)
{
    p_Item_ = (map_.ViewData() == iter.map_.ViewData()) ? iter.p_Item_ : 0;

    return *this;
}

template<typename K, typename V, class CMP>
inline bool MapIter<K,V,CMP>::operator== (const MapChangeIter<K,V,CMP>& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapIter<K,V,CMP>::operator!= (const MapChangeIter<K,V,CMP>& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapIter<K,V,CMP>::operator== (const MapIter<K,V,CMP>& iter) const
{
    return (p_Item_ == iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool MapIter<K,V,CMP>::operator!= (const MapIter<K,V,CMP>& iter) const
{
    return (p_Item_ != iter.p_Item_);
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>::MapItem (const K& key, const V& value, MapItem<K,V,CMP>* p_Parent)
    : key_      (key)
    , value_    (value)
    , p_Left_   (0)
    , p_Right_  (0)
    , p_Parent_ (p_Parent)
    , b_Red_    (true)
    , b_Left_   (true)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>::MapItem (const MapItem<K,V,CMP>& item)
    : key_      (item.key_)
    , value_    (item.value_)
    , p_Left_   (0)
    , p_Right_  (0)
    , p_Parent_ (0)
    , b_Red_    (item.b_Red_)
    , b_Left_   (item.b_Left_)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>::~MapItem ()
{
    delete p_Left_;
    delete p_Right_;
}

template<typename K, typename V, class CMP>
inline const K& MapItem<K,V,CMP>::Key () const
{
    return key_;
}

template<typename K, typename V, class CMP>
inline const V& MapItem<K,V,CMP>::Value () const
{
    return value_;
}

template<typename K, typename V, class CMP>
inline V& MapItem<K,V,CMP>::Value ()
{
    return value_;
}

template<typename K, typename V, class CMP>
inline void MapItem<K,V,CMP>::SetValue_ (const V& value)
{
    value_ = value;
}

template<typename K, typename V, class CMP>
inline MapRef<K,V,CMP>::MapRef (Map<K,V,CMP>& map, const K& key)
    : map_ (map)
    , key_ (key)
{
    // nothing
}

template<typename K, typename V, class CMP>
void CopyMapTree (MapItem<K,V,CMP>* p_Dest, const MapItem<K,V,CMP>* p_Source)
{
    if (p_Source->p_Left_ != 0)
    {
        p_Dest->p_Left_ = new(std::nothrow) MapItem<K,V,CMP> (*p_Source->p_Left_);

        if (p_Dest->p_Left_ == 0)
        {
            throw Exception ("Map: Out of memory");
        }

        p_Dest->p_Left_->p_Parent_ = p_Dest;

        CopyMapTree (p_Dest->p_Left_, p_Source->p_Left_);
    }

    if (p_Source->p_Right_ != 0)
    {
        p_Dest->p_Right_ = new(std::nothrow) MapItem<K,V,CMP> (*p_Source->p_Right_);

        if (p_Dest->p_Right_ == 0)
        {
            throw Exception ("Map: Out of memory");
        }

        p_Dest->p_Right_->p_Parent_ = p_Dest;

        CopyMapTree (p_Dest->p_Right_, p_Source->p_Right_);
    }
}

template<typename K, typename V, class CMP>
inline MapStorage<K,V,CMP>::MapStorage (const MapStorage<K,V,CMP>& storage)
    : p_Root_       (0)
    , u_NumItems_   (storage.u_NumItems_)
{
    if (u_NumItems_ != 0)
    {
        PointerHolder<Item> root (new(std::nothrow) Item (*storage.p_Root_));

        if (root.IsNull())
        {
            throw Exception ("Map: Out of memory");
        }

        p_Root_ = root;

        CopyMapTree (p_Root_, storage.p_Root_);

        root.Release();
    }
}

template<typename K, typename V, class CMP>
inline MapStorage<K,V,CMP>::MapStorage ()
    : p_Root_     (0)
    , u_NumItems_ (0)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline MapStorage<K,V,CMP>::~MapStorage ()
{
    delete p_Root_;
}

template<typename K, typename V, class CMP>
StringList MapStorage<K,V,CMP>::Debug () const
{
    StringList strl_Info;
    String     str_Line;

    str_Line  = "   Key Type:          ";
    str_Line += Typename<K>();
    str_Line += '\n';

    strl_Info.Append (str_Line);

    str_Line  = "   Value Type:        ";
    str_Line += Typename<V>();
    str_Line += '\n';

    strl_Info.Append (str_Line);

    str_Line  = "   Comparison Type:   ";
    str_Line += Typename<CMP>();
    str_Line += '\n';

    strl_Info.Append (str_Line);

    str_Line  = "   Number of Items:   ";
    str_Line += u_NumItems_;
    str_Line += '\n';

    strl_Info.Append (str_Line);

    return strl_Info;
}

template<typename K, typename V, class CMP>
inline Map<K,V,CMP>::Map ()
    : SharedResource (RESOURCE_COPY_ON_WRITE, new(std::nothrow) Storage)
{
    // nothing
}

template<typename K, typename V, class CMP>
inline const MapStorage<K,V,CMP>* Map<K,V,CMP>::ViewData () const
{
    return (const Storage*) SharedResource::ViewData();
}

template<typename K, typename V, class CMP>
inline MapStorage<K,V,CMP>* Map<K,V,CMP>::ModifyData ()
{
    return (Storage*) SharedResource::ModifyData();
}

template<typename K, typename V, class CMP>
inline MapStorage<K,V,CMP>* Map<K,V,CMP>::MakeCopyOfSharedData_ (const SharedData* p_Data) const
{
    const Storage* p_OldData = (const Storage*) p_Data;
          Storage* p_NewData = new(std::nothrow) Storage (*p_OldData);

    if (p_NewData == 0)
    {
        throw Exception ("Map: Out of memory");
    }

    return p_NewData;
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>*& Map<K,V,CMP>::RootMod_ ()
{
    return ModifyData()->p_Root_;
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* Map<K,V,CMP>::Root_ () const
{
    return ViewData()->p_Root_;
}

template<typename K, typename V, class CMP>
inline uintsys& Map<K,V,CMP>::NumItemsMod_ ()
{
    return ModifyData()->u_NumItems_;
}

template<typename K, typename V, class CMP>
inline uintsys Map<K,V,CMP>::NumItems () const
{
    return ViewData()->u_NumItems_;
}

template<typename K, typename V, class CMP>
inline const MapItem<K,V,CMP>* Map<K,V,CMP>::ParentOf_ (const MapItem<K,V,CMP>* p_Item)
{
    return p_Item->Parent();
}

template<typename K, typename V, class CMP>
inline MapItem<K,V,CMP>*& Map<K,V,CMP>::ParentOf_ (MapItem<K,V,CMP>* p_Item)
{
    return p_Item->p_Parent_;
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::IsBlack_ (const MapItem<K,V,CMP>* p_Item)
{
    return (p_Item == 0) || p_Item->IsBlack();
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::IsRed_ (const MapItem<K,V,CMP>* p_Item)
{
    return p_Item && p_Item->IsRed();
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Blacken_ (MapItem<K,V,CMP>* p_Item)
{
    if (p_Item) p_Item->Blacken();
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Redden_ (MapItem<K,V,CMP>* p_Item)
{
    if (p_Item) p_Item->Redden();
}

template<typename K, typename V, class CMP>
const MapItem<K,V,CMP>* Map<K,V,CMP>::FindItem_ (const K& key) const
{
    const Item* p_Item = Root_();

    CMP cmp;

    while (p_Item != 0)
    {
        if (cmp (key, p_Item->Key()))
        {
            p_Item = p_Item->Left();
        }
        else if (cmp (p_Item->Key(), key))
        {
            p_Item = p_Item->Right();
        }
        else
        {
            break;
        }
    }

    return p_Item;
}

template<typename K, typename V, class CMP>
MapItem<K,V,CMP>* Map<K,V,CMP>::FindItem_ (const K& key)
{
    Item* p_Item = RootMod_();

    CMP cmp;

    while (p_Item != 0)
    {
        if (cmp (key, p_Item->Key()))
        {
            p_Item = p_Item->Left();
        }
        else if (cmp (p_Item->Key(), key))
        {
            p_Item = p_Item->Right();
        }
        else
        {
            break;
        }
    }

    return p_Item;
}

// LeftRotate_, RightRotate_, InsertFixUp_, DeleteFixUp_, CreateItem_, and Delete
// based on algorithms from "Introduction to Algorithms" Second Edition

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::LeftRotate_ (MapItem<K,V,CMP>* p_Item)
{
    Item* p_NewParent = p_Item->p_Right_;

    if (p_NewParent != 0)
    {
        p_Item->p_Right_ = p_NewParent->p_Left_;

        if (p_Item->p_Right_ != 0)
        {
            ParentOf_ (p_Item->p_Right_) = p_Item;
            p_Item->p_Right_->b_Left_    = false;
        }

        ParentOf_ (p_NewParent) = ParentOf_ (p_Item);

        if (ParentOf_ (p_NewParent) == 0)
        {
            RootMod_() = p_NewParent;
        }
        else if (p_Item->b_Left_)
        {
            ParentOf_ (p_NewParent)->p_Left_ = p_NewParent;
            p_NewParent->b_Left_             = true;
        }
        else
        {
            ParentOf_ (p_NewParent)->p_Right_ = p_NewParent;
            p_NewParent->b_Left_              = false;
        }

        p_NewParent->p_Left_ = p_Item;

        ParentOf_ (p_Item) = p_NewParent;
        p_Item->b_Left_    = true;
    }
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::RightRotate_ (MapItem<K,V,CMP>* p_Item)
{
    Item* p_NewParent = p_Item->p_Left_;

    if (p_NewParent != 0)
    {
        p_Item->p_Left_ = p_NewParent->p_Right_;

        if (p_Item->p_Left_ != 0)
        {
            p_Item->p_Left_->p_Parent_ = p_Item;
            p_Item->p_Left_->b_Left_   = true;
        }

        ParentOf_ (p_NewParent) = ParentOf_ (p_Item);

        if (ParentOf_ (p_NewParent) == 0)
        {
            RootMod_() = p_NewParent;
        }
        else if (p_Item->b_Left_)
        {
            p_NewParent->p_Parent_->p_Left_ = p_NewParent;
            p_NewParent->b_Left_            = true;
        }
        else
        {
            p_NewParent->p_Parent_->p_Right_ = p_NewParent;
            p_NewParent->b_Left_             = false;
        }

        p_NewParent->p_Right_ = p_Item;

        ParentOf_ (p_Item) = p_NewParent;
        p_Item->b_Left_    = false;
    }
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::InsertFixUp_ (MapItem<K,V,CMP>* p_Item)
{
    Item* p_Parent = ParentOf_ (p_Item);

    while (IsRed_ (p_Parent))
    {
        Item* p_Uncle;

        if (p_Parent->IsLeft())
        {
            p_Uncle = p_Parent->Parent()->Right();

            if (IsRed_ (p_Uncle))
            {
                Blacken_ (p_Parent);
                Blacken_ (p_Uncle);
                Redden_  (ParentOf_ (p_Parent));

                p_Item   = ParentOf_ (p_Parent);
                p_Parent = ParentOf_ (p_Item);
            }
            else
            {
                if (p_Item->IsRight())
                {
                    p_Item = p_Parent;

                    LeftRotate_ (p_Item);

                    p_Parent = ParentOf_ (p_Item);
                }

                Blacken_     (p_Parent);
                Redden_      (ParentOf_ (p_Parent));
                RightRotate_ (ParentOf_ (p_Parent));
            }
        }
        else
        {
            p_Uncle = p_Parent->Parent()->Left();

            if (IsRed_ (p_Uncle))
            {
                Blacken_ (p_Parent);
                Blacken_ (p_Uncle);
                Redden_  (ParentOf_ (p_Parent));

                p_Item   = ParentOf_ (p_Parent);
                p_Parent = ParentOf_ (p_Item);
            }
            else
            {
                if (p_Item->IsLeft())
                {
                    p_Item = p_Parent;

                    RightRotate_ (p_Item);

                    p_Parent = ParentOf_ (p_Item);
                }

                Blacken_    (p_Parent);
                Redden_     (ParentOf_ (p_Parent));
                LeftRotate_ (ParentOf_ (p_Parent));
            }
        }
    }

    Blacken_ (RootMod_());
}

template<typename K, typename V, class CMP>
MapItem<K,V,CMP>* Map<K,V,CMP>::CreateItem_ (const K& key, const V& value)
{
    Storage* p_Storage = ModifyData();

    if (p_Storage->p_Root_ == 0)
    {
        p_Storage->p_Root_ = new(std::nothrow) Item (key, value, 0);

        if (p_Storage->p_Root_ == 0)
        {
            throw Exception ("Map: Out of memory");
        }

        Blacken_ (p_Storage->p_Root_);

        p_Storage->u_NumItems_ = 1;

        return p_Storage->p_Root_;
    }

    Item* p_Item = p_Storage->p_Root_;

    CMP cmp;

    for (;;)
    {
        if (cmp (key, p_Item->Key()))
        {
            if (p_Item->Left() == 0)
            {
                p_Item->p_Left_ = new(std::nothrow) Item (key, value, p_Item);

                if (p_Item->Left() == 0)
                {
                    throw Exception ("Map: Out of Memory");
                }

                p_Item = p_Item->Left();

                p_Item->b_Left_ = true;

                ++p_Storage->u_NumItems_;

                InsertFixUp_ (p_Item);

                return p_Item;
            }
            else
            {
                p_Item = p_Item->Left();
            }
        }
        else if (cmp (p_Item->Key(), key))
        {
            if (p_Item->Right() == 0)
            {
                p_Item->p_Right_ = new(std::nothrow) Item (key, value, p_Item);

                if (p_Item->Right() == 0)
                {
                    throw Exception ("Map: Out of Memory");
                }

                p_Item = p_Item->Right();

                p_Item->b_Left_ = false;

                ++p_Storage->u_NumItems_;

                InsertFixUp_ (p_Item);

                return p_Item;
            }
            else
            {
                p_Item = p_Item->Right();
            }
        }
        else
        {
            p_Item->value_ = value;

            return p_Item;
        }
    }
}

template<typename K, typename V, class CMP>
inline const MapIter<K,V,CMP> Map<K,V,CMP>::Begin () const
{
    return Iter (*this);
}

template<typename K, typename V, class CMP>
inline const MapChangeIter<K,V,CMP> Map<K,V,CMP>::Begin ()
{
    return ChangeIter (*this);
}

template<typename K, typename V, class CMP>
inline const MapIter<K,V,CMP> Map<K,V,CMP>::End () const
{
    const Item* p_Item = Root_();

    if (p_Item != 0)
    {
        while (p_Item->Right() != 0)
        {
            p_Item = p_Item->Right();
        }
    }

    return Iter (*this, p_Item);
}

template<typename K, typename V, class CMP>
inline const MapChangeIter<K,V,CMP> Map<K,V,CMP>::End ()
{
    Item* p_Item = RootMod_();

    if (p_Item != 0)
    {
        while (p_Item->Right() != 0)
        {
            p_Item = p_Item->Right();
        }
    }

    return ChangeIter (*this, p_Item);
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Erase (MapChangeIter<K,V,CMP>& iter)
{
    if (IsShared())
    {
        throw Exception ("Map: Attempt to Erase shared element");
    }

    if (!iter)
    {
        return;
    }

    if (ViewData() != iter.map_.ViewData())
    {
        throw Exception ("Map: Attempt to erase element with invalid iterator");
    }

    Delete_ (iter++.p_Item_);
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::Exists (const K& key) const
{
    return (FindItem_ (key) != 0);
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::Find (const K& key, V& value) const
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item != 0)
    {
        value = p_Item->Value();

        return true;
    }

    return false;
}

template<typename K, typename V, class CMP>
inline const MapIter<K,V,CMP> Map<K,V,CMP>::Find (const K& key) const
{
    const Item* p_Item = FindItem_ (key);

    return Iter (*this, p_Item);
}

template<typename K, typename V, class CMP>
inline const MapChangeIter<K,V,CMP> Map<K,V,CMP>::Find (const K& key)
{
    Item* p_Item = FindItem_ (key);

    return ChangeIter (*this, p_Item);
}

template<typename K, typename V, class CMP>
inline const V Map<K,V,CMP>::Get (const K& key) const
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item != 0)
    {
        return p_Item->Value();
    }

    return V();
}

template<typename K, typename V, class CMP>
inline const V Map<K,V,CMP>::Get (const K& key, const V& v_Default)
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item != 0)
    {
        return p_Item->Value();
    }

    Set (key, v_Default);

    return v_Default;
}

template<typename T>
void foo (const T& t)
{
    std::cout << t << std::endl;
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Set (const K& key, const V& value)
{
    CreateItem_ (key, value);
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Swap (Map<K,V,CMP>& map)
{
    SharedResource::Swap (map);
}

template<typename K, typename V, class CMP>
inline const V Map<K,V,CMP>::operator[] (const K& key) const
{
    return Get (key);
}

template<typename K, typename V, class CMP>
inline V& Map<K,V,CMP>::operator[] (const K& key)
{
    Item* p_Item = FindItem_ (key);

    if (p_Item == 0)
    {
        p_Item = CreateItem_ (key, V());
    }

    return p_Item->Value();
}

template<typename K, typename V, class CMP>
inline const V Map<K,V,CMP>::operator() (const K& key) const
{
    return Get (key);
}

template<typename K, typename V, class CMP>
inline MapRef<K,V,CMP> Map<K,V,CMP>::operator() (const K& key)
{
    return Ref (*this, key);
}

template<typename K, typename V, class CMP>
inline MapRef<K,V,CMP>::operator const V () const
{
    return map_.Get (key_);
}

template<typename K, typename V, class CMP>
inline V* MapRef<K,V,CMP>::operator-> ()
{
    return &map_[key_];
}

template<typename K, typename V, class CMP>
inline MapRef<K,V,CMP>& MapRef<K,V,CMP>::operator= (const V& value)
{
    map_.Set (key_, value);

    return *this;
}

template<typename K, typename V, class CMP>
inline MapRef<K,V,CMP>& MapRef<K,V,CMP>::operator= (const MapRef<K,V,CMP>& ref)
{
    map_.Set (key_, ref);

    return *this;
}

template<typename K, typename V, class CMP>
inline bool MapRef<K,V,CMP>::operator== (const V& value) const
{
    return (map_.Get (key_) == value);
}

template<typename K, typename V, class CMP>
inline bool MapRef<K,V,CMP>::operator!= (const V& value) const
{
    return !operator== (value);
}

template<typename K, typename V, class CMP>
template<typename U>
inline bool MapRef<K,V,CMP>::operator== (const U& value) const
{
    return (map_.Get (key_) == V(value));
}

template<typename K, typename V, class CMP>
template<typename U>
inline bool MapRef<K,V,CMP>::operator!= (const U& value) const
{
    return !operator== (value);
}

template<typename K, typename V, class CMP>
inline void Map<K,V,CMP>::Clear ()
{
    MapType map;

    Swap (map);
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::IsEmpty () const
{
    return (NumItems() == 0);
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::DeleteFixUpLeft_ (MapItem<K,V,CMP>* p_Root, MapItem<K,V,CMP>*& p_Item,
                                     MapItem<K,V,CMP>*& p_Parent)
{
    Item* p_Sibling = p_Parent->Right();

    if (IsRed_(p_Sibling))
    {
        Blacken_    (p_Sibling);
        Redden_     (p_Parent);
        LeftRotate_ (p_Parent);

        p_Sibling = p_Parent->Right();
    }

    if (IsBlack_(p_Sibling->Left()) && IsBlack_(p_Sibling->Right()))
    {
        Redden_ (p_Sibling);

        p_Item = p_Parent;

        p_Parent = ParentOf_ (p_Item);
    }
    else
    {
        if (IsBlack_(p_Sibling->Right()))
        {
            Blacken_     (p_Sibling->Left());
            Redden_      (p_Sibling);
            RightRotate_ (p_Sibling);

            p_Sibling = p_Parent->Right();
        }

        p_Sibling->b_Red_ = p_Parent->b_Red_;

        Blacken_    (p_Parent);
        Blacken_    (p_Sibling->Right());
        LeftRotate_ (p_Parent);

        p_Item = p_Root;
    }
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::DeleteFixUpRight_ (MapItem<K,V,CMP>* p_Root, MapItem<K,V,CMP>*& p_Item,
                                      MapItem<K,V,CMP>*& p_Parent)
{
    Item* p_Sibling = p_Parent->Left();

    if (IsRed_ (p_Sibling))
    {
        Blacken_     (p_Sibling);
        Redden_      (p_Parent);
        RightRotate_ (p_Parent);

        p_Sibling = p_Parent->Left();
    }

    if (IsBlack_(p_Sibling->Left()) && IsBlack_(p_Sibling->Right()))
    {
        Redden_ (p_Sibling);

        p_Item = p_Parent;

        p_Parent = ParentOf_ (p_Item);
    }
    else
    {
        if (IsBlack_ (p_Sibling->Left()))
        {
            Blacken_    (p_Sibling->Right());
            Redden_     (p_Sibling);
            LeftRotate_ (p_Sibling);

            p_Sibling = p_Parent->Left();
        }

        p_Sibling->b_Red_ = p_Parent->b_Red_;

        Blacken_     (p_Parent);
        Blacken_     (p_Sibling->Left());
        RightRotate_ (p_Parent);

        p_Item = p_Root;
    }
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::DeleteFixUp_ (MapItem<K,V,CMP>* p_Item, MapItem<K,V,CMP>* p_Parent)
{
    Item* p_Root = RootMod_();

    while (IsBlack_(p_Item) && (p_Item != p_Root))
    {
        if (p_Item ? p_Item->IsLeft() : p_Parent->Left() == 0)
        {
            DeleteFixUpLeft_  (p_Root, p_Item, p_Parent);
        }
        else
        {
            DeleteFixUpRight_ (p_Root, p_Item, p_Parent);
        }
    }

    Blacken_ (p_Item);
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::Delete_ (MapItem<K,V,CMP>* p_Item)
{
    if (p_Item != 0)
    {
        if (NumItems() == 1)
        {
            delete p_Item;

            RootMod_() = 0;

            NumItemsMod_() = 0;

            return;
        }

        Item* p_ToBeDeleted = p_Item;
        Item* p_ToBeKept;
        Item* p_Parent;

        if ((p_Item->Left() != 0) && (p_Item->Right() != 0))
        {
            ChangeIter iter (*this, p_Item);

            ++iter;

            p_ToBeDeleted = (Item*) iter.p_Item_;
        }

        if (p_ToBeDeleted->Left() == 0)
        {
            p_ToBeKept = p_ToBeDeleted->Right();
        }
        else
        {
            p_ToBeKept = p_ToBeDeleted->Left();
        }

        p_Parent = ParentOf_ (p_ToBeDeleted);

        if (p_ToBeKept != 0)
        {
            ParentOf_ (p_ToBeKept) = ParentOf_ (p_ToBeDeleted);
            p_ToBeKept->b_Left_    = p_ToBeDeleted->b_Left_;

            if (ParentOf_ (p_ToBeKept) == 0)
            {
                RootMod_() = p_ToBeKept;
            }
            else if (p_ToBeKept->b_Left_)
            {
                ParentOf_ (p_ToBeKept)->p_Left_ = p_ToBeKept;
            }
            else
            {
                ParentOf_ (p_ToBeKept)->p_Right_ = p_ToBeKept;
            }
        }
        else if (p_ToBeDeleted->b_Left_)
        {
            ParentOf_ (p_ToBeDeleted)->p_Left_ = 0;
        }
        else
        {
            ParentOf_ (p_ToBeDeleted)->p_Right_ = 0;
        }

        if (p_Item != p_ToBeDeleted)
        {
            p_Item->key_   = p_ToBeDeleted->key_;
            p_Item->value_ = p_ToBeDeleted->value_;
        }

        --NumItemsMod_();

        if (IsBlack_ (p_ToBeDeleted))
        {
            DeleteFixUp_ (p_ToBeKept, p_Parent);
        }

        p_ToBeDeleted->p_Left_  = 0;
        p_ToBeDeleted->p_Right_ = 0;

        delete p_ToBeDeleted;
    }
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::Delete (const K& key)
{
    Item* p_Item = FindItem_ (key);

    Delete_ (p_Item);

    return (p_Item != 0);
}

template<typename K, typename V, class CMP>
uintsys Map<K,V,CMP>::Delete (const List<K>& list_Keys)
{
    uintsys u_Count = 0;

    ListIter<K> iter (list_Keys);

    while (iter)
    {
        if (Delete (*iter))
        {
            ++u_Count;
        }

        ++iter;
    }

    return u_Count;
}

template<typename K, typename V, class CMP>
const List<K> Map<K,V,CMP>::Keys () const
{
    List<K> list_Keys;

    Iter iter (*this);

    while (iter)
    {
        list_Keys.Append (iter.Key());

        ++iter;
    }

    return list_Keys;
}

template<typename K, typename V, class CMP>
inline const List<K> Map<K,V,CMP>::SortedKeys () const
{
    return Keys();
}

template<typename K, typename V, class CMP>
const List<V> Map<K,V,CMP>::Values () const
{
    List<V> list_Values;

    Iter iter (*this);

    while (iter)
    {
        list_Values.Append (iter.Value());

        ++iter;
    }

    return list_Values;
}

template<typename K, typename V, class CMP>
void Map<K,V,CMP>::AddKeyValue_ (MapType& map, const K& key, const V& value, ImportOptions options)
{
    if (options.CanCreate())
    {
        if (options.CanReplace())
        {
            map.Set (key, value);
        }
        else
        {
            map.Get (key, value);
        }
    }
    else if (options.CanReplace() && map.Exists (key))
    {
        map.Set (key, value);
    }
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
void Map<K,V,CMP>::Import_ (const CONTAINER& c, ImportOptions options, MikesToolboxSimpleContainer)
{
    if (!c.IsEmpty())
    {
        MapType map (*this);

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            K key (*iter);

            if (++iter)
            {
                V value (*iter);

                AddKeyValue_ (map, key, value, options);

                ++iter;
            }
        }

        Swap (map);
    }
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
void Map<K,V,CMP>::Import_ (const CONTAINER& c, ImportOptions options, MikesToolboxKeyValueContainer)
{
    if (!c.IsEmpty())
    {
        MapType map (*this);

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            K key   (iter->Key());
            V value (iter->Value());

            AddKeyValue_ (map, key, value, options);

            ++iter;
        }

        Swap (map);
    }
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
void Map<K,V,CMP>::Import_ (const CONTAINER& c, ImportOptions options, StandardCPlusPlusSimpleContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    MapType map (*this);

    while (iter1 != iter2)
    {
        K key (*iter1);

        if (++iter1 != iter2)
        {
            V value (*iter1);

            AddKeyValue_ (map, key, value, options);

            ++iter1;
        }
    }

    Swap (map);
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
void Map<K,V,CMP>::Import_ (const CONTAINER& c, ImportOptions options, StandardCPlusPlusKeyValueContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    MapType map (*this);

    while (iter1 != iter2)
    {
        K key   (iter1->first);
        V value (iter1->second);

        AddKeyValue_ (map, key, value, options);

        ++iter1;
    }

    Swap (map);
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
inline void Map<K,V,CMP>::Import_ (const CONTAINER&, ImportOptions, UnknownTypeOfContainer)
{
    // nothing good can come from doing anything here

    throw Exception ("Map::Import: Unknown container type");
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
inline void Map<K,V,CMP>::Import (const CONTAINER& c, ImportOptions options)
{
    Import_ (c, options, typename ContainerType<CONTAINER>::Type());
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
inline void Map<K,V,CMP>::Import (const CONTAINER& c)
{
    Import (c, ImportOptions());
}

template<typename K, typename V, class CMP>
template<typename CONTAINER>
inline Map<K,V,CMP>::Map (const CONTAINER& c)
    : SharedResource (RESOURCE_COPY_ON_WRITE, new(std::nothrow) Storage)
{
    Import (c);
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
inline Map<K,V,CMP>& Map<K,V,CMP>::operator+= (const CONTAINER& c)
{
    Import (c);

    return *this;
}

template<typename K, typename V, class CMP>
template<class CONTAINER>
inline Map<K,V,CMP>& Map<K,V,CMP>::operator= (const CONTAINER& c)
{
    MapType map (c);

    Swap (map);

    return *this;
}

template<typename K, typename V, class CMP>
bool Map<K,V,CMP>::operator== (const Map<K,V,CMP>& map) const
{
    const void* p_Storage1 = ViewData();
    const void* p_Storage2 = map.ViewData();

    if (p_Storage1 == p_Storage2)
    {
        return true;
    }

    uintsys u_NumItemsThis = NumItems();
    uintsys u_NumItemsThat = map.NumItems();

    if (u_NumItemsThis != u_NumItemsThat)
    {
        return false;
    }

    Iter iter1 (*this);
    Iter iter2 (map);

    while (iter1 && iter2)
    {
        const K& key1 = iter1->Key();
        const K& key2 = iter2->Key();

        if (!(key1 == key2))
        {
            return false;
        }

        const V& value1 = iter1->Value();
        const V& value2 = iter2->Value();

        if (!(value1 == value2))
        {
            return false;
        }

        ++iter1;
        ++iter2;
    }

    // sanity check

    if (iter1 || iter2)
    {
        return false;
    }

    return true;
}

template<typename K, typename V, class CMP>
inline bool Map<K,V,CMP>::operator!= (const Map<K,V,CMP>& map) const
{
    return !operator== (map);
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::Check () const
{
    return (!p_Left_  ||  p_Left_->b_Left_)   &&            // structural
           (!p_Right_ || !p_Right_->b_Left_)  &&            // structural
           (!b_Red_   || !p_Parent_ || !p_Parent_->b_Red_); // proper colors
}

template<typename K, typename V, class CMP>
inline bool MapItem<K,V,CMP>::IsChild () const
{
    return (!p_Left_ && !p_Right_);
}

template<typename K, typename V, class CMP>
void MapItem<K,V,CMP>::GetHeight (uintsys& u_Height, uintsys& u_BlackHeight) const
{
    u_Height      = 0;
    u_BlackHeight = 0;

    const Item* p_Item = this;

    while (p_Item)
    {
        ++u_Height;

        if (p_Item->IsBlack())
        {
            ++u_BlackHeight;
        }

        p_Item = p_Item->p_Parent_;
    }
}

template<typename K, typename V, class CMP>
const StringList Map<K,V,CMP>::Debug () const       // NO_METRIC
{
    Iter iter (*this);

    uintsys u_MinHeight      = MAX_UINTSYS;
    uintsys u_MaxHeight      = 0;
    uintsys u_MinBlackHeight = MAX_UINTSYS;
    uintsys u_MaxBlackHeight = 0;
    uintsys u_Violations     = 0;

    while (iter)
    {
        const Item* p_Item = iter++.p_Item_;

        if (!p_Item->Check())
        {
            ++u_Violations;
        }

        if (p_Item->IsChild())
        {
            uintsys u_Height      = 0;
            uintsys u_BlackHeight = 0;

            p_Item->GetHeight (u_Height, u_BlackHeight);

            u_MinHeight = Minimum (u_MinHeight, u_Height);
            u_MaxHeight = Maximum (u_MaxHeight, u_Height);

            u_MinBlackHeight = Minimum (u_MinBlackHeight, u_BlackHeight);
            u_MaxBlackHeight = Maximum (u_MaxBlackHeight, u_BlackHeight);
        }
    }

    StringList strl_Info;
    String     str_Line;

    str_Line = "Map:\n";

    strl_Info.Append (str_Line);

    strl_Info.Append (ViewData()->Debug ());

    if (!IsEmpty())
    {
        str_Line  = "   Tree Height:       ";
        str_Line += u_MinHeight;

        if (u_MaxHeight != u_MinHeight)
        {
            str_Line += "..";
            str_Line += u_MaxHeight;
        }

        str_Line += '\n';

        strl_Info.Append (str_Line);

        str_Line  = "   Black Height:      ";
        str_Line += u_MinBlackHeight;

        if (u_MaxBlackHeight != u_MinBlackHeight)
        {
            str_Line += "..";
            str_Line += u_MaxBlackHeight;
        }

        str_Line += '\n';

        strl_Info.Append (str_Line);

        str_Line  = "   Violations:        ";
        str_Line += u_Violations;
        str_Line += '\n';

        strl_Info.Append (str_Line);
    }

    return strl_Info;
}

template<typename K, typename V, class CMP>
inline void swap (Map<K,V,CMP>& map1, Map<K,V,CMP>& map2)
{
    map1.Swap (map2);
}

} // namespace mikestoolbox

