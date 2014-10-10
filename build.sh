#!/bin/bash -e

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Default variable values:
clean_build=0
run_tests=0
build_dir="./outDir"

function show_help {
echo "Help not implemeted"
#TODO: display instruction
}

while getopts "h?ctd:" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    c)  clean_build=1
        ;;
    t)  run_tests=1
        ;;
    d)  build_dir=$OPTARG
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift


if [ $clean_build -eq 1 ]; then

    if [ -d ./build ]; then
        rm -rf $build_dir 
    fi
fi

mkdir -p $build_dir
cd $build_dir

cmake ..
#TODO made number of used cores a setable option
make -j2 

if [ $run_tests -eq 1 ]; then
    ./runUnitTests
fi

