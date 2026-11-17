#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './systemnotification.ui'
**
** Created: Sam Jan 24 20:26:26 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "systemnotification.h"

#include <qvariant.h>
#include <klistview.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "./systemnotification.ui.h"

/* 
 *  Constructs a SystemNotification as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
SystemNotification::SystemNotification( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "SystemNotification" );
    SystemNotificationLayout = new QGridLayout( this, 1, 1, 11, 6, "SystemNotificationLayout"); 

    m_List = new KListView( this, "m_List" );
    m_List->addColumn( QString::null );
    m_List->header()->setClickEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->header()->setResizeEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->addColumn( QString::null );
    m_List->header()->setClickEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->header()->setResizeEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->addColumn( QString::null );
    m_List->header()->setClickEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->header()->setResizeEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->addColumn( QString::null );
    m_List->header()->setClickEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->header()->setResizeEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->addColumn( QString::null );
    m_List->header()->setClickEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->header()->setResizeEnabled( FALSE, m_List->header()->count() - 1 );
    m_List->addColumn( tr2i18n( "Actions" ) );
    m_List->setFrameShape( KListView::StyledPanel );
    m_List->setFrameShadow( KListView::Sunken );
    m_List->setAllColumnsShowFocus( TRUE );
    m_List->setResizeMode( KListView::LastColumn );

    SystemNotificationLayout->addMultiCellWidget( m_List, 0, 0, 0, 1 );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    btnPlay = new QPushButton( groupBox1, "btnPlay" );
    btnPlay->setEnabled( FALSE );
    btnPlay->setAutoMask( TRUE );

    groupBox1Layout->addWidget( btnPlay, 0, 1 );

    m_sound = new QCheckBox( groupBox1, "m_sound" );

    groupBox1Layout->addWidget( m_sound, 0, 0 );

    m_app = new QCheckBox( groupBox1, "m_app" );
    m_app->setEnabled( FALSE );

    groupBox1Layout->addMultiCellWidget( m_app, 2, 2, 0, 1 );

    m_event = new QCheckBox( groupBox1, "m_event" );
    m_event->setEnabled( TRUE );

    groupBox1Layout->addMultiCellWidget( m_event, 4, 4, 0, 1 );

    m_popup = new QCheckBox( groupBox1, "m_popup" );

    groupBox1Layout->addMultiCellWidget( m_popup, 3, 3, 0, 2 );

    m_file = new QCheckBox( groupBox1, "m_file" );

    groupBox1Layout->addWidget( m_file, 1, 0 );

    btnSound = new QPushButton( groupBox1, "btnSound" );
    btnSound->setEnabled( FALSE );

    groupBox1Layout->addWidget( btnSound, 0, 3 );

    btnLog = new QPushButton( groupBox1, "btnLog" );
    btnLog->setEnabled( FALSE );

    groupBox1Layout->addWidget( btnLog, 1, 3 );

    btnApp = new QPushButton( groupBox1, "btnApp" );
    btnApp->setEnabled( FALSE );

    groupBox1Layout->addWidget( btnApp, 2, 3 );

    m_soundFile = new QLineEdit( groupBox1, "m_soundFile" );
    m_soundFile->setEnabled( FALSE );

    groupBox1Layout->addWidget( m_soundFile, 0, 2 );

    m_fileName = new QLineEdit( groupBox1, "m_fileName" );
    m_fileName->setEnabled( FALSE );

    groupBox1Layout->addWidget( m_fileName, 1, 2 );

    m_appFile = new QLineEdit( groupBox1, "m_appFile" );
    m_appFile->setEnabled( FALSE );

    groupBox1Layout->addWidget( m_appFile, 2, 2 );

    SystemNotificationLayout->addMultiCellWidget( groupBox1, 1, 1, 0, 1 );

    m_animate = new QCheckBox( this, "m_animate" );
    m_animate->setEnabled( TRUE );

    SystemNotificationLayout->addWidget( m_animate, 2, 0 );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    layout1->addWidget( textLabel1 );

    m_timeout = new QSpinBox( this, "m_timeout" );
    layout1->addWidget( m_timeout );

    textLabel2 = new QLabel( this, "textLabel2" );
    layout1->addWidget( textLabel2 );

    SystemNotificationLayout->addLayout( layout1, 3, 0 );
    QSpacerItem* spacer = new QSpacerItem( 251, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    SystemNotificationLayout->addItem( spacer, 3, 1 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding );
    SystemNotificationLayout->addItem( spacer_2, 4, 0 );
    languageChange();
    resize( QSize(567, 390).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( m_sound, SIGNAL( toggled(bool) ), m_soundFile, SLOT( setEnabled(bool) ) );
    connect( m_sound, SIGNAL( toggled(bool) ), btnPlay, SLOT( setEnabled(bool) ) );
    connect( m_file, SIGNAL( toggled(bool) ), m_fileName, SLOT( setEnabled(bool) ) );
    connect( m_app, SIGNAL( toggled(bool) ), m_appFile, SLOT( setEnabled(bool) ) );
    connect( m_sound, SIGNAL( toggled(bool) ), btnSound, SLOT( setEnabled(bool) ) );
    connect( m_file, SIGNAL( toggled(bool) ), btnLog, SLOT( setEnabled(bool) ) );
    connect( m_sound, SIGNAL( toggled(bool) ), this, SLOT( checkerToggled(bool) ) );
    connect( m_file, SIGNAL( toggled(bool) ), this, SLOT( checkerToggled(bool) ) );
    connect( m_app, SIGNAL( toggled(bool) ), this, SLOT( checkerToggled(bool) ) );
    connect( m_popup, SIGNAL( toggled(bool) ), this, SLOT( checkerToggled(bool) ) );
    connect( m_event, SIGNAL( toggled(bool) ), this, SLOT( checkerToggled(bool) ) );
    connect( m_List, SIGNAL( selectionChanged(QListViewItem*) ), this, SLOT( selectionChanged(QListViewItem*) ) );
    connect( btnPlay, SIGNAL( clicked() ), this, SLOT( playSound() ) );
    connect( btnSound, SIGNAL( clicked() ), this, SLOT( openSound() ) );
    connect( btnLog, SIGNAL( clicked() ), this, SLOT( openLog() ) );
    connect( btnApp, SIGNAL( clicked() ), this, SLOT( openApp() ) );
    connect( m_soundFile, SIGNAL( textChanged(const QString&) ), this, SLOT( soundChanged(const QString&) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
SystemNotification::~SystemNotification()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SystemNotification::languageChange()
{
    setCaption( tr2i18n( "System Notifications" ) );
    m_List->header()->setLabel( 0, QString::null );
    m_List->header()->setLabel( 1, QString::null );
    m_List->header()->setLabel( 2, QString::null );
    m_List->header()->setLabel( 3, QString::null );
    m_List->header()->setLabel( 4, QString::null );
    m_List->header()->setLabel( 5, tr2i18n( "Actions" ) );
    groupBox1->setTitle( tr2i18n( "Aktionen" ) );
    btnPlay->setText( QString::null );
    m_sound->setText( tr2i18n( "Play a sound:" ) );
    m_app->setText( tr2i18n( "Execute a program:" ) );
    m_event->setText( tr2i18n( "Log to event panel:" ) );
    m_popup->setText( tr2i18n( "Show a message in a pop-up window:" ) );
    m_file->setText( tr2i18n( "Log to a file:" ) );
    btnSound->setText( QString::null );
    btnLog->setText( QString::null );
    btnApp->setText( QString::null );
    m_animate->setText( tr2i18n( "Animate system tray icon" ) );
    textLabel1->setText( tr2i18n( "Default timeout for pop-up messages" ) );
    textLabel2->setText( tr2i18n( "secs." ) );
}

#include "systemnotification.moc"
