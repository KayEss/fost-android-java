/**
    Copyright 2014-2018 Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-android.hpp"
#include <fost/insert>
#include <fost/base16>
#include <fost/log>
#include <fost/urlhandler>
#include <stellar/storage.hpp>

namespace {
    const fostlib::module c_module(fostlib::c_android, "wallet");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_felspar_android_Wallet_signTx(
    JNIEnv *env, jobject self, jstring jUnsignedXdr, jstring jHashTx
) {
    try {
        fostlib::string
            unsigneedXdr = fostlib::jni_cast<fostlib::string>(env, jUnsignedXdr),
            hashTx = fostlib::jni_cast<fostlib::string>(env, jHashTx);
         // detect wallet on local
        auto const wallet = stellar::current_wallet();
            if (not wallet) {
                throw "wallet not found";
        }
        // convert unsigned to base64
        fostlib::base64_string const xdr64 = fostlib::ascii_string{unsigneedXdr};
        // convert transaction to binary
        auto const bintx =
            fostlib::coerce<std::vector<unsigned char>>(xdr64);
        // hash
        auto h16std = static_cast<std::string>(hashTx);
        std::transform(
            h16std.begin(), h16std.end(), h16std.begin(),
            [](unsigned char c) { return std::toupper(c); });
        fostlib::base16_string const hash16 = fostlib::ascii_string{h16std};
        auto hashv = fostlib::coerce<std::vector<unsigned char>>(hash16);
        if (hashv.size() != 32u) {
            throw fostlib::exceptions::not_implemented(
            __PRETTY_FUNCTION__, "Hash must be 32 bytes long");
        }
        std::array<f5::byte, 32> hash;
        std::copy(hashv.begin(), hashv.end(), hash.begin());
        // signed transaction
        std::vector<unsigned char> const tx = wallet.value().sign(bintx, hash);
        // convert vector unsigned to base 64
        auto base64Tx = fostlib::coerce<fostlib::base64_string>(tx);
        return env->NewStringUTF(base64Tx._data());
    } catch ( fostlib::exceptions::exception &e ) {
        fostlib::log::error(c_module)
            ("exception", e.what())
            ("data", e.data());
    }
    return env->NewStringUTF("");
}
