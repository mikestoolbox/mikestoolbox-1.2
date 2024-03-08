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
//  File:       Error.cpp
//
//  Synopsis:   Functions that deal with error codes
//----------------------------------------------------------------------------

#include "mikestoolbox-1.2.h"

namespace mikestoolbox {

static const char* apz_ErrorMessages[] = {
    "Success",
    "Exception caught",
    "System out of memory",
    "File open failed",
    "File read failed",
    "File write failed",
    "Random number generator failed",
    "Pseudo-random number generator failed",
    "Cryptographic hash computation failed",
    "Unsupported HMAC hash algorithm",
    "Cipher is not a block cipher",
    "Bad network connect string",
    "Network port unspecified",
    "Network address lookup failed",
    "Network address blocked",
    "Socket create failed",
    "Socket bind failed",
    "Socket listen failed",
    "Socket accept failed",
    "Socket connect failed",
    "Socket select failed",
    "Socket read failed",
    "Socket send failed",
    "Socket shutdown failed",
    "Socket close failed",
    "Socket not open",
    "Socket bound",
    "Socket not bound",
    "Socket listening",
    "Socket not listening",
    "Socket connecting",
    "Socket not connected",
    "Socket connection closed",
    "Socket not readable",
    "Socket not writable",
    "Socket operation timed out",
    "Socket operation canceled",
    "Socket closed by peer",
    "Socket line too long",
    "Socket line incomplete",
    "Socket message too big",
    "Socket address blocked",
    "TLS protocol error",
    "TLS data verification error",
    "TLS padding error",
    "TLS decryption failure",
    "TLS record overflow",
    "TLS decompression failure",
    "TLS certificate missing",
    "TLS certificate bad",
    "TLS access denied",
    "TLS decode error",
    "TLS export restriction",
    "TLS version mismatch",
    "TLS security weak",
    "TLS session cancelled",
    "TLS server name bad",
    "TLS OCSP response bad",
    "TLS session resumption error",
    "TLS server key exchange failure",
    "TLS Diffie-Hellman error",
    "No common TLS ciphers",
    "TLS finished message invalid",
    "TLS internal error",
    "TLS read channel closed",
    "TLS send channel closed",
    "Empty TLS message",
    "Unexpected TLS message received",
    "Unrecognized TLS content type received",
    "TLS alert received with unknown message",
    "TLS alert received with unknown severity level",
    "Multiple TLS extensions of the same type",
    "TLS data received with wrong protection",
    "TLS domain name bad",
    "TLS identity missing certificate chain",
    "TLS identity missing private key",
    "TLS wrong RSA secret size",
    "TLS RSA encryption failure",
    "TLS RSA decryption failure",
    "TLS version rollback",
    "Failed to sign DH parameters",
    "Failed to generate TLS master secret",
    "Failed to generate TLS session keys",
    "Failed to generate TLS finished message",
    "TLS finished message invalid",
    "TLS handshake message size limit exceeded",
    "TLS certificate size limit exceeded",
    "TLS certificate chain size limit exceeded",
    "Client did not authenticate",
    "Unknown client certificate type",
    "No matching server identity found",
    "Unsafe TLS renegotiation prevented",
    "Unsafe TLS renegotiation prevented",
    "Certificate encoding error",
    "Certificate version error",
    "Certificate public key error",
    "Certificate expired",
    "Certificate path length exceeded",
    "Certificate policies not supported",
    "Certificate signature invalid",
    "Unknown certificate authority",
    "Untrusted certificate authority",
    "Certificate revoked",
    "Need to obtain current CRL",
    "Certificate modified",
    "Certificate issuer incorrect",
    "Certificate issuer not a CA",
    "Certificate issuer cannot sign",
    "Certificate subject empty",
    "Unknown critical certificate extension",
    "Unknown certificate key type",
    "Certificate algorithm mismatch",
    "Unknown certificate signature algorithm",
    "Distinguished Name (DirName) encoding error",
    "Syntax error in Distinguished Name (DirName)",
    "Hex encoding used in Distinguished Name (DirName)",
    "CRL encoding error",
    "CRL version error",
    "CRL signature invalid",
    "Unknown certificate authority",
    "Untrusted certificate authority",
    "CRL modified",
    "CRL issuer incorrect",
    "CRL issuer not a CA",
    "CRL issuer cannot sign",
    "CRL missing subject name",
    "Unknown critical CRL extension",
    "Unknown CRL issuer key type",
    "CRL algorithm mismatch",
    "Unknown CRL signature algorithm"
};

static const uintsys u_NumberOfErrors = sizeof (apz_ErrorMessages) / sizeof (apz_ErrorMessages[0]);

String getErrorString (uintsys u_ErrorCode)
{
    if (u_ErrorCode < u_NumberOfErrors)
    {
        return apz_ErrorMessages[u_ErrorCode];
    }

    String str_ErrorCode (u_ErrorCode);

    return "Unknown Error Code: " + str_ErrorCode;
}

} // namespace mikestoolbox

