#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './chatsetting.ui'
**
** Created: Sam Jan 24 20:26:10 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "chatsetting.h"

#include <qvariant.h>
#include <kcolorbutton.h>
#include <kfontcombo.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "./chatsetting.ui.h"
static const char* const img0_chatsetting[] = { 
"20 20 141 2",
"Qt c None",
".Q c #000000",
".a c #080808",
".d c #101410",
".b c #181818",
"#h c #201400",
".m c #291c00",
"#L c #392800",
".e c #393439",
".6 c #413000",
"#M c #413429",
"#D c #4a2c00",
"#x c #4a3008",
".5 c #4a3400",
".l c #523400",
"#r c #523800",
".# c #525052",
".h c #5a4000",
".c c #5a595a",
"#l c #624400",
"#K c #624c10",
"#q c #6a4c00",
"#T c #7b5000",
"#g c #7b5008",
"#k c #7b5900",
".r c #7b5d20",
"#5 c #835d08",
".n c #836d4a",
".X c #838183",
".3 c #8b5d00",
".i c #8b6100",
".B c #8b6510",
".w c #8b6918",
"#d c #8b7141",
".P c #946500",
".9 c #946d20",
".W c #947118",
".S c #947129",
".Z c #947939",
"#. c #947d4a",
"#b c #948162",
"#c c #94856a",
".u c #949194",
"## c #9c6900",
"#4 c #a47100",
".j c #a47510",
".K c #a4a5b4",
"#J c #ac7d18",
".R c #aca5ac",
".2 c #b47900",
"#y c #b48929",
"#N c #b49141",
".v c #b4b6c5",
".E c #b4babd",
".t c #b4bac5",
"#U c #bd8910",
"#a c #bd954a",
".L c #c5c2c5",
".7 c #cd8d00",
"#6 c #cd9518",
".s c #cdcede",
"#s c #d59100",
"#E c #d59108",
"#C c #d59918",
".F c #d5a139",
".4 c #d5d6de",
"#e c #de9500",
"#i c #dea529",
"#W c #e6a110",
".k c #e6aa18",
"#V c #e6aa31",
"#w c #eea108",
"#X c #eea500",
".M c #eee6ee",
".J c #f6a100",
"#I c #f6aa00",
"#u c #f6c252",
".x c #f6c662",
".Y c #f6f2f6",
"#t c #ffaa00",
".N c #ffae00",
".A c #ffb200",
"#z c #ffb208",
".q c #ffb600",
".V c #ffb608",
".1 c #ffb610",
"#S c #ffb618",
".p c #ffba00",
"#9 c #ffba08",
"#P c #ffba10",
"#7 c #ffba18",
"ad c #ffba20",
"#f c #ffbe00",
".8 c #ffbe08",
"a. c #ffbe18",
"#F c #ffbe29",
"#2 c #ffbe31",
".U c #ffc208",
"#O c #ffc210",
"#8 c #ffc218",
"a# c #ffc229",
".G c #ffc231",
"#R c #ffc241",
"ai c #ffc610",
"#m c #ffc618",
"aa c #ffc631",
"ak c #ffc641",
"#Q c #ffc64a",
".0 c #ffca08",
".g c #ffca10",
".o c #ffca18",
"#A c #ffca29",
"#n c #ffca39",
".O c #ffce10",
"ac c #ffce18",
"#Y c #ffce31",
".H c #ffce4a",
"ah c #ffd220",
"#p c #ffd252",
"#H c #ffd262",
".T c #ffd620",
"aj c #ffd631",
"ae c #ffd639",
".I c #ffd641",
"#o c #ffd66a",
"#3 c #ffd673",
"#1 c #ffd67b",
"ag c #ffda39",
"#v c #ffda7b",
"#G c #ffda8b",
"#Z c #ffde7b",
"#0 c #ffde8b",
"#j c #ffde94",
"ab c #ffe65a",
"#B c #ffea94",
".y c #ffee8b",
".z c #fffaac",
".C c #fffaff",
"af c #ffffd5",
".f c #ffffee",
".D c #ffffff",
"QtQtQtQtQtQtQt.#.a.b.c.d.eQtQtQtQtQtQtQt",
"QtQtQtQtQt.f.g.h.i.j.k.l.m.nQtQtQtQtQtQt",
"QtQtQtQt.o.p.q.r.s.t.u.v.w.x.y.zQtQtQtQt",
"QtQtQt.o.q.A.B.C.D.D.D.D.E.F.G.H.IQtQtQt",
"QtQt.o.q.q.J.K.D.L.D.M.D.D.i.q.N.p.oQtQt",
"Qt.O.q.A.A.P.D.D.Q.R.Q.D.D.S.q.A.A.q.TQt",
"Qt.U.V.A.q.W.D.D.X.Y.#.D.D.Z.p.A.V.q.0Qt",
".1.2.3.p.q.P.D.D.D.4.D.D.D.w.q.p.i.5.UQt",
".1.5.6.7.8.N.9#.###a#b#c#d#e#f.A#g#h#i#j",
".A.p.q#k#l.N#m#n.A#o#p.p#f.A#q#r#s#t#u#v",
".A.A.V.q#w.5#x#y#z#A#B#C#D#h#E.p.q#F#G#H",
"#z.A.N.A.q#f#I#J#K#L.m#M#N#O.q#P#Q#G#R.G",
"#S.N.A.A#z.A#T#U#V#W#X#Y#Z#0#j#G#1#2.q#3",
"#2.q.N.A.N.q.A#4#5.w#6.A.N.1#7.1.N.A#8Qt",
"Qt.U.A.A.A.A.q.q#9a.a#.G#F#P#P#7#FaaabQt",
"Qtac.q.A.N.N.N.A.N.A#z#7ad#F#F#F#S#AQtQt",
"QtQtae.q.A.A.A.A.A.N.A.A#z#z.V#9#mafQtQt",
"QtQtQtag.8.q.N.A.N.A.N.A.N.A.q.oQtQtQtQt",
"QtQtQtQtQtahai.8.q.N.A.q.UaiajQtQtQtQtQt",
"QtQtQtQtQtQtQtQt#F.A#zakQtQtQtQtQtQtQtQt"};


/* 
 *  Constructs a ChatSetting as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
ChatSetting::ChatSetting( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl ),
      image0( (const char **) img0_chatsetting )
{
    if ( !name )
	setName( "ChatSetting" );
    ChatSettingLayout = new QGridLayout( this, 1, 1, 6, 6, "ChatSettingLayout"); 

    layout2 = new QGridLayout( 0, 1, 1, 0, 6, "layout2"); 

    m_color = new KColorButton( this, "m_color" );
    m_color->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, m_color->sizePolicy().hasHeightForWidth() ) );

    layout2->addWidget( m_color, 1, 1 );

    m_font = new KFontCombo( this, "m_font" );

    layout2->addWidget( m_font, 0, 1 );

    m_defaultFont = new QCheckBox( this, "m_defaultFont" );

    layout2->addWidget( m_defaultFont, 0, 2 );

    textLabel4 = new QLabel( this, "textLabel4" );

    layout2->addWidget( textLabel4, 1, 0 );
    QSpacerItem* spacer = new QSpacerItem( 131, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer, 0, 3 );

    textLabel3 = new QLabel( this, "textLabel3" );

    layout2->addWidget( textLabel3, 0, 0 );

    ChatSettingLayout->addLayout( layout2, 0, 0 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 140, QSizePolicy::Minimum, QSizePolicy::Expanding );
    ChatSettingLayout->addItem( spacer_2, 2, 0 );

    layout3 = new QGridLayout( 0, 1, 1, 0, 6, "layout3"); 

    m_logFile = new QLineEdit( this, "m_logFile" );
    m_logFile->setEnabled( TRUE );
    m_logFile->setMaximumSize( QSize( 200, 32767 ) );

    layout3->addMultiCellWidget( m_logFile, 2, 2, 1, 3 );
    QSpacerItem* spacer_3 = new QSpacerItem( 141, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer_3, 0, 3 );

    PixmapLabel1 = new QLabel( this, "PixmapLabel1" );
    PixmapLabel1->setPixmap( image0 );
    PixmapLabel1->setScaledContents( FALSE );

    layout3->addWidget( PixmapLabel1, 0, 2 );

    textLabel6 = new QLabel( this, "textLabel6" );
    textLabel6->setEnabled( TRUE );

    layout3->addWidget( textLabel6, 2, 0 );

    m_emotions = new QCheckBox( this, "m_emotions" );
    m_emotions->setChecked( FALSE );

    layout3->addMultiCellWidget( m_emotions, 0, 0, 0, 1 );

    m_logging = new QCheckBox( this, "m_logging" );

    layout3->addMultiCellWidget( m_logging, 1, 1, 0, 3 );

    ChatSettingLayout->addLayout( layout3, 1, 0 );
    languageChange();
    resize( QSize(457, 326).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( m_logging, SIGNAL( toggled(bool) ), m_logFile, SLOT( setEnabled(bool) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ChatSetting::~ChatSetting()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ChatSetting::languageChange()
{
    setCaption( tr2i18n( "Chat Setting" ) );
    m_color->setText( QString::null );
    m_defaultFont->setText( tr2i18n( "Set as default font" ) );
    textLabel4->setText( tr2i18n( "Color:" ) );
    textLabel3->setText( tr2i18n( "Font:" ) );
    m_logFile->setText( tr2i18n( "%E.log" ) );
    QToolTip::add( m_logFile, tr2i18n( "Valid placeholders are: <br>%E = E-Mail Address<br>%N = Nick name<br>%T = Time" ) );
    textLabel6->setText( tr2i18n( "Filename:" ) );
    m_emotions->setText( tr2i18n( "Enable emotion icons" ) );
    m_logging->setText( tr2i18n( "Enable logging" ) );
}

#include "chatsetting.moc"
