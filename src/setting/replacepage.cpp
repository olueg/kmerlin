#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './replacepage.ui'
**
** Created: Sam Jan 24 20:25:23 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "replacepage.h"

#include <qvariant.h>
#include <kpushbutton.h>
#include <qheader.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "./replacepage.ui.h"

/* 
 *  Constructs a ReplacePage as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
ReplacePage::ReplacePage( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ReplacePage" );
    ReplacePageLayout = new QGridLayout( this, 1, 1, 11, 6, "ReplacePageLayout"); 

    Layout1 = new QGridLayout( 0, 1, 1, 0, 6, "Layout1"); 

    newTxt = new QLineEdit( this, "newTxt" );

    Layout1->addWidget( newTxt, 0, 1 );

    orgTxt = new QLineEdit( this, "orgTxt" );

    Layout1->addWidget( orgTxt, 0, 0 );

    KPushButton1 = new KPushButton( this, "KPushButton1" );
    KPushButton1->setAutoDefault( TRUE );
    KPushButton1->setDefault( TRUE );

    Layout1->addWidget( KPushButton1, 0, 2 );

    ReplacePageLayout->addLayout( Layout1, 1, 0 );

    List = new QListView( this, "List" );
    List->addColumn( tr2i18n( "Name" ) );
    List->addColumn( tr2i18n( "Replace with" ) );
    List->setAllColumnsShowFocus( TRUE );
    List->setShowSortIndicator( TRUE );
    List->setResizeMode( QListView::AllColumns );

    ReplacePageLayout->addWidget( List, 0, 0 );

    Layout2 = new QGridLayout( 0, 1, 1, 0, 6, "Layout2"); 
    QSpacerItem* spacer = new QSpacerItem( 221, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout2->addItem( spacer, 0, 0 );

    btnRemove = new KPushButton( this, "btnRemove" );

    Layout2->addWidget( btnRemove, 0, 1 );

    ReplacePageLayout->addLayout( Layout2, 2, 0 );
    languageChange();
    resize( QSize(658, 406).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( orgTxt, SIGNAL( textChanged(const QString&) ), this, SLOT( orgTxt_textChanged(const QString&) ) );
    connect( newTxt, SIGNAL( textChanged(const QString&) ), this, SLOT( newTxt_textChanged(const QString&) ) );
    connect( KPushButton1, SIGNAL( pressed() ), this, SLOT( KPushButton1_pressed() ) );
    connect( List, SIGNAL( selectionChanged(QListViewItem*) ), this, SLOT( List_selectionChanged(QListViewItem*) ) );

    // tab order
    setTabOrder( orgTxt, newTxt );
    setTabOrder( newTxt, KPushButton1 );
    setTabOrder( KPushButton1, List );
}

/*
 *  Destroys the object and frees any allocated resources
 */
ReplacePage::~ReplacePage()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ReplacePage::languageChange()
{
    setCaption( tr2i18n( "Text Replace" ) );
    KPushButton1->setText( tr2i18n( "&Add new" ) );
    List->header()->setLabel( 0, tr2i18n( "Name" ) );
    List->header()->setLabel( 1, tr2i18n( "Replace with" ) );
    btnRemove->setText( tr2i18n( "&Remove" ) );
}

#include "replacepage.moc"
