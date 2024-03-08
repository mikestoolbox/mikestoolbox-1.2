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
//  File:       Hash.inl
//
//  Synopsis:   Method definitions for the Hash class
//----------------------------------------------------------------------------

namespace mikestoolbox {

// use the Fowler-Noll-Vo (FNV) hash algorithm, but we simply
// use the low bits of the hash instead of xor-mixing them

const uint32 gu_32BitFowlerNollVoHashInit_ =           2166136261U;
const uint32 gu_32BitFowlerNollVoHashMult_ =             16777619U;
const uint64 gu_64BitFowlerNollVoHashInit_ = 14695981039346656037ULL;
const uint64 gu_64BitFowlerNollVoHashMult_ =        1099511628211ULL;

inline uint32 FowlerNollVoHash32::ComputeHash (const uchar* ps_Chars, uintsys u_NumChars)
{
    uint32 u_Hash = gu_32BitFowlerNollVoHashInit_;

    while (u_NumChars)
    {
        u_Hash *= gu_32BitFowlerNollVoHashMult_;
        u_Hash ^= *ps_Chars;

        ++ps_Chars;
        --u_NumChars;
    }

    return u_Hash;
}

template<typename T>
inline uint32 FowlerNollVoHash32::ComputeHash (const T* p)
{
    return ComputeHash ((const uchar*)&p, sizeof(p));
}

template<typename T>
inline uint32 FowlerNollVoHash32::ComputeHash (const T& t)
{
    return ComputeHash ((const uchar*)&t, sizeof(T));
}

inline uint32 FowlerNollVoHash32::ComputeHash (const String& str)
{
    const uchar* ps_Chars   = str.PointerToFirstByte();
    uintsys      u_NumChars = str.Length();
    uint32       u_Hash     = ComputeHash (u_NumChars);

    while (u_NumChars)
    {
        u_Hash *= gu_32BitFowlerNollVoHashMult_;
        u_Hash ^= ByteToLower (*ps_Chars);

        ++ps_Chars;
        --u_NumChars;
    }

    return u_Hash;
}

inline uint32 FowlerNollVoHash32::ComputeHash (const char* pz)
{
    String str (pz);

    return ComputeHash (str);
}

template<typename K, typename V, typename H>
inline HashItem<K,V,H>::HashItem (const HashItem<K,V,H>& item)
    : key_       (item.key_)
    , value_     (item.value_)
    , u_KeyHash_ (item.u_KeyHash_)
    , p_Next_    (0)
{
    // nothing
}

template<typename K, typename V, typename H>
inline HashItem<K,V,H>::HashItem (const K& key, const V& value, typename H::HashInt u_KeyHash)
    : key_       (key)
    , value_     (value)
    , u_KeyHash_ (u_KeyHash)
    , p_Next_    (0)
{
    // nothing
}

template<typename K, typename V, typename H>
inline const K& HashItem<K,V,H>::Key () const
{
    return key_;
}

template<typename K, typename V, typename H>
inline const V& HashItem<K,V,H>::Value () const
{
    return value_;
}

template<typename K, typename V, typename H>
inline V& HashItem<K,V,H>::Value ()
{
    return value_;
}

template<typename K, typename V, typename H>
inline typename H::HashInt HashItem<K,V,H>::KeyHash () const
{
    return u_KeyHash_;
}

template<typename K, typename V, typename H>
inline void HashItem<K,V,H>::SetValue_ (const V& value)
{
    value_ = value;
}

template<typename K, typename V, typename H>
inline HashRef<K,V,H>::HashRef (Hash<K,V,H>& hash, const K& key)
    : hash_   (hash)
    , key_    (key)
{
    // nothing
}

template<typename K, typename V, typename H>
inline HashStorage<K,V,H>::HashStorage ()
    : pp_Buckets_   (0)
    , u_NumBuckets_ (0)
    , u_HashMask_   (0)
    , u_NumItems_   (0)
{
    // nothing
}

template<typename K, typename V, typename H>
void HashStorage<K,V,H>::Destroy_ ()
{
    for (uintsys u=0; u<u_NumBuckets_; ++u)
    {
        Item* p_Item = pp_Buckets_[u];

        while (p_Item)
        {
            Item* p_Next = p_Item->p_Next_;

            delete p_Item;

            p_Item = p_Next;
        }
    }

    delete [] pp_Buckets_;

    pp_Buckets_   = 0;
    u_NumBuckets_ = 0;
    u_HashMask_   = 0;
    u_NumItems_   = 0;
}

template<typename K, typename V, typename H>
HashStorage<K,V,H>::HashStorage (const HashStorage<K,V,H>& storage)
    : pp_Buckets_   (0)
    , u_NumBuckets_ (storage.u_NumBuckets_)
    , u_HashMask_   (storage.u_HashMask_)
    , u_NumItems_   (storage.u_NumItems_)
{
    if (u_NumBuckets_ != 0)
    {
        pp_Buckets_ = new(std::nothrow) Item* [u_NumBuckets_];

        if (pp_Buckets_ == 0)
        {
            throw Exception ("Hash: Out of memory");
        }

        for (uintsys u=0; u<u_NumBuckets_; ++u)
        {
            pp_Buckets_[u] = 0;
        }

        try
        {
            for (uintsys u=0; u<u_NumBuckets_; ++u)
            {
                const Item* p_Item = storage.pp_Buckets_[u];

                while (p_Item != 0)
                {
                    Item* p_NewItem = new(std::nothrow) Item (*p_Item);

                    if (p_NewItem == 0)
                    {
                        throw Exception ("Hash: Out of memory");
                    }

                    p_NewItem->p_Next_ = pp_Buckets_[u];

                    pp_Buckets_[u] = p_NewItem;

                    p_Item = p_Item->p_Next_;
                }
            }
        }
        catch (...)
        {
            Destroy_();

            throw;
        }
    }
}

template<typename K, typename V, typename H>
inline HashStorage<K,V,H>::~HashStorage ()
{
    Destroy_();
}

template<typename K, typename V, typename H>
inline Hash<K,V,H>::Hash ()
    : SharedResource (RESOURCE_COPY_ON_WRITE, new(std::nothrow) Storage)
{
    // nothing
}

template<typename K, typename V, typename H>
inline const HashStorage<K,V,H>* Hash<K,V,H>::ViewData () const
{
    return (const Storage*) SharedResource::ViewData();
}

template<typename K, typename V, typename H>
inline HashStorage<K,V,H>* Hash<K,V,H>::ModifyData ()
{
    return (Storage*) SharedResource::ModifyData();
}

template<typename K, typename V, typename H>
HashStorage<K,V,H>* Hash<K,V,H>::MakeCopyOfSharedData_ (const SharedData* p_Data) const
{
    const Storage* p_OldStorage = (const Storage*) p_Data;
          Storage* p_NewStorage = new(std::nothrow) Storage (*p_OldStorage);

    if (p_NewStorage == 0)
    {
        throw Exception ("Hash: Out of memory");
    }

    return p_NewStorage;
}

template<typename K, typename V, typename H>
HashStorage<K,V,H>* Hash<K,V,H>::MakeEmptySharedData_ () const
{
    Storage* p_NewStorage = new(std::nothrow) Storage;

    if (p_NewStorage == 0)
    {
        throw Exception ("Hash: Out of memory");
    }

    return p_NewStorage;
}

template<typename K, typename V, typename H>
inline uintsys Hash<K,V,H>::NumItems () const
{
    return ViewData()->u_NumItems_;
}

template<typename K, typename V, typename H>
inline bool Hash<K,V,H>::IsEmpty () const
{
    return (NumItems() == 0);
}

template<typename K, typename V, typename H>
inline void Hash<K,V,H>::Clear ()
{
    HashType hash;

    Swap (hash);
}

template<typename K, typename V, typename H>
uintsys Hash<K,V,H>::OptimumNumBuckets_ (uintsys u_NumItems)
{
    // one billion buckets should be enough for anybody

    const uintsys u_MinBuckets = 128;
    const uintsys u_MaxBuckets = 1U<<30;

    uintsys u_Optimum = u_NumItems * 2;

    if (IsTopBitSet (u_NumItems))
    {
        u_Optimum = u_MaxBuckets;
    }
    else if (u_Optimum <= u_MinBuckets)
    {
        u_Optimum = u_MinBuckets;
    }
    else
    {
        uintsys u_Multiple = 1;

        u_Optimum /= u_MinBuckets;

        while (u_Optimum)
        {
            u_Optimum  /= 2;
            u_Multiple *= 2;
        }

        if (u_Multiple >= (u_MaxBuckets/u_MinBuckets))
        {
            u_Optimum = u_MaxBuckets;
        }
        else
        {
            u_Optimum = u_Multiple * u_MinBuckets;
        }
    }

    return u_Optimum;
}

template<typename K, typename V, typename H>
void Hash<K,V,H>::Resize_ (uintsys u_NumItems)
{
    Storage* p_Storage = ModifyData();

    uintsys u_NumBuckets    = p_Storage->u_NumBuckets_;
    uintsys u_NewNumBuckets = OptimumNumBuckets_ (u_NumItems);

    if (u_NumBuckets >= u_NewNumBuckets)
    {
        return;
    }

    Item** pp_NewBuckets = new(std::nothrow) Item* [u_NewNumBuckets];

    if (pp_NewBuckets == 0)
    {
        throw Exception ("Hash: Out of memory");
    }

    for (uintsys u=0; u<u_NewNumBuckets; ++u)
    {
        pp_NewBuckets[u] = 0;
    }

    if (p_Storage->pp_Buckets_ == 0)
    {
        p_Storage->pp_Buckets_   = pp_NewBuckets;
        p_Storage->u_NumBuckets_ = u_NewNumBuckets;
        p_Storage->u_HashMask_   = u_NewNumBuckets - 1;

        return;
    }

    uintsys u_NewHashMask = u_NewNumBuckets - 1;

    Item** pp_OldBuckets = p_Storage->pp_Buckets_;

    for (uintsys u=0; u<u_NumBuckets; ++u)
    {
        Item* p_Item = pp_OldBuckets[u];
        Item* p_Next = 0;

        while (p_Item != 0)
        {
            p_Next = p_Item->p_Next_;

            uintsys u_NewBucket = p_Item->u_KeyHash_ & u_NewHashMask;

            p_Item->p_Next_ = pp_NewBuckets[u_NewBucket];

            pp_NewBuckets[u_NewBucket] = p_Item;

            p_Item = p_Next;
        }
    }

    delete [] pp_OldBuckets;

    p_Storage->pp_Buckets_   = pp_NewBuckets;
    p_Storage->u_NumBuckets_ = u_NewNumBuckets;
    p_Storage->u_HashMask_   = u_NewHashMask;
}

template<typename K, typename V, typename H>
const HashItem<K,V,H>* Hash<K,V,H>::FindItem_ (const K& key) const
{
    if (IsEmpty())
    {
        return 0;
    }

    const Storage* p_Storage = ViewData();

    uintsys u_Hash   = H::ComputeHash (key);
    uintsys u_Bucket = u_Hash & p_Storage->u_HashMask_;

    const Item* p_Item = p_Storage->pp_Buckets_[u_Bucket];

    while (p_Item != 0)
    {
        if ((p_Item->KeyHash() == u_Hash) && (p_Item->Key() == key))
        {
            break;
        }

        p_Item = p_Item->p_Next_;
    }

    return p_Item;
}

template<typename K, typename V, typename H>
HashItem<K,V,H>* Hash<K,V,H>::FindItem_ (const K& key)
{
    const HashType& hash (*this);

    const Item* p_Find = hash.FindItem_ (key);

    if (p_Find == 0)
    {
        return 0;
    }

    Storage* p_Storage = ModifyData();

    uintsys u_Hash   = p_Find->KeyHash();
    uintsys u_Bucket = u_Hash & p_Storage->u_HashMask_;

    Item* p_Item = p_Storage->pp_Buckets_[u_Bucket];

    while (p_Item)
    {
        if ((p_Item->KeyHash() == u_Hash) && (p_Item->Key() == key))
        {
            break;
        }

        p_Item = p_Item->p_Next_;
    }

    return p_Item;
}

template<typename K, typename V, typename H>
HashItem<K,V,H>* Hash<K,V,H>::CreateItem_ (const K& key, const V& value, uintsys u_Hash)
{
    Storage* p_Storage = ModifyData();

    if (u_Hash == 0)
    {
        u_Hash = H::ComputeHash (key);
    }

    uintsys u_Bucket = u_Hash & p_Storage->u_HashMask_;

    Item* p_Item = 0;

    if (p_Storage->pp_Buckets_ != 0)
    {
        p_Item = p_Storage->pp_Buckets_[u_Bucket];
    }

    while (p_Item)
    {
        if ((p_Item->KeyHash() == u_Hash) && (p_Item->Key() == key))
        {
            p_Item->SetValue_ (value);

            return p_Item;
        }

        p_Item = p_Item->p_Next_;
    }

    try
    {
        Resize_ (p_Storage->u_NumItems_ + 1);

        u_Bucket = u_Hash & p_Storage->u_HashMask_;

        p_Item = new(std::nothrow) Item (key, value, u_Hash);

        p_Item->p_Next_ = p_Storage->pp_Buckets_[u_Bucket];

        p_Storage->pp_Buckets_[u_Bucket] = p_Item;

        ++p_Storage->u_NumItems_;
    }
    catch (...)
    {
        p_Item = 0;
    }

    if (p_Item == 0)
    {
        throw Exception ("Hash: Out of memory");
    }

    return p_Item;
}

template<typename K, typename V, typename H>
inline bool Hash<K,V,H>::Find (const K& key, V& value) const
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item != 0)
    {
        value = p_Item->Value();

        return true;
    }

    return false;
}

template<typename K, typename V, typename H>
inline bool Hash<K,V,H>::Exists (const K& key) const
{
    return (FindItem_ (key) != 0);
}

template<typename K, typename V, typename H>
inline const V Hash<K,V,H>::Get (const K& key) const
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item)
    {
        return p_Item->Value();
    }

    return V();
}

template<typename K, typename V, typename H>
inline void Hash<K,V,H>::Set (const K& key, const V& value)
{
    CreateItem_ (key, value);
}

template<typename K, typename V, typename H>
inline const V Hash<K,V,H>::Get (const K& key, const V& v_Default)
{
    const Item* p_Item = FindItem_ (key);

    if (p_Item)
    {
        return p_Item->Value();
    }

    Set (key, v_Default);

    return v_Default;
}

template<typename K, typename V, typename H>
inline void Hash<K,V,H>::Swap (Hash<K,V,H>& hash)
{
    SharedResource::Swap (hash);
}

template<typename K, typename V, typename H>
inline const V Hash<K,V,H>::operator[] (const K& key) const
{
    return Get (key);
}

template<typename K, typename V, typename H>
inline V& Hash<K,V,H>::operator[] (const K& key)
{
    Item* p_Item = FindItem_ (key);

    if (p_Item == 0)
    {
        p_Item = CreateItem_ (key, V());
    }

    return p_Item->Value();
}

template<typename K, typename V, typename H>
inline const V Hash<K,V,H>::operator() (const K& key) const
{
    return operator[] (key);
}

template<typename K, typename V, typename H>
inline HashRef<K,V,H> Hash<K,V,H>::operator() (const K& key)
{
    return Ref (*this, key);
}

template<typename K, typename V, typename H>
inline HashRef<K,V,H>::operator const V () const
{
    return hash_.Get (key_);
}

template<typename K, typename V, typename H>
inline V* HashRef<K,V,H>::operator-> ()
{
    return &hash_[key_];
}

template<typename K, typename V, typename H>
inline HashRef<K,V,H>& HashRef<K,V,H>::operator= (const V& value)
{
    hash_.Set (key_, value);

    return *this;
}

template<typename K, typename V, typename H>
inline HashRef<K,V,H>& HashRef<K,V,H>::operator= (const HashRef<K,V,H>& ref)
{
    hash_.Set (key_, ref);

    return *this;
}

template<typename K, typename V, typename H>
inline bool HashRef<K,V,H>::operator== (const V& value) const
{
    return (hash_.Get (key_) == value);
}

template<typename K, typename V, typename H>
inline bool HashRef<K,V,H>::operator!= (const V& value) const
{
    return !operator== (value);
}

template<typename K, typename V, typename H>
template<typename U>
inline bool HashRef<K,V,H>::operator== (const U& value) const
{
    return (hash_.Get (key_) == V(value));
}

template<typename K, typename V, typename H>
template<typename U>
inline bool HashRef<K,V,H>::operator!= (const U& value) const
{
    return !operator== (value);
}

template<typename K, typename V, typename H>
inline bool Hash<K,V,H>::operator!= (const Hash<K,V,H>& hash) const
{
    return !operator== (hash);
}

template<typename K, typename V, typename H>
bool Hash<K,V,H>::Delete (const K& key)
{
    if (IsEmpty())
    {
        return false;
    }

    const HashType& hash (*this);

    const Item* p_Find = hash.FindItem_ (key);

    if (p_Find == 0)
    {
        return false;
    }

    uintsys u_Hash = p_Find->KeyHash();

    Storage* p_Storage = ModifyData();

    uintsys u_Bucket = u_Hash & p_Storage->u_HashMask_;

    Item** pp_Buckets = p_Storage->pp_Buckets_;
    Item*  p_Item     = pp_Buckets[u_Bucket];
    Item*  p_Prev     = 0;

    while (p_Item != 0)
    {
        if ((p_Item->KeyHash() == u_Hash) && (p_Item->Key() == key))
        {
            if (p_Prev == 0)
            {
                pp_Buckets[u_Bucket] = p_Item->p_Next_;
            }
            else
            {
                p_Prev->p_Next_ = p_Item->p_Next_;
            }

            delete p_Item;

            --p_Storage->u_NumItems_;

            return true;
        }

        p_Prev = p_Item;
        p_Item = p_Item->p_Next_;
    }

    return false;
}

template<typename K, typename V, typename H>
uintsys Hash<K,V,H>::Delete (const List<K>& list_Keys)
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

template<typename K, typename V, typename H>
inline void Hash<K,V,H>::Reserve (uintsys u_NumItems)
{
    ModifyData();

    Resize_ (u_NumItems);
}

template<typename K, typename V, typename H>
double Hash<K,V,H>::Efficiency () const
{
    uintsys u_FilledBuckets = 0;
    double  d_PathLength    = 0.0;

    Item** const pp_Buckets   = ViewData()->pp_Buckets_;
    uintsys      u_NumBuckets = ViewData()->u_NumBuckets_;

    for (uintsys u=0; u<u_NumBuckets; ++u)
    {
        const Item* p_Item = pp_Buckets[u];

        if (p_Item == 0)
        {
            continue;
        }

        ++u_FilledBuckets;

        uintsys u_Depth = 0;

        while (p_Item != 0)
        {
            ++u_Depth;

            d_PathLength += u_Depth;

            p_Item = p_Item->p_Next_;
        }
    }

    if (u_FilledBuckets != 0)
    {
        d_PathLength /= u_FilledBuckets;
    }

    return d_PathLength;
}

inline String CreateHashDebugLine (const char* pz_Label, const String& str_Text)
{
    String str_Line (Preallocate (str_Text.Length() + 20));

    str_Line.Append (pz_Label);
    str_Line.Append (':');
    str_Line.PadEnd (18);
    str_Line.Append (str_Text);
    str_Line.Append ('\n');

    return str_Line;
}

template<typename K, typename V, typename H>
const StringList Hash<K,V,H>::Debug () const
{
    StringList strl_Info;
    String     str_Line;

    uintsys u_FilledBuckets = 0;
    uintsys u_KeyDepth      = 0;
    double  d_PathLength    = 0.0;

    const uintsys u_MaxDepth = 10;

    uintsys au_Lengths[u_MaxDepth + 1];

    ZeroMemory (au_Lengths, sizeof(uintsys) * (u_MaxDepth + 1));

    Item** const pp_Buckets   = ViewData()->pp_Buckets_;
    uintsys      u_NumBuckets = ViewData()->u_NumBuckets_;

    for (uintsys u=0; u<u_NumBuckets; ++u)
    {
        Item* p_Item = pp_Buckets[u];

        if (p_Item == 0)
        {
            continue;
        }

        ++u_FilledBuckets;

        uintsys u_Depth = 0;

        while (p_Item != 0)
        {
            ++u_Depth;

            d_PathLength += u_Depth;

            p_Item = p_Item->p_Next_;
        }

        if (u_Depth < u_MaxDepth)
        {
            ++au_Lengths[u_Depth];
        }
        else
        {
            ++au_Lengths[u_MaxDepth];
        }

        if (u_Depth > u_KeyDepth)
        {
            u_KeyDepth = u_Depth;
        }
    }

    if (u_FilledBuckets != 0)
    {
        d_PathLength /= u_FilledBuckets;
    }

    strl_Info.Append (CreateHashDebugLine ("Hash Key Type",   Typename<K>()));
    strl_Info.Append (CreateHashDebugLine ("Hash Value Type", Typename<V>()));
    strl_Info.Append (CreateHashDebugLine ("Hash Function",   Typename<H>()));
    strl_Info.Append (CreateHashDebugLine ("Number Of Items", String(NumItems())));
    strl_Info.Append (CreateHashDebugLine ("Filled Buckets",  String(u_FilledBuckets)));
    strl_Info.Append (CreateHashDebugLine ("Total Buckets",   String(u_NumBuckets)));
    strl_Info.Append (CreateHashDebugLine ("Maximum Depth",   String(u_KeyDepth)));
    strl_Info.Append (CreateHashDebugLine ("Average Depth",   String(d_PathLength)));

    return strl_Info;
}

template<typename K, typename V, typename H>
inline HashChangeIter<K,V,H>::HashChangeIter (Hash<K,V,H>& hash, const K& key)
    : hash_     (hash)
    , p_Item_   (hash_.FindItem_ (key))
    , u_Bucket_ (0)
{
    if (p_Item_ != 0)
    {
        u_Bucket_ = p_Item_->KeyHash() & hash_.ViewData()->u_HashMask_;
    }
}

template<typename K, typename V, typename H>
HashChangeIter<K,V,H>::HashChangeIter (Hash<K,V,H>& hash)
    : hash_     (hash)
    , p_Item_   (0)
    , u_Bucket_ (0)
{
    Storage* p_Storage = hash_.ModifyData();

    uintsys u_NumBuckets = p_Storage->u_NumBuckets_;
    Item**  pp_Buckets   = p_Storage->pp_Buckets_;

    for (uintsys u=0; u<u_NumBuckets; ++u)
    {
        p_Item_ = pp_Buckets[u];

        if (p_Item_ != 0)
        {
            u_Bucket_ = u;

            break;
        }
    }
}

template<typename K, typename V, typename H>
inline HashChangeIter<K,V,H>::operator bool () const
{
    return p_Item_ != 0;
}

template<typename K, typename V, typename H>
inline void HashChangeIter<K,V,H>::MoveTo (const K& key)
{
    p_Item_ = hash_.FindItem_ (key);

    u_Bucket_ = p_Item_ ? (p_Item_->KeyHash() & hash_.ViewData()->u_HashMask_) : 0;
}

template<typename K, typename V, typename H>
inline const K& HashChangeIter<K,V,H>::Key () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Key();
    }

    throw Exception ("HashChangeIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
inline V& HashChangeIter<K,V,H>::Value () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Value();
    }

    throw Exception ("HashChangeIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
inline HashItem<K,V,H>* HashChangeIter<K,V,H>::operator-> () const
{
    if (p_Item_ != 0)
    {
        return p_Item_;
    }

    throw Exception ("HashChangeIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
HashChangeIter<K,V,H>& HashChangeIter<K,V,H>::operator++ ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->p_Next_;

        if (p_Item_ == 0)
        {
            Storage* p_Storage = hash_.ModifyData();

            uintsys u_NumBuckets = p_Storage->u_NumBuckets_;

            Item** pp_Buckets = p_Storage->pp_Buckets_;

            for (uintsys u=u_Bucket_+1; u<u_NumBuckets; ++u)
            {
                p_Item_ = pp_Buckets[u];

                if (p_Item_ != 0)
                {
                    u_Bucket_ = u;

                    break;
                }
            }
        }
    }

    return *this;
}

template<typename K, typename V, typename H>
inline const HashChangeIter<K,V,H> HashChangeIter<K,V,H>::operator++ (int)
{
    ChangeIter iter (*this);

    operator++ ();

    return iter;
}

template<typename K, typename V, typename H>
inline HashChangeIter<K,V,H>& HashChangeIter<K,V,H>::operator= (const HashChangeIter<K,V,H>& iter)
{
    if (hash_.ViewData() == iter.hash_.ViewData())
    {
        p_Item_   = iter.p_Item_;
        u_Bucket_ = iter.u_Bucket_;
    }
    else
    {
        p_Item_   = 0;
        u_Bucket_ = 0;
    }

    return *this;
}

template<typename K, typename V, typename H>
inline bool HashChangeIter<K,V,H>::operator== (const HashChangeIter<K,V,H>& iter) const
{
    return p_Item_ == iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashChangeIter<K,V,H>::operator== (const HashIter<K,V,H>& iter) const
{
    return p_Item_ == iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashChangeIter<K,V,H>::operator!= (const HashChangeIter<K,V,H>& iter) const
{
    return p_Item_ != iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashChangeIter<K,V,H>::operator!= (const HashIter<K,V,H>& iter) const
{
    return p_Item_ != iter.p_Item_;
}

template<typename K, typename V, typename H>
inline HashIter<K,V,H>::HashIter (const Hash<K,V,H>& hash, const K& key)
    : hash_         (hash)
    , p_Item_       (hash_.FindItem_ (key))
    , u_Bucket_     (0)
{
    if (p_Item_ != 0)
    {
        u_Bucket_ = p_Item_->KeyHash() & hash_.ViewData()->u_HashMask_;
    }
}

template<typename K, typename V, typename H>
HashIter<K,V,H>::HashIter (const Hash<K,V,H>& hash)
    : hash_         (hash)
    , p_Item_       (0)
    , u_Bucket_     (0)
{
    Item** const pp_Buckets   = hash_.ViewData()->pp_Buckets_;
    uintsys      u_NumBuckets = hash_.ViewData()->u_NumBuckets_;

    for (uintsys u=0; u<u_NumBuckets; ++u)
    {
        p_Item_ = pp_Buckets[u];

        if (p_Item_ != 0)
        {
            u_Bucket_ = u;

            break;
        }
    }
}

template<typename K, typename V, typename H>
inline HashIter<K,V,H>::HashIter (const HashChangeIter<K,V,H>& iter)
    : hash_     (iter.hash_)
    , p_Item_   (iter.p_Item_)
    , u_Bucket_ (iter.u_Bucket_)
{
    // nothing
}

template<typename K, typename V, typename H>
inline HashIter<K,V,H>::operator bool () const
{
    return p_Item_ != 0;
}

template<typename K, typename V, typename H>
inline void HashIter<K,V,H>::MoveTo (const K& key)
{
    p_Item_ = hash_.FindItem_ (key);

    u_Bucket_ = p_Item_ ? (p_Item_->KeyHash() & hash_.ViewData()->u_HashMask_) : 0;
}

template<typename K, typename V, typename H>
inline const K& HashIter<K,V,H>::Key () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Key();
    }

    throw Exception ("HashIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
inline const V& HashIter<K,V,H>::Value () const
{
    if (p_Item_ != 0)
    {
        return p_Item_->Value();
    }

    throw Exception ("HashIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
inline const HashItem<K,V,H>* HashIter<K,V,H>::operator-> () const
{
    if (p_Item_ != 0)
    {
        return p_Item_;
    }

    throw Exception ("HashIter: Dereferencing invalid iterator");
}

template<typename K, typename V, typename H>
HashIter<K,V,H>& HashIter<K,V,H>::operator++ ()
{
    if (p_Item_ != 0)
    {
        p_Item_ = p_Item_->p_Next_;

        if (p_Item_ == 0)
        {
            Item** const pp_Buckets   = hash_.ViewData()->pp_Buckets_;
            uintsys      u_NumBuckets = hash_.ViewData()->u_NumBuckets_;

            for (uintsys u=u_Bucket_+1; u<u_NumBuckets; ++u)
            {
                p_Item_ = pp_Buckets[u];

                if (p_Item_ != 0)
                {
                    u_Bucket_ = u;

                    break;
                }
            }
        }
    }

    return *this;
}

template<typename K, typename V, typename H>
inline const HashIter<K,V,H> HashIter<K,V,H>::operator++ (int)
{
    Iter iter (*this);

    operator++ ();

    return iter;
}

template<typename K, typename V, typename H>
inline HashIter<K,V,H>& HashIter<K,V,H>::operator= (const HashChangeIter<K,V,H>& iter)
{
    if (hash_.ViewData() == iter.hash_.ViewData())
    {
        p_Item_   = iter.p_Item_;
        u_Bucket_ = iter.u_Bucket_;
    }
    else
    {
        p_Item_   = 0;
        u_Bucket_ = 0;
    }

    return *this;
}

template<typename K, typename V, typename H>
inline HashIter<K,V,H>& HashIter<K,V,H>::operator= (const HashIter<K,V,H>& iter)
{
    if (hash_.ViewData() == iter.hash_.ViewData())
    {
        p_Item_   = iter.p_Item_;
        u_Bucket_ = iter.u_Bucket_;
    }
    else
    {
        p_Item_   = 0;
        u_Bucket_ = 0;
    }

    return *this;
}

template<typename K, typename V, typename H>
inline bool HashIter<K,V,H>::operator== (const HashIter<K,V,H>& iter) const
{
    return p_Item_ == iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashIter<K,V,H>::operator== (const HashChangeIter<K,V,H>& iter) const
{
    return p_Item_ == iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashIter<K,V,H>::operator!= (const HashIter<K,V,H>& iter) const
{
    return p_Item_ != iter.p_Item_;
}

template<typename K, typename V, typename H>
inline bool HashIter<K,V,H>::operator!= (const HashChangeIter<K,V,H>& iter) const
{
    return p_Item_ != iter.p_Item_;
}

template<typename K, typename V, typename H>
inline const HashIter<K,V,H> Hash<K,V,H>::Begin () const
{
    return Iter (*this);
}

template<typename K, typename V, typename H>
inline const HashChangeIter<K,V,H> Hash<K,V,H>::Begin ()
{
    return ChangeIter (*this);
}

template<typename K, typename V, typename H>
inline bool Hash<K,V,H>::Contains (const K& key, const V& value) const
{
    Iter iter (*this, key);

    return iter && (iter->Value() == value);
}

template<typename K, typename V, typename H>
bool Hash<K,V,H>::operator== (const Hash<K,V,H>& hash) const
{
    if (ViewData() == hash.ViewData())
    {
        return true;
    }

    uintsys u_NumItems = NumItems();

    if (hash.NumItems() != u_NumItems)
    {
        return false;
    }

    if (u_NumItems == 0)
    {
        return true;
    }

    Iter iter (*this);

    while (iter)
    {
        const K& key   = iter->Key();
        const V& value = iter->Value();

        if (!hash.Contains (key, value))
        {
            return false;
        }

        ++iter;
    }

    return true;
}

template<typename K, typename V, typename H>
const List<K> Hash<K,V,H>::Keys () const
{
    List<K> list_Keys;

    Iter iter (*this);

    while (iter)
    {
        const K& key = iter->Key();

        list_Keys.Append (key);

        ++iter;
    }

    return list_Keys;
}

template<typename K, typename V, typename H>
inline const List<K> Hash<K,V,H>::SortedKeys () const
{
    List<K> list_Keys (Keys());

    list_Keys.Sort();

    return list_Keys;
}

template<typename K, typename V, typename H>
const List<V> Hash<K,V,H>::Values () const
{
    List<V> list_Values;

    Iter iter (*this);

    while (iter)
    {
        const V& value = iter->Value();

        list_Values.Append (value);

        ++iter;
    }

    return list_Values;
}

template<typename K, typename V, typename H>
void Hash<K,V,H>::AddKeyValue_ (HashType& hash, const K& key, const V& value, ImportOptions options)
{
    if (options.CanCreate())
    {
        if (options.CanReplace())
        {
            hash.Set (key, value);
        }
        else
        {
            hash.Get (key, value);
        }
    }
    else if (options.CanReplace() && hash.Exists (key))
    {
        hash.Set (key, value);
    }
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
void Hash<K,V,H>::Import_ (const CONTAINER& c, ImportOptions options, MikesToolboxSimpleContainer)
{
    uintsys u_Extra = c.NumItems() / 2;

    if (u_Extra != 0)
    {
        HashType hash (*this);

        hash.Reserve (NumItems() + u_Extra);

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            K key (*iter);

            if (++iter)
            {
                V value (*iter);

                AddKeyValue_ (hash, key, value, options);

                ++iter;
            }
        }

        Swap (hash);
    }
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
void Hash<K,V,H>::Import_ (const CONTAINER& c, ImportOptions options, MikesToolboxKeyValueContainer)
{
    uintsys u_Extra = c.NumItems();

    if (u_Extra != 0)
    {
        HashType hash (*this);

        hash.Reserve (NumItems() + u_Extra);

        typename CONTAINER::Iter iter (c);

        while (iter)
        {
            K key   (iter->Key());
            V value (iter->Value());

            AddKeyValue_ (hash, key, value, options);

            ++iter;
        }

        Swap (hash);
    }
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
void Hash<K,V,H>::Import_ (const CONTAINER& c, ImportOptions options, StandardCPlusPlusSimpleContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    HashType hash (*this);

    while (iter1 != iter2)
    {
        K key (*iter1);

        if (++iter1 != iter2)
        {
            V value (*iter1);

            AddKeyValue_ (hash, key, value, options);

            ++iter1;
        }
    }

    Swap (hash);
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
void Hash<K,V,H>::Import_ (const CONTAINER& c, ImportOptions options, StandardCPlusPlusKeyValueContainer)
{
    typename CONTAINER::const_iterator iter1 (c.begin());
    typename CONTAINER::const_iterator iter2 (c.end());

    HashType hash (*this);

    while (iter1 != iter2)
    {
        K key   (iter1->first);
        V value (iter1->second);

        AddKeyValue_ (hash, key, value, options);

        ++iter1;
    }

    Swap (hash);
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
void Hash<K,V,H>::Import_ (const CONTAINER&, ImportOptions, UnknownTypeOfContainer)
{
    // nothing good can come from doing anything here

    throw Exception ("Hash::Import: Unknown container type");
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
inline void Hash<K,V,H>::Import (const CONTAINER& c, ImportOptions options)
{
    Import_ (c, options, typename ContainerType<CONTAINER>::Type());
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
inline void Hash<K,V,H>::Import (const CONTAINER& c)
{
    Import (c, ImportOptions());
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
inline Hash<K,V,H>::Hash (const CONTAINER& c)
    : SharedResource (RESOURCE_COPY_ON_WRITE, new(std::nothrow) Storage)
{
    Import (c);
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
inline Hash<K,V,H>& Hash<K,V,H>::operator= (const CONTAINER& c)
{
    HashType hash (c);

    Swap (hash);

    return (*this);
}

template<typename K, typename V, typename H>
template<typename CONTAINER>
inline Hash<K,V,H>& Hash<K,V,H>::operator+= (const CONTAINER& c)
{
    Import (c);

    return *this;
}

template<typename K, typename V, typename H>
inline void swap (Hash<K,V,H>& hash1, Hash<K,V,H>& hash2)
{
    hash1.Swap (hash2);
}

template<typename K, typename V>
Hash<V,K> InvertHash (const Hash<K,V>& hash)
{
    Hash<V,K> hash_New;

    HashIter<K,V> iter (hash);

    while (iter)
    {
        const K& k (iter->Key());
        const V& v (iter->Value());

        hash_New[v] = k;

        ++iter;
    }

    return hash_New;
}

} // namespace mikestoolbox

