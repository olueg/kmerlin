#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './identitysetting.ui'
**
** Created: Sam Jan 24 20:25:45 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "identitysetting.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "./identitysetting.ui.h"
static const char* const img0_identitysetting[] = { 
"16 16 186 2",
".B c None",
"## c #000000",
"aS c #000010",
"aR c #000c31",
"aH c #001c41",
"aq c #001c94",
"aI c #00287b",
"#Q c #0028bd",
"ap c #0030ac",
"ao c #0034bd",
"#E c #003cc5",
"#t c #0048cd",
"#i c #0055d5",
"#N c #0055ee",
"#s c #005dde",
"#Z c #005dee",
".8 c #0061d5",
".V c #006dde",
"#g c #0075f6",
"aQ c #081439",
"aP c #081c41",
"aO c #081c4a",
"aN c #082052",
"aM c #082462",
"aL c #082862",
"aJ c #08286a",
"aK c #082c6a",
"#D c #0859de",
"#h c #086dee",
".7 c #0875e6",
".Y c #101020",
".M c #101420",
"aF c #102062",
"#R c #1844b4",
"#M c #187df6",
"#u c #2055c5",
".9 c #2069cd",
".J c #2085e6",
".U c #2089ee",
".6 c #2091f6",
"#F c #2950c5",
"#j c #2961cd",
".W c #2975d5",
"#0 c #2975ee",
".K c #297dd5",
"a# c #2981ee",
".A c #313441",
"ar c #3148a4",
"aC c #3161b4",
"aA c #3165cd",
"az c #3171de",
"ax c #317df6",
"aw c #3185ff",
"aY c #393831",
"a0 c #393c39",
"aD c #3959ac",
"aB c #3969cd",
"ay c #397de6",
"#r c #398df6",
".R c #39aeff",
".4 c #39b2ff",
"aX c #413831",
"aV c #413c31",
"aZ c #413c39",
"a1 c #414041",
"af c #415db4",
"an c #4171de",
"#Y c #4195ff",
".I c #41a1f6",
".T c #41a5ff",
".H c #41aeff",
".G c #41b6ff",
"#c c #41c2ff",
"aW c #4a4039",
".S c #4aaeff",
".5 c #4ab2ff",
".2 c #4ac6ff",
"aE c #5265a4",
".z c #527dbd",
"ac c #527dde",
"#P c #5285de",
"aa c #5289f6",
"#L c #52aaff",
"#y c #52baff",
"#4 c #5a75cd",
"#3 c #5a75d5",
"av c #5aa5ff",
".y c #5aaaee",
"#8 c #5ab6ff",
"#V c #5abaff",
"#J c #5ac2ff",
"#w c #5ac6ff",
".O c #5aceff",
"as c #626d9c",
"ad c #6289d5",
"ak c #62a5ff",
"#X c #62aeff",
".x c #62b6ff",
".w c #62baff",
".Q c #62c6ff",
"a2 c #6a656a",
"#5 c #6a79ac",
".n c #6a9dcd",
"au c #6aaef6",
".v c #6ac2ff",
".u c #6ac6ff",
".1 c #6ad6ff",
"aU c #73716a",
"ag c #737dac",
"#G c #7381bd",
"#S c #7385b4",
"#k c #7385bd",
".X c #7389bd",
"#. c #7389c5",
".L c #7391c5",
".m c #73b2ee",
"#q c #73baff",
".E c #73daff",
".l c #7bbef6",
".k c #7bc2f6",
"#U c #7bc6ff",
"#I c #7bcaff",
"#v c #7bceff",
"#f c #83c2ff",
"#7 c #83c6f6",
".j c #83caff",
".t c #83d2ff",
".3 c #83d6ff",
".N c #83e2ff",
"#B c #8bbeff",
"ai c #8bc6f6",
"#W c #8bc6ff",
"aj c #8bcaff",
".i c #8bd2ff",
".F c #8bd6ff",
"#b c #8bdaff",
"al c #94baf6",
"#o c #94ceff",
".D c #94e6ff",
"ae c #9caade",
"am c #9cb6ee",
"a. c #9cc6ff",
"#p c #9cceff",
".h c #9cdaff",
".0 c #9ce2ff",
"#O c #a4c6ff",
"#A c #a4ceff",
".g c #a4deff",
".s c #a4e2ff",
".r c #a4e6ff",
".q c #a4eaff",
"at c #acbecd",
"ab c #acc6f6",
"#C c #accaff",
"#e c #acdaff",
"#d c #ace2ff",
".f c #ace6ff",
"aG c #b4aeac",
"#2 c #b4c6f6",
"#6 c #b4cade",
"ah c #b4cae6",
"#z c #b4daff",
".o c #bdbebd",
"#H c #bdd6e6",
"#T c #bdd6ee",
"#m c #bde2ff",
".e c #bdeaff",
"#l c #c5deee",
"#x c #c5e6ff",
"#K c #c5eaff",
".p c #c5f2ff",
".Z c #cde6ee",
"#a c #cde6f6",
"#n c #cdeeff",
"#1 c #d5deff",
".d c #d5f6ff",
".C c #deeef6",
".P c #def2ff",
".c c #def6ff",
".b c #deffff",
"#9 c #e6eeff",
"aT c #eeeeee",
"a3 c #f6f2f6",
".a c #f6faff",
".# c #f6ffff",
"Qt c #ffffff",
"QtQtQtQtQtQtQt.#Qt.a.a.a.aQtQtQt",
".#.b.c.d.e.f.g.h.i.j.k.l.m.n.oQt",
".a.d.p.q.r.s.t.u.v.w.x.y.m.z.A.B",
".C.p.D.E.fQt.F.G.H.H.I.J.K.L.M.B",
".C.f.N.O.s.P.Q.R.S.T.U.V.W.X.Y.B",
".Z.0.1.2.3.g.4.5.S.6.7.8.9#.##.B",
"#a#b.O#c#d.P#e#e#f#g#h#i#j#k##.B",
"#l.t#c.t#m#n#o#p#q#r#s#t#u#k##.B",
"#l#v#w#x#y.j#z#A#B#C#D#E#F#G##.B",
"#H#I#J#K.S#L#L#M#N#O#P#Q#R#S##.B",
"#T#U#V#m#W#X#Y#Z#0#1#2#3#4#5##.B",
"#6#7#8#W#9a.a#aaabacadaeafag##.B",
"ahaiaj#Xakal#Oamanaoapaqaras##.B",
"atau#favawaxayazaAaBaCaDaEaF##.B",
"aGaHaIaJaKaLaMaNaNaOaPaQaRaS##.B",
"aTaUaVaWaXaXaYaZaYaZaYaZa0a1a2a3"};

static const char* const img1_identitysetting[] = { 
"16 16 143 2",
"Qt c None",
".y c #000000",
"#x c #000008",
"am c #000400",
"ai c #000408",
"aj c #000410",
"#L c #000818",
".u c #00614a",
".t c #007539",
".G c #007541",
".s c #008129",
".Q c #008d39",
".r c #009120",
".2 c #009131",
".q c #009918",
".E c #009931",
".p c #009d18",
".1 c #00a131",
".D c #00a531",
"#c c #00a539",
".C c #00aa29",
".O c #00aa31",
".M c #00ae29",
".0 c #00ae39",
".Z c #00b231",
"#b c #00ba39",
"#o c #00ba4a",
".Y c #00be39",
"#G c #081020",
"#s c #082041",
"#S c #08285a",
".F c #088d39",
".P c #089d39",
".X c #08aa31",
".N c #08b239",
".9 c #08be52",
".U c #101010",
"ad c #102839",
"#T c #102c5a",
"#R c #103431",
"#F c #10385a",
"#j c #108d5a",
"#k c #18304a",
"#8 c #18344a",
"ac c #18345a",
"#w c #185983",
".6 c #187d4a",
".R c #18855a",
".T c #203c5a",
"#J c #2059a4",
"#g c #20617b",
"#H c #20699c",
"#v c #209194",
"#t c #20959c",
"#d c #20a152",
"#C c #20ba5a",
"#U c #293c41",
"#K c #2961a4",
"#I c #296db4",
".B c #29aa39",
"ak c #314873",
"#Z c #314c52",
"ah c #314c6a",
"#u c #31aaac",
"#h c #31b29c",
"#B c #31c66a",
"#a c #31ce73",
"#y c #395583",
".x c #395d83",
"al c #415973",
"#M c #41598b",
".h c #41915a",
".f c #419d52",
".m c #4a5d7b",
"#9 c #4a75b4",
".H c #4a958b",
".g c #4a9962",
".e c #4aa55a",
"#r c #5275b4",
"#Q c #5295ee",
".i c #52997b",
"#V c #5a595a",
"ab c #5a91cd",
".j c #5a998b",
".3 c #5aae94",
"#W c #626d83",
".5 c #6295c5",
"#i c #62c6e6",
".V c #6a798b",
"#l c #6a81ac",
"ae c #6a95cd",
".8 c #6ad2ac",
".J c #7ba1d5",
"#p c #7bcaac",
".c c #839db4",
"ag c #83aacd",
"#Y c #83b2ff",
"#7 c #83baf6",
".o c #83c2a4",
".L c #83c694",
".7 c #83cabd",
"#N c #8bb2d5",
".k c #8bb6bd",
".v c #8bb6c5",
"#n c #8be6b4",
"#X c #9cbef6",
"#E c #9cc6ff",
"aa c #9ccaff",
"#A c #9ce6de",
"#. c #a4caff",
"#0 c #a4d6ff",
"#O c #a4dad5",
"#z c #acc2ee",
"#f c #accaff",
"a. c #accef6",
".S c #acd2de",
".d c #acd2ee",
"#P c #acd2ff",
".A c #b4d6e6",
"#6 c #b4daff",
"a# c #b4deff",
".W c #bddad5",
".a c #bddaff",
".z c #bddeff",
"af c #bde2ff",
"#m c #bde6ff",
".K c #c5daff",
".# c #c5deff",
".l c #c5e2ff",
"#1 c #c5e6ff",
".n c #cde2ff",
"#e c #cde6ee",
".w c #cde6ff",
"#D c #cdeaf6",
".b c #cdeaff",
"#q c #d5eaff",
".I c #d5eeff",
"## c #d5f6ee",
"#5 c #deeeff",
".4 c #def2ff",
"#4 c #e6f2ff",
"#3 c #eefaff",
"#2 c #eeffff",
"QtQtQt.#.#.#.#.a.#.#.b.cQtQtQtQt",
"QtQt.#.d.e.f.g.h.i.j.k.l.mQtQtQt",
"QtQt.n.o.p.q.r.s.t.u.v.w.x.yQtQt",
"Qt.z.A.B.C.D.E.F.G.H.I.J.yQtQtQt",
".#.K.L.M.N.O.P.Q.R.S.#.T.U.VQtQt",
".#.W.X.Y.Z.0.1.2.3.4.5.6.7.8.9Qt",
"#.###a#b#b.Z#c#d#e#f#g#h#i.D#jQt",
"#k#l#m#n#o.Z.D#p#q#r#s#t#u#v#wQt",
"Qt#x#y#z#A#B#C#D#E#F#G#H#I#J#KQt",
"QtQtQt#L#M#N#O#P#Q#R.yQt#S#TQtQt",
"QtQtQt#U#V#W#X#Y#E#Z.yQt.y.yQtQt",
"QtQt#0#1#2#3#4#5#6#7#8.yQt.y.yQt",
"QtQt#9a..4#q#qa#aaabac.yQtQt.yQt",
"QtQtQtadaeafafagahai.y.y.y.y.yQt",
"QtQtQtQtajakalam.y.yQt.y.y.y.yQt",
"QtQtQtQtQtQt.y.yQtQtQtQtQtQtQtQt"};


/* 
 *  Constructs a IdentitySetting as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
IdentitySetting::IdentitySetting( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl ),
      image0( (const char **) img0_identitysetting ),
      image1( (const char **) img1_identitysetting )
{
    if ( !name )
	setName( "IdentitySetting" );
    IdentitySettingLayout = new QGridLayout( this, 1, 1, 11, 6, "IdentitySettingLayout"); 

    layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 

    btnNew = new QPushButton( this, "btnNew" );
    layout4->addWidget( btnNew );

    btnModify = new QPushButton( this, "btnModify" );
    layout4->addWidget( btnModify );

    btnRename = new QPushButton( this, "btnRename" );
    layout4->addWidget( btnRename );

    btnRemove = new QPushButton( this, "btnRemove" );
    layout4->addWidget( btnRemove );

    btnDefault = new QPushButton( this, "btnDefault" );
    layout4->addWidget( btnDefault );

    IdentitySettingLayout->addLayout( layout4, 0, 1 );
    QSpacerItem* spacer = new QSpacerItem( 20, 181, QSizePolicy::Minimum, QSizePolicy::Expanding );
    IdentitySettingLayout->addMultiCell( spacer, 1, 2, 1, 1 );

    m_list = new QListView( this, "m_list" );
    m_list->addColumn( tr2i18n( "Identity Name" ) );
    m_list->header()->setLabel( m_list->header()->count() - 1, image0, tr2i18n( "Identity Name" ) );
    m_list->addColumn( tr2i18n( "E-Mail Address" ) );
    m_list->header()->setLabel( m_list->header()->count() - 1, image1, tr2i18n( "E-Mail Address" ) );
    m_list->setAllColumnsShowFocus( FALSE );
    m_list->setShowSortIndicator( TRUE );
    m_list->setResizeMode( QListView::AllColumns );

    IdentitySettingLayout->addMultiCellWidget( m_list, 0, 1, 0, 0 );

    m_auto = new QCheckBox( this, "m_auto" );
    m_auto->setEnabled( TRUE );

    IdentitySettingLayout->addWidget( m_auto, 2, 0 );

    layout14 = new QHBoxLayout( 0, 0, 6, "layout14"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout14->addWidget( textLabel1 );

    m_idle = new QSpinBox( this, "m_idle" );
    m_idle->setMaximumSize( QSize( 50, 32767 ) );
    m_idle->setValue( 15 );
    layout14->addWidget( m_idle );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setEnabled( TRUE );
    layout14->addWidget( textLabel2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout14->addItem( spacer_2 );

    IdentitySettingLayout->addLayout( layout14, 3, 0 );
    languageChange();
    resize( QSize(789, 601).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( btnNew, SIGNAL( clicked() ), this, SLOT( slotNew() ) );
    connect( m_list, SIGNAL( selectionChanged() ), this, SLOT( slotSelectionChanged() ) );
    connect( btnModify, SIGNAL( clicked() ), this, SLOT( slotModify() ) );
    connect( btnRename, SIGNAL( clicked() ), this, SLOT( slotRename() ) );
    connect( btnRemove, SIGNAL( clicked() ), this, SLOT( slotRemove() ) );
    connect( btnDefault, SIGNAL( clicked() ), this, SLOT( slotDefault() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
IdentitySetting::~IdentitySetting()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void IdentitySetting::languageChange()
{
    setCaption( tr2i18n( "Identity Settings" ) );
    btnNew->setText( tr2i18n( "&New ..." ) );
    btnModify->setText( tr2i18n( "&Modify..." ) );
    btnRename->setText( tr2i18n( "&Rename..." ) );
    btnRemove->setText( tr2i18n( "Remo&ve" ) );
    btnDefault->setText( tr2i18n( "Set as &Default" ) );
    m_list->header()->setLabel( 0, tr2i18n( "Identity Name" ) );
    m_list->header()->setLabel( 1, tr2i18n( "E-Mail Address" ) );
    m_auto->setText( tr2i18n( "Auto connect on start up" ) );
    textLabel1->setText( tr2i18n( "Set status to away after:" ) );
    QToolTip::add( m_idle, tr2i18n( "0 disaable this feature" ) );
    textLabel2->setText( tr2i18n( "Minutes" ) );
}

#include "identitysetting.moc"
