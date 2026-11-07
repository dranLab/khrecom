#!/bin/bash
# tools/helpers/ifeshow.sh <unit> <mangled>: show the #if0/#else draft's funcdiff with -C 3
u=$1; m=$2; f=$(ls src/$u.cpp src/$u.c 2>/dev/null | head -1); cp $f build/ifeshow.orig
.venv/bin/python - "$f" "$m" <<'PY'
import sys; sys.path.insert(0,'tools/helpers'); import ifelsefmt
f,m=sys.argv[1:3]; t=open(f,encoding='latin-1').read(); open(f,'w',encoding='latin-1').write(ifelsefmt.enable(t,m))
PY
rm -f build/SLUS_217.99/src/$u.o; ninja build/SLUS_217.99/src/$u.o >/dev/null 2>&1; .venv/bin/python tools/funcdiff.py $u $m -C ${3:-3} | grep -v "^   R_"
cp build/ifeshow.orig $f; rm -f build/SLUS_217.99/src/$u.o; ninja build/SLUS_217.99/src/$u.o >/dev/null 2>&1
