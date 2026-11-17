/**********************************************************************
** Copyright (C) 2002 Olaf Lueg.  All rights reserved.
** Copyright (C) 2002 KMerlin Developers Team.  All rights reserved.
**
** This file is part of KMerlin.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://kmerlin.olsd.com/gpl/ for GPL licensing information.
**
** Contact olueg@olsd.de if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "settingsdialog.h"
#include "chatsetting.h"
#include "replacepage.h"
#include "../kmerlin.h"
#include "../kmservice.h"
#include "../kmtextreplace.h"
#include "../kmerlinprofile.h"
#include "../kmerlinnotify.h"
#include "identityeditdialog.h"
#include "identitysetting.h"
#include "systemnotification.h"


#include <qvariant.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qbutton.h>
#include <qcheckbox.h>
#include <qspinbox.h>

#include <klocale.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kfontcombo.h>
#include <kcolorbutton.h>
#include <kio/passdlg.h>
#include <kurlrequester.h>

SettingsDialog::SettingsDialog( QWidget *parent, const char *name )
		: KDialogBase( IconList, i18n( "Settings" ), Help | Ok | Cancel, Ok, parent, name, false, true )
{
	theApp = KMerlin::getInstance();
	QFrame *identitySettingFrame = addPage( i18n( "Identity" ) , i18n( "Identity" ) ,
	                                        KGlobal::instance() ->iconLoader() ->loadIcon( "identity" , KIcon::NoGroup , KIcon::SizeMedium ) );

	QFrame * notificationSettingFrame = addPage( i18n( "Notification" ) , i18n( "Notification Settings" ) ,
	                                    KGlobal::instance() ->iconLoader() ->loadIcon( "date" , KIcon::NoGroup , KIcon::SizeMedium ) );

	QFrame *chatSettingFrame = addPage( i18n( "Chat" ) , i18n( "Chat" ) ,
	                                    KGlobal::instance() ->iconLoader() ->loadIcon( "desktop" , KIcon::NoGroup , KIcon::SizeMedium ) );
	QFrame *replaceFrame = addPage( i18n( "Replace" ) , i18n( "Replace" ) ,
	                                KGlobal::instance() ->iconLoader() ->loadIcon( "edit" , KIcon::NoGroup , KIcon::SizeMedium ) );


	QVBoxLayout *identitySettingLayout = new QVBoxLayout( identitySettingFrame );
	QVBoxLayout *notificationSettingLayout = new QVBoxLayout( notificationSettingFrame );
	QVBoxLayout *chatSettingLayout = new QVBoxLayout( chatSettingFrame );
	QVBoxLayout *replaceLayout = new QVBoxLayout( replaceFrame );


	identitySetting = new IdentitySetting( identitySettingFrame, "Identity" );
	systemNotifications = new SystemNotification( notificationSettingFrame, "NotificationSettings" );
	chatSetting = new ChatSetting( chatSettingFrame, "Chat" );
	replacePage = new ReplacePage( replaceFrame, "Replace Window" );


	identitySettingLayout->addWidget( identitySetting );
	notificationSettingLayout->addWidget( systemNotifications );
	chatSettingLayout->addWidget( chatSetting );
	replaceLayout->addWidget( replacePage );

	// connections from baseSetting
	readConfig();
	connect( ( QObject* ) replacePage->btnRemove, SIGNAL( clicked() ), this, SLOT( slotRemoveReplaceText() ) );
	connect( identitySetting, SIGNAL( profileChanged() ), this, SIGNAL( profileChanged() ) );
	
}
/**/
SettingsDialog::~SettingsDialog()
{}
/**/
void SettingsDialog::init()
{
	
}
// read the config file
void SettingsDialog::readConfig()
{
	KConfig * config = KGlobal::config();
	config->setGroup( "General" );
	identitySetting->m_idle->setValue( config->readNumEntry( "MaxIdle", 15 ) );

	config->setGroup( "Text Replacement" );
	replaceList = config->readListEntry( "Replace List" );
	for ( uint j = 0; j < replaceList.count(); j++ ) {
		new QListViewItem( replacePage->List, replaceList[ j ], config->readEntry( replaceList[ j ], QString::null ) );
	}

}

// save to config file
void SettingsDialog::saveConfig()
{
	chatSetting->save();
	systemNotifications->save();
    theApp->autoConnect = identitySetting->m_auto->isChecked();
	KConfig *config = KGlobal::config();
	config->setGroup( "Profiles" );
	config->writeEntry( "Profiles", theApp->profiles );

	config->setGroup( "General" );
	theApp->maxIdle = identitySetting->m_idle->value();
	config->writeEntry( "MaxIdle", theApp->maxIdle );
	theApp->setIdleTimer();
	
	config->setGroup( "Text Replacement" );
	// delete old entrys
	for ( uint j = 0; j < deletedReplace.count() ;j++ ) {
		config->deleteEntry( deletedReplace[ j ] );
	}
	replaceList.clear();
	QListViewItemIterator it( replacePage->List );
	for ( ; it.current(); ++it ) {
		replaceList.append( it.current() ->text( 0 ) );
		config->writeEntry( it.current() ->text( 0 ), it.current() ->text( 1 ) );
	}
	config->writeEntry( "Replace List", replaceList );
	config->sync();
	  // Read Settings for the notification system
  KMerlinNotify::readSettings( config );
	theApp->readReplaceList( config );
}

/**/
void SettingsDialog::slotOk()
{
	saveConfig();
	hide();
	delete this;
}

/**/
void SettingsDialog::slotCancel()
{
	hide();
	delete this;
}
/**/
void SettingsDialog::slotRemoveReplaceText()
{
	if ( replacePage->List->currentItem() != 0 ) {
		deletedReplace.append( replacePage->List->currentItem() ->text( 0 ) );
		delete replacePage->List->currentItem() ;
	}
}
//#include "settingsdialog.moc"
