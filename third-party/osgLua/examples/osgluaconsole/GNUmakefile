TOPDIR = ../..
include $(TOPDIR)/Make/makedefs 

#Very important!!!
LDFLAGS += -Wl,-E
INC += -I/usr/include/lua5.1

CXXFILES =\
	osgluaconsole.cpp\
	console.cpp\

LIBS     += -losgLua -losg -losgProducer $(GL_LIBS) $(X_LIBS) $(OTHER_LIBS) 

INSTFILES = \
	$(CXXFILES)\
	GNUmakefile.inst=GNUmakefile

EXEC = osgluaconsole

INC +=  $(X_INC)

include $(TOPDIR)/Make/makerules 

