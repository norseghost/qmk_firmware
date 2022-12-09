
EXTRAFLAGS += -flto
TAP_DANCE_ENABLE = yes
CAPS_WORD_ENABLE = yes
INTROSPECTION_KEYMAP_C = norseghost.c
ifneq ("$(wildcard $(USER_PATH)/secrets.c)","")
  SRC += secrets.c
endif

ifeq ($(strip $(NO_SECRETS)), yes)
    OPT_DEFS += -DNO_SECRETS
endif
