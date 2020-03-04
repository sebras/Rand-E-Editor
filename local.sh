#!/bin/sh
# 'local.sh' : shell script to update the 'NewVersion' shell script with the
#   correct path to 'bash'.
# The program name can be specified, if it must be something else than
#   the default 'e'.
# This shell can run on linux and on AIX.
# F. Perriollat March 2000

synopsis="synopsis : $0 [-h] [--help] [local_program_name]"

if [ $# -gt 0 ]; then
    if test $1 = "-h" -o $1 = "--help"; then
	echo "Script to update some Rand editor files according to the local environement"
	echo $synopsis
	exit 0;
    else
	if [ $# -gt 1 ]; then
	    echo $0 : Invalid parameters \"$@\"
	    echo $synopsis
	    exit 1;
	fi;
    fi;
fi;

fnv="e19/NewVersion"
fmk="./Makefile"

BASH=`which bash`
OS=`uname -s`

# Update the NewVersion script
if ! grep -q -e "^#\!$BASH" $fnv; then \
    rm -f $fnv.new; \
    sed "/^#\!/ s|\(^#\!\)\(.*\)|\1$BASH|" $fnv > $fnv.new; \
    touch -r $fnv $fnv.new; \
    chmod a+x $fnv.new; \
    ls -l $f $fnv.new; \
    mv -f $fnv.new $fnv; \
    echo "$fnv is updated"; \
else \
    echo "$fnv is OK"; \
fi

if [ $# -eq 0 ]; then
    exit 0;
fi

# A program name is provided, update PROG in ./Makefile

echo "On $OS the local program name will be \"$1\""
echo "  Are you sure ?"

sed_script="./Rand_sed.script"

rm -f $sed_script
cat > $sed_script <<-EOF
    /^ifeq (.*, $OS)/,/^endif/ {
	/PROG=.*/ d
	/^endif/i\\
    PROG=$1
    }
EOF
sed -f $sed_script $fmk > $fmk.new
if [ $? -ne 0 ]; then
    echo "Error in updating $fmk"
    exit 2
fi
rm -f $sed_script

# save previous version of Makefile
rm -f $fmk.ref
mv $fmk $fmk.ref
mv $fmk.new $fmk

echo "On $OS the local program name will be \"$1\""
echo "Previous version on \"$fmk\" is saved in \"$fmk.ref\""

exit 0

