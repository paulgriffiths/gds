LOCAL_DIR := src
LOCAL_LIB := $(LIBDIR)/libpggds.a
LOCAL_SRC := $(wildcard $(LOCAL_DIR)/*.c)
LOCAL_OBJ := $(subst .c,.o,$(LOCAL_SRC))

LIBRARIES += $(LOCAL_LIB)
SOURCES   += $(LOCAL_SRC)

$(LOCAL_LIB): $(LOCAL_OBJ)
	@$(AR) $(ARFLAGS) $@ $^

