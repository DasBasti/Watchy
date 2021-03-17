#!/bin/sh
HASH=`git rev-parse HEAD`
BUILDTIME=`date "+%Y-%m-%d %H:%M"`

echo "#include \"version.h\"" > src/version.c
echo "const char *build_date = \"$BUILDTIME\";" >> src/version.c
echo "const char *build_git_sha = \"$HASH\";" >> src/version.c