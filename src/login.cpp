#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './login.ui'
**
** Created: Sam Jan 24 20:33:58 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "login.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "./login.ui.h"
/* 
 *  Constructs a Login as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
Login::Login( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "Login" );
    setMaximumSize( QSize( 400, 300 ) );
    LoginLayout = new QGridLayout( this, 1, 1, 0, 0, "LoginLayout"); 
    QSpacerItem* spacer = new QSpacerItem( 20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LoginLayout->addItem( spacer, 1, 0 );

    m_frame = new QFrame( this, "m_frame" );
    m_frame->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, m_frame->sizePolicy().hasHeightForWidth() ) );
    m_frame->setMinimumSize( QSize( 400, 300 ) );
    m_frame->setMaximumSize( QSize( 400, 300 ) );
    m_frame->setPaletteBackgroundPixmap( getBgPixmap(  ) );
    m_frame->setBackgroundOrigin( QFrame::AncestorOrigin );
    m_frame->setFrameShape( QFrame::StyledPanel );
    m_frame->setFrameShadow( QFrame::Raised );
    m_frame->setLineWidth( 0 );

    textLabel1 = new QLabel( m_frame, "textLabel1" );
    textLabel1->setGeometry( QRect( 183, 117, 200, 21 ) );
    textLabel1->setMaximumSize( QSize( 200, 32767 ) );
    textLabel1->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel1->setBackgroundOrigin( QLabel::AncestorOrigin );

    m_profile = new QComboBox( FALSE, m_frame, "m_profile" );
    m_profile->setGeometry( QRect( 180, 139, 200, 30 ) );
    m_profile->setMaximumSize( QSize( 200, 32767 ) );
    m_profile->setAutoMask( TRUE );

    m_login = new QLineEdit( m_frame, "m_login" );
    m_login->setGeometry( QRect( 180, 179, 200, 22 ) );
    m_login->setMaximumSize( QSize( 200, 32767 ) );

    m_password = new QLineEdit( m_frame, "m_password" );
    m_password->setGeometry( QRect( 180, 241, 200, 22 ) );
    m_password->setMaximumSize( QSize( 200, 32767 ) );
    m_password->setEchoMode( QLineEdit::Password );

    textLabel2 = new QLabel( m_frame, "textLabel2" );
    textLabel2->setGeometry( QRect( 20, 180, 150, 21 ) );
    textLabel2->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel2->setBackgroundOrigin( QLabel::AncestorOrigin );
    textLabel2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );

    textLabel3 = new QLabel( m_frame, "textLabel3" );
    textLabel3->setGeometry( QRect( 20, 240, 150, 21 ) );
    textLabel3->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel3->setBackgroundOrigin( QLabel::AncestorOrigin );
    textLabel3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );

    textLabel3_2 = new QLabel( m_frame, "textLabel3_2" );
    textLabel3_2->setGeometry( QRect( 20, 210, 150, 21 ) );
    textLabel3_2->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel3_2->setPaletteBackgroundPixmap( getBgPixmap(  ) );
    textLabel3_2->setBackgroundOrigin( QLabel::AncestorOrigin );
    textLabel3_2->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter | QLabel::AlignRight ) );

    m_nick = new QLineEdit( m_frame, "m_nick" );
    m_nick->setGeometry( QRect( 180, 210, 200, 22 ) );

    m_save = new QCheckBox( m_frame, "m_save" );
    m_save->setGeometry( QRect( 20, 270, 200, 22 ) );
    m_save->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    m_save->setBackgroundOrigin( QCheckBox::AncestorOrigin );

    btnConnect = new QPushButton( m_frame, "btnConnect" );
    btnConnect->setGeometry( QRect( 260, 266, 120, 30 ) );
    btnConnect->setBackgroundOrigin( QPushButton::AncestorOrigin );
    btnConnect->setAutoMask( TRUE );
    btnConnect->setOn( FALSE );
    btnConnect->setAutoRepeat( TRUE );
    btnConnect->setAutoDefault( TRUE );
    btnConnect->setDefault( TRUE );
    btnConnect->setFlat( FALSE );

    LoginLayout->addWidget( m_frame, 0, 0 );
    languageChange();
    resize( QSize(400, 300).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( btnConnect, SIGNAL( clicked() ), this, SLOT( slotConnect() ) );
    connect( m_login, SIGNAL( textChanged(const QString&) ), this, SLOT( slotDataChanged(const QString&) ) );
    connect( m_nick, SIGNAL( textChanged(const QString&) ), this, SLOT( slotDataChanged(const QString&) ) );
    connect( m_password, SIGNAL( textChanged(const QString&) ), this, SLOT( slotDataChanged(const QString&) ) );
    connect( m_profile, SIGNAL( activated(int) ), this, SLOT( slotActivated(int) ) );

    // tab order
    setTabOrder( m_profile, m_login );
    setTabOrder( m_login, m_nick );
    setTabOrder( m_nick, m_password );
    setTabOrder( m_password, btnConnect );
    setTabOrder( btnConnect, m_save );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
Login::~Login()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Login::languageChange()
{
    setCaption( tr2i18n( "Login" ) );
    textLabel1->setText( tr2i18n( "<b>Choose Profile</b>" ) );
    textLabel2->setText( tr2i18n( "<b>Login:</b>" ) );
    textLabel3->setText( tr2i18n( "<b>Password:</b>" ) );
    textLabel3_2->setText( tr2i18n( "<b>Nick name:</b>" ) );
    m_save->setText( tr2i18n( "Save Profile" ) );
    btnConnect->setText( tr2i18n( "&Connect" ) );
    btnConnect->setAccel( QKeySequence( tr2i18n( "Ctrl+C" ) ) );
}

#include "login.moc"
