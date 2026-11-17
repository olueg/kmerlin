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

#include "kmerlin.h"
#include "kmerlinview.h"
#include "login.h"
#include "qcategorywidget.h"
#include "CContactWidget.h"

#include <qpainter.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qvbox.h>

#include <kmessagebox.h>
#include <krun.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kstddirs.h>

KMerlinView::KMerlinView( QWidget *parent )
		: QWidget( parent ),
		DCOPObject( "KMerlinIface" )
{

	QVBoxLayout * top_layout1 = new QVBoxLayout( this );
	top_layout1->setAutoAdd( true );


}

KMerlinView::~KMerlinView()
{}

/**/
void KMerlinView::initView()
{
	theApp = KMerlin::getInstance();
	theApp->groupWidget = new QCategoryWidget( this );
	theApp->onlineWidget = new QCategoryWidget( this );
	w = new Login( this, "KMerlin" );
	KStandardDirs dir;
	//resize( QSize( 418, 312 ) );
	w->show();
	theApp->groupWidget->hide();
	theApp->onlineWidget->hide();
	theApp->onlineBar = new KToolBar( theApp, theApp->onlineWidget, false, "Online", false, false );
	theApp->onlineBar->setEnableContextMenu( false );
	theApp->onlineBar->setFrameStyle( QFrame::NoFrame );
	theApp->onlineBar->setOrientation( Qt::Vertical );
	theApp->onlineWidget->addCategory( "Online", theApp->onlineBar, theApp->onlineMenu, KGlobal::instance()->iconLoader()->loadIcon( "2downarrow" ,KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance()->iconLoader()->loadIcon( "2leftarrow" ,KIcon::NoGroup , KIcon::SizeSmall ) );
	theApp->onlineContainer = new CContactWidget( theApp->onlineBar, "Online Container" );
	theApp->onlineBar->insertWidget( 1, 1, theApp->onlineContainer, -1 );

	theApp->offlineBar = new KToolBar( theApp, theApp->onlineWidget, false, "Offline", false, false );
	theApp->offlineBar->setEnableContextMenu( false );
	theApp->offlineBar->setFrameStyle( QFrame::NoFrame );
	theApp->offlineBar->setOrientation( Qt::Vertical );
	theApp->onlineWidget->addCategory( "Offline", theApp->offlineBar, theApp->offlineMenu, KGlobal::instance()->iconLoader()->loadIcon( "2downarrow" ,KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance()->iconLoader()->loadIcon( "2leftarrow" ,KIcon::NoGroup , KIcon::SizeSmall )  );
	theApp->offlineContainer = new CContactWidget( theApp->offlineBar, "Offline Container" );
	theApp->offlineBar->insertWidget( 1, 1, theApp->offlineContainer, -1 );

}
void KMerlinView::print( QPainter *p, int height, int width )
{
	// do the actual printing, here
	// p->drawText(etc..)
}

void KMerlinView::updateView( bool f, int layout )
{
	if ( f )  // online
	{
		switch ( layout )
		{
				case 1: {
					KMerlin::getInstance() ->groupWidget->show();
					KMerlin::getInstance() ->onlineWidget->hide();
					w->hide();
					break;
				}
				case 2: {
					KMerlin::getInstance() ->groupWidget->hide();
					KMerlin::getInstance() ->onlineWidget->show();
					w->hide();
					break;
				}
		}
	}
	else {
		KMerlin::getInstance() ->groupWidget->hide();
		KMerlin::getInstance() ->onlineWidget->hide();
		KMerlin::getInstance() ->onlineContainer->clear();
		KMerlin::getInstance() ->offlineContainer->clear();
		//w->setInfoMsg( "<center>Welcome to KMerlin</center>" );
		w->show();
	}
}

void KMerlinView::slotSetTitle( const QString& title )
{
	emit signalChangeCaption( title );
}
/**/

void KMerlinView::slotStatusMsg( const QString &msg )
{
	//w->setInfoMsg( msg );
}
#include "kmerlinview.moc"
