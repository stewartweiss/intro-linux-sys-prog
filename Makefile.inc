# Definitions to include in chapter-specific makefiles

SPL_INCLUDE_DIR = ../include
SPL_LIB_DIR     = ../lib
SPL_LIB         = $(SPL_LIB_DIR)/libspl.a
VPATH           = ../include:../common
SPL_HDRS        = \
#common_defs.h\
common_hdrs.h\
dir_utils.h\
error_exits.h\
escapes.h\
get_nums.h\
hash.h\
preamble\
ps_utils.h\
show_time.h\
sys_hdrs.h\
time_utils.h
