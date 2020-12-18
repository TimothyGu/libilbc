#!/bin/sh

version="$1"
if [ -z "$version" ]; then
  echo "Usage: $0 <version>"
  exit 1
fi

set -ex

git clean -fxd
git submodule foreach --recursive --quiet 'cd $toplevel/$sm_path; git clean -fxd'

tmpdir=$(mktemp -d)
out=$tmpdir/libilbc-$version
mkdir $out

cur=$(pwd)

# We can't use git archive since we want to bundle the submodule.

{
  git grep --cached -l ''
  git submodule foreach --recursive --quiet 'git ls-files --with-tree="$sha1" | sed "s#^#$path/#"' | grep -v 'testdata\|_test\.cc$'
} | xargs -I{} cp -a {} $out

cd $tmpdir

tar -cf $cur/libilbc-$version.tar libilbc-$version
zstd -19 $cur/libilbc-$version.tar
gzip -9 $cur/libilbc-$version.tar
zip -r -9 $cur/libilbc-$version.zip libilbc-$version

rm -rf $tmpdir
