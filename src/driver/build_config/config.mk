#
# project specific configuration file
# to be placed in main project directory
#

# set default mode
export MODE:=default

# set default profile
export PROFILE:=release

# set default toolchain
export TC:=gcc

# enable distcc
#export WITH_DISTCC:=1

# enable ccache
export WITH_CCACHE:=1

# default features set
export FEATURES:=

# project-specific flags
TMP:=-std=c++0x
export USER_OPT_FLAGS:=$(TMP)
export USER_DBG_FLAGS:=$(TMP)
export USER_PRF_FLAGS:=$(TMP)
# project-specific link flags
export USER_OPT_LDFLAGS:=
export USER_DBG_LDFLAGS:=
export USER_PRF_LDFLAGS:=
