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
//  File:       ListSort.inl
//
//  Synopsis:   Sorting methods
//----------------------------------------------------------------------------

namespace mikestoolbox {

#define AFTER(x)  (x)->p_Next_
#define BEFORE(x) (x)->p_Prev_

template<typename T>
template<class CMP>
bool ListItem<T>::SortsBefore (const Item* p_Other, const CMP& cmp) const
{
    if (cmp (Value(), p_Other->Value()))
    {
        return true;
    }

    if (cmp (p_Other->Value(), Value()))
    {
        return false;
    }

    return (u_SortPos_ < p_Other->u_SortPos_);
}

template<typename T>
template<class CMP>
inline void ListData<T>::PutInOrder_ (Item* p1, Item* p2, const CMP& cmp)
{
    if (p2->SortsBefore (p1, cmp))
    {
        p1->SwapValue (p2);
    }
}

template<typename T>
template<class CMP>
inline void
    ListData<T>::PutInOrder_ (Item* p1, Item* p2, Item* p3, const CMP& cmp)
{
    PutInOrder_ (p1, p2, cmp);
    PutInOrder_ (p2, p3, cmp);
    PutInOrder_ (p1, p2, cmp);
}

template<typename T>
template<class CMP>
inline void ListData<T>::PutInOrder_ (Item* p1, Item* p2, Item* p3, Item* p4,
                                      const CMP& cmp)
{
    PutInOrder_ (p1, p2, cmp);
    PutInOrder_ (p3, p4, cmp);
    PutInOrder_ (p1, p3, cmp);
    PutInOrder_ (p2, p4, cmp);
    PutInOrder_ (p2, p3, cmp);
}

template<typename T>
template<class CMP>
bool ListData<T>::SmallSort_ (Item* p1, Item* p2, uintsys u_NumItems,
                              const CMP& cmp)
{
    switch (u_NumItems)
    {
        case 0:                                                   return true;
        case 1:                                                   return true;
        case 2: PutInOrder_ (p1,                        p2, cmp); return true;
        case 3: PutInOrder_ (p1, AFTER(p1),             p2, cmp); return true;
        case 4: PutInOrder_ (p1, AFTER(p1), BEFORE(p2), p2, cmp); return true;
    }

    return false;
}

template<typename T>
template<class CMP>
void ListData<T>::Sort_ (Item* p_Start, Item* p_End, uintsys u_NumItems,
                         const CMP& cmp)
{
    for (;;)
    {
        if (SmallSort_ (p_Start, p_End, u_NumItems, cmp))
        {
            return;
        }

        Item* p_Pivot = p_Start;
        Item* p_Low   = AFTER(p_Start);
        Item* p_Mid   = p_Low;
        Item* p_High  = p_End;

        uintsys u_Low  = 1;
        uintsys u_High = u_NumItems-1;

        for (uintsys u=u_NumItems/2; u>1; --u)
        {
            p_Mid = AFTER(p_Mid);
        }

        PutInOrder_ (p_Low, p_Mid, p_End, cmp);

        p_Pivot->SwapValue (p_Mid);

        for (;;)
        {
            do
            {
                p_Low = AFTER(p_Low);
                ++u_Low;
            }
            while (p_Low->SortsBefore (p_Pivot, cmp));

            do
            {
                p_High = BEFORE(p_High);
                --u_High;
            }
            while (p_Pivot->SortsBefore (p_High, cmp));

            if (u_High < u_Low)
            {
                break;
            }

            p_Low->SwapValue (p_High);
        }

        p_Pivot->SwapValue (p_High);

        Sort_ (p_Low, p_End, u_NumItems-u_Low, cmp);

        p_End      = BEFORE(p_High);
        u_NumItems = u_High;
    }
}

template<typename T>
template<class CMP>
inline void ListData<T>::Sort (const CMP& cmp)
{
    if (u_NumItems_ > 1)
    {
        Item* p_Item = p_Root_;

        // annotate items with original order to achieve a stable quicksort

        p_Item->u_SortPos_ = 0;

        for (uintsys u=1; u<u_NumItems_; ++u)
        {
            p_Item = AFTER(p_Item);

            p_Item->u_SortPos_ = u;
        }

        Sort_ (p_Root_, BEFORE(p_Root_), u_NumItems_, cmp);
    }
}

#undef AFTER
#undef BEFORE

} // namespace mikestoolbox

