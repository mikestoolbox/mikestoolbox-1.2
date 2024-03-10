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
//  File:       PerlRegex.inl
//
//  Synopsis:   Implementation of methods for objects that represent regular
//              expressions
//----------------------------------------------------------------------------

namespace mikestoolbox {

#ifndef PCRE_NEWLINE_CR
#define PCRE_NEWLINE_CR   0
#define PCRE_NEWLINE_CRLF 0
#define PCRE_NEWLINE_LF   0
#define PCRE_NEWLINE_ANY  0
#endif

inline PerlRegexOptions::PerlRegexOptions ()
    : b_AnchoredMatch_   (false)
    , b_AnchoredPattern_ (false)
    , b_CaseSensitive_   (true)
    , b_DollarNewline_   (true)
    , b_DotAll_          (false)
    , b_Extended_        (false)
    , b_FirstLine_       (false)
    , b_Greedy_          (true)
    , b_Multiline_       (false)
    , b_Study_           (false)
    , b_UTF8_            (false)
    , n_Newline_         (PCRE_NEWLINE_LF)
    , n_Offset_          (0)
{
    // nothing
}

inline PerlRegexOptions& PerlRegexOptions::AnchoredPattern (bool b)
{
    b_AnchoredPattern_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::CaseSensitive (bool b)
{
    b_CaseSensitive_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::DollarMatchesNewline (bool b)
{
    b_DollarNewline_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::DotMatchesAnyChar (bool b)
{
    b_DotAll_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::ExtendedPattern (bool b)
{
    b_Extended_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::FirstLineMatching (bool b)
{
    b_FirstLine_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::GreedyMatching (bool b)
{
    b_Greedy_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::MultilineMatching (bool b)
{
    b_Multiline_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::StudyPattern (bool b)
{
    b_Study_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::UTF8Mode (bool b)
{
    b_UTF8_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::NewlineIsCR ()
{
    n_Newline_ = PCRE_NEWLINE_CR;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::NewlineIsCRLF ()
{
    n_Newline_ = PCRE_NEWLINE_CRLF;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::NewlineIsLF ()
{
    n_Newline_ = PCRE_NEWLINE_LF;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::NewlineIsAny ()
{
    n_Newline_ = PCRE_NEWLINE_ANY;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::AnchoredMatching (bool b)
{
    b_AnchoredMatch_ = b;

    return *this;
}

inline PerlRegexOptions& PerlRegexOptions::StartingOffset (intsys n_Offset)
{
    n_Offset_ = n_Offset;

    return *this;
}

inline PerlRegexData::PerlRegexData ()
    : pcre_                 (0)
    , pcre_extra_           (0)
    , pz_CompileError_      (0)
    , n_CompileErrorOffset_ (-1)
{
    // nothing
}

inline PerlRegexData::~PerlRegexData ()
{
    pcre_free (pcre_);
    pcre_free (pcre_extra_);
}

inline const PerlRegexData* PerlRegex::ViewData () const
{
    return (const PerlRegexData*) SharedResource::ViewData();
}

inline PerlRegexData* PerlRegex::ModifyData ()
{
    return (PerlRegexData*) SharedResource::ModifyData();
}

inline PerlRegexData* PerlRegex::MakeNewSharedData () const
{
    PerlRegexData* p_NewData = new(std::nothrow) PerlRegexData;

    if (p_NewData == 0)
    {
        throw Exception ("PerlRegex: Out of memory");
    }

    return p_NewData;
}

inline void PerlRegex::Swap (PerlRegex& regex)
{
    SharedResource::Swap (regex);
}

inline PerlRegex::operator bool () const
{
    return (ViewData()->pcre_ != 0);
}

inline const char* PerlRegex::Error () const
{
    return ViewData()->pz_CompileError_;
}

inline int PerlRegex::ErrorOffset () const
{
    return ViewData()->n_CompileErrorOffset_;
}

inline intsys PerlRegex::Match (const uchar* ps, uintsys u_Length,
                                const PerlRegexOptions& options,
                                PerlRegexMatches& matches) const
{
    return Match ((const char*)ps, u_Length, options, matches);
}

inline intsys PerlRegex::Match (const String& str, const PerlRegexOptions& options,
                                PerlRegexMatches& matches) const
{
    return Match (str.PointerToFirstChar(), str.Length(), options, matches);
}

inline PerlRegexMatches::PerlRegexMatches ()
    : ps_              (0)
    , n_NumSubstrings_ (0)
{
    // nothing
}

inline uintsys PerlRegexMatches::GetMatchLength (uintsys u_Index) const
{
    if (u_Index < (uint)n_NumSubstrings_)
    {
        return an_Substrings_[2*u_Index+1] - an_Substrings_[2*u_Index];
    }

    return 0;
}

inline const StringList PerlRegexMatches::GetMatches () const
{
    StringList strl;

    for (int n=0; n<n_NumSubstrings_; ++n)
    {
        strl.Append (GetMatch (n));
    }

    return strl;
}

} // namespace mikestoolbox

namespace std {

template<>
inline void swap<mikestoolbox::PerlRegex> (mikestoolbox::PerlRegex& regex1,
                                           mikestoolbox::PerlRegex& regex2)
{
    regex1.Swap (regex2);
}

} // namespace std

