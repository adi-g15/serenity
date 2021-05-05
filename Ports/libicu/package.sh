#!/usr/bin/env -S bash ../.port_include.sh
port=libicu
version=69.1
useconfigure=true
workdir=icu/source
configopts=--with-cross-build=$(pwd)/${workdir}/../host-build
files="https://github.com/unicode-org/icu/releases/download/release-${version//./-}/icu4c-${version//./_}-src.tgz icu4c-${version//./_}-src.tgz 4cba7b7acd1d3c42c44bb0c14be6637098c7faf2b330ce876bc5f3b915d09745"
auth_type=sha256

configure() {
    host_env
    run mkdir -p ../host-build
    run sh -c "cd ../host-build && ../source/configure && make $makeopts"
    target_env
    run ./configure --host="${SERENITY_ARCH}-pc-serenity" $configopts
}

export CFLAGS="-DU_HAVE_NL_LANGINFO_CODESET=0 -DLC_MESSAGES=1"
export CXXFLAGS="-DU_HAVE_NL_LANGINFO_CODESET=0 -DLC_MESSAGES=1"
