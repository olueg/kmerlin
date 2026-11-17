/**********************************************************************
** Copyright (C) 2002 Olaf Lueg.  All rights reserved.
** Copyright (C) 2002 KMerlin Developer Team.  All rights reserved.
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
#include "kmcontact.h"
#include "kmerlin.h"
#include "kmservice.h"
#include "kmgroup.h"
#include "kmcontactaction.h"
#include "CContactWidget.h"
#include "qcategorywidget.h"
#include "kmemotionicon.h"

#include <kaction.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <kdebug.h>

#include <qcursor.h>



/**/
KMContact::KMContact( QString handle , QString msnNick )
		: QObject()
{
	m_nick = msnNick;
	m_handle = handle;
	m_msnNick = msnNick;
	parsedNick = msnNick;
	m_status = "FLN";
	theApp = KMerlin::getInstance();
	theApp->contactList.append( this );
	actionList.setAutoDelete( true );
	_groupList.setAutoDelete( false );
	init();
}
/**/
KMContact::KMContact( )
		: QObject()
{}
/**/
KMContact::~KMContact()
{
	_groupList.clear();
	actionList.clear();
}


/**/
void KMContact::init( )
{
	_blocked = false;
	_reverse = false;
	_forward = false;
	_allowed = false;
	contextMenu = new KPopupMenu;
	contextMenu->setTitle( "KMerlin" );
	chatAction = new KAction( i18n( "Send Message" ), "edit", 0, this, SLOT( sendMessage( ) ) );
	connect( chatAction, SIGNAL( activated() ), this, SLOT( sendMessage() ) );
	blockAction = new KAction( i18n( "Block this Contact" ), "editdelete", 0, this, SLOT( blockContact( ) ) );
	connect( blockAction, SIGNAL( activated() ), this, SLOT( blockContact() ) );
	allowAction = new KAction( i18n( "Allow this Contact" ), "unlock", 0, this, SLOT( allowContact( ) ) );
	connect( allowAction, SIGNAL( activated() ), this, SLOT( allowContact() ) );
	removeAction = new KAction( i18n( "Remove this Contact" ), "editshred", 0, this, SLOT( removeContact( ) ) );
	connect( removeAction, SIGNAL( activated() ), this, SLOT( removeContact() ) );
	nickAction = new KAction( i18n( "Change nick name" ), "tux", 0, this, SLOT( changeNick( ) ) );
	nickAction->setEnabled( false );
	connect( nickAction, SIGNAL( activated() ), this, SLOT( changeNick() ) );
	chatAction->plug( contextMenu );
	blockAction->plug( contextMenu );
	allowAction->plug( contextMenu );
	removeAction->plug( contextMenu );
	nickAction->plug( contextMenu );
	blockAction->setEnabled( true );
	allowAction->setEnabled( FALSE );

	_toolTip = "<center><b>" + getNick() + " </b></center>"
	           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
	           + i18n( "Status:" ) + " <b> " + i18n( "Offline" ) + "</B><br>"
	           + i18n( "Left click to display the context menu!" ) ;
	_statusTip = i18n( "Right click to display the context menu" );
	_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmoffline" , KIcon::NoGroup , KIcon::SizeSmall );
}

void KMContact::setForward( QString nick, QString group )
{
	_forward = true;
	// create the offline action
	m_msnNick = nick;
	parseNick( nick );
	_toolTip = "<center><b>" + getNick() + " </b></center>"
	           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
	           + i18n( "Status:" ) + " <b>" + i18n( "Offline" ) + "</B><br>"
	           + i18n( "Left click to display the context menu!" ) ;
	_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmoffline" , KIcon::NoGroup , KIcon::SizeSmall );
	KMContactAction * action = new KMContactAction( theApp->offlineContainer, getNick() );
	action->setIconSet( _iconSet );
	action->setToolTip( _toolTip );
	action->setText( getNick() );
	action->setSort( "8" + getNick() );
	action->setParsedNick( parsedNick );
	theApp->offlineContainer->addWidget( action );
	theApp->offlineAction.insert( m_handle, action );
	connect( action, SIGNAL( activated() ) , this, SLOT( displayMenu() ) );
	// update the groups
	addToGroup( group );
}

void KMContact::setStatus( const QString &status )
{
	m_status = status;
	blockAction->setEnabled( true );
	allowAction->setEnabled( false );
	chatAction->setEnabled( true );
	removeAction->setEnabled( true );
	if ( m_status == "FLN" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Offline" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmoffline" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		chatAction->setEnabled( false );
		sort = "8" + getNick();
		_strStatus = "   ";
	} else if ( m_status == "NLN" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Online" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmonline" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		sort = "1" + getNick();
		_strStatus = " ";
	} else if ( m_status == "BRB" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Be right back" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmback" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_strStatus = " ( " + i18n( "Be right back" ) + " )";
		sort = "4" + getNick();
	} else if ( m_status == "PHN" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "On the phone" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmphone" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_strStatus = " ( " + i18n( "On the phone" ) + " )";
		sort = "5" + getNick();
	} else if ( m_status == "AWY" || m_status == "IDL" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Away" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_strStatus = " ( " + i18n( "Away" ) + " )";
		sort = "2" + getNick();
	} else if ( m_status == "BSY" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Busy" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmbsy" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_strStatus = " ( " + i18n( "Busy" ) + " )";
		sort = "3" + getNick();
	} else if ( m_status == "LUN" ) {
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Out to lunch" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "kmlunch" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_strStatus = " ( " + i18n( "Out to lunch" ) + " )";
		sort = "6" + getNick();
	}
	if ( _blocked ) {
		_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "stop" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		_toolTip = "<center><b>" + getNick() + " </b></center>"
		           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
		           + i18n( "Status:" ) + " <b>" + i18n( "Blocked" ) + "</B><br>"
		           + i18n( "Left click to display the context menu!" ) ;
		_strStatus = " ( " + i18n( "Blocked" ) + " )";
		sort = "7" + getNick();
		allowAction->setEnabled( true );
		blockAction->setEnabled( false );
	}
	// update the views
	updateActions();
	emit statusChanged( m_status );
}
void KMContact::updateActions()
{
	// Group view
	for ( KMContactAction * action = actionList.first(); action != 0; action = actionList.next() ) {
		action->setToolTip( _toolTip );
		action->setSort( sort );
		action->setParsedNick( parsedNick + _strStatus );
		//      action->setStatusTip( _statusTip );
		action->setIconSet( _iconSet );
		action->setText( getNick() + _strStatus );
	}
	for ( KMGroup * group = _groupList.first(); group != 0; group = _groupList.next() ) {
		group->groupContainer->sort(); // call this to sort and update the actions
		group->groupContainer->repaint();
	}
	// Online View
	KMContactAction *onlineAction = 0, *offlineAction = 0;
	if ( !theApp->onlineAction.isEmpty() )
		onlineAction = theApp->onlineAction.find( m_handle );
	if ( !theApp->offlineAction.isEmpty() )
		offlineAction = theApp->offlineAction.find( m_handle );

	if ( m_status != "FLN" ) {
		if ( offlineAction != 0 ) {
			offlineAction->hide();
			theApp->offlineContainer->removeWidget( offlineAction );
			theApp->offlineAction.remove( m_handle );
			onlineAction = new KMContactAction( theApp->onlineContainer, getNick() );
			onlineAction->setSort( sort );
			onlineAction->setParsedNick( parsedNick + _strStatus );
			theApp->onlineContainer->addWidget( onlineAction );
			//theApp->onlineBar->insertWidget( 1, 1, onlineAction, -1 );
			theApp->onlineAction.insert( m_handle, onlineAction );
			connect( onlineAction, SIGNAL( activated() ) , this, SLOT( displayMenu() ) );
			onlineAction->setToolTip( _toolTip );
			onlineAction->setIconSet( _iconSet );
			onlineAction->setText( getNick() + _strStatus );
		} else {
			onlineAction->setSort( sort );
			theApp->onlineContainer->sort();
			onlineAction->setToolTip( _toolTip );
			onlineAction->setIconSet( _iconSet );
			onlineAction->setParsedNick( parsedNick + _strStatus );
			onlineAction->setText( getNick() + _strStatus );
		}
	} else {
		if ( onlineAction != 0 ) {
			onlineAction->hide();
			theApp->onlineContainer->removeWidget( onlineAction );
			theApp->onlineAction.remove( m_handle );
			offlineAction = new KMContactAction( theApp->offlineContainer, getNick() );
			offlineAction->setSort( sort );
			offlineAction->setParsedNick( parsedNick + _strStatus );
			theApp->offlineContainer->addWidget( offlineAction );
			//			theApp->offlineBar->insertWidget( 1, 1, offlineAction, -1 );
			connect( offlineAction, SIGNAL( activated() ) , this, SLOT( displayMenu() ) );
			theApp->offlineAction.insert( m_handle, offlineAction );
			offlineAction->setToolTip( _toolTip );
			offlineAction->setIconSet( _iconSet );
			offlineAction->setText( getNick() );
			offlineAction->setParsedNick( parsedNick + _strStatus );
		} else {
			offlineAction->setSort( sort );
			theApp->offlineContainer->sort();
			offlineAction->setToolTip( _toolTip );
			offlineAction->setIconSet( _iconSet );
			offlineAction->setText( getNick() );
			offlineAction->setParsedNick( parsedNick + _strStatus );
		}
	}
	theApp->onlineContainer->repaint();
	theApp->offlineContainer->repaint();
}

void KMContact::setNickName( const QString &nick, bool update )
{

	parseNick( nick );
	m_msnNick = nick;
	_toolTip = i18n( "<center><b>%1</b></center>E-Mail Address: <b>%2</b><br>Status: <b>%3</B><br>" ).arg( getNick() ).arg( m_handle ).arg( getFullStatus() )
	           + i18n( "Left click to display the context menu!" ) ;
	if ( update )
		updateActions();
}

void KMContact::addToGroup( QString group )
{
	KMGroup * kmGroup;
	QStringList groups;
	_forward = true;
	groups = QStringList::split( ",", group, false );
	for ( QStringList::Iterator it = groups.begin(); it != groups.end(); ++it ) {
		for ( kmGroup = theApp->groupList.first(); kmGroup != 0; kmGroup = theApp->groupList.next() ) {
			if ( kmGroup->toolId == ( *it ) ) {
				//action->addTo( kmGroup->groupBar );
				KMContactAction * action = new KMContactAction( kmGroup->groupContainer, getNick() );
				action->setSort( "8" + getNick() );
				action->setIconSet( _iconSet );
				action->setToolTip( _toolTip );
				action->setText( m_msnNick );
				action->setParsedNick( parsedNick );
				actionList.append( action );
				kmGroup->actions.insert( m_handle, action );
				kmGroup->groupContainer->addWidget( action );
				connect( action, SIGNAL( activated() ) , this, SLOT( displayMenu() ) );
				//				groupList.append( ( *it ) );
				_groupList.append( kmGroup );
				connect( this, SIGNAL( infoChanged() ), kmGroup, SLOT( slotInfoChanged() ) );
			}
		}
	}
}

/**/
void KMContact::displayMenu()
{
	contextMenu->exec( QCursor::pos() );
}
/**/
void KMContact::sendMessage( )
{
	theApp->imService->slotStartChatSession( m_handle );
}
/**/
void KMContact::allowContact()
{
	theApp->imService->allowContact( m_handle, getNick() );
}
/**/
void KMContact::blockContact()
{
	theApp->imService->blockContact( m_handle, getNick() );
}
/**/
void KMContact::removeContact()
{
	KMGroup * kmGroup;
	for ( kmGroup = theApp->groupList.first(); kmGroup != 0; kmGroup = theApp->groupList.next() ) {
		debug( kmGroup->toolLabel + " " + theApp->groupWidget->groupLabel() );
		if ( kmGroup->toolLabel == theApp->groupWidget->groupLabel() ) {
			theApp->imService->removeContact( m_handle, kmGroup->toolId );
			return ;
		}
	}
}
/**/
void KMContact::changeNick()
{}

void KMContact::cleanUp()
{
	KMGroup * kmGroup;
	if ( !_groupList.isEmpty() ) {
		for ( kmGroup = _groupList.first(); kmGroup != 0; kmGroup = _groupList.next() ) {
			KMContactAction * action = kmGroup->actions.find( m_handle );
			action->hide();
			kmGroup->groupContainer->removeWidget( action );
			theApp->imService->removeContact( m_handle, kmGroup->toolId );
		}
	}
	actionList.clear();
	_groupList.clear();
	if ( _allowed ) {
		theApp->imService->removeContactL( m_handle, "AL" );
	} else if ( _blocked ) {
		theApp->imService->removeContactL( m_handle, "BL" );
	}
	_forward = false;
	// delete it from Online/offline
	KMContactAction * onlineAction = 0, *offlineAction = 0;
	if ( !theApp->onlineAction.isEmpty() )
		onlineAction = theApp->onlineAction.find( m_handle );
	if ( !theApp->offlineAction.isEmpty() )
		offlineAction = theApp->offlineAction.find( m_handle );
	if ( offlineAction != 0 ) {
		offlineAction->hide();
		theApp->offlineContainer->removeWidget( offlineAction );
		theApp->offlineAction.remove( m_handle );
	}
	if ( onlineAction != 0 ) {
		onlineAction->hide();
		theApp->onlineContainer->removeWidget( onlineAction );
		theApp->onlineAction.remove( m_handle );
	}
	// ### send a notification to the chat, chat has a pointer to this contact
	theApp->contactList.remove( this );
}

/**/
void KMContact::removed( const QString & handle, const QString & list, const QString & group )
{
	KMGroup * kmGroup;
	if ( list == "FL" ) {
		for ( kmGroup = theApp->groupList.first(); kmGroup != 0; kmGroup = theApp->groupList.next() ) {
			if ( kmGroup->toolId == group ) {
				KMContactAction * action = kmGroup->actions.find( handle );
				action->hide();
				kmGroup->groupContainer->removeWidget( action );
				kdDebug() << "Action removed from " << kmGroup->toolId << endl;
				actionList.remove( action );
				_groupList.remove( kmGroup );
			}
		}
		if ( _groupList.isEmpty() ) {
			_forward = false;
			// delete it from Online/offline
			KMContactAction * onlineAction = 0, *offlineAction = 0;
			if ( !theApp->onlineAction.isEmpty() )
				onlineAction = theApp->onlineAction.find( m_handle );
			if ( !theApp->offlineAction.isEmpty() )
				offlineAction = theApp->offlineAction.find( m_handle );
			if ( offlineAction != 0 ) {
				offlineAction->hide();
				theApp->offlineContainer->removeWidget( offlineAction );
				theApp->offlineAction.remove( m_handle );
			}
			if ( onlineAction != 0 ) {
				onlineAction->hide();
				theApp->onlineContainer->removeWidget( onlineAction );
				theApp->onlineAction.remove( m_handle );
			}
		}
		return ;
	} else if ( list == "AL" ) {
		_allowed = false;
		return ;
	} else if ( list == "BL" ) {
		_blocked = false;
		return ;
	} else if ( list == "RL" ) {
		// clean up this contact
		_reverse = false;

		return ;
	}
}
/**/
const QIconSet& KMContact::iconSet()
{
	return _iconSet;
}
/**/
QString KMContact::getNick()
{
	//  if( m_nick != m_msnNick )
	//    return m_nick;
	return m_msnNick;
}
/**/
bool KMContact::available()
{
	if ( m_status != "FLN" && !_blocked )
		return true;
	return false;
}
void KMContact::setBlocked( bool f )
{
	_blocked = f;
	if( !f ){
		setStatus( m_status );
		allowAction->setEnabled( f );
		blockAction->setEnabled( !f );
		return;
	}
	_iconSet = KGlobal::instance() ->iconLoader() ->loadIcon( "stop" , KIcon::NoGroup , KIcon::SizeSmall ) ;
	_toolTip = "<center><b>" + getNick() + " </b></center>"
	           + i18n( "E-Mail:" ) + " <b>" + m_handle + "</b><br>"
	           + i18n( "Status:" ) + " <b>" + i18n( "Blocked" ) + "</B><br>"
	           + i18n( "Left click to display the context menu!" ) ;
	allowAction->setEnabled( f );
	blockAction->setEnabled( !f );
	_strStatus = " ( " + i18n( "Blocked" ) + " )";
	sort = "9" + getNick();
	if( _forward )
		updateActions();
//	else if( !_forward && f ) // only activate this when you have an contact blocked with 1.3Beta1 (Auth dlg)
//	KMerlin::getInstance()->imService->addContact( m_handle, "0" );
}
void KMContact::parseNick( QString nick )
{
	QString t = nick;
	// parse the emo's

	KMEmotionIcon *icon;
	picCount = 0;
	if ( !t.isEmpty() )   // no text - no parsing
	{
		for ( icon = theApp->emotionList.first(); icon != 0; icon = theApp->emotionList.next() )
		{
			if ( t.left( icon->getDisplay().length() + 1 ) == icon->getDisplay() + " " )   // start with a emo
			{
				t = t.right( t.length() - icon->getDisplay().length() );
				t.prepend( "<img src=\"" + icon->getFilePath() + "\"height=\"14\" width=\"14\" align=\"middle\">&nbsp;" );
				picCount++;
			}
			if ( t.right( icon->getDisplay().length() + 1 ) == " " + icon->getDisplay() )   // end with a emo
			{
				t = t.left( t.length() - icon->getDisplay().length() );
				t = t + "<img src=\"" + icon->getFilePath() + "\"height=\"14\" width=\"14\" align=\"middle\">" ;
				picCount++;
			}
			int i = 0;
			if ( ( i = t.contains( icon->getDisplay() ) ) > 0 ) {
				t = t.replace( QRegExp( icon->getRegExp() ), " <img src=\"" + icon->getFilePath() + "\"height=\"14\" width=\"14\" align=\"middle\">&nbsp;" );
				picCount += i;
			}
		}
	}
	parsedNick = t;
}
QString KMContact::getFullStatus()
{
	if ( m_status == "FLN" )
		return i18n( "Offline" );
	else if ( m_status == "NLN" )
		return "";
	else if ( m_status == "BRB" )
		return i18n( "Be right back" );
	else if ( m_status == "PHN" )
		return i18n( "On the phone" );
	else if ( m_status == "AWY" || m_status == "IDL" )
		return i18n( "Away" );
	else if ( m_status == "BSY" )
		return i18n( "Busy" );
	else if ( m_status == "LUN" )
		return i18n( "Out to lunch" );
	if ( _blocked )
		return i18n( "Blocked" );
	return "";
}
#include "kmcontact.moc"

