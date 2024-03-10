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
//  File:       Typename.inl
//
//  Synopsis:   Functions to return the name of a type
//----------------------------------------------------------------------------

namespace mikestoolbox {

template<class T>
inline String TypenameGen<T>::operator() ()
{
    return String("unknown");
}

template<class T>
inline String TypenameGen<T*>::operator() ()
{
    String str_Type = TypenameGen<T>()();

    str_Type += '*';

    return str_Type;
}

static inline void CloseTypenameTemplate (String& str_Type)
{
    str_Type.Append ((str_Type[-1] == '>') ? " >" : ">");
}

template<class T>
inline String TypenameGen<PointerHolder<T> >::operator() ()
{
    String str_Type ("PointerHolder<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<>
inline String TypenameGen<void>::operator() ()
{
    return String("void");
}

template<>
inline String TypenameGen<char>::operator() ()
{
    return String("char");
}

template<>
inline String TypenameGen<uchar>::operator() ()
{
    return String("uchar");
}

template<>
inline String TypenameGen<int16>::operator() ()
{
    return String("int16");
}

template<>
inline String TypenameGen<uint16>::operator() ()
{
    return String("uint16");
}

template<>
inline String TypenameGen<int32>::operator() ()
{
    return String("int32");
}

template<>
inline String TypenameGen<uint32>::operator() ()
{
    return String("uint32");
}

template<>
inline String TypenameGen<int64>::operator() ()
{
    return String("int64");
}

template<>
inline String TypenameGen<uint64>::operator() ()
{
    return String("uint64");
}

template<>
inline String TypenameGen<long>::operator() ()
{
    return String("long");
}

template<>
inline String TypenameGen<unsigned long>::operator() ()
{
    return String("unsigned long");
}

template<>
inline String TypenameGen<float>::operator() ()
{
    return String("float");
}

template<>
inline String TypenameGen<double>::operator() ()
{
    return String("double");
}

template<>
inline String TypenameGen<std::string>::operator() ()
{
    return String("std::string");
}

template<>
inline String TypenameGen<HeapMemory>::operator() ()
{
    return String("HeapMemory");
}

template<class MEMORY>
inline String TypenameGen<SharedMemory<MEMORY> >::operator() ()
{
    String str_Type ("SharedMemory<");

    str_Type += TypenameGen<MEMORY>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<>
inline String TypenameGen<String>::operator() ()
{
    return String("String");
}

template<>
inline String TypenameGen<StringIter>::operator() ()
{
    return String("StringIter");
}

template<>
inline String TypenameGen<SubString>::operator() ()
{
    return String("SubString");
}

template<>
inline String TypenameGen<StringList>::operator() ()
{
    return String("StringList");
}

template<>
inline String TypenameGen<StringListIter>::operator() ()
{
    return String("StringListIter");
}

template<>
inline String TypenameGen<StringListChangeIter>::operator() ()
{
    return String("StringListChangeIter");
}

template<>
inline String TypenameGen<StringListByteIter>::operator() ()
{
    return String("StringListByteIter");
}

template<class T>
inline String TypenameGen<List<T> >::operator() ()
{
    String str_Type ("List<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class T>
inline String TypenameGen<ListIter<T> >::operator() ()
{
    String str_Type ("ListIter<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class T>
inline String TypenameGen<ListChangeIter<T> >::operator() ()
{
    String str_Type ("ListChangeIter<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class H>
inline String TypenameGen<Hash<K,V,H> >::operator() ()
{
    String str_Type ("Hash<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<H>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class H>
inline String TypenameGen<HashIter<K,V,H> >::operator() ()
{
    String str_Type ("HashIter<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<H>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class H>
inline String TypenameGen<HashChangeIter<K,V,H> >::operator() ()
{
    String str_Type ("HashChangeIter<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<H>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class H>
inline String TypenameGen<HashRef<K,V,H> >::operator() ()
{
    String str_Type ("HashRef<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<H>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class CMP>
inline String TypenameGen<Map<K,V,CMP> >::operator() ()
{
    String str_Type ("Map<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<CMP>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class CMP>
inline String TypenameGen<MapIter<K,V,CMP> >::operator() ()
{
    String str_Type ("MapIter<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<CMP>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class CMP>
inline String TypenameGen<MapChangeIter<K,V,CMP> >::operator() ()
{
    String str_Type ("MapChangeIter<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<CMP>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class K, class V, class CMP>
inline String TypenameGen<MapRef<K,V,CMP> >::operator() ()
{
    String str_Type ("MapRef<");

    str_Type += TypenameGen<K>()();
    str_Type += ',';
    str_Type += TypenameGen<V>()();
    str_Type += ',';
    str_Type += TypenameGen<CMP>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class T>
inline String TypenameGen<Less<T> >::operator() ()
{
    String str_Type ("Less<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class T>
inline String TypenameGen<Greater<T> >::operator() ()
{
    String str_Type ("Greater<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<class T>
inline String TypenameGen<std::allocator<T> >::operator() ()
{
    String str_Type ("std::allocator<");

    str_Type += TypenameGen<T>()();

    CloseTypenameTemplate (str_Type);

    return str_Type;
}

template<>
inline String TypenameGen<AutoPrintTimer>::operator() ()
{
    return String("AutoPrintTimer");
}

template<>
inline String TypenameGen<CharRef>::operator() ()
{
    return String("CharRef");
}

template<>
inline String TypenameGen<Condition>::operator() ()
{
    return String("Condition");
}

template<>
inline String TypenameGen<Date>::operator() ()
{
    return String("Date");
}

template<>
inline String TypenameGen<DefaultHasher>::operator() ()
{
    return String("DefaultHasher");
}

template<>
inline String TypenameGen<Exception>::operator() ()
{
    return String("Exception");
}

template<>
inline String TypenameGen<File>::operator() ()
{
    return String("File");
}

template<>
inline String TypenameGen<Index>::operator() ()
{
    return String("Index");
}

template<>
inline String TypenameGen<IpAddressRange>::operator() ()
{
    return String("IpAddressRange");
}

template<>
inline String TypenameGen<Mutex>::operator() ()
{
    return String("Mutex");
}

template<>
inline String TypenameGen<MutexLocker>::operator() ()
{
    return String("MutexLocker");
}

template<>
inline String TypenameGen<Network>::operator() ()
{
    return String("Network");
}

template<>
inline String TypenameGen<PerlRegex>::operator() ()
{
    return String("PerlRegex");
}

template<>
inline String TypenameGen<PerlRegexMatches>::operator() ()
{
    return String("PerlRegexMatches");
}

template<>
inline String TypenameGen<PerlRegexOptions>::operator() ()
{
    return String("PerlRegexOptions");
}

template<>
inline String TypenameGen<RecursiveMutex>::operator() ()
{
    return String("RecursiveMutex");
}

template<>
inline String TypenameGen<RefCount>::operator() ()
{
    return String("RefCount");
}

template<>
inline String TypenameGen<Repeat>::operator() ()
{
    return String("Repeat");
}

template<>
inline String TypenameGen<SharedData>::operator() ()
{
    return String("SharedData");
}

template<>
inline String TypenameGen<SharedResource>::operator() ()
{
    return String("SharedResource");
}

template<>
inline String TypenameGen<SimpleThread>::operator() ()
{
    return String("SimpleThread");
}

template<>
inline String TypenameGen<Socket>::operator() ()
{
    return String("Socket");
}

template<>
inline String TypenameGen<SocketAddress>::operator() ()
{
    return String("SocketAddress");
}

template<>
inline String TypenameGen<StringException>::operator() ()
{
    return String("StringException");
}

template<>
inline String TypenameGen<TcpListener>::operator() ()
{
    return String("TcpListener");
}

template<>
inline String TypenameGen<TcpSocket>::operator() ()
{
    return String("TcpSocket");
}

template<>
inline String TypenameGen<Thread>::operator() ()
{
    return String("Thread");
}

template<>
inline String TypenameGen<Timer>::operator() ()
{
    return String("Timer");
}

template<>
inline String TypenameGen<ByteRef>::operator() ()
{
    return String("ByteRef");
}

template<>
inline String TypenameGen<UdpSocket>::operator() ()
{
    return String("UdpSocket");
}

#ifdef PLATFORM_WINDOWS

template<>
inline String TypenameGen<WindowsString>::operator() ()
{
    return String("WindowsString");
}

#endif // PLATFORM_WINDOWS

template<class T>
inline String Typename ()
{
    return TypenameGen<T>()();
}

template<class T>
inline String Typename (const T& t)
{
    return TypenameGen<T>()();
}

} // namespace mikestoolbox

