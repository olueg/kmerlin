#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './authdlg.ui'
**
** Created: Sam Jan 24 20:34:15 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "authdlg.h"

#include <qvariant.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "./authdlg.ui.h"
/* 
 *  Constructs a AuthDlg as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
AuthDlg::AuthDlg( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "AuthDlg" );

    frame5 = new QFrame( this, "frame5" );
    frame5->setGeometry( QRect( 0, 0, 400, 300 ) );
    frame5->setPaletteBackgroundPixmap( getBgPixmap(  ) );
    frame5->setBackgroundOrigin( QFrame::AncestorOrigin );
    frame5->setFrameShape( QFrame::StyledPanel );
    frame5->setFrameShadow( QFrame::Raised );
    frame5->setLineWidth( 0 );

    textLabel4 = new QLabel( frame5, "textLabel4" );
    textLabel4->setGeometry( QRect( 190, 120, 190, 30 ) );
    textLabel4->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel4->setBackgroundOrigin( QLabel::AncestorOrigin );
    QFont textLabel4_font(  textLabel4->font() );
    textLabel4_font.setBold( TRUE );
    textLabel4->setFont( textLabel4_font ); 

    btnOK = new QPushButton( frame5, "btnOK" );
    btnOK->setGeometry( QRect( 140, 260, 111, 30 ) );
    btnOK->setAutoMask( TRUE );
    btnOK->setDefault( TRUE );

    m_block = new QCheckBox( frame5, "m_block" );
    m_block->setGeometry( QRect( 70, 220, 270, 22 ) );
    m_block->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    m_block->setBackgroundOrigin( QCheckBox::AncestorOrigin );
    m_block->setAutoMask( TRUE );

    m_allow = new QCheckBox( frame5, "m_allow" );
    m_allow->setGeometry( QRect( 70, 190, 290, 22 ) );
    m_allow->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    m_allow->setBackgroundOrigin( QCheckBox::AncestorOrigin );
    m_allow->setAutoMask( TRUE );
    m_allow->setChecked( TRUE );

    m_handle = new QLabel( frame5, "m_handle" );
    m_handle->setGeometry( QRect( 160, 150, 230, 20 ) );
    m_handle->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    m_handle->setBackgroundOrigin( QLabel::AncestorOrigin );
    QFont m_handle_font(  m_handle->font() );
    m_handle_font.setBold( TRUE );
    m_handle->setFont( m_handle_font ); 
    m_handle->setAlignment( int( QLabel::AlignCenter ) );
    languageChange();
    resize( QSize(400, 299).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( m_allow, SIGNAL( clicked() ), m_block, SLOT( toggle() ) );
    connect( m_block, SIGNAL( clicked() ), m_allow, SLOT( toggle() ) );

    // tab order
    setTabOrder( m_allow, m_block );
    setTabOrder( m_block, btnOK );
}

/*
 *  Destroys the object and frees any allocated resources
 */
AuthDlg::~AuthDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AuthDlg::languageChange()
{
    setCaption( tr2i18n( "Authenticate" ) );
    textLabel4->setText( tr2i18n( "<h3><b>New contact received!</b></h3>" ) );
    btnOK->setText( tr2i18n( "&Ok" ) );
    m_block->setText( tr2i18n( "Block Contact" ) );
    m_allow->setText( tr2i18n( "Allow this contact to  see your online state" ) );
    m_handle->setText( QString::null );
}

#include "authdlg.moc"
