TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

#Very important!!!
LDFLAGS += -Wl,-E

INC += -I/usr/include/lua5.1

CFILES =\
	lua.c\

LIBS     += -losgLua $(OTHER_LIBS)

INSTFILES = \
	$(CXXFILES)\
	$(CFILES)\
	GNUmakefile.inst=GNUmakefile

EXEC = osglua

INC +=  $(X_INC)

include $(TOPDIR)/Make/makerules 

