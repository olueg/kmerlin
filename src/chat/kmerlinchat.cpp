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
#include "kmerlinchat.h"
#include "smileywidget.h"
#include "../kmerlin.h"
#include "../qcategorywidget.h"
#include "../kmcontact.h"
#include "../kmservice.h"
#include "../kmcontactaction.h"
#include "../kmerlindock.h"
#include "../kmerlinnotify.h"

#include <qdragobject.h>
#include <qlineedit.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qdockwindow.h>
#include <qtoolbar.h>
#include <qcolordialog.h>

#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kfiledialog.h>
#include <kfontdialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurlrequesterdlg.h>
#include <kdebug.h>
#include <kstddirs.h>
#include <kedittoolbar.h>
#include <kio/passdlg.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kmessagebox.h>
#include <ktoolbar.h>
#include <kpopupmenu.h>
#include <knotifyclient.h>

#include <time.h>

/**/
KMChatContact::KMChatContact( QString handle, QWidget *parent )
		: QObject()
{
	KMerlin * theApp = KMerlin::getInstance();
	KMContact *co;
	KMerlinChat *chat = ( KMerlinChat* ) parent;
	for ( co = theApp->contactList.first(); co != 0; co = theApp->contactList.next() ) {
		if ( co->getHandle() == handle ) {
			contact = co;
			action = new KMContactAction( chat->coToolBar, co->getNick() );
			action->setParsedNick( co->getParsedNick() );
			action->setText( contact->getNick() );
			chat->coToolBar->insertWidget( 1, 1, action, -1 );
			action->setIconSet( contact->iconSet() );
			connect( contact, SIGNAL( nickChanged( QString ) ), action, SLOT( setText( QString ) ) );
			connect( contact, SIGNAL( statusChanged( const QString& ) ), this, SLOT( statusChanged( const QString& ) ) );
			return;
		}
	}
	// not found - we have a unknow contact ### 
	// ### THIS NEED'S CHANGES IN CLASS KMCONTACT !!!!	
	contact = new KMContact( handle, handle );
	action = new KMContactAction( chat->coToolBar, handle );
	action->setParsedNick( handle );
	action->setText( handle );
	chat->coToolBar->insertWidget( 1, 1, action, -1 );
	// create a acion to add this unknow contact
}
/**/
KMChatContact::KMChatContact( )
		: QObject()
{}
/**/
KMChatContact::~KMChatContact( )
{
	// action->removeFrom( ( (KMerlinChat*)parent() )->coToolBar );
	delete action;
}
/**/
void KMChatContact::timerEvent( QTimerEvent* e )
{
	if ( e->timerId() == timer ) {
		action->setIconSet( contact->iconSet() ) ;
		action->setToolTip( i18n( "%1 is listening" ).arg( contact->nickName() ) );
		action->setStatusTip( action->toolTip() );
		killTimer( timer );
	}
}
/**/
void KMChatContact::setWriting( bool f )
{
	killTimer( timer );
	if ( f ) {
		timer = startTimer( 6000 );
		action->setIconSet( KGlobal::instance() ->iconLoader() ->loadIcon( "edit" , KIcon::NoGroup , KIcon::SizeSmall ) );
		action->setToolTip( i18n( "%1 is writing a message" ).arg( contact->nickName() ) );
		action->setStatusTip( "Not supported" );
	} else {
		action->setIconSet( contact->iconSet() ) ;
		action->setToolTip( i18n( "%1 is listening" ).arg( contact->nickName() ) );
		action->setStatusTip( "Not supported" );
	}
}
/**/
void KMChatContact::statusChanged( const QString &status )
{
	if ( status != i18n( "NLN" ) ) {
		action->setIconSet( contact->iconSet() );
		action->setToolTip( i18n( "Attention<br>%1 has changed the status to: %2<br>It's possible that the message can't received!" ).arg( contact->nickName() ).arg( status ) );
	} else {
		action->setIconSet( contact->iconSet() );
		action->setToolTip( i18n("Boring") );
	}
}
void KMChatContact::setEnabled( bool enable )
{
	action->setEnabled( enable );
}

///////////////////////////////////////// KMerlinChat //////////////////////////////
/**/
KMerlinChat::KMerlinChat( )
		: KMainWindow( 0, "Short Message" ),
		m_view( new KMerlinChatView( this ) ),
		m_printer( 0 )
{
	setCentralWidget( m_view );
	setupActions();
	contactList.setAutoDelete( true );
	statusBar() ->show();
	ID = time( ( time_t * ) NULL );
	_active = false;
	_logging = false;
	pending = true;
	pendingMessage = "";
	pendingFormat = "";
	pendingSystemMsg = "";
	dummy = new KPopupMenu( this );
	dw = new QDockWindow( QDockWindow::InDock, this );
	dw->setResizeEnabled( TRUE );
	dw->setCloseMode( QDockWindow::Always );
	dw->setMovingEnabled( true );
	toolBox = new QCategoryWidget( dw );
	addToolBar( dw, Qt::DockRight );
	dw->setWidget( toolBox );
	dw->setFixedExtentWidth( 145 );
	dw->setCaption( i18n( "Sidebar" ) );
	setDockEnabled( dw, Qt::DockTop, FALSE );
	setDockEnabled( dw, Qt::DockBottom, FALSE );
	coToolBar = new KToolBar( this, toolBox, FALSE, "Contacts" );
	coToolBar->setEnableContextMenu( false );
	toolBox->setUpdatesEnabled( TRUE );
	coToolBar->setUpdatesEnabled( TRUE );
	coToolBar->setFrameStyle( QFrame::NoFrame );
	coToolBar->setOrientation( Qt::Vertical );
	toolBox->addCategory( "Contacts", coToolBar, dummy, KGlobal::instance() ->iconLoader() ->loadIcon( "2downarrow" , KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance() ->iconLoader() ->loadIcon( "2leftarrow" , KIcon::NoGroup , KIcon::SizeSmall ) );
	//  coToolBar->show();
	smToolBar = new KToolBar( 0, toolBox, FALSE, "Smileys" );
	smToolBar->setFrameStyle( QFrame::NoFrame );
	smToolBar->setOrientation( Qt::Vertical );
	toolBox->addCategory( i18n( "Smileys" ), smToolBar, dummy, KGlobal::instance() ->iconLoader() ->loadIcon( "2downarrow" , KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance() ->iconLoader() ->loadIcon( "2leftarrow" , KIcon::NoGroup , KIcon::SizeSmall ) );
	smileyWidget = new SmileyWidget( smToolBar, "sm" );
	smToolBar->insertWidget( 1, 1, smileyWidget );
	ivToolBar = new KToolBar( this, toolBox, FALSE, "Invite" );
	ivToolBar->setEnableContextMenu( false );
	ivToolBar->setUpdatesEnabled( TRUE );
	ivToolBar->setFrameStyle( QFrame::NoFrame );
	ivToolBar->setOrientation( Qt::Vertical );
	toolBox->addCategory( "Invite", ivToolBar, dummy, KGlobal::instance() ->iconLoader() ->loadIcon( "2downarrow" , KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance() ->iconLoader() ->loadIcon( "2leftarrow" , KIcon::NoGroup , KIcon::SizeSmall ) );
	connect( smileyWidget, SIGNAL( signalClicked( QString ) ), m_view, SLOT( pasteSmiley( QString ) ) );
	connect( m_view, SIGNAL( sendMsg( QString, QString ) ), this, SLOT( sendMessage( QString, QString ) ) );
	connect( dw, SIGNAL( visibilityChanged(bool) ), this, SLOT( visibilityChanged( bool ) ) );
	timer = startTimer( 5000 );
	extraSmiley->setChecked( true );
	inviteList.setAutoDelete( true );
	viewTool->setChecked( true );
}
/**/
KMerlinChat::~KMerlinChat()
{
	closeSwitchBoard();
	chatAction->unplug( KMerlin::getInstance()->panel->menu );
	KMerlin::getInstance() ->widgetList.remove( this ) ;
}
/**/
void KMerlinChat::setupActions()
{
	KStdAction::print( this, SLOT( filePrint() ), actionCollection() );
	KStdAction::keyBindings( this, SLOT( optionsConfigureKeys() ), actionCollection() );
	KStdAction::configureToolbars( this, SLOT( optionsConfigureToolbars() ), actionCollection() );
	KStdAction::preferences( KMerlin::getInstance(), SLOT( optionsPreferences() ), actionCollection() );
	KStdAction::save( m_view, SLOT( fileSave() ), actionCollection() );
	KStdAction::quit( this, SLOT( fileQuit() ), actionCollection() );

	KStdAction::copy( m_view, SLOT( copy() ), actionCollection() );
	KStdAction::paste( m_view, SLOT( paste() ), actionCollection() );
	KStdAction::cut( m_view, SLOT( cut() ), actionCollection() );

	formatBold = new KToggleAction( i18n( "Bold" ), "text_bold", 0, m_view, SLOT( toggleBold() ), actionCollection(), "bold" );
	formatStrike	= new KToggleAction( i18n( "Strikeout" ), "text_strike", 0, m_view, SLOT( toggleStrikeout() ), actionCollection(), "strikeout" );
	formatUnderline = new KToggleAction( i18n( "Underline" ), "text_under", 0, m_view, SLOT( toggleUnderline() ), actionCollection(), "underline" );
	formatItalic	= new KToggleAction( i18n( "Italic" ), "text_italic", 0, m_view, SLOT( toggleItalic() ), actionCollection(), "italic" );
	formatFont	= new KAction( i18n( "&Set Font" ), "charset", 0, this, SLOT( setFont() ), actionCollection(), "font" );
	formatColor	= new KAction( i18n( "&Set Color" ), "colorize", 0, this, SLOT( setColor() ), actionCollection(), "color" );

	extraTime = new KToggleAction( i18n( "&Display time" ), "date", 0, m_view, SLOT( toggleTime() ), actionCollection(), "extraTime" );
	extraSmiley = new KToggleAction( i18n( "&Display Smileys" ), "kmerlin", 0, m_view, SLOT( toggleSmiley() ), actionCollection(), "extraSmiley" );
	extraLog = new KToggleAction( i18n( "&Enable Logging" ), "filesave", 0, this, SLOT( toggleLogging() ), actionCollection(), "extraLog" );
	viewTool = new KToggleAction( i18n( "&View Sidebar" ), "edit", 0, this, SLOT( toggleToolWindow() ), actionCollection(), "toolWindow" );
	createGUI( "kmerlinchatui.rc" );
	chatAction = new KAction( i18n( "[New] Chat" ), "desktop", 0, this, SLOT( displayChat() ), actionCollection() );
	chatAction->plug( KMerlin::getInstance()->panel->menu );
}
void KMerlinChat::fileQuit()
{
	delete this;
}
void KMerlinChat::closeEvent( QCloseEvent * event )
{
	event->ignore();
	delete this;
}
/**/
void KMerlinChat::optionsShowToolbar()
{
	// this is all very cut and paste code for showing/hiding the
	// toolbar
	if ( m_toolbarAction->isChecked() )
		toolBar() ->show();
	else
		toolBar() ->hide();
}
/**/
void KMerlinChat::optionsShowStatusbar()
{
	// this is all very cut and paste code for showing/hiding the
	// statusbar
	if ( m_statusbarAction->isChecked() )
		statusBar() ->show();
	else
		statusBar() ->hide();
}
/**/
void KMerlinChat::optionsConfigureKeys()
{
	KKeyDialog::configureKeys( actionCollection(), "kmerlinchatui.rc" );
}
/**/
void KMerlinChat::optionsConfigureToolbars()
{
	// use the standard toolbar editor
	KEditToolbar dlg( actionCollection() );
	if ( dlg.exec() ) {
		// recreate our GUI
		createGUI( "kmerlinchatui.rc" );
	}
}
/**/
void KMerlinChat::changeStatusbar( const QString& text )
{
	// display the text on the statusbar
	statusBar() ->message( text );
}
/**/
bool KMerlinChat::eventFilter( QObject *o, QEvent *e )
{
	// Capture the Enter key and tell it not to make a new line.
	if ( !m_view->CtrlDown() ) {

		if ( e->type() == QEvent::KeyPress ) {
			QKeyEvent * k = ( QKeyEvent* ) e;
			if ( ( k->key() == Key_Return ) || ( k->key() == Key_Enter ) ) {
				m_view->sendText();
				return true;
			}
		}
	}
	return QWidget::eventFilter( o, e );
}
/**/
void KMerlinChat::timerEvent( QTimerEvent* e )
{
	if ( e->timerId() == timer ) {
		if ( m_view->textChanged ) {
			emit typingMsg();
			m_view->textChanged = false;
		}
	}
}
/**/
void KMerlinChat::closeSwitchBoard()
{
	if ( _active ) {
		emit closeSession();
	}
}
/**/
void KMerlinChat::setBaseContact( const QString &handle )
{
	KMChatContact * contact;
	pending = true;
	ivToolBar->setEnabled( true );
	if ( contactList.count() == 1 ) {
		contactList.remove( handle );
	}
	chatAction->setText( i18n("Chat with: %1 ").arg(handle) );
	setCaption( i18n("Chat with: %1").arg(handle) );
	baseContact = handle;
	KMerlin::getInstance()->panel->setChat();
	contact = new KMChatContact( handle, this );
	contactList.insert( handle, contact );
	KMContact *co;
	// fill the invite Bar
	inviteList.clear();
	for ( co = KMerlin::getInstance() ->contactList.first(); co != 0; co = KMerlin::getInstance() ->contactList.next() ) {
		if ( co->getHandle() != handle ) {
			if ( co->available() ) {
				KMContactAction * action = new KMContactAction( ivToolBar, co->getHandle() );
				action->setParsedNick( co->getParsedNick() );
				action->setText( co->getNick() );
				action->setIconSet( co->iconSet() );
				connect( action, SIGNAL( activated() ), this, SLOT( inviteContact() ) );
				action->setToolTip( i18n( "Left click to invite this contact" ) );
				ivToolBar->insertWidget( 1, 1, action, -1 );
				inviteList.insert( co->getHandle(), action );
			}
		}
	}
}
/**/
void KMerlinChat::messageReceived( QString handle, QString message, QString format )
{
	KMChatContact * contact;
	if ( handle == KMerlin::getInstance() ->imService->getHandle() )  // thats me
	{
		m_view->messageReceived( KMerlin::getInstance() ->imService->getNickName(), message, format );
		chatAction->setText( i18n("Chat with: %1").arg( baseContact ) );
		setCaption( i18n("Chat with: %1").arg( baseContact ) );
		KMerlin::getInstance()->panel->setDefault();
		return ;
	}
	contact = contactList.find( handle );
		
	
	if( !isActiveWindow() ){
		// very good idea is to animate the window caption
		chatAction->setText( i18n("[Changed] Chat with: %1").arg( baseContact ) );
		KMerlin::getInstance()->panel->setChat();
		setCaption( i18n("[Changed] Chat with: %1").arg( baseContact ) );
		QString notifyMsg = QString("%1<br>%2").arg( contact->contact->nickName() ).arg( message ) ;
		if( message.length() > 40 )
			KMerlinNotify::message( this, contact->contact->nickName(), message.left(40) +"..." );
		else
			KMerlinNotify::message( this, contact->contact->nickName(), message );
	}
	else{
		chatAction->setText( i18n("Chat with: %1").arg( baseContact ) );
		KMerlin::getInstance()->panel->setDefault();
		setCaption( i18n("Chat with: %1").arg( baseContact ) );
	}	
	if ( contact != 0 ) {
		contact->setWriting( false );
		m_view->messageReceived( contact->contact->nickName(), message, format );
	} else {
		kdDebug() << "There was a problem in ChatWindw, the contact: " << handle << " was not found!!" << endl;
		m_view->messageReceived( handle, message, format );
	}
}
/**/
void KMerlinChat::messageReceived( bool f )
{
	QString strTime;
	strTime = KGlobal::locale() ->formatDateTime( QDateTime::currentDateTime(), false, true );
	if ( f ) {
		statusBar() ->message( i18n( "Your last message was received at : %1" ).arg( strTime ) );
	} else {
		statusBar() ->message( i18n( "Your last message was not received : %1" ).arg( strTime ) );
	}
}
/**/
void KMerlinChat::contactTyping( QString handle )
{
	KMChatContact * contact;
	contact = contactList.find( handle );
	if( contact != 0 )
		contact->setWriting( true );
}
/**/
void KMerlinChat::sendMessage( QString msg, QString format )
{
	if ( active() ) {
		if ( !pending ) {
			emit message( msg, format );
			pendingMessage = "";
		} else {
			QDictIterator<KMChatContact> it( contactList );
			KMChatContact *contact = it.toFirst();
			pendingMessage = msg;
			pendingFormat = format;
			statusBar() ->message( i18n( "Please wait..." ) );
			emit inviteContact( contact->contact->getHandle() );
		}
	} else {
		QDictIterator<KMChatContact> it( contactList );
		KMChatContact *contact = ( KMChatContact* ) it.toFirst();
		pendingMessage = msg;
		pendingFormat = format;
		statusBar() ->message( i18n( "Please wait..." ) );
		emit createSession( contact->contact->getHandle() );
	}
}


/**/
void KMerlinChat::contactReceived( const QString &handle, const QString &command, bool last )
{
	kdDebug() << "Contact Received : " << handle << endl;
	kdDebug() << "command : " << command << endl;
	kdDebug() << "Pending is : " << pending << endl;
	kdDebug() << "Is active : " << active() << endl;
	KMChatContact *contact;
	if ( command == "BYE" ) {
		if ( contactList.count() == 1 ) {
			contact = contactList.find( handle );
			contact->action->setEnabled( false );
			pending = true ;
			// we must disable the invite actions
			ivToolBar->setEnabled( false );
		} else {
			contact = contactList.find( handle );
			m_view->systemMessage( i18n( "%1 has left the Chat Session" ).arg( handle ) );
			KMContactAction *action = new KMContactAction( ivToolBar, handle );
			action->setText( contact->contact->getNick() );
			action->setIconSet( contact->contact->iconSet() );
			action->setParsedNick( contact->contact->getParsedNick() );
			connect( action, SIGNAL( activated() ), this, SLOT( inviteContact() ) );
			action->setToolTip( i18n( "Left click to invite this contact" ) );
			contactList.remove( handle );
			ivToolBar->insertWidget( 1, 1, action, -1 );
			inviteList.insert( handle, action );
			if( contactList.count() == 1 ){
				QDictIterator<KMChatContact> it( contactList );
				contact = it.toFirst();
				setCaption( contact->contact->getHandle() );
			}
		}
		return ;
	} else if ( command == "IRO" ) {
		pending = false;
		contact = contactList.find( handle );
		if ( contact != 0 ) {
			contact->action->setEnabled( true );
			if ( last && !pendingMessage.isEmpty() ) {
				pendingSystemMsg += handle ;
				m_view->systemMessage( i18n( "You are talking with:%1 %2" ).arg( pendingSystemMsg ).arg( "<br>" ) );
				pendingSystemMsg = "";
			} else if ( !last ) {
				pendingSystemMsg += handle + "<br>";
			}
		} else {
			contact = new KMChatContact( handle, this );
			contactList.insert( handle, contact );
			inviteList.remove( handle );
			if ( last ) {
				pendingSystemMsg += handle ;
				m_view->systemMessage( i18n( "You are talking with:%1 %2" ).arg( pendingSystemMsg ).arg( "<br>" ) );
				pendingSystemMsg = "";
			} else {
				pendingSystemMsg += handle + "<br>";
			}
		}
	} else if ( command == "JOI" ) {
		pending = false;
		contact = contactList.find( handle );
		if ( contact != 0 ) {
			contact->setEnabled( true );
			if ( !pendingMessage.isEmpty() ) {
				emit sendMessage( pendingMessage, pendingFormat );
				pendingMessage = "";
				pendingFormat = "";
			}
		} else {
			contact = new KMChatContact( handle, this );
			contactList.insert( handle, contact );
			inviteList.remove( handle );
			m_view->systemMessage( i18n( "%1 has joined the Chat Session!" ).arg( handle ) );
		}
	}
}
/**/
void KMerlinChat::toggleLogging()
{
	_logging = extraLog->isChecked();
	if ( _logging ) {
		// create the log file
		QDir::setCurrent( QDir::homeDirPath() + "/kmerlin/log" );
		QString strTime;
		strTime = KGlobal::locale() ->formatDateTime( QDateTime::currentDateTime(), true, false );
		QDictIterator<KMChatContact> it( contactList );
		KMChatContact *contact = ( KMChatContact* ) it.toFirst();
		QString fileName = KMerlin::getInstance()->m_logFile;
		if( fileName.isEmpty() ) fileName = contact->contact->getHandle();
		fileName = fileName.replace(QRegExp("%E"),contact->contact->getHandle() );
		logFile.setName( fileName);
		if ( !logFile.open( IO_ReadWrite | IO_Append ) ) {
			logFile.close();
			kdDebug() << "Cannot create log file" << endl;
		} else {
			QTextStream stream( &logFile );
			
			stream << "\n>>>>>>>>>>>>>>>>>>>>\n" <<  "KMerlin log file" << "\nDate: " << strTime << "\n\n";
			logFile.flush();
		}
	} else {
		//
		if ( logFile.exists() ) {
			logFile.close();
		}
	}
}
/**/
void KMerlinChat::setFont()
{
	QFont font(KMerlin::getInstance() ->fontName);
	KFontDialog::getFont( font );
	KMerlin::getInstance() ->fontName = font.family();
}
/**/
void KMerlinChat::setColor()
{
	KMerlin::getInstance() ->fontColor = QColorDialog::getColor( KMerlin::getInstance() ->fontColor );
}
/**/
void KMerlinChat::statusReceived( const QString &handle, const QString &status )
{
	KMContactAction * action = 0;
	KMChatContact *contact = contactList.find( handle );
	if ( contact != 0 )
		return ; // already in chat
	if ( !inviteList.isEmpty() ) {
		action = inviteList.find( handle );
	}
	if ( action != 0 && status == "FLN" ) {
		inviteList.remove( handle ); // not longer available
		return ;
	}
	KMContact *co;
	for ( co = KMerlin::getInstance() ->contactList.first(); co != 0; co = KMerlin::getInstance() ->contactList.next() ) {
		if ( co->getHandle() == handle ) {
			if ( co->available() ) {
				if ( action == 0 ) { // not in invite list , create it
					action = new KMContactAction( ivToolBar, co->getHandle() );
					ivToolBar->insertWidget( 1, 1, action, -1 );
					inviteList.insert( co->getHandle(), action );
					connect( action, SIGNAL( activated() ), this, SLOT( inviteContact() ) );
				}
				action->setParsedNick( co->getParsedNick() );
				action->setText( co->getNick() );
				action->setIconSet( co->iconSet() );
				action->setToolTip( i18n( "Left click to invite this contact" ) );
			}
		}
	}
}
void KMerlinChat::inviteContact()
{
	if ( active() ) {
		if ( !pending ) {
			emit inviteContact( sender() ->name() );
		} else {
			setBaseContact( sender() ->name() );
			statusBar() ->message( i18n( "Please wait..." ) );
			emit inviteContact( sender() ->name() );
		}
	} else {
		setBaseContact( sender() ->name() );
		statusBar() ->message( i18n( "Please wait..." ) );
		emit createSession( sender() ->name() );
	}
}

void KMerlinChat::displayChat()
{
	// ## this->show() doesn't work :(
	KMerlin::getInstance()->slotActivateChat( ID );
}
void KMerlinChat::toggleToolWindow()
{
	if( dw->isHidden() )
		dw->show();
	else
		dw->hide();
}
void KMerlinChat::visibilityChanged( bool visible )
{
	viewTool->setChecked( visible );
}

