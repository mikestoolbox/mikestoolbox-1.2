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
//  File:       Regex.cpp
//
//  Synopsis:   Implementation of methods for objects that represent
//              regular expressions
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

PerlRegex::PerlRegex (const String& str_Pattern, const PerlRegexOptions& options)
    : SharedResource (RESOURCE_NEW_ON_WRITE, new(std::nothrow) PerlRegexData)
{
    int n_Options = (options.b_AnchoredPattern_ ? PCRE_ANCHORED  : 0)                   |
                    (options.b_CaseSensitive_   ? 0              : PCRE_CASELESS)       |
                    (options.b_DollarNewline_   ? 0              : PCRE_DOLLAR_ENDONLY) |
                    (options.b_DotAll_          ? PCRE_DOTALL    : 0)                   |
                    (options.b_Extended_        ? PCRE_EXTENDED  : 0)                   |
                    (options.b_FirstLine_       ? PCRE_FIRSTLINE : 0)                   |
                    (options.b_Greedy_          ? 0              : PCRE_UNGREEDY)       |
                    (options.b_Multiline_       ? PCRE_MULTILINE : 0)                   |
                    (options.b_UTF8_            ? PCRE_UTF8      : 0)                   |
                     options.n_Newline_;

    PerlRegexData* p_Data = ModifyData();

    p_Data->pcre_ = pcre_compile (str_Pattern.C(),                // the Perl-style regex
                                  n_Options,                      // compile options
                                  &p_Data->pz_CompileError_,      // text error message
                                  &p_Data->n_CompileErrorOffset_, // location of the error
                                  0);                             // character mapping table

    if (options.b_Study_ && p_Data->pcre_)
    {
        const char* pz_ErrorMessage = 0;

        p_Data->pcre_extra_ = pcre_study (p_Data->pcre_,     // compiled pattern
                                          0,                 // options (none defined)
                                          &pz_ErrorMessage); // returned error message
    }
}

PerlRegex::PerlRegex (const String& str_Pattern)
    : SharedResource (RESOURCE_NEW_ON_WRITE, new(std::nothrow) PerlRegexData)
{
    int n_Options =  0;

    PerlRegexData* p_Data = ModifyData();

    p_Data->pcre_ = pcre_compile (str_Pattern.C(),                // the Perl-style regex
                                  n_Options,                      // compile options
                                  &p_Data->pz_CompileError_,      // text error message
                                  &p_Data->n_CompileErrorOffset_, // location of the error
                                  0);                             // character mapping table
}

PerlRegex::PerlRegex (const char* pz_Pattern, const PerlRegexOptions& options)
    : SharedResource (RESOURCE_NEW_ON_WRITE, new(std::nothrow) PerlRegexData)
{
    int n_Options = (options.b_AnchoredPattern_ ? PCRE_ANCHORED  : 0)                   |
                    (options.b_CaseSensitive_   ? 0              : PCRE_CASELESS)       |
                    (options.b_DollarNewline_   ? 0              : PCRE_DOLLAR_ENDONLY) |
                    (options.b_DotAll_          ? PCRE_DOTALL    : 0)                   |
                    (options.b_Extended_        ? PCRE_EXTENDED  : 0)                   |
                    (options.b_FirstLine_       ? PCRE_FIRSTLINE : 0)                   |
                    (options.b_Greedy_          ? 0              : PCRE_UNGREEDY)       |
                    (options.b_Multiline_       ? PCRE_MULTILINE : 0)                   |
                    (options.b_UTF8_            ? PCRE_UTF8      : 0)                   |
                     options.n_Newline_;

    PerlRegexData* p_Data = ModifyData();

    p_Data->pcre_ = pcre_compile (pz_Pattern,                     // the Perl-style regex
                                  n_Options,                      // compile options
                                  &p_Data->pz_CompileError_,      // text error message
                                  &p_Data->n_CompileErrorOffset_, // location of the error
                                  0);                             // character mapping table

    if (options.b_Study_ && p_Data->pcre_)
    {
        const char* pz_ErrorMessage = 0;

        p_Data->pcre_extra_ = pcre_study (p_Data->pcre_,     // compiled pattern
                                          0,                 // options (none defined)
                                          &pz_ErrorMessage); // returned error message
    }
}

PerlRegex::PerlRegex (const char* pz_Pattern)
    : SharedResource (RESOURCE_NEW_ON_WRITE, new(std::nothrow) PerlRegexData)
{
    int n_Options = 0;

    PerlRegexData* p_Data = ModifyData();

    p_Data->pcre_ = pcre_compile (pz_Pattern,                     // the Perl-style regex
                                  n_Options,                      // compile options
                                  &p_Data->pz_CompileError_,      // text error message
                                  &p_Data->n_CompileErrorOffset_, // location of the error
                                  0);                             // character mapping table
}

intsys PerlRegex::Match (const char* ps, uintsys u_Length, const PerlRegexOptions& options,
                         PerlRegexMatches& matches) const
{
    const PerlRegexData* p_Data = ViewData();

    if (p_Data->pcre_ == 0)
    {
        return -1;
    }

    int n_Options = (options.b_AnchoredMatch_ ? PCRE_ANCHORED : 0) |
                     options.n_Newline_;

    int n_Result = pcre_exec (p_Data->pcre_,          // compiled pattern
                              p_Data->pcre_extra_,    // studied info
                              ps,                     // string to match
                              u_Length,               // length of string to match
                              options.n_Offset_,      // where to start matching
                              n_Options,              // options
                              matches.an_Substrings_, // substring matches
                              303);                   // substring array size

    if (n_Result < 0)
    {
        matches.n_NumSubstrings_ = 0;

        return -1;
    }

    matches.ps_              = ps;
    matches.n_NumSubstrings_ = n_Result;

    return matches.an_Substrings_[0];
}

const String PerlRegexMatches::GetMatch (uintsys u_Index) const
{
    String str_Substring;

    if (u_Index < static_cast<uintsys> (n_NumSubstrings_))
    {
        int n_Offset = an_Substrings_[2*u_Index];
        int n_Length = an_Substrings_[2*u_Index+1] - n_Offset;

        uchar* puc = str_Substring.Allocate (n_Length);

        std::memcpy (puc, ps_ + n_Offset, n_Length);
    }

    return str_Substring;
}

} // namespace mikestoolbox

