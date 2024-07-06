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
//  File:       TimeZone.cpp
//
//  Synopsis:   Convert timezone name to corresponding offset (minutes)
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

#define UINT16(a,b)     (((a)<<8)+(b))
#define UINT24(a,b,c)   (((a)<<16)+UINT16(b,c))
#define UINT32(a,b,c,d) (((a)<<24)+UINT24(b,c,d))
#define SPLIT(x)        (((x)/100)*60+((x)%100))
#define OFFSET(tz)      ((tz)<0 ? -SPLIT(-(tz)) : SPLIT(tz))

static intsys DecodeNumericTimeZone (const String& str_Zone)
{
    StringIter iter (str_Zone);

    ParseError error;

    uchar   uc_Sign  = 0;
    uintsys u_Offset = 0;
    intsys  n_Offset = 0;

    if (iter.ExtractByte (uc_Sign, error) && iter.ParseNumber (u_Offset, error))
    {
        n_Offset = SPLIT(u_Offset) * ((uc_Sign == '-') ? -1 : 1);
    }

    return n_Offset;
}

intsys DecodeTimeZoneName (const String& str_Zone)       // NO_METRIC
{
    StringIter iter (str_Zone);

    uintsys u_Zone = 0;

    ParseError error;

    if ((iter[0] == '+') || (iter[0] == '-'))
    {
        return DecodeNumericTimeZone (str_Zone);
    }

    if (!(iter.ExtractUint32BE (u_Zone, error) ||
          iter.ExtractUint24BE (u_Zone, error) ||
          iter.ExtractUint16BE (u_Zone, error)))
    {
        return 0;
    }

    switch (u_Zone)
    {
        // USA timezones

        case UINT24('E','D','T'):     return OFFSET(-400);
        case UINT24('E','S','T'):     return OFFSET(-500);
        case UINT24('C','D','T'):     return OFFSET(-500);
        case UINT24('C','S','T'):     return OFFSET(-600);
        case UINT24('M','D','T'):     return OFFSET(-600);
        case UINT24('M','S','T'):     return OFFSET(-700);
        case UINT24('P','D','T'):     return OFFSET(-700);
        case UINT24('P','S','T'):     return OFFSET(-800);

        // International timezones

        case UINT16('C','T'):         return OFFSET(800);
        case UINT16('N','T'):         return OFFSET(-330);

        case UINT24('A','C','T'):     return OFFSET(800);
        case UINT24('A','D','T'):     return OFFSET(-300);
        case UINT24('A','F','T'):     return OFFSET(430);
        case UINT24('A','R','T'):     return OFFSET(-300);
        case UINT24('A','Z','T'):     return OFFSET(400);
        case UINT24('B','D','T'):     return OFFSET(800);
        case UINT24('B','I','T'):     return OFFSET(-1200);
        case UINT24('B','O','T'):     return OFFSET(-400);
        case UINT24('B','R','T'):     return OFFSET(-300);
        case UINT24('B','S','T'):     return OFFSET(600);
        case UINT24('B','T','T'):     return OFFSET(600);
        case UINT24('C','A','T'):     return OFFSET(200);
        case UINT24('C','C','T'):     return OFFSET(630);
        case UINT24('C','E','T'):     return OFFSET(100);
        case UINT24('C','I','T'):     return OFFSET(800);
        case UINT24('C','K','T'):     return OFFSET(-1000);
        case UINT24('C','L','T'):     return OFFSET(-400);
        case UINT24('C','O','T'):     return OFFSET(-500);
        case UINT24('C','V','T'):     return OFFSET(-100);
        case UINT24('C','X','T'):     return OFFSET(700);
        case UINT24('D','F','T'):     return OFFSET(100);
        case UINT24('E','A','T'):     return OFFSET(300);
        case UINT24('E','E','T'):     return OFFSET(200);
        case UINT24('E','G','T'):     return OFFSET(-100);
        case UINT24('E','I','T'):     return OFFSET(900);
        case UINT24('F','E','T'):     return OFFSET(300);
        case UINT24('F','J','T'):     return OFFSET(1200);
        case UINT24('F','K','T'):     return OFFSET(-400);
        case UINT24('F','N','T'):     return OFFSET(-200);
        case UINT24('G','E','T'):     return OFFSET(400);
        case UINT24('G','F','T'):     return OFFSET(-300);
        case UINT24('G','I','T'):     return OFFSET(-900);
        case UINT24('G','Y','T'):     return OFFSET(-400);
        case UINT24('H','K','T'):     return OFFSET(800);
        case UINT24('H','M','T'):     return OFFSET(500);
        case UINT24('H','S','T'):     return OFFSET(-1000);
        case UINT24('I','C','T'):     return OFFSET(700);
        case UINT24('I','D','T'):     return OFFSET(300);
        case UINT24('I','O','T'):     return OFFSET(300);
        case UINT24('J','S','T'):     return OFFSET(900);
        case UINT24('K','G','T'):     return OFFSET(600);
        case UINT24('K','S','T'):     return OFFSET(900);
        case UINT24('M','E','T'):     return OFFSET(100);
        case UINT24('M','H','T'):     return OFFSET(1200);
        case UINT24('M','I','T'):     return OFFSET(-930);
        case UINT24('M','M','T'):     return OFFSET(630);
        case UINT24('M','S','K'):     return OFFSET(300);
        case UINT24('M','U','T'):     return OFFSET(400);
        case UINT24('M','V','T'):     return OFFSET(500);
        case UINT24('M','Y','T'):     return OFFSET(800);
        case UINT24('N','C','T'):     return OFFSET(1100);
        case UINT24('N','D','T'):     return OFFSET(-230);
        case UINT24('N','F','T'):     return OFFSET(1130);
        case UINT24('N','P','T'):     return OFFSET(545);
        case UINT24('N','S','T'):     return OFFSET(-330);
        case UINT24('N','U','T'):     return OFFSET(-1100);
        case UINT24('P','E','T'):     return OFFSET(-500);
        case UINT24('P','G','T'):     return OFFSET(1000);
        case UINT24('P','K','T'):     return OFFSET(500);
        case UINT24('P','Y','T'):     return OFFSET(-400);
        case UINT24('R','E','T'):     return OFFSET(400);
        case UINT24('S','B','T'):     return OFFSET(1100);
        case UINT24('S','C','T'):     return OFFSET(400);
        case UINT24('S','G','T'):     return OFFSET(800);
        case UINT24('S','R','T'):     return OFFSET(-300);
        case UINT24('T','H','A'):     return OFFSET(700);
        case UINT24('T','F','T'):     return OFFSET(500);
        case UINT24('T','J','T'):     return OFFSET(500);
        case UINT24('T','K','T'):     return OFFSET(1300);
        case UINT24('T','L','T'):     return OFFSET(900);
        case UINT24('T','M','T'):     return OFFSET(500);
        case UINT24('T','O','T'):     return OFFSET(1300);
        case UINT24('T','V','T'):     return OFFSET(1200);
        case UINT24('U','Y','T'):     return OFFSET(-300);
        case UINT24('U','Z','T'):     return OFFSET(500);
        case UINT24('V','E','T'):     return OFFSET(-430);
        case UINT24('V','U','T'):     return OFFSET(1100);
        case UINT24('W','A','T'):     return OFFSET(100);
        case UINT24('W','I','T'):     return OFFSET(700);
        case UINT24('W','S','T'):     return OFFSET(800);

        case UINT32('A','C','D','T'): return OFFSET(1030);
        case UINT32('A','C','S','T'): return OFFSET(930);
        case UINT32('A','E','D','T'): return OFFSET(1100);
        case UINT32('A','E','S','T'): return OFFSET(1000);
        case UINT32('A','K','D','T'): return OFFSET(-800);
        case UINT32('A','K','S','T'): return OFFSET(-900);
        case UINT32('A','W','D','T'): return OFFSET(900);
        case UINT32('A','W','S','T'): return OFFSET(800);
        case UINT32('B','I','O','T'): return OFFSET(600);
        case UINT32('C','E','D','T'): return OFFSET(200);
        case UINT32('C','E','S','T'): return OFFSET(200);
        case UINT32('C','H','O','T'): return OFFSET(800);
        case UINT32('C','h','S','T'): return OFFSET(1000);
        case UINT32('C','H','U','T'): return OFFSET(1000);
        case UINT32('C','I','S','T'): return OFFSET(-800);
        case UINT32('C','L','S','T'): return OFFSET(-300);
        case UINT32('C','O','S','T'): return OFFSET(-400);
        case UINT32('C','W','S','T'): return OFFSET(845);
        case UINT32('D','A','V','T'): return OFFSET(700);
        case UINT32('D','D','U','T'): return OFFSET(1000);
        case UINT32('E','A','S','T'): return OFFSET(-600);
        case UINT32('E','E','D','T'): return OFFSET(300);
        case UINT32('E','E','S','T'): return OFFSET(300);
        case UINT32('E','G','S','T'): return OFFSET(000);
        case UINT32('F','K','S','T'): return OFFSET(-300);
        case UINT32('G','A','L','T'): return OFFSET(-600);
        case UINT32('G','A','M','T'): return OFFSET(-900);
        case UINT32('G','I','L','T'): return OFFSET(1200);
        case UINT32('H','A','D','T'): return OFFSET(-900);
        case UINT32('H','A','E','C'): return OFFSET(200);
        case UINT32('H','A','S','T'): return OFFSET(-1000);
        case UINT32('H','O','V','T'): return OFFSET(700);
        case UINT32('I','R','D','T'): return OFFSET(430);
        case UINT32('I','R','K','T'): return OFFSET(800);
        case UINT32('I','R','S','T'): return OFFSET(330);
        case UINT32('K','O','S','T'): return OFFSET(1100);
        case UINT32('K','R','A','T'): return OFFSET(700);
        case UINT32('L','I','N','T'): return OFFSET(1400);
        case UINT32('M','A','G','T'): return OFFSET(1200);
        case UINT32('M','A','R','T'): return OFFSET(-930);
        case UINT32('M','A','W','T'): return OFFSET(500);
        case UINT32('M','E','S','T'): return OFFSET(200);
        case UINT32('M','I','S','T'): return OFFSET(1100);
        case UINT32('N','Z','D','T'): return OFFSET(1300);
        case UINT32('N','Z','S','T'): return OFFSET(1200);
        case UINT32('O','M','S','T'): return OFFSET(600);
        case UINT32('O','R','A','T'): return OFFSET(500);
        case UINT32('P','E','T','T'): return OFFSET(1200);
        case UINT32('P','H','O','T'): return OFFSET(1300);
        case UINT32('P','M','D','T'): return OFFSET(-200);
        case UINT32('P','M','S','T'): return OFFSET(-300);
        case UINT32('P','O','N','T'): return OFFSET(1100);
        case UINT32('P','Y','S','T'): return OFFSET(-300);
        case UINT32('R','O','T','T'): return OFFSET(-300);
        case UINT32('S','A','K','T'): return OFFSET(1100);
        case UINT32('S','A','M','T'): return OFFSET(400);
        case UINT32('S','A','S','T'): return OFFSET(200);
        case UINT32('S','L','S','T'): return OFFSET(530);
        case UINT32('S','R','E','T'): return OFFSET(1100);
        case UINT32('S','Y','O','T'): return OFFSET(300);
        case UINT32('T','A','H','T'): return OFFSET(-1000);
        case UINT32('U','L','A','T'): return OFFSET(800);
        case UINT32('U','S','Z','1'): return OFFSET(200);
        case UINT32('U','Y','S','T'): return OFFSET(-200);
        case UINT32('V','L','A','T'): return OFFSET(1000);
        case UINT32('V','O','L','T'): return OFFSET(400);
        case UINT32('V','O','S','T'): return OFFSET(600);
        case UINT32('W','A','K','T'): return OFFSET(1200);
        case UINT32('W','A','S','T'): return OFFSET(200);
        case UINT32('W','E','D','T'): return OFFSET(100);
        case UINT32('W','E','S','T'): return OFFSET(100);
        case UINT32('Y','A','K','T'): return OFFSET(900);
        case UINT32('Y','E','K','T'): return OFFSET(500);
        case UINT32('A','Z','O','S'): return OFFSET(-100);  // AZOST
        case UINT32('C','H','A','D'): return OFFSET(1345);  // CHADT
        case UINT32('C','H','A','S'): return OFFSET(1245);  // CHAST
        case UINT32('E','A','S','S'): return OFFSET(-500);  // EASST

        // equivalent to UTC:   Z, GMT, UCT, WET

        // ambiguous timezone abbreviations:

        /*
            AMT     UTC−04
            AMT     UTC+04
            AST     UTC+03
            AST     UTC−04
            CDT     UTC−05
            CDT     UTC−04
            CST     UTC−06
            CST     UTC+08
            CST     UTC+09:30
            CST     UTC+10:30
            CST     UTC−05
            ECT     UTC−04
            ECT     UTC−05
            EST     UTC−05
            EST     UTC+10
            GST     UTC−02
            GST     UTC+04
            IST     UTC+05:30
            IST     UTC+01
            IST     UTC+02
            MST     UTC+08
            MST     UTC−07
            MST     UTC+06:30
            PST     UTC−08
            PST     UTC+08
            SST     UTC−11
            SST     UTC+08
            AMST    UTC−03
            AMST    UTC+05
            LHST    UTC+10:30
            LHST    UTC+11
        */
    }

    return 0;
}

} // namespace mikestoolbox

/*
    // timezone names and offsets are from Wikipedia (consulted 10 Feb 2015)

    ACDT    UTC+10:30  Australian Central Daylight Savings Time
    ACST    UTC+09:30  Australian Central Standard Time
    ACT     UTC+08     ASEAN Common Time
    ADT     UTC−03     Atlantic Daylight Time
    AEDT    UTC+11     Australian Eastern Daylight Savings Time
    AEST    UTC+10     Australian Eastern Standard Time
    AFT     UTC+04:30  Afghanistan Time
    AKDT    UTC−08     Alaska Daylight Time
    AKST    UTC−09     Alaska Standard Time
    AMST    UTC−03     Amazon Summer Time (Brazil)[1]
    AMST    UTC+05     Armenia Summer Time
    AMT     UTC−04     Amazon Time (Brazil)[2]
    AMT     UTC+04     Armenia Time
    ART     UTC−03     Argentina Time
    AST     UTC+03     Arabia Standard Time
    AST     UTC−04     Atlantic Standard Time
    AWDT    UTC+09     Australian Western Daylight Time
    AWST    UTC+08     Australian Western Standard Time
    AZOST   UTC−01     Azores Standard Time
    AZT     UTC+04     Azerbaijan Time
    BDT     UTC+08     Brunei Time
    BIOT    UTC+06     British Indian Ocean Time
    BIT     UTC−12     Baker Island Time
    BOT     UTC−04     Bolivia Time
    BRT     UTC−03     Brasilia Time
    BST     UTC+06     Bangladesh Standard Time
    BST     UTC+01     British Summer Time (British Standard Time from
                           Feb 1968 to Oct 1971)
    BTT     UTC+06     Bhutan Time
    CAT     UTC+02     Central Africa Time
    CCT     UTC+06:30  Cocos Islands Time
    CDT     UTC−05     Central Daylight Time (North America)
    CDT     UTC−04     Cuba Daylight Time[3]
    CEDT    UTC+02     Central European Daylight Time
    CEST    UTC+02     Central European Summer Time (Cf. HAEC)
    CET     UTC+01     Central European Time
    CHADT   UTC+13:45  Chatham Daylight Time
    CHAST   UTC+12:45  Chatham Standard Time
    CHOT    UTC+08     Choibalsan
    ChST    UTC+10     Chamorro Standard Time
    CHUT    UTC+10     Chuuk Time
    CIST    UTC−08     Clipperton Island Standard Time
    CIT     UTC+08     Central Indonesia Time
    CKT     UTC−10     Cook Island Time
    CLST    UTC−03     Chile Summer Time
    CLT     UTC−04     Chile Standard Time
    COST    UTC−04     Colombia Summer Time
    COT     UTC−05     Colombia Time
    CST     UTC−06     Central Standard Time (North America)
    CST     UTC+08     China Standard Time
    CST     UTC+09:30  Central Standard Time (Australia)
    CST     UTC+10:30  Central Summer Time (Australia)
    CST     UTC−05     Cuba Standard Time
    CT      UTC+08     China time
    CVT     UTC−01     Cape Verde Time
    CWST    UTC+08:45  Central Western Standard Time (Australia) unofficial
    CXT     UTC+07     Christmas Island Time
    DAVT    UTC+07     Davis Time
    DDUT    UTC+10     Dumont d'Urville Time
    DFT     UTC+01     AIX specific equivalent of Central European Time[4]
    EASST   UTC−05     Easter Island Standard Summer Time
    EAST    UTC−06     Easter Island Standard Time
    EAT     UTC+03     East Africa Time
    ECT     UTC−04     Eastern Caribbean Time (does not recognise DST)
    ECT     UTC−05     Ecuador Time
    EDT     UTC−04     Eastern Daylight Time (North America)
    EEDT    UTC+03     Eastern European Daylight Time
    EEST    UTC+03     Eastern European Summer Time
    EET     UTC+02     Eastern European Time
    EGST    UTC+00     Eastern Greenland Summer Time
    EGT     UTC−01     Eastern Greenland Time
    EIT     UTC+09     Eastern Indonesian Time
    EST     UTC−05     Eastern Standard Time (North America)
    EST     UTC+10     Eastern Standard Time (Australia)
    FET     UTC+03     Further-eastern European Time
    FJT     UTC+12     Fiji Time
    FKST    UTC−03     Falkland Islands Standard Time
    FKST    UTC−03     Falkland Islands Summer Time
    FKT     UTC−04     Falkland Islands Time
    FNT     UTC−02     Fernando de Noronha Time
    GALT    UTC−06     Galapagos Time
    GAMT    UTC−09     Gambier Islands
    GET     UTC+04     Georgia Standard Time
    GFT     UTC−03     French Guiana Time
    GILT    UTC+12     Gilbert Island Time
    GIT     UTC−09     Gambier Island Time
    GMT     UTC        Greenwich Mean Time
    GST     UTC−02     South Georgia and the South Sandwich Islands
    GST     UTC+04     Gulf Standard Time
    GYT     UTC−04     Guyana Time
    HADT    UTC−09     Hawaii-Aleutian Daylight Time
    HAEC    UTC+02     Heure Avancée d'Europe Centrale francised name for CEST
    HAST    UTC−10     Hawaii-Aleutian Standard Time
    HKT     UTC+08     Hong Kong Time
    HMT     UTC+05     Heard and McDonald Islands Time
    HOVT    UTC+07     Khovd Time
    HST     UTC−10     Hawaii Standard Time
    ICT     UTC+07     Indochina Time
    IDT     UTC+03     Israel Daylight Time
    IOT     UTC+03     Indian Ocean Time
    IRDT    UTC+04:30  Iran Daylight Time
    IRKT    UTC+08     Irkutsk Time
    IRST    UTC+03:30  Iran Standard Time
    IST     UTC+05:30  Indian Standard Time
    IST     UTC+01     Irish Standard Time[5]
    IST     UTC+02     Israel Standard Time
    JST     UTC+09     Japan Standard Time
    KGT     UTC+06     Kyrgyzstan time
    KOST    UTC+11     Kosrae Time
    KRAT    UTC+07     Krasnoyarsk Time
    KST     UTC+09     Korea Standard Time
    LHST    UTC+10:30  Lord Howe Standard Time
    LHST    UTC+11     Lord Howe Summer Time
    LINT    UTC+14     Line Islands Time
    MAGT    UTC+12     Magadan Time
    MART    UTC−09:30  Marquesas Islands Time
    MAWT    UTC+05     Mawson Station Time
    MDT     UTC−06     Mountain Daylight Time (North America)
    MET     UTC+01     Middle European Time Same zone as CET
    MEST    UTC+02     Middle European Saving Time Same zone as CEST
    MHT     UTC+12     Marshall Islands
    MIST    UTC+11     Macquarie Island Station Time
    MIT     UTC−09:30  Marquesas Islands Time
    MMT     UTC+06:30  Myanmar Time
    MSK     UTC+03     Moscow Time
    MST     UTC+08     Malaysia Standard Time
    MST     UTC−07     Mountain Standard Time (North America)
    MST     UTC+06:30  Myanmar Standard Time
    MUT     UTC+04     Mauritius Time
    MVT     UTC+05     Maldives Time
    MYT     UTC+08     Malaysia Time
    NCT     UTC+11     New Caledonia Time
    NDT     UTC−02:30  Newfoundland Daylight Time
    NFT     UTC+11:30  Norfolk Time
    NPT     UTC+05:45  Nepal Time
    NST     UTC−03:30  Newfoundland Standard Time
    NT      UTC−03:30  Newfoundland Time
    NUT     UTC−11     Niue Time
    NZDT    UTC+13     New Zealand Daylight Time
    NZST    UTC+12     New Zealand Standard Time
    OMST    UTC+06     Omsk Time
    ORAT    UTC+05     Oral Time
    PDT     UTC−07     Pacific Daylight Time (North America)
    PET     UTC−05     Peru Time
    PETT    UTC+12     Kamchatka Time
    PGT     UTC+10     Papua New Guinea Time
    PHOT    UTC+13     Phoenix Island Time
    PKT     UTC+05     Pakistan Standard Time
    PMDT    UTC−02     Saint Pierre and Miquelon Daylight time
    PMST    UTC−03     Saint Pierre and Miquelon Standard Time
    PONT    UTC+11     Pohnpei Standard Time
    PST     UTC−08     Pacific Standard Time (North America)
    PST     UTC+08     Philippine Standard Time
    PYST    UTC−03     Paraguay Summer Time (South America)[6]
    PYT     UTC−04     Paraguay Time (South America)[7]
    RET     UTC+04     Réunion Time
    ROTT    UTC−03     Rothera Research Station Time
    SAKT    UTC+11     Sakhalin Island time
    SAMT    UTC+04     Samara Time
    SAST    UTC+02     South African Standard Time
    SBT     UTC+11     Solomon Islands Time
    SCT     UTC+04     Seychelles Time
    SGT     UTC+08     Singapore Time
    SLST    UTC+05:30  Sri Lanka Time
    SRET    UTC+11     Srednekolymsk Time
    SRT     UTC−03     Suriname Time
    SST     UTC−11     Samoa Standard Time
    SST     UTC+08     Singapore Standard Time
    SYOT    UTC+03     Showa Station Time
    TAHT    UTC−10     Tahiti Time
    THA     UTC+07     Thailand Standard Time
    TFT     UTC+05     Indian/Kerguelen
    TJT     UTC+05     Tajikistan Time
    TKT     UTC+13     Tokelau Time
    TLT     UTC+09     Timor Leste Time
    TMT     UTC+05     Turkmenistan Time
    TOT     UTC+13     Tonga Time
    TVT     UTC+12     Tuvalu Time
    UCT     UTC        Coordinated Universal Time
    ULAT    UTC+08     Ulaanbaatar Time
    USZ1    UTC+02     Kaliningrad Time
    UTC     UTC        Coordinated Universal Time
    UYST    UTC−02     Uruguay Summer Time
    UYT     UTC−03     Uruguay Standard Time
    UZT     UTC+05     Uzbekistan Time
    VET     UTC−04:30  Venezuelan Standard Time
    VLAT    UTC+10     Vladivostok Time
    VOLT    UTC+04     Volgograd Time
    VOST    UTC+06     Vostok Station Time
    VUT     UTC+11     Vanuatu Time
    WAKT    UTC+12     Wake Island Time
    WAST    UTC+02     West Africa Summer Time
    WAT     UTC+01     West Africa Time
    WEDT    UTC+01     Western European Daylight Time
    WEST    UTC+01     Western European Summer Time
    WET     UTC        Western European Time
    WIT     UTC+07     Western Indonesian Time
    WST     UTC+08     Western Standard Time
    YAKT    UTC+09     Yakutsk Time
    YEKT    UTC+05     Yekaterinburg Time
    Z       UTC        Zulu Time (Coordinated Universal Time)
*/

