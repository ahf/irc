#! /bin/sh
#
# $Id$
# 

if test "`uname -s`" = AIX
then
  revdir="AIX-`oslevel 2>/dev/null|sed -e \"s/<//g\" -e \"s/>//g\" -e
\"s@/@-@g\"`"
else
  uname -p >/dev/null 2>&1
  if test "$?" != 0
  then
    revdir="`uname -s`-`uname -r|sed -e \"s/\\([^-]*\\).*/\1/\" -e \"s@/@-@g\"`"
  else
    revdir="`uname -p`-`uname -s`-`uname -r|sed -e \"s/\\([^-]*\\).*/\1/\" -e \"s@/@-@g\"`"
  fi
fi
echo "$revdir"
