#!/bin/bash -e

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Default variable values:
clean_build=0
run_tests=0
debug_build=0

cmake_options=""

#oneliner from internet
repo_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

build_dir="$repo_dir/outDir/Release/"

function show_help {
echo "Usage: ./build [OPITONS]"
echo ""
echo "  -h,      displays help"
echo "  -c,      clears build directory"
echo "  -t,      launches tests afer succesfull build"
echo "  -d,      builds as debug"
echo ""
}

#Script options handling
while getopts "h?ctd" opt; do
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    c)  clean_build=1
        ;;
    t)  run_tests=1
        ;;
    d)  debug_build=1
    esac
done

shift $((OPTIND-1))

[ "$1" = "--" ] && shift

#debug/release setup
if [ $debug_build -eq 1 ]; then
    build_dir="$repo_dir/outDir/Debug/"
    cmake_options="-DCMAKE_BUILD_TYPE=Debug"
else
    build_dir="$repo_dir/outDir/Release/"
    cmake_options="-DCMAKE_BUILD_TYPE=Release"
fi

#cleaning build folder
if [ $clean_build -eq 1 ]; then

    if [ -d $build_dir ]; then
        rm -rf $build_dir 
    fi
fi

#actual build
mkdir -p $build_dir
pushd $build_dir

cmake $repo_dir $cmake_options

#TODO made number of used cores a setable option
time make -j2 

#test launching
if [ $run_tests -eq 1 ]; then
    ./runUnitTests
fi

popd
