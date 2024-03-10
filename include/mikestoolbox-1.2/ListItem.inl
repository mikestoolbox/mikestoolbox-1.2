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
//  File:       ListItem.inl
//
//  Synopsis:   Methods of ListItem<T>
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<typename T>
inline ListItem<T>::ListItem (const Item& item)
    : t_      (item.Value())
    , p_Next_ (this)
    , p_Prev_ (this)
    , u_SortPos_ (0)
{
    // nothing
}

template<typename T>
inline ListItem<T>::ListItem (const T& item)
    : t_      (item)
    , p_Next_ (this)
    , p_Prev_ (this)
    , u_SortPos_ (0)
{
    // nothing
}

template<typename T>
template<typename U>
inline ListItem<T>::ListItem (const U& item)
    : t_      (item)
    , p_Next_ (this)
    , p_Prev_ (this)
    , u_SortPos_ (0)
{
    // nothing
}

template<typename T>
inline ListItem<T>::ListItem ()
    : t_      ()
    , p_Next_ (this)
    , p_Prev_ (this)
    , u_SortPos_ (0)
{
    // nothing
}

template<typename T>
inline T& ListItem<T>::Value ()
{
    return t_;
}

template<typename T>
inline const T& ListItem<T>::Value () const
{
    return t_;
}

template<typename T>
inline bool ListItem<T>::Check () const
{
    return ((p_Next_ != 0) && (p_Next_->p_Prev_ == this) &&
            (p_Prev_ != 0) && (p_Prev_->p_Next_ == this));
}

template<typename T>
inline void ListItem<T>::Append (Item* p_Item)
{
    p_Next_ = p_Item;

    p_Item->p_Prev_ = this;
}

template<typename T>
inline ListItem<T>* ListItem<T>::Extract ()
{
    p_Next_->p_Prev_ = p_Prev_;
    p_Prev_->p_Next_ = p_Next_;

    return this;
}

template<typename T>
inline ListItem<T>::~ListItem ()
{
    Extract();
}

template<typename T>
inline void ListItem<T>::SwapValue (Item* p_Item)
{
    using std::swap;

    swap (t_,         p_Item->t_);
    swap (u_SortPos_, p_Item->u_SortPos_);
}

template<typename T>
inline ListItem<T>& ListItem<T>::operator= (const Item& item)
{
    t_ = item.t_;

    return *this;
}

template<typename T>
inline ListItem<T>& ListItem<T>::operator= (const T& t)
{
    t_ = t;

    return *this;
}

template<typename T>
template<typename U>
inline ListItem<T>& ListItem<T>::operator= (const U& u)
{
    t_ = T(u);

    return *this;
}

} // namespace mikestoolbox

