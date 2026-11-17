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

#include "kmerlindock.h"
#include "kmerlin.h"
#include "kmservice.h"

#include <klocale.h>
#include <kglobal.h>
#include <kpopupmenu.h>
#include <kiconloader.h>

#include <qtooltip.h>
#include <qcursor.h>
#include <qpixmapcache.h>
#include <qimage.h>

/**/
KMerlinDock::KMerlinDock( QWidget *parent, const char *name )
		: KSystemTray( parent, name )
{
	menu = new KPopupMenu;
	menu->setTitle( "KMerlin" );
	mDefaultIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmoffline" , KIcon::NoGroup , KIcon::SizeSmall );
	setPixmap( mDefaultIcon );
	mStatusIcon = mDefaultIcon;
	timer = 0L;
	mStep = 0;
}

/**/
KMerlinDock::~KMerlinDock()
{}

/**/
void KMerlinDock::mousePressEvent( QMouseEvent *e )
{
	killTimer( timer );
	timer = 0L;
	this->setPixmap( mStatusIcon );
	if ( e->button() == RightButton ) {
		menu->exec( QCursor::pos() );
	} else {
		KMerlin::getInstance() ->toggleApp(); //hide or show the app
	}
}

/**/
void KMerlinDock::timerEvent( QTimerEvent *e )
{
	if ( e->timerId() == timer ) {
		switch ( status ) {
				case 1:  // connecting
				{
					mDefaultIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall ) ;
					switchIcon( "connecting" );
					break;
				}
				case 2:  // e-mail
				{
					mDefaultIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "hotmail" , KIcon::NoGroup , KIcon::SizeSmall ) ;
					switchIcon( "email" );
					break;
				}
				case 3:  // not supported
				{
					mDefaultIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmback" , KIcon::NoGroup , KIcon::SizeSmall ) ;
					switchIcon( "no" );
					break;
				}
				case 4:  // working
				{
					mDefaultIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "konqueror3" , KIcon::NoGroup , KIcon::SizeSmall ) ;
					switchIcon( "working" );
					break;
				}

				case 5:  // Chat
				{
					mDefaultIcon = KGlobal::instance()->iconLoader()->loadIcon( "chat" ,KIcon::NoGroup , KIcon::SizeSmall ) ;
					switchIcon( "chat" );
					break;
				}
		}
	}
}
void KMerlinDock::setEmail( const QString &text )
{
	if ( timer != 0L )
		killTimer( timer );
	status = 2;
	timer = startTimer( 60 );
	QToolTip::add( this, i18n( "You have a new e-mail received:<br>%1" ).arg( text ) );
}

/**/
void KMerlinDock::setConnecting( )
{
	if ( timer != 0L )
		killTimer( timer );
	status = 1;
	timer = startTimer( 60 );
	QToolTip::add( this, i18n( "Connecting to MSN as: %1" ).arg( KMerlin::getInstance() ->imService->getHandle() ) );
}
/**/
void KMerlinDock::setConnected( bool ok )
{
	if ( timer != 0L )
		killTimer( timer );
	timer = 0L;
	if ( ok ) {
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: %2" ).arg( KMerlin::getInstance() ->imService->getHandle() ).arg( KMerlin::getInstance() ->imService->getStatus() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmonline" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	} else {
		QToolTip::add( this, i18n( "Offline" ) );
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmoffline" , KIcon::NoGroup , KIcon::SizeSmall ) ;
		this->setPixmap( mStatusIcon );
	}
}
void KMerlinDock::setWorking()
{
	if ( timer != 0L )
		killTimer( timer );
	status = 4;
	timer = startTimer( 60 );
}

void KMerlinDock::switchIcon(const QString &strIcon )
{
	//kdDebug(5006) << "step is " << mStep << endl;

	QString iconName = QString( "%1_icon%2" ).arg( strIcon ).arg( mStep );
	QPixmap icon;

	if ( !QPixmapCache::find( iconName, icon ) ) {
		int w = mDefaultIcon.width() + ( mStep * 1 );
		int h = mDefaultIcon.width() + ( mStep * 1 );

		//kdDebug(5006) << "Initting icon " << iconName << endl;

		// Scale icon out from default
		icon.convertFromImage( mDefaultIcon.convertToImage().smoothScale( w, h ) );

		QPixmapCache::insert( iconName, icon );
	}

	if ( mStep == 9 ) mInverted = true;
	if ( mStep == 0 ) mInverted = false;

	if ( mInverted ) --mStep;
	else ++mStep;

	this->setPixmap( icon );
}

void KMerlinDock::setChat( )
{
	if ( timer != 0L )
		killTimer( timer );
	status = 5;
	timer = startTimer( 60 );
}
void KMerlinDock::setDefault()
{
	killTimer( timer );
	timer = 0L;
	this->setPixmap( mStatusIcon );
}

void KMerlinDock::setStatus( const QString &status )
{
	if( status == "NLN" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Online" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmonline" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "HDN" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Invisible" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmback" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "AWY" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Away" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "BRB" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Be right back" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "LUN" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Out to lunch" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "IDL" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Away" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "PHN" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: On the phone" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
	else if( status == "BSY" ){
		QToolTip::add( this, i18n( "Connected to MSN as: %1<br>Your Status is: Busy" ).arg(KMerlin::getInstance()->imService->getHandle() ) ) ;
		mStatusIcon = KGlobal::instance() ->iconLoader() ->loadIcon( "kmaway" , KIcon::NoGroup , KIcon::SizeSmall );
		this->setPixmap( mStatusIcon );
	}
}
