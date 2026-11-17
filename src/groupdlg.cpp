#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './groupdlg.ui'
**
** Created: Sam Jan 24 20:34:25 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "groupdlg.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "./groupdlg.ui.h"
/* 
 *  Constructs a GroupDlg as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
GroupDlg::GroupDlg( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )

{
    if ( !name )
	setName( "GroupDlg" );
    setMinimumSize( QSize( 400, 300 ) );
    setMaximumSize( QSize( 400, 300 ) );
    setPaletteBackgroundPixmap( getBgPixmap(  ) );

    m_info = new QLabel( this, "m_info" );
    m_info->setGeometry( QRect( 30, 170, 340, 20 ) );
    m_info->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    m_info->setBackgroundOrigin( QLabel::AncestorOrigin );

    m_group = new QLabel( this, "m_group" );
    m_group->setGeometry( QRect( 180, 140, 200, 20 ) );
    m_group->setPaletteForegroundColor( QColor( 170, 0, 0 ) );
    m_group->setBackgroundOrigin( QLabel::AncestorOrigin );
    m_group->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    m_caption = new QLabel( this, "m_caption" );
    m_caption->setGeometry( QRect( 190, 120, 190, 24 ) );
    m_caption->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    m_caption->setBackgroundOrigin( QLabel::AncestorOrigin );
    m_caption->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );

    m_text = new QLineEdit( this, "m_text" );
    m_text->setGeometry( QRect( 30, 200, 340, 22 ) );

    btnOk = new QPushButton( this, "btnOk" );
    btnOk->setGeometry( QRect( 150, 260, 112, 30 ) );
    btnOk->setAutoMask( TRUE );
    btnOk->setDefault( TRUE );
    languageChange();
    resize( QSize(400, 300).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( btnOk, SIGNAL( clicked() ), this, SLOT( accept() ) );

    // tab order
    setTabOrder( m_text, btnOk );
}

/*
 *  Destroys the object and frees any allocated resources
 */
GroupDlg::~GroupDlg()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void GroupDlg::languageChange()
{
    setCaption( tr2i18n( "Group" ) );
    m_info->setText( tr2i18n( "Type in the new group name" ) );
    m_group->setText( QString::null );
    m_caption->setText( tr2i18n( "Rename Group" ) );
    btnOk->setText( tr2i18n( "&Ok" ) );
}

#include "groupdlg.moc"
