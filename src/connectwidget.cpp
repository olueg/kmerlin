#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './connectwidget.ui'
**
** Created: Sam Jan 24 20:33:50 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "connectwidget.h"

#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* 
 *  Constructs a ConnectWidget as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
ConnectWidget::ConnectWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ConnectWidget" );
    setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    languageChange();
    resize( QSize(307, 256).expandedTo(minimumSizeHint()) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConnectWidget::~ConnectWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConnectWidget::languageChange()
{
    setCaption( tr2i18n( "Form1" ) );
}

#include "connectwidget.moc"
