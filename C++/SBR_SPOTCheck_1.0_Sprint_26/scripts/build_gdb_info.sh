#!/bin/sh

PROJECT=/home/hstools/workspace/source
GDB_INIT_PATH=/home/hstools/workspace

find $PROJECT/ -name "*.h*" -o -name "*.c*" | sed 's:/[^/]*$::' | sort  | uniq  | sed 's/^/directory /' > $GDB_INIT_PATH/gdbinit

exit $?

