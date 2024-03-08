/*
  Copyright (C) 2002-2024 Michael S. D'Errico.  All Rights Reserved.

  This source code is the property of Michael S. D'Errico and is
  protected under international copyright laws.
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

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

#include "mikestoolbox-1.2.h"
#include "Test.h"

using namespace mikestoolbox;

String ToHex (uint32 u);

bool UTF8Test  ();
bool UTF16Test ();
bool UTF32Test ();

int main (int, char** argv)
{
    Tester check (argv[0]);

    try
    {
        // Constructors

        {
            std::string ss = "1234";
            const char* pz = "1234";
            int16       s  =  1234;
            int32       i  =  1234;
            int64       l  =  1234;
            uint16      us =  1234;
            uint32      ui =  1234;
            uint64      ul =  1234;
            double      d  =  1234.0;
            float       f  =  1234.0;

            String str1  (ss);
            String str2  (pz);
            String str3  (s);
            String str4  (i);
            String str5  (l);
            String str6  (us);
            String str7  (ui);
            String str8  (ul);
            String str9  (d);
            String str10 (f);

            check (str1 == str2);
            check (str2 == str3);
            check (str3 == str4);
            check (str4 == str5);
            check (str5 == str6);
            check (str6 == str7);
            check (str7 == str8);
            check (str8 == str9);
            check (str9 == str10);

            char  c  = 'a';
            uchar uc = 'a';

            String str11 (c,  Repeat(12));
            String str12 (uc, Repeat(12));

            check (str11 == "aaaaaaaaaaaa");
            check (str11 == str12);

            String str13 ("abcdefg", 4);

            check (str13 == "abcd");

            String str14 ("abc");

            str14.Append (str14);

            check (str14 == "abcabc");

            String str15 (str14);

            str15.Append (str15);

            check (str14 == "abcabc");
            check (str15 == "abcabcabcabc");

            String str16 (str15);

            check (str16 == "abcabcabcabc");

            str16.Destroy();

            check (str16.IsEmpty());
            check (str15 == "abcabcabcabc");

            String str17 (str15);

            check (str17 == "abcabcabcabc");

            str17.Clear();

            check (str17.IsEmpty());
            check (str15 == "abcabcabcabc");

            String str18 ("abc");

            str18.Prepend (str18);

            check (str18 == "abcabc");

            String str19 (str18);

            str19.Prepend (str19);

            check (str18 == "abcabc");
            check (str19 == "abcabcabcabc");

            String str20 (str19);
            String str21 (str19);
            String str22 (str19);

            str20.Insert ('x', str20.Length());

            check (str19 == "abcabcabcabc");
            check (str20 == "abcabcabcabcx");

            str21.Insert ('x', 0);

            check (str19 == "abcabcabcabc");
            check (str21 == "xabcabcabcabc");

            str22.Insert ("xyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxy", -1);

            check (str19 == "abcabcabcabc");
            check (str22 == "abcabcabcabxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyc");

            String str23 (str22);

            str23.Segment (1, str23.Length() - 4) = "FF";

            check (str22 == "abcabcabcabxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyxyc");
            check (str23 == "aFFxyc");
        }

        {
            String        str1 ("abc");
            const String& str2 (str1);

            str1.Append (str2);

            check (str1 == "abcabc");
            check (str2 == "abcabc");
        }

        {
            String str1 ("FooBarBaz");
            String str2 ("FOObarbAz");

            check (str1 != str2);

            str1.IgnoreCase();

            check (str1 == str2);
        }

        // regular expressions

        {
            String str1 ("Hello, world.");

            str1.Replace (".", "$& ", true);

            check (str1 == "H e l l o ,   w o r l d . ");
        }

        // string increment

        {
            String str1 ("3D4F");

            str1.IncrementBase16();

            check (str1 == "3D50");

            str1 = "az9Z";

            str1.IncrementASCII();

            check (str1 == "ba0A");

            str1 = "\x01\xFF\xFE";

            str1.IncrementBinary();

            check (str1 == "\x01\xFF\xFF");

            str1.IncrementBinary();

            String str2 ("\x02\x00\x00", 3);

            check (str1 == str2);
        }

        // Erase

        {
            String str1 ("abcdefghijklmnopqrstuvwxyz");
            String str2 (str1);

            str1.Erase (4, 2);

            check (str1 == "abcdghijklmnopqrstuvwxyz");

            str2 = str1;

            str1.Erase (-4, 2);

            check (str2 == "abcdghijklmnopqrstuvwxyz");
            check (str1 == "abcdghijklmnopqrstuvyz");

            str2 = str1;

            str1.Erase (20, 10);

            check (str1 == "abcdghijklmnopqrstuv");
            check (str2 == "abcdghijklmnopqrstuvyz");

            str2 = str1;

            str1.Erase (-2, 10);

            check (str1 == "abcdghijklmnopqrst");
            check (str2 == "abcdghijklmnopqrstuv");

            str2 = str1;

            str1.Erase (30, 5);

            check (str1 == "abcdghijklmnopqrst");
            check (str2 == "abcdghijklmnopqrst");

            str2 = str1;

            str1.Erase (-30, 5);

            check (str1 == "abcdghijklmnopqrst");
            check (str2 == "abcdghijklmnopqrst");

            str2 = str1;

            str1.EraseFront (3);

            check (str1 == "dghijklmnopqrst");
            check (str2 == "abcdghijklmnopqrst");

            str2 = str1;

            str1.EraseEnd (4);

            check (str1 == "dghijklmnop");
            check (str2 == "dghijklmnopqrst");
        }

        // logical operations

        {
            String str_X ("\x01\x23\x45\x67\x89\xAB\xCD\xEF");
            String str_Y ("\xFE\xDC\xBA\x98\x76\x54\x32\x10");
            String str_Z ((uchar)0x00, Repeat(8));

            String str1  ("\x01\x23\x45\x67\x89\xAB\xCD\xEF");
            String str2  ("\xFE\xDC\xBA\x98\x76\x54\x32\x10");
            String str3  ((uchar)0x00, Repeat(8));
            String str4  ((uchar)0xFF, Repeat(8));

            String str5 (str1);

            str5.AND (str1);

            check (str5 == str_X);

            str5.OR (str1);

            check (str5 == str_X);

            str5.NOT();

            check (str5 == str_Y);

            str5.AND (str1);

            check (str5 == str_Z);

            str5.OR (str2);

            check (str5 == str_Y);

            str5.XOR (str3);

            check (str5 == str_Y);

            str5.XOR (str4);

            check (str5 == str_X);

            str5.XNOR (str2);

            check (str5 == str_Z);

            str5.NOR (str1);

            check (str5 == str_Y);

            str5.NAND (str4);

            check (str5 == str_X);
        }

        {
            String str1 ("ABC ABC ABC");
            String str2 ("ABC ABC ABC");
            String str3 ("ABC ABC ABCD");
            String str4 ("ABC ABC ABA");

            uintsys u_Matches    = 0;
            uintsys u_Mismatches = 0;

            check ( str1.ByteCompare (str2, u_Matches, u_Mismatches));
            check (u_Matches == str1.Length());
            check (u_Mismatches == 0);

            check (!str1.ByteCompare (str3, u_Matches, u_Mismatches));
            check (u_Matches == str1.Length());
            check (u_Mismatches == 1);

            check (!str1.ByteCompare (str4, u_Matches, u_Mismatches));
            check (u_Matches == str1.Length() - 1);
            check (u_Mismatches == 1);

            check (!str3.ByteCompare (str4, u_Matches, u_Mismatches));
            check (u_Matches == str4.Length() - 1);
            check (u_Mismatches == 2);
        }

        {
            String str ("\x01\x23\x45\x67\x89\xAB\xCD\xEF\x01");

            StringIter iter (str);

            ParseError error;

            uintsys u = 0;

            check (iter.ExtractUint16 (u, error) && (u == 0x0123));
            check (iter.ExtractUint24 (u, error) && (u == 0x456789));
            check (iter.ExtractUint32 (u, error) && (u == 0xABCDEF01));
            check (!iter);

            iter = str;

            iter.SetLittleEndian();

            check (iter.ExtractUint16 (u, error) && (u == 0x2301));
            check (iter.ExtractUint24 (u, error) && (u == 0x896745));
            check (iter.ExtractUint32 (u, error) && (u == 0x01EFCDAB));
            check (!iter);
        }

        {
            // 10100100 01000010 00001000 00010000 00010000 00001000 00000011

            String str ("\xA4\x42\x08\x10\x10\x08\x03");

            StringBitIter iter (str);

            ParseError error;

            uintsys u = 0;

            check (iter.ExtractBits (0,  u, error) && (u == 0));
            check (iter.ExtractBits (1,  u, error) && (u == 1));
            check (iter.ExtractBits (2,  u, error) && (u == 1));
            check (iter.ExtractBits (3,  u, error) && (u == 1));
            check (iter.ExtractBits (4,  u, error) && (u == 1));
            check (iter.ExtractBits (5,  u, error) && (u == 1));
            check (iter.ExtractBits (6,  u, error) && (u == 1));
            check (iter.ExtractBits (7,  u, error) && (u == 1));
            check (iter.ExtractBits (8,  u, error) && (u == 1));
            check (iter.ExtractBits (9,  u, error) && (u == 1));
            check (iter.ExtractBits (10, u, error) && (u == 1));
            check (!iter.ExtractBits (2, u, error));
            check (iter);
            check (iter.ExtractBits (1, u, error) && (u == 1));
            check (!iter);
        }
        
        {
            // 01001011 10000100 00100000 00010000 00010000 00100000 10000000

            String str ("\x4B\x84\x20\x10\x10\x20\x80");

            StringBitIter iter (str);

            iter.SetLittleEndian();

            ParseError error;

            uintsys u = 0;

            check (iter.ExtractBits (0,  u, error) && (u == 0));
            check (iter.ExtractBits (1,  u, error) && (u == 1));
            check (iter.ExtractBits (2,  u, error) && (u == 1));
            check (iter.ExtractBits (3,  u, error) && (u == 1));
            check (iter.ExtractBits (4,  u, error) && (u == 1));
            check (iter.ExtractBits (5,  u, error) && (u == 1));
            check (iter.ExtractBits (6,  u, error) && (u == 1));
            check (iter.ExtractBits (7,  u, error) && (u == 1));
            check (iter.ExtractBits (8,  u, error) && (u == 1));
            check (iter.ExtractBits (9,  u, error) && (u == 1));
            check (iter.ExtractBits (10, u, error) && (u == 1));
            check (!iter.ExtractBits (2, u, error));
            check (iter);
            check (iter.ExtractBits (1, u, error) && (u == 1));
            check (!iter);
        }

        // Unicode

        check (UTF8Test());
        check (UTF16Test());
        check (UTF32Test());
    }
    catch (Exception& e)
    {
        std::cout << "Exception caught: " << e.Message() << std::endl;
    }

    check.Done();

    return 0;
}

bool UTF8Test ()
{
    String str_UTF8 (Preallocate(0x110000*4));

    for (uintsys u=0; u<0x110000; ++u)
    {
        str_UTF8.AppendUTF8 (u);
    }

    StringIter iter (str_UTF8);
    ParseError error;
    uintsys    u_CodePoint = 0;

    for (uintsys u=0; u<0x110000; ++u)
    {
        if (!(iter.ParseUTF8Char (u_CodePoint, error) && (u == u_CodePoint)))
        {
            std::cout << "UTF8Test: " << u << std::endl;

            return false;
        }
    }

    return true;
}

bool UTF16Test ()
{
    String str_UTF16 (Preallocate(0x110000*4));

    for (uintsys u=0; u<0x110000; ++u)
    {
        if (u == 0xD800)
        {
            u = 0xE000;
        }

        str_UTF16.AppendUTF16 (u);
    }

    StringIter iter (str_UTF16);
    ParseError error;
    uintsys    u_CodePoint = 0;

    for (uintsys u=0; u<0x110000; ++u)
    {
        if (u == 0xD800)
        {
            u = 0xE000;
        }

        if (!(iter.ParseUTF16Char (u_CodePoint, error) && (u == u_CodePoint)))
        {
            std::cout << "UTF16Test: " << u << std::endl;

            return false;
        }
    }

    return true;
}

bool UTF32Test ()
{
    String str_UTF32 (Preallocate(0x110000*4));

    for (uintsys u=0; u<0x110000; ++u)
    {
        if (u == 0xD800)
        {
            u = 0xE000;
        }

        str_UTF32.AppendUTF32 (u);
    }

    StringIter iter (str_UTF32);
    ParseError error;
    uintsys    u_CodePoint = 0;

    for (uintsys u=0; u<0x110000; ++u)
    {
        if (u == 0xD800)
        {
            u = 0xE000;
        }

        if (!(iter.ParseUTF32Char (u_CodePoint, error) && (u == u_CodePoint)))
        {
            std::cout << "UTF32Test: " << u << std::endl;

            return false;
        }
    }

    return true;
}

String ToHex (uint32 u)
{
    static const char* pz_Hex = "0123456789ABCDEF";

    String str_Hex ("0x");

    if (u <= 0xFF)
    {
        uchar u1 = (u >> 4) & 0x0F;
        uchar u2 =  u       & 0x0F;

        str_Hex += pz_Hex[u1];
        str_Hex += pz_Hex[u2];
    }
    else if (u <= 0xFFFF)
    {
        uchar u1 = (u >> 12) & 0x0F;
        uchar u2 = (u >>  8) & 0x0F;
        uchar u3 = (u >>  4) & 0x0F;
        uchar u4 =  u        & 0x0F;

        str_Hex += pz_Hex[u1];
        str_Hex += pz_Hex[u2];
        str_Hex += pz_Hex[u3];
        str_Hex += pz_Hex[u4];
    }
    else if (u <= 0xFFFFFF)
    {
        uchar u1 = (u >> 20) & 0x0F;
        uchar u2 = (u >> 16) & 0x0F;
        uchar u3 = (u >> 12) & 0x0F;
        uchar u4 = (u >>  8) & 0x0F;
        uchar u5 = (u >>  4) & 0x0F;
        uchar u6 =  u        & 0x0F;

        str_Hex += pz_Hex[u1];
        str_Hex += pz_Hex[u2];
        str_Hex += pz_Hex[u3];
        str_Hex += pz_Hex[u4];
        str_Hex += pz_Hex[u5];
        str_Hex += pz_Hex[u6];
    }
    else
    {
        uchar u1 = (u >> 28) & 0x0F;
        uchar u2 = (u >> 24) & 0x0F;
        uchar u3 = (u >> 20) & 0x0F;
        uchar u4 = (u >> 16) & 0x0F;
        uchar u5 = (u >> 12) & 0x0F;
        uchar u6 = (u >>  8) & 0x0F;
        uchar u7 = (u >>  4) & 0x0F;
        uchar u8 =  u        & 0x0F;

        str_Hex += pz_Hex[u1];
        str_Hex += pz_Hex[u2];
        str_Hex += pz_Hex[u3];
        str_Hex += pz_Hex[u4];
        str_Hex += pz_Hex[u5];
        str_Hex += pz_Hex[u6];
        str_Hex += pz_Hex[u7];
        str_Hex += pz_Hex[u8];
    }

    return str_Hex;
}

