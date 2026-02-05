#!/bin/bash

## This script is used to build newlib locally

set -e
set -u

TRIPLET_DEFAULT=kvx-mbr
TOOLS_DEFAULT=all
STAGES_DEFAULT=all
TOOLROOT_DEFAULT=/opt/kalray/accesscore
COMPILER_DEFAULT=gcc

INSTALLDIR_DEFAULT="$PWD/devimage/toolchain-$TRIPLET_DEFAULT/"

NEWLIBDIR_DEFAULT="$PWD/newlib"

ONLY=0
echo NEWLIBDIR_DEFAULT=$NEWLIBDIR_DEFAULT
usage () {

    echo -e "-j JOBS\n\tSpecifies the number of jobs used by make."
    echo -e "--compiler ARG\n\tTarget compiler to use. (default: $COMPILER_DEFAULT)"
    echo -e "--triplet ARG\n\tTarget triplet for toolchain (default: $TRIPLET_DEFAULT)"
    echo -e "--toolroot ARG\n\tPath to toolchain (default: $TOOLROOT_DEFAULT)"
    echo -e "--tools TOOLS\n\tTools to build (default: $TOOLS_DEFAULT)"
    echo -e "--stages STAGES\n\tStages to execute (default: $STAGES_DEFAULT)"
    echo -e "-h --help\n\tThis help text."
    echo -e "-i INSTALL_DIR\n\tInstallation directory"
    echo -e "--newlib-dir NEWLIB_SOURCE_PATH\n\tPath to newlib source tree (default: $NEWLIBDIR_DEFAULT)"
    echo -e "--dry-run\n\tOnly outputs the scheduled stages, do not execute anything"
    echo -e "--from\n\tExecute all stages starting from STAGE"
    echo -e "--only\n\tExecute all specified steps in order"
    echo -e "--build PATH\n\tSpecify the build directory."
}

## this is the stages list
configure_newlib=10
build_newlib=11
install_newlib=12

## stages =
## configure_newlib, build_newlib, install_newlib

if ! TEMP=$(getopt -o  j:hi: -l triplet:,compiler:,dry-run,from:,only:,help,tools:,toolroot:,stages:,gcc-download-req,gcc-extract-req-from:,build:,gcc-dir:,newlib-dir:,uclibc-dir:, -- "$@"); then
    usage
    exit 1
fi

if [ $? != 0 ] ; then
    usage
    exit 1
fi
eval set -- "$TEMP"

while [ "$1" != "--" ];
do
    case $1 in
	--dry-run)
	    DRYRUN=1
	    ;;

	--build)
	    BUILD_ROOT="$2"
	    shift
	    ;;

	--only)
	    ONLY="${2}"
	    shift
	    ;;

	--from)
	    FROM="${2}"
	    shift
	    ;;

	-j)
	    MAKEFLAGS="-j $2"
	    export MAKEFLAGS
	    shift
	    ;;

	-i)
	    if [[ ! -d "${2}" ]]; then
		mkdir -p "${2}"
	    fi
	    INSTALLDIR=$(realpath "${2}")
	    shift
	    ;;

	--compiler)
	    COMPILER_DEFAULT="${2}"
	    shift
	    ;;

	--triplet)
	    TRIPLET="${2}"

	    INSTALLDIR_DEFAULT="$PWD/devimage/toolchain-$TRIPLET"/
	    shift
	    ;;

	--toolroot)
	    TOOLROOT="${2}"
	    shift
	    ;;

	-h|--help)
	    usage
	    exit 0
	    ;;

	--newlib-dir)
	    NEWLIBDIR="${2}"
	    shift
	    ;;

	--tools)
	    TOOLS_TARGETS="${2}"
	    shift
	    ;;

	--stages)
	    TOOLS_STAGES="${2}"
	    shift
	    ;;

	-*)
	    echo "Unexpected option: $1"
	    usage
	    exit 1
	    ;;

    esac
    shift
done


## No need to change anything below.

## defaults
echo NEWLIBDIR=$NEWLIBDIR
NEWLIBDIR=${NEWLIBDIR:-$NEWLIBDIR_DEFAULT}
FROM=${FROM:-0}
DRYRUN=${DRYRUN:-0}

TRIPLET=${TRIPLET:-$TRIPLET_DEFAULT}
TOOLS_PREFIX=${TRIPLET}

TOOLROOT=${TOOLROOT:-${TOOLROOT_DEFAULT}}

COMPILER=${TOOLROOT}/bin/${TOOLS_PREFIX}-${COMPILER_DEFAULT}
if [[ ${COMPILER_DEFAULT} == "llvm" ]]; then
    TARGET=
    case $TRIPLET in
      kvx-cos)
        TARGET="kvx-cos"
        ;;
      riscv64-cos-elf)
        TARGET="riscv64-cos-elf"
        ;;
      kvx-mbr)
        TARGET="kvx-mbr"
        ;;
    esac
    COMPILER="${TOOLROOT}/bin/clang -v --target=${TARGET}"
    TOOLS_PREFIX=$TARGET
fi
ASSEMBLER=${TOOLROOT}/bin/${TOOLS_PREFIX}-as
LINKER=${TOOLROOT}/bin/${TOOLS_PREFIX}-ld
RANLIB=${TOOLROOT}/bin/${TOOLS_PREFIX}-ranlib
ARCHIVER=${TOOLROOT}/bin/${TOOLS_PREFIX}-ar
READELF=${TOOLROOT}/bin/${TOOLS_PREFIX}-readelf

INSTALLDIR=${INSTALLDIR:-$INSTALLDIR_DEFAULT}
TOOLS_STAGES=${TOOLS_STAGES:-$TOOLS_DEFAULT}
TOOLS_TARGETS=${TOOLS_TARGETS:-$STAGES_DEFAULT}
## end of conf

BUILD_ROOT=${BUILD_ROOT:-"$PWD/build"}

BUILD_NEWLIB_DIR="$BUILD_ROOT/build-newlib"

## $1 : tool
## $2 : step
function should_do() {
    tool="$1"
    step="$2"

    mangled_step=${step}_${tool}

    if [[ $ONLY != 0 ]]; then
	if [[ $ONLY =~ ${mangled_step} ]]; then
	    echo 1
	    return
	else
	    echo 0
	    return
	fi
    fi

    if [[ $FROM != 0 ]]; then
	cur_val=${!mangled_step}
	from_step=${!FROM}

	if [[ $cur_val -ge $from_step ]]; then
	    echo 1
	    return
	fi
    else
	if [[ $TOOLS_TARGETS =~ $tool || $TOOLS_TARGETS =~ all  ]]; then
	    if [[ ${TOOLS_STAGES} =~ ${mangled_step} || $TOOLS_STAGES =~ all  ]]; then
		echo 1
		return
	    fi
	fi
    fi
    echo 0
}

# NEWLIB
if [[ $(should_do "newlib" "configure") == "1" ]]; then
    if [[ $DRYRUN == 1 ]]; then
	echo Configure newlib
    else
	echo "====> Newlib configure"
	additionnal_opts="--enable-newlib-multithread"

	set -x
	mkdir -p ${BUILD_NEWLIB_DIR}
	cd ${BUILD_NEWLIB_DIR}

	TARGET_CFLAGS=
	if [[ ${COMPILER_DEFAULT} == "llvm" ]]; then
		TARGET_CFLAGS+=" -w -std=gnu89 --target=${TOOLS_PREFIX} --sysroot=${TOOLROOT} -isystem ${TOOLROOT}/${TRIPLET}/include"
		case $TRIPLET in
		  kvx-elf|kvx-mbr)
		    TARGET_CFLAGS+=" -U__CLUSTER_OS__"
		  ;;
                esac
	fi
	if [[ ${TRIPLET} == "riscv64-cos-elf" ]]; then
	  TARGET_CFLAGS=" --sysroot=${TOOLROOT} -isystem ${TOOLROOT}/${TRIPLET}/include"
	fi

	if [[ ${TRIPLET} == "riscv64-unknown-elf" || ${TRIPLET} == "riscv64-cos-elf" ]]; then
	      additionnal_opts+=" --with-multilib-generator=rv64g-lp64-- --with-arch=rv64g "
	fi
	CC_FOR_TARGET=${COMPILER} \
	CFLAGS_FOR_TARGET="${TARGET_CFLAGS}" \
	AS_FOR_TARGET=${ASSEMBLER} \
	LD_FOR_TARGET=${LINKER} \
	RANLIB_FOR_TARGET=${RANLIB} \
	AR_FOR_TARGET=${ARCHIVER} \
	READELF_FOR_TARGET=${READELF} \
	${NEWLIBDIR}/configure \
	    ${additionnal_opts} \
	    --with-sysroot=${TOOLROOT} \
	    --enable-initfini-array \
	    --enable-newlib-mb \
	    --enable-multilib \
	    --enable-target-optspace=no \
	    --enable-newlib-io-c99-formats \
	    --target=${TRIPLET} \
	    --prefix=${INSTALLDIR}
	set +x
    fi
fi

if [[ $(should_do "newlib" "build") == "1" ]]; then
    if [[ $DRYRUN == 1 ]]; then
	echo Build newlib
    else
	echo "====> Newlib build"
	set -x
	cd ${BUILD_NEWLIB_DIR}
	make
	set +x
    fi
fi

if [[ $(should_do "newlib" "install") == "1" ]]; then
    if [[ $DRYRUN == 1 ]]; then
	echo Install newlib
    else
	echo "====> Newlib install"
	set -x
	cd ${BUILD_NEWLIB_DIR}
	make install
	set +x
    fi
fi


