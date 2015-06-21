#!/bin/bash

# Copyright (C) 2011 by Mike Gabriel <mike.gabriel@das-netzwerkteam.de>
# Copyright (C) 2012 by Reinhard Tartler <siretart@tauware.de>
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

# Thanks to Jonas Smedegaard <dr@jones.dk> for inspiration...

# Formatting/cleanups by siretart in December 2012

set -e

usage() {
    echo "To be called at the project root of an nx-libs checkout"
    echo "usage: $(basename "$0") {<release-version>,HEAD} {server|client}"
    exit 1
}

PROJECT="nx-libs"
NULL=""

test -d ".git" || usage
RELEASE="$1"
test -n "${RELEASE}" || usage
CHECKOUT="$2"
test -n "$CHECKOUT" || usage

if [ "x$CHECKOUT" = "xserver" ] || [ "x${CHECKOUT}" = "xfull" ]; then
    MODE="full"
    CHECKOUT="redist-server/${RELEASE}"
    RELEASE_SUFFIX='-full'
elif [ "x$CHECKOUT" = "xclient" ] || [ "x${CHECKOUT}" = "xlite" ]; then
    MODE="lite"
    CHECKOUT="redist-client/${RELEASE}"
    RELEASE_SUFFIX='-lite'
else
    usage
fi

if [ x"$RELEASE" == "xHEAD" ]; then
    CHECKOUT="HEAD"
fi

if ! git rev-parse --verify -q "$CHECKOUT" >/dev/null; then
    echo "   '${RELEASE}' is not a valid release number because there is no git tag named ${CHECKOUT}."
    echo "   Please specify one of the following releases:"
    echo "HEAD"
    git tag -l | grep "^redist" | cut -f2 -d"/" | sort -u
    exit 1
fi

TARGETDIR=".."

MANIFEST="$(mktemp)"
TEMP_DIR="$(mktemp -d)"

trap "rm -f \"${MANIFEST}\"; rm -rf \"${TEMP_DIR}\"" 0

# create local copy of Git project at temp location
git archive --format=tar "${CHECKOUT}" --prefix="${PROJECT}-${RELEASE}/" | ( cd "$TEMP_DIR"; tar xf - )

echo "Created tarball for $CHECKOUT"

cd "${TEMP_DIR}/${PROJECT}-${RELEASE}/"

mkdir -p "doc/applied-patches"

# prepare patches for lite and full tarball
if [ "x$MODE" = "xfull" ]; then
    cat "debian/patches/series" | sort | grep -v '^#' | egrep "([0-9]+(_|-).*\.(full|full\+lite)\.patch)" | while read file
    do
        cp -v "debian/patches/$file" "doc/applied-patches/"
        echo "${file##*/}" >> "doc/applied-patches/series"
    done
else
    rm -Rf "nxcompshad"*
    rm -Rf "nxcompext"*
    rm -Rf "nx-X11"*
    cat "debian/patches/series" | sort | grep -v '^#' | egrep "([0-9]+(_|-).*\.full\+lite\.patch)" | while read file
    do
        cp -v "debian/patches/$file" "doc/applied-patches/"
        echo "${file##*/}" >> "doc/applied-patches/series"
    done
fi

# apply all patches shipped in debian/patches and create a copy of them that we ship with the tarball
if [ -s "doc/applied-patches/series" ]; then
    QUILT_PATCHES="doc/applied-patches" quilt --quiltrc /dev/null push -a -q
else
    echo "No patches applied at all. Very old release?"
fi

# very old release did not add any README
for f in $(ls README* 2>/dev/null); do
    mv -v "$f" "doc/";
done

# remove folders that we do not want to roll into the tarball
rm -Rf ".pc/"
rm -Rf "debian/"

# remove files, that we do not want in the tarballs (build cruft)
rm -Rf nx*/configure nx*/autom4te.cache*

cd "$OLDPWD"

# create target location for tarball
mkdir -p "${TARGETDIR}/_releases_/source/${PROJECT}/"

# roll the ball...
cd "$TEMP_DIR"
find "${PROJECT}-${RELEASE}" -type f | sort > "$MANIFEST"
cd "$OLDPWD"

tar c -C "$TEMP_DIR" \
    --owner 0 \
    --group 0 \
    --numeric-owner \
    --no-recursion \
    --files-from "$MANIFEST" \
    --gzip \
    > "$TARGETDIR/_releases_/source/${PROJECT}/${PROJECT}-${RELEASE}${RELEASE_SUFFIX}.tar.gz"

echo "$TARGETDIR/_releases_/source/${PROJECT}/${PROJECT}-${RELEASE}${RELEASE_SUFFIX}.tar.gz  is ready"
