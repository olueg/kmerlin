#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './identityeditdialog.ui'
**
** Created: Sam Jan 24 20:25:35 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "identityeditdialog.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const char* const img0_identityeditdialog[] = { 
"16 16 131 2",
"Qt c None",
".V c #000000",
".X c #000400",
"#b c #080408",
".M c #080808",
"#. c #101410",
".B c #181818",
"#C c #181820",
"## c #181c18",
"#e c #201c20",
"#O c #202020",
"#r c #202029",
"#m c #292020",
"#q c #292029",
".e c #292420",
"#f c #292429",
".m c #292829",
".E c #292c29",
"#2 c #312820",
".N c #312829",
".U c #312c31",
".l c #313031",
".v c #393439",
".C c #393839",
"#p c #413c39",
"#a c #413c41",
"#7 c #4a2c31",
"#8 c #4a3831",
".d c #4a444a",
".k c #4a4c4a",
"aa c #523439",
"#B c #524c5a",
".u c #525052",
".9 c #525552",
".w c #5a555a",
".O c #5a595a",
"a# c #622c29",
"#3 c #624c39",
"#u c #62656a",
"a. c #6a2820",
".Y c #6a696a",
"#9 c #734031",
"#N c #735939",
".j c #737173",
".W c #737573",
".D c #7b757b",
".c c #7b797b",
"#g c #7b7d7b",
"#6 c #833c39",
"#5 c #834029",
".5 c #837d83",
".H c #838173",
"#W c #83817b",
".b c #838183",
"#A c #83818b",
".I c #83857b",
"#4 c #8b6139",
"#V c #8b817b",
".r c #8b898b",
".i c #8b8d8b",
"#l c #949194",
"#M c #9c8d41",
".a c #9c959c",
"#t c #9c999c",
"#z c #9c99a4",
".n c #9c9d9c",
".y c #9c9da4",
".f c #a4a1a4",
"#o c #aca5ac",
".# c #acaaac",
"#s c #acaeac",
"#y c #acaeb4",
"#v c #acaebd",
"#1 c #b45531",
".g c #b4aeb4",
"#E c #b4b283",
".L c #b4b2b4",
"#J c #bdb6a4",
"#k c #bdb6bd",
".P c #bdb6c5",
"#I c #bdbabd",
".h c #bdbebd",
"#F c #c5c27b",
"#L c #c5c294",
".J c #c5c2b4",
".s c #c5c2c5",
"#w c #c5c2cd",
"#K c #c5c6ac",
".A c #c5c6c5",
"#H c #cdbea4",
".t c #cdc6cd",
"#d c #cdcacd",
"#j c #cdcecd",
"#x c #cdced5",
".x c #d5ced5",
"#n c #d5d2d5",
".Q c #d5d6ac",
".q c #d5d6d5",
"#U c #de7d52",
"#X c #deae5a",
"#G c #deca8b",
".8 c #dedade",
"#h c #dedede",
".3 c #e6d6bd",
"#c c #e6dee6",
".Z c #e6e2e6",
".7 c #e6e2ee",
"#P c #eeda5a",
".6 c #eee6d5",
".4 c #eee6ee",
".G c #eeeae6",
".F c #eeeaee",
"#i c #eeeeee",
"#0 c #f69941",
".0 c #f6eef6",
".z c #f6f2f6",
"#T c #ff9531",
"#Z c #ffba6a",
"#Y c #ffc683",
"#S c #ffca4a",
".2 c #ffe24a",
".T c #ffe273",
"#Q c #ffe662",
"#R c #ffee6a",
".S c #fff641",
".K c #fff6ff",
".R c #fffa52",
".o c #fffaff",
".1 c #ffff94",
"#D c #ffffbd",
".p c #ffffff",
"QtQtQtQtQt.#.a.b.b.c.d.eQtQtQtQt",
"QtQtQtQt.f.g.h.f.i.j.k.l.mQtQtQt",
"QtQtQt.a.n.o.p.q.r.s.t.u.vQtQtQt",
"QtQt.s.w.s.p.o.x.y.z.p.A.B.CQtQt",
"QtQt.D.E.g.F.G.H.I.J.K.L.M.v.NQt",
"QtQt.C.B.O.P.Q.R.S.T.A.U.V.v.mQt",
"Qt.W.B.X.Y.Z.0.1.2.3.4.5.M.m.UQt",
".Y.m.V.M.s.p.o.o.6.7.Z.8.9#.##.U",
"#a#.#b.k.4.0.o.o.K.F#c#d.r.m.B.m",
"#e#f#.#g#h.4#i.z.F.Z#j#k#l.d.e#m",
"QtQt.C.b.q#h.4.4.Z#n.s#o.r#p#qQt",
"QtQt#r.r.A#n.8.8#n.A#s#t#g.vQtQt",
"QtQtQt#u#v#w#x.t.s#y#z#A#B#CQtQt",
"Qt.1#D#E#F#G#H#I.L#J#K#L#M#N#OQt",
"Qt#P#Q#R#S#T#U#V#W#X#Y#Z#0#1#qQt",
"Qt#2#3#4#5#6#7#e###8#9a.a#aa#eQt"};


/* 
 *  Constructs a IdentityEditDialog as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
IdentityEditDialog::IdentityEditDialog( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
,
      image0( (const char **) img0_identityeditdialog )
{
    if ( !name )
	setName( "IdentityEditDialog" );
    setIcon( image0 );
    IdentityEditDialogLayout = new QGridLayout( this, 1, 1, 11, 6, "IdentityEditDialogLayout"); 

    tabWidget6 = new QTabWidget( this, "tabWidget6" );

    tab = new QWidget( tabWidget6, "tab" );
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout"); 

    textLabel1 = new QLabel( tab, "textLabel1" );

    tabLayout->addWidget( textLabel1, 0, 0 );

    m_login = new QLineEdit( tab, "m_login" );
    m_login->setMinimumSize( QSize( 200, 0 ) );
    m_login->setEdited( TRUE );

    tabLayout->addWidget( m_login, 0, 1 );

    m_password = new QLineEdit( tab, "m_password" );
    m_password->setEchoMode( QLineEdit::Password );

    tabLayout->addWidget( m_password, 1, 1 );

    textLabel2 = new QLabel( tab, "textLabel2" );

    tabLayout->addWidget( textLabel2, 1, 0 );

    m_nick = new QLineEdit( tab, "m_nick" );

    tabLayout->addWidget( m_nick, 2, 1 );

    textLabel3 = new QLabel( tab, "textLabel3" );

    tabLayout->addWidget( textLabel3, 2, 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( spacer, 3, 1 );
    tabWidget6->insertTab( tab, "" );

    IdentityEditDialogLayout->addMultiCellWidget( tabWidget6, 0, 0, 0, 2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    IdentityEditDialogLayout->addItem( spacer_2, 1, 0 );

    pushButton15 = new QPushButton( this, "pushButton15" );

    IdentityEditDialogLayout->addWidget( pushButton15, 1, 1 );

    pushButton14 = new QPushButton( this, "pushButton14" );

    IdentityEditDialogLayout->addWidget( pushButton14, 1, 2 );
    languageChange();
    resize( QSize(347, 197).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( pushButton15, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( pushButton14, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
IdentityEditDialog::~IdentityEditDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void IdentityEditDialog::languageChange()
{
    setCaption( tr2i18n( "Edit Identity" ) );
    textLabel1->setText( tr2i18n( "Your login:" ) );
    textLabel2->setText( tr2i18n( "Password:" ) );
    textLabel3->setText( tr2i18n( "Your nick name:" ) );
    tabWidget6->changeTab( tab, tr2i18n( "&General" ) );
    pushButton15->setText( tr2i18n( "&Ok" ) );
    pushButton14->setText( tr2i18n( "&Cancel" ) );
}

#include "identityeditdialog.moc"
