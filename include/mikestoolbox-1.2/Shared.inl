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
//  File:       Shared.inl
//
//  Synopsis:   Implementation of Shared base class
//----------------------------------------------------------------------------

namespace mikestoolbox {

inline SharedData::SharedData ()
    : refs_        (1)
    , b_CountRefs_ (true)
{
    // nothing
}

inline SharedData::SharedData (AlwaysShared)
    : refs_        (2)
    , b_CountRefs_ (false)
{
    // nothing
}

inline SharedData::~SharedData ()
{
    // nothing
}

inline uintsys SharedData::IncrementRefs_ () const
{
    if (b_CountRefs_)
    {
        return ++refs_;
    }

    return 2;
}

inline uintsys SharedData::DecrementRefs_ () const
{
    if (b_CountRefs_)
    {
        return --refs_;
    }

    return 2;
}

inline uintsys SharedResource::References_ () const
{
    return p_Data_->refs_;
}

inline bool SharedResource::IsShared () const
{
    return p_Data_->refs_ != 1;
}

inline uintsys SharedResource::IncrementRefs_ () const
{
    return p_Data_->IncrementRefs_();
}

inline uintsys SharedResource::DecrementRefs_ () const
{
    return p_Data_->DecrementRefs_();
}

inline SharedResource::SharedResource (const SharedResource& shared)
    : p_Data_ (shared.p_Data_)
    , n_Type_ (shared.n_Type_)
{
    IncrementRefs_();
}

inline SharedResource::SharedResource (int n_Type, SharedData* p_Data)
    : p_Data_ (p_Data)
    , n_Type_ (n_Type)
{
    if (p_Data_ == 0)
    {
        throw Exception ("SharedResource: Out of memory");
    }
}

inline SharedResource::~SharedResource ()
{
    if ((References_() == 1) || (DecrementRefs_() == 0))
    {
        delete p_Data_;
    }
}

inline SharedResource& SharedResource::operator= (const SharedResource& shared)
{
    // the normal case is to not assign to ourself, so we don't waste time
    // checking for it explicitly (of course the logic below does handle it)

    shared.IncrementRefs_();

    SharedData* p_Delete = 0;   // don't delete anything by default

    if ((References_() == 1) || (DecrementRefs_() == 0))
    {
        p_Delete = p_Data_;     // ok, we do want to delete our old data
    }

    p_Data_ = shared.p_Data_;
    n_Type_ = shared.n_Type_;

    delete p_Delete;

    return *this;
}

inline SharedData* SharedResource::MakeCopyOfSharedData_ (const SharedData* p_OldData) const
{
    throw Exception ("SharedResource: copy on write not implemented");
}

inline SharedData* SharedResource::MakeEmptySharedData_ () const
{
    throw Exception ("SharedResource: no method to create data");
}

inline SharedData* SharedResource::MakeNewSharedData_ () const
{
    throw Exception ("SharedResource: no method to create data");
}

inline const SharedData* SharedResource::ViewData () const
{
    return p_Data_;
}

inline SharedData* SharedResource::ModifyData ()
{
    switch (n_Type_)
    {
        case RESOURCE_MUTABLE:
        {
            break;
        }

        case RESOURCE_COPY_ON_WRITE:
        {
            if (IsShared())
            {
                SharedData* p_Data = MakeCopyOfSharedData_ (p_Data_);

                if (DecrementRefs_() == 0)
                {
                    delete p_Data_;
                }

                p_Data_ = p_Data;
            }

            break;
        }

        case RESOURCE_NEW_ON_WRITE:
        {
            if (IsShared())
            {
                SharedData* p_Data = MakeNewSharedData_();

                if (DecrementRefs_() == 0)
                {
                    delete p_Data_;
                }

                p_Data_ = p_Data;
            }

            break;
        }
    }

    return p_Data_;
}

inline void SharedResource::ClearData ()
{
    SharedData* p_Data = MakeEmptySharedData_();

    if ((References_() == 1) || (DecrementRefs_() == 0))
    {
        delete p_Data_;
    }

    p_Data_ = p_Data;
}

inline void SharedResource::NewData ()
{
    SharedData* p_Data = MakeNewSharedData_();

    if ((References_() == 1) || (DecrementRefs_() == 0))
    {
        delete p_Data_;
    }

    p_Data_ = p_Data;
}

inline void SharedResource::Swap (SharedResource& resource)
{
    SharedData* p_Temp = p_Data_;

    p_Data_ = resource.p_Data_;

    resource.p_Data_ = p_Temp;
}

} // namespace mikestoolbox

