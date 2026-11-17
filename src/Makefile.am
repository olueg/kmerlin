## Makefile.am for kmerlin
if NEED_COMPAT
compat_sub = kdecompat
compat_lib = ./kdecompat/libcompat.a
compat_include = -Ikdecompat
endif
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kmerlin

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes) $(compat_include)

# the library search path. 
kmerlin_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kmerlin_LDADD   = ./setting/libsetting.a ./chat/libchat.a $(LIB_KFILE) $(compat_lib) 

# which sources should be compiled for kmerlin
kmerlin_SOURCES = main.cpp kmerlin.cpp kmerlinview.cpp kmerliniface.skel kmcontact.cpp kmcontact.h kmservice.h kmservice.cpp kmchatservice.h kmchatservice.cpp qcategorywidget.cpp qcategorywidget.h kmgroup.h kmgroup.cpp connectwidget.ui kmerlinprofile.cpp kmerlinprofile.h kmtextreplace.cpp kmtextreplace.h kmemotionicon.cpp kmemotionicon.h imchatservice.cpp imchatservice.h imfilereceive.cpp imfilereceive.h kmcontactaction.cpp kmcontactaction.h kmerlinnotify.cpp kmerlinnotify.h kmerlindock.h kmerlindock.cpp xautolock.cpp xautolock.h CContactWidget.cpp CContactWidget.h login.ui authdlg.ui authdlg.ui.h login.ui.h groupdlg.ui groupdlg.ui.h

# these are the headers for your project
noinst_HEADERS = kmerlin.h kmerlinview.h 

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) -C -ktr2i18n *.cpp *.ui.h ./chat/*.cpp *.ui.h ./setting/*.cpp ./setting/*.ui.h ./kdecompat/*.cpp -o $(podir)/kmerlin.pot

KDE_ICON = kmerlin

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Internet
kdelnk_DATA = kmerlin.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kmerlin
rc_DATA = kmerlinui.rc kmerlinchatui.rc
SUBDIRS = setting chat $(compat_sub)
