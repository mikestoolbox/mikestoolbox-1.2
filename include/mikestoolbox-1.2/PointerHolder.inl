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

namespace mikestoolbox {

template<typename T>
inline PointerHolder<T>::PointerHolder (T* p)
    : p_ (p)
{
    // nothing
}

template<typename T>
inline PointerHolder<T>::PointerHolder ()
    : p_ (0)
{
    // nothing
}

template<typename T>
inline PointerHolder<T>::~PointerHolder ()
{
    delete p_;
}

template<typename T>
inline bool PointerHolder<T>::IsNull () const
{
    return (p_ == 0);
}

template<typename T>
inline T* PointerHolder<T>::Release ()
{
    T* p_Temp = p_;

    p_ = 0;

    return p_Temp;
}

template<typename T>
inline PointerHolder<T>::operator T* ()
{
    return p_;
}

template<typename T>
inline T* PointerHolder<T>::operator-> ()
{
    if (IsNull())
    {
        throw Exception ("PointerHolder: Null pointer dereference");
    }

    return p_;
}

template<typename T>
inline PointerHolder<T>& PointerHolder<T>::operator= (T* p)
{
    delete p_;

    p_ = p;

    return *this;
}

template<typename T>
inline void PointerHolder<T>::Swap (PointerHolder<T>& other)
{
    using std::swap;

    swap (p_, other.p_);
}

template<typename T>
inline ArrayPointerHolder<T>::ArrayPointerHolder (T* p)
    : p_ (p)
{
    // nothing
}

template<typename T>
inline ArrayPointerHolder<T>::ArrayPointerHolder ()
    : p_ (0)
{
    // nothing
}

template<typename T>
inline ArrayPointerHolder<T>::~ArrayPointerHolder ()
{
    delete [] p_;
}

template<typename T>
inline bool ArrayPointerHolder<T>::IsNull () const
{
    return (p_ == 0);
}

template<typename T>
inline T* ArrayPointerHolder<T>::Release ()
{
    T* p_Temp = p_;

    p_ = 0;

    return p_Temp;
}

template<typename T>
inline ArrayPointerHolder<T>::operator T* ()
{
    return p_;
}

template<typename T>
inline T& ArrayPointerHolder<T>::operator[] (uintsys u_Index)
{
    if (IsNull())
    {
        throw Exception ("ArrayPointerHolder: Null pointer dereference");
    }

    return p_[u_Index];
}

template<typename T>
inline ArrayPointerHolder<T>& ArrayPointerHolder<T>::operator= (T* p)
{
    delete [] p_;

    p_ = p;

    return *this;
}

template<typename T>
inline void ArrayPointerHolder<T>::Swap (ArrayPointerHolder<T>& other)
{
    using std::swap;

    swap (p_, other.p_);
}

} // namespace std

