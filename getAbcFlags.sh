#!/bin/sh

# test for argument number
if [ $# -lt 1 ]; then

	echo "missing argument!";
	echo "format: getAbcFlags <abcDir>";
	exit 1;

fi

abcDir=$1
currentDir=$(pwd)
abcFlags=""

cd ${abcDir}

# init abc submodule when needed
if [ -z "$(ls)" ]; then

  git submodule init    > /dev/null 2>&1
  git submodule update  > /dev/null 2>&1

fi

# parse abc CFLAGS
for string in $( make | grep "CFLAGS" ); do

	if [ $( echo ${string} | grep "\-D.*" ) ]; then

		abcFlags+=" ${string}";

	fi

done
cd ${currentDir}

echo ${abcFlags}
