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
** Some part are from Kopete to get MSNP9 working
**     Kopete    (c) 2002-2003 by the Kopete developers  <kopete-devel@kde.org>
**     
**
**********************************************************************/

#include "kmservice.h"
#include "kmcontact.h"
#include "kmgroup.h"
#include "kmerlin.h"
#include "imchatservice.h"
#include "qcategorywidget.h"
#include "authdlg.h"

// qt include
#include <qregexp.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtimer.h>

// kde include
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kmdcodec.h>
#include <ktempfile.h>
#include <kapp.h>
#include <kstddirs.h>
#include <kdeversion.h>
#include <krun.h>
#include <kio/job.h>

#define	Forward		0x01
#define Allow		0x02
#define Block		0x04
#define Reverse		0x08

/**/
KMService::KMService()
		: QObject()
{
	theApp = KMerlin::getInstance();
	connected = false;
	_serial = "0";
	_silent = false; // remove later
	autoRename = true; // set new nick names in all lists
	connect( this, SIGNAL( signalConnected( bool ) ), KMerlin::getInstance(), SLOT( slotConnected( bool ) ) );
	lastPing = true;
	socket = 0L;
	mailCount =0;
}
/**/
KMService::~KMService()
{}
/**/
void KMService::cleanUp()
{
	//clean up all lists , but make sure that chat window has cleaned before
	theApp->groupList.clear();
	theApp->contactList.clear();
	theApp->groupWidget->cleanUp();
	theApp->onlineAction.clear();
	theApp->offlineAction.clear();
	//  theApp->onlineWidget->cleanUp();
}
/**/
void KMService::slotDataReceived( )
{

	QString data;
	if ( socket->canReadLine() ) {
		data = socket->readLine();
	} else {
		return ;
	}
	data = QString::fromUtf8( data );
	//kdDebug() << "KMService::slotDataReceived: "<< data << endl;
	if ( data.left( 3 ) == "911" ) {
		emit signalConnected( false );
		if ( !_silent ) {
			KMessageBox::error( 0, i18n( "Authentication failed" ) );
			kdDebug() << "Connection error: Authentication failed!" << endl;
		}
		return ;
	}
	if ( data.left( 3 ) == "280" || data.left( 3 ) == "281" ) {
		KMessageBox::error( 0, i18n( "Sorry!\nThere is a problem with the server." ) );
		return ;
	}
	parseCommand( data );
	while ( socket->canReadLine() ) {
		data = socket->readLine();
		data = QString::fromUtf8( data );
		parseCommand( data );
	}
}
/**/
void KMService::parseCommand( QString data )
{
	QString miss, len;
	char dat[ 1024 ];
	data = data.replace( QRegExp( "\r\n" ), "" );
	QString command = data.left( 3 );
	//kdDebug() << "KMService:parseCommand: "<< data << endl;
	if ( !connected ) {
		if ( command == "XFR" ) {
			// new server reconnect
			emit signalStatus( i18n( "Try other server" ) );
			newConnect( data );
			return ;
		}
	}
	if ( command == "VER" ) {
		sendServerPolicy( command);
		return ;
    }
    if ( command == "CVR" ) {
        sendServerPolicy( command);
        return ;
     }
    if ( command == "INF" ) {
		emit signalStatus( i18n( "Try to login" ) );
		sendInitialInfo();
		return ;
	}

    if( command == "USR" )
    {
        if ( kstr.word( data, 3 ) == "S" )
        {
            m_authData=data.section( ' ' , 4 , 4 );
            kdDebug() << "KMService::parseCommand m_authData: " << m_authData << endl;

            m_kv=QString::null;
            if( myHandle.contains("@hotmail.com") )
                m_sid="loginnet.passport.com";
            else if( myHandle.contains("@msn.com") ||  myHandle.contains("@compaq.net") ||  myHandle.contains("@webtv.net") )
                m_sid="msnialogin.passport.com";
            else
                m_sid="login.passport.com";

            QString authURL="https://"+m_sid+"/login.srf?" + m_authData;
            authURL.replace("," , "&" ) ;

            kdDebug() << "KMService::parseCommand: " << authURL << endl;

            KIO::Job *job = KIO::get( authURL , false, false );
            job->addMetaData("cookies", "manual");
            QObject::connect( job, SIGNAL(result( KIO::Job *)), this, SLOT(slotAuthJobDone( KIO::Job *)) );
        }
        else
        {
            // Successful auth
            receivedNick = kstr.word( data, 4 );
	    KMerlin::decode( receivedNick );
            emit signalStatus( i18n( "Actual nick name received: %1" ).arg( receivedNick ) ) ;
            sendSerial();
        }
        return ;
    }
	if ( command == "QNG" ) {
		lastPing = true;
		return ;
	}
	if ( command == "NLN" || command == "ILN" || command == "FLN" ) {
		statusReceived( data );
		return ;
	}
	if ( command == "LSG" ) {
		groupReceived( data );
		return ;
	}
	if ( command == "LST" ) {
		contactReceived( data );
        //kdDebug() << "KMService::parseCommand: LST (data)" << data << endl;
				if ( !connected ) {
					checkRemovedContacts();
					connected = true;
					if ( myNick.isEmpty() )
						myNick = receivedNick;
					if ( myNick != receivedNick )
						changePublicName( myNick );
					emit signalStatus( i18n( "Succesful connected, switching to Online" ) );
					setStatus( "NLN" );
					emit signalConnected( true );
					lastPing = true;
					QTimer::singleShot( 60000, this, SLOT( slotPing() ) );
				}
    	return ;
	}
	if ( command == "CHL" ) {
		sendFinalAuthentication( kstr.word( data, 2 ) );
		return ;
	}
	if ( command == "RNG" ) {
		// SessionID, Address, AuthInfo, handle, publicName
		slotCreateChat( kstr.word( data, 1 ), kstr.word( data, 2 ), kstr.word( data, 4 ), kstr.word( data, 5 ), kstr.word( data, 6 ).replace( QRegExp( "%20" ), " " ) );
		return ;
	}
	if ( command == "XFR" ) {
		// Address, AuthInfo
		slotCreateChat( kstr.word( data, 3 ), kstr.word( data, 5 ) );
		return ;
	}
	if ( command == "MSG" ) {
		len = kstr.word( data, 3 );
		socket->readBlock( dat, len.toUInt() );
		miss = QString::fromUtf8( dat );
		kdDebug() << miss << endl;
		if ( miss.contains( "Inbox-Unread:" ) ) {
			//this sends the server if we are going online, contains the unread message count
			miss = miss.right( miss.length() - miss.find( "Inbox-Unread:" ) );
			miss = miss.left( miss.find( "\r\n" ) );
			mailCount = miss.right( miss.length() - miss.find( " " ) - 1 ).toUInt();
			emit newMail( "", mailCount, "" );
			return ;
		}
		if ( miss.contains( "Message-Delta:" ) ) {
			// do not count deletings in trash folder or junk mail
			if ( miss.contains( "Dest-Folder: .!!trAsH" ) ) {
				return ;
			}
			if ( miss.contains( "Src-Folder: ACTIVE" ) ) {
				//this sends the server if mails are deleted, only delete mail from "Posteingang"
				miss = miss.right( miss.length() - miss.find( "Message-Delta:" ) );
				miss = miss.left( miss.find( "\r\n" ) );
				mailCount = mailCount - miss.right( miss.length() - miss.find( " " ) - 1 ).toUInt();
				emit newMail( "", mailCount, "" );
				return ;
			}
		}
		if ( miss.contains( "From-Addr:" ) ) {
			//this sends the server if a new mail has arrived
			kdDebug() << "the hole packet:" << miss << "\n" << endl;
			//Message-URL: /cgi-bin/getmsg?msg=MSG1029401739.3&start=1610592&len=402&curmbox=ACTIVE
			//Post-URL: https://lc1.law13.hotmail.passport.com/ppsecure/domessengerlogin/EN
			//Subject: Hi
			QString subject, postUrl, messageUrl;
			subject = miss;
			postUrl = miss;
			messageUrl = miss;

			// subject
			subject = subject.right( subject.length() - subject.find( "Subject: " ) );
			subject = subject.left( subject.find( "\r\n" ) );
			subject = subject.remove( 0, 9 ); //
			kdDebug() << "Message Subject: " << subject << "\n" << endl;

			// Message-URL
			messageUrl = messageUrl.right( messageUrl.length() - messageUrl.find( "Message-URL: " ) );
			messageUrl = messageUrl.left( messageUrl.find( "\r\n" ) );
			messageUrl = messageUrl.remove( 0, 13 ); //
			kdDebug() << "Message URL: " << messageUrl << "\n" << endl;

			// Post-URL
			postUrl = postUrl.right( postUrl.length() - postUrl.find( "Post-URL: " ) );
			postUrl = postUrl.left( postUrl.find( "\r\n" ) );
			postUrl = postUrl.remove( 0, 10 ); //
			kdDebug() << "Post URL: " << postUrl << "\n" << endl;

			miss = miss.right( miss.length() - miss.find( "From-Addr:" ) );
			miss = miss.left( miss.find( "\r\n" ) );
			mailCount++;
			miss = miss.right( miss.length() - miss.find( " " ) - 1 );
			emit newMail( miss, mailCount, subject );
			return ;
		} else if ( miss.contains( "text/x-msmsgsprofile" ) ) {

			//Hotmail profile
			if ( miss.contains( "MSPAuth:" ) ) {
				QRegExp rx( "MSPAuth: ([A-Za-z0-9$!*]*)" );
				rx.search( miss );
				m_MSPAuth = rx.cap( 1 );
			}
			if ( miss.contains( "sid:" ) ) {
				QRegExp rx( "sid: ([0-9]*)" );
				rx.search( miss );
				m_sid = rx.cap( 1 );
			}
			if ( miss.contains( "kv:" ) ) {
				QRegExp rx( "kv: ([0-9]*)" );
				rx.search( miss );
				m_kv = rx.cap( 1 );
			}
			loginTime = time( ( time_t * ) NULL );
			//write the tmp file

			//m_isHotmailAccount = true;
			//emit hotmailSeted(true);
		}
		return ;
	}

	if( command == "SYN" )
	{
		// this is the current serial on the server, if its different with the own we can get the user list
		QString serial = data.section( ' ', 2, 2 );
        kdDebug() << "KMService::parseCommand: " <<  serial << endl;


		if( serial != _serial )
		{
			//emit newContactList();  // remove all contacts datas, msn sends a new contact list
            _serial = serial;
            //m_account->setPluginData(m_account->protocol() , "serial" , data.section( ' ', 0, 0 ) );
		}
		else //we have already the contactlist
		{
			//receive even the grouplist, since it is not correctly saved
		}/*
        	QString commando = "LSG 5\r\n";
	        socket->writeBlock( commando, commando.length() );
	        kdDebug() << "KMService::parseCommand (send)" << commando << endl;

            commando = "CHG 6 NLN\r\n";
	        socket->writeBlock( commando, commando.length() );
	        kdDebug() << "KMService::parseCommand (send)" << commando << endl;*/
    }


	if ( command == "RMG" ) { // serial, id
		_serial = kstr.word( data, 2 );
		KMGroup *group;
		for ( group = theApp->groupList.first(); group != 0; group = theApp->groupList.next() ) {
			if ( group->toolId == kstr.word( data, 3 ) ) {
				group->remove
				();
				return ;
			}
		}
	}
	if ( command == "REG" ) {
		// groupName, serial, group
		KMGroup * group;
		_serial = kstr.word( data, 2 );
		for ( group = theApp->groupList.first(); group != 0; group = theApp->groupList.next() ) {
			if ( group->toolId == kstr.word( data, 3 ) ) {
				group->rename( kstr.word( data, 4 ).replace( QRegExp( "%20" ), " " ) );
				return ;
			}
		}
		return ;
	}
	if ( command == "ADG" ) {
		// groupName, serial, group
		_serial = kstr.word( data, 2 );
		new KMGroup( kstr.word( data, 3 ).replace( QRegExp( "%20" ), " " ), kstr.word( data, 4 ) );
		return ;
	}
	if ( command == "CHG" ) {
		m_status = kstr.word( data, 2 );
		emit statusChanged( kstr.word( data, 2 ) );
		return ;
	}
	if ( command == "OUT" ) {
		if ( kstr.word( data, 1 ) == "OTH" ) {
			KMessageBox::error( 0, i18n( "You have connected\nfrom an other client" ) );
		}
		//      slotSocketClosed();
		//		emit sessionClosed( kstr.word( txt,1) );
		return ;
	}
	if ( command == "REM" )     // someone is removed from a list
	{
		KMContact * contact;
		_serial = kstr.word( data, 3 );
		QString handle = kstr.word( data, 4 );
		for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() )
		{
			if ( contact->getHandle() == handle ) {
				if ( kstr.word( data, 2 ) == "FL" ) {
					contact->removed( handle, "FL", kstr.word( data, 5 ) );
					return ;
				}
				else if( kstr.word( data, 2) == "BL" ){
					contact->setBlocked( false );
					return ;
				}
				else if( kstr.word( data, 2 ) == "RL" ){
					contact->cleanUp(); // removes the contact with no info
					return;
				}
			}
		}
		return ;
	}
	if ( command == "ADD" )     // ####fixme - same as contactReceived
	{
		QString handle = kstr.word( data, 4 );
		QString nick = kstr.word( data, 5 );
		KMerlin::decode( nick );
		QString list = kstr.word( data, 2 );
		_serial = kstr.word( data, 3 );
		QString group = 0L;
		KMContact *contact;
		if ( list == "FL" )
		{
			group = kstr.word( data, 6 );
			for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() ) {
				if ( contact->getHandle() == handle ) {
					if ( contact->isForward() )
						contact->addToGroup( group );
					else
						contact->setForward( nick, group );
					return ;
				}
			}
			contact = new KMContact( handle, nick );
			contact->setForward( nick, group );
			return ;
		}
		for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() )
		{
			if ( contact->getHandle() == handle ) {
				if ( list == "AL" )
					contact->setAllow( true );
				else if ( list == "BL" )
					contact->setBlocked( true );
				else if ( list == "RL" )
					contact->setReverse( true );
				return ;
			}
		}
		contact = new KMContact( handle, nick );
		// ##  need contact auth #############
		AuthDlg * auth = new AuthDlg( 0, "Authenticate" );
		auth->m_handle->setText( handle );
		connect( auth->btnOK, SIGNAL( clicked() ), this, SLOT( authenticate() ) );
		auth->show();
		if ( list == "AL" )
			contact->setAllow( true );
		else if ( list == "BL" )
			contact->setBlocked( true );
		else if ( list == "RL" )
			contact->setReverse( true );
	} // and ADD
	if ( command == "REA" ) {
		QString handle = kstr.word( data, 3 );
		QString nick = kstr.word( data, 4 );
		KMerlin::decode( nick );
		QString id = kstr.word( data, 1 );
		_serial = kstr.word( data, 2 );
		if ( handle == myHandle ) {
			emit nickChanged( nick );
			myNick = nick;
			return ;
		} else if ( id == "0" ) {
			//do nothing
			return ;
		}
		kdDebug() << "REA command?: " << data << endl;
	}


}
/**/
void KMService::contactReceived( QString txt )
{
    //LST janjansen73@hotmail.com Hallo%3F 11 0
    QString handle = ( txt.section( ' ', 1, 1 ) );
    //kdDebug() << "KMService::contactReceived:handle " <<  handle << endl;

    QString nick = ( txt.section( ' ', 2, 2 ) );
    KMerlin::decode( nick );
    //kdDebug() << "KMService::contactReceived:nick " <<  nick << endl;

    QString group = ( txt.section( ' ', 4, 4 ) );
    //kdDebug() << "KMService::contactReceived:Group " <<  group << endl;
    if ( handle == "" ) return ;
   

    //QString list = kstr.word( txt, 2 );
    QString list = ( txt.section( ' ', 3, 3 ) );
	int flags = list.toInt();

	KMContact *contact;
	contact = new KMContact( handle, nick );
	if( flags & Forward )
		contact->setForward( nick, group );
	if( flags & Allow )
		contact->setAllow( TRUE );
	if( flags & Block )
		contact->setBlocked( TRUE );
	if( flags & Reverse )
		contact->setReverse( TRUE );

	// new contact! open auth dialog
	if( !(flags & Forward) && !(flags & Allow) && !(flags & Block) && ( flags & Reverse ) ){
		AuthDlg * auth = new AuthDlg( 0, "Authenticate" );
		auth->m_handle->setText( handle );
		connect( auth->btnOK, SIGNAL( clicked() ), this, SLOT( authenticate() ) );
		auth->show();
	}
}

void KMService::statusReceived( QString txt )
{
	KMContact * contact;
	QString handle;
	QString nick;
	QString status;
	if ( txt.left( 3 ) == "NLN" ) {
		handle = kstr.word( txt, 2 );
		nick = kstr.word( txt, 3 );
		KMerlin::decode( nick );
		status = kstr.word( txt, 1 );
		if ( handle == myHandle ) {
			emit statusChanged( status );
			return ;
		} else {
			for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() ) {
				if ( contact->getHandle() == handle ) {
					// check status, and nickname. ### only send status notify when new status // check for status
					if ( contact->getNick() != nick ){
						changeNickname( handle, nick ); // if nick is different as conta->nick rename it
						contact->setNickName( nick, true );
					}
					if ( contact->getStatus() != status ) {
						contact->setStatus( status );
						emit statusChanged( handle, status );
					}
					return ;
				}
			}
		}
	} else if ( txt.left( 3 ) == "ILN" ) {
		// initial status
		handle = kstr.word( txt, 3 );
		nick = kstr.word( txt, 4 );
		KMerlin::decode( nick );
		status = kstr.word( txt, 2 );
		for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() ) {
			if ( contact->getHandle() == handle ) {
				if ( contact->getNick() != nick ){
					changeNickname( handle, nick );
					contact->setNickName( nick, false );
				}
				contact->setStatus( status );
				return ;
			}
		}
	} else if ( txt.left( 3 ) == "FLN" ) {
		handle = kstr.word( txt , 1 );
		status = "FLN";
		for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() ) {
			if ( contact->getHandle() == handle ) {
				contact->setStatus( status );
				emit statusChanged( handle, "FLN" );
			}
		}
	}
}

/**/
void KMService::groupReceived( QString txt )
{

    /// LSG 0 vrienden 0
    //kdDebug() << "MSNNotifySocket::parseCommand: txt: " << txt << endl;
    //the LSG syntax depends if it is called from SYN or from LSG
    QString group_name;
    QString group_id;
	if(txt.contains(' ') > 4) //FROM LSG
    {
        //LSG 5 1712 1 2 0 vrienden 0
        group_name = ( txt.section( ' ', 6, 6 ) );
        group_name = group_name.replace( QRegExp( "%20" ), " " ).utf8( );
        //kdDebug() << "KMService::contactReceived:group_name " <<  group_name << endl;
        group_id = ( txt.section( ' ', 7, 7 ) );
        //kdDebug() << "KMService::contactReceived:group_id " <<  group_id << endl;

    }
    else //from SYN
    {
        // LSG 0 vrienden 0
        group_name = ( txt.section( ' ', 2, 2 ) );
        group_name = group_name.replace( QRegExp( "%20" ), " " ).utf8( );
        //kdDebug() << "KMService::contactReceived:group_name " <<  group_name << endl;
        group_id = ( txt.section( ' ', 1, 1 ) );
        //kdDebug() << "KMService::contactReceived:group_id " <<  group_id << endl;
    }
    KMGroup * group;
	if ( txt.contains( "0 ~ 0" ) ) {
		group = new KMGroup( i18n( "Friends" ), "0" );
		renameGroup( i18n( "Friends" ), "0" );
		return ;
	}
	group = new KMGroup( group_name , group_id );
}
/**/
void KMService::closeService()
{
	QString command = "OUT\r\n";
	socket->writeBlock( command, command.length() );
	kdDebug() << "Send OUT" << endl;
}
void KMService::slotSocketClosed()
{
	if ( !connected ){
		if( socket != 0L ){
			delete socket;
			socket = 0L;
		}
		cleanUp();
		emit signalConnected( false );
		return ;
	}
	kdDebug() << "Connection to MSN server closed!" << endl;
	emit signalConnected( false );
	connected = false;
	cleanUp();
	delete socket;
	socket = 0L;
	_serial = "0";
}
/**/
void KMService::killSocket()
{
	slotSocketClosed();
}
/**/
void KMService::sendFinalAuthentication( QString res )
{
	ID = time( ( time_t * ) NULL );
	res.replace( QRegExp( "\r\n" ), "" );
	QString command;
	QString str3 = res + "Q1P7W2E4J9R8U3S5" ;
	KMD5 context( str3 );
	QString str5 = context.hexDigest( );
	command.sprintf( "QRY %lu msmsgs@msnmsgr.com 32\r\n", ID );
	command += str5 ;
	socket->writeBlock( command, command.length( ) );
}

/**/
void KMService::sendProtocol()
{
	ID = time( ( time_t * ) NULL );
	QString command;
    command.sprintf( "VER 0 MSNP9\r\n");
	socket->writeBlock( command, command.length() );
	kdDebug() << "KMService::sendProtocol : " << command << endl;
}

/**/
void KMService::newConnect( QString data )
{
	QString port, server;
	data = kstr.word ( data, 3 );
	port = data.right( data.length() - data.findRev( ":" ) - 1 );
	server = data.right( data.length() - data.findRev ( " " ) - 1 );
	server = server.left( server.find( ":" ) );
	kdDebug() << " KMService::newConnect :: Connecting to new Server: Server: " << server << "Port: " << port << endl;
	socket->connectToHost( server, port.toUInt() );
}

/**/
void KMService::sendServerPolicy( QString cmd )
{
    if( cmd == "VER" )
    {
        //sendCommand("CVR" , "0x0409 winnt 5.1 i386 MSNMSGR 5.0.0 MSMSGS " + m_msnId );
        QString command;
        //QString agent=KGlobal::config()->readEntry( "UserAgent", QString::null );
        QString agent = "0x0409 winnt 5.1 i386 MSNMSGR 5.0.0 MSMSGS ";
        /*
        if(agent.isEmpty())
        {
            struct utsname utsBuf;
            uname (&utsBuf);
            agent= i18n("MS Local code, see http://www.microsoft.com/globaldev/reference/oslocversion.mspx","0x0409") +
                " " + escape(utsBuf.sysname) + " " + escape(utsBuf.release) + " " + escape(utsBuf.machine) +" Kopete "+ escape(kapp->aboutData()->version()) + " Kopete";
        }
        */
        command.sprintf( "CVR 1 "+ agent +" "+ myHandle +"\r\n" );
        socket->writeBlock( command, command.length() );
        kdDebug() << "KMService::sendServerPolicy: " << command << endl;
        }
        else if ( cmd == "CVR" ) //else if( cmd == "INF" )
        {
            QString command;
            command.sprintf( "USR 2 TWN I "+ myHandle +"\r\n" );
            socket->writeBlock( command, command.length() );
            kdDebug() << "KMService::sendServerPolicy: " << command << endl;
        }
        else
        {
            kdDebug() << "KMService::sendServerPolicy: Unimplemented command." << endl;
                //<< cmd << " " << id << " " << data << "' from server!" << endl;
        }
}
/**/
void KMService::sendInitialInfo()
{
	md5_tr = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "USR %lu MD5 I ", md5_tr );
	command += myHandle.utf8() + "\r\n";
	socket->writeBlock( command, command.length() );
	kdDebug() << "Sending initial Authentication" << endl;
}
/**/
void KMService::sendResponseInfo()
{
	if ( hashRes.contains( "\r\n", true ) ) {
		hashRes = hashRes.left( hashRes.find( "\r\n", 0, true ) );
	}
	QString str3 = hashRes + myPassword;
	KMD5 context( str3 );
	QString strcommand1, str5 = context.hexDigest();
	strcommand1.sprintf( "USR %lu ", md5_tr );
	strcommand1 += "MD5 S ";
	strcommand1 += str5 + "\r\n";
	socket->writeBlock( strcommand1, strcommand1.length() );
	kdDebug() << "Send Response Info: " << strcommand1 << endl;


}
/**/
void KMService::sendSerial()
{
	ID = time( ( time_t * ) NULL );
	QString command;

    //_serial = "1712";
	kdDebug() << "KMService::sendSerial:_serial " << _serial << endl;

	command.sprintf( "SYN 4 ");
	command += _serial + "\r\n" ;
	kdDebug() << "KMService::sendSerial: " << command << endl;
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::setStatus( QString status )
{
	ID = time( ( time_t * ) NULL );
	QString command;
	if ( status == "NLN" ) {
		command.sprintf( "CHG %lu NLN\r\n", ID );
	} else if ( status == "BSY" ) {
		command.sprintf( "CHG %lu BSY\r\n", ID );
	} else if ( status == "BRB" ) {
		command.sprintf( "CHG %lu BRB\r\n", ID );
	} else if ( status == "AWY" ) {
		command.sprintf( "CHG %lu AWY\r\n", ID );
	} else if ( status == "PHN" ) {
		command.sprintf( "CHG %lu PHN\r\n", ID );
	} else if ( status == "LUN" ) {
		command.sprintf( "CHG %lu LUN\r\n", ID );
	} else if ( status == "FLN" ) {
		command.sprintf( "CHG %lu FLN\r\n", ID );
	} else if ( status == "HDN" ) {
		command.sprintf( "CHG %lu HDN\r\n", ID );
	} else if ( status == "IDL" ) {
		command.sprintf( "CHG %lu IDL\r\n", ID );
	}

	socket->writeBlock( command, command.length() );
}
/**/
void KMService::slotConnect()
{
	socket = new QSocket();
	connect( socket, SIGNAL( readyRead() ), this, SLOT( slotDataReceived() ) );
	connect( socket, SIGNAL( error( int ) ), this, SLOT( slotSocketError( int ) ) );
	connect( socket, SIGNAL( connected() ), this, SLOT( slotSocketConnected() ) );
	connect( socket, SIGNAL( connectionClosed() ), this, SLOT( slotSocketClosed() ) );
	//  connect(socket,SIGNAL(hostFound()),this,SLOT(slotHostFound()));
	kdDebug() << "Create new connection to: Server: messenger.hotmail.com Port:1863" << endl;
	socket->connectToHost( "messenger.hotmail.com", 1863 );
}
/**/
void KMService::slotSocketError( int error )
{
	if ( !_silent ) {
		switch ( error ) {
				case 0: {
					KMessageBox::error( 0, i18n( "Connection refused" ) );
					kdDebug() << "Connection error: Connection refused!" << endl;
					break;
				}
				case 1: {
					kdDebug() << "Connection error: Host not found!" << endl;
					KMessageBox::error( 0, i18n( "Host not found" ) );
					break;
				}
				case 2: {
					kdDebug() << "Connection error: Socket error!" << endl;
					KMessageBox::error( 0, i18n( "Socket error" ) );
					break;
				}
		}
	}
	emit signalConnected( false );
	connected = false;
}
/**/
void KMService::slotSocketConnected()
{
	sendProtocol();
}

/** change the nick name for a contact- this change it in all lists
  id = 0 , this will be ignored when receiving the server answer
*/
void KMService::changeNickname( const QString &handle, QString nick )
{
	QString command;
	ID = 0;
	command.sprintf( "REA %lu ", ID );
	KMerlin::encode( nick );
	command += handle.utf8() + " " + nick.utf8( ) + "\r\n";
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::changePublicName( QString publicName )
{
	QString command;
	ID = time( ( time_t* ) NULL );
	command.sprintf( "REA %lu ", ID );
	KMerlin::encode( publicName );
	command += myHandle.utf8() + " " + publicName.utf8( ) + "\r\n";
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::removeContact( const QString &handle, const QString &group, const QString &list )
{
	ID = time( ( time_t * ) NULL );
	QString command;
	if ( list == "FL" ) {
		command.sprintf( "REM %lu FL ", ID );
		command += handle.utf8() + " " + group.utf8() + "\r\n";
	} else if ( list == "AL" ) {
		command.sprintf( "REM %lu AL ", ID );
		command += handle.utf8() + "\r\n";
	} else {
		command.sprintf( "REM %lu BL ", ID );
		command += handle.utf8() + "\r\n";
	}
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::addContact( const QString &handle, QString publicName, const QString &group, const QString &list )
{
	time_t ID = time( ( time_t * ) NULL );
	QString command;
	if ( list == "FL" ) {
		command.sprintf( "ADD %lu FL ", ID );
		command += handle.utf8() + " " + handle.utf8() + " " + group.utf8() + "\r\n";
	} else if ( list == "AL" ) {
		command.sprintf( "ADD %lu AL ", ID );
		command += handle.utf8() + " " + publicName.replace( QRegExp( " " ), "%20" ).utf8() + "\r\n";
	} else {
		command.sprintf( "ADD %lu BL ", ID );
		command += handle.utf8() + " " + publicName.replace( QRegExp( " " ), "%20" ).utf8() + "\r\n";
	}
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::addGroup( QString groupName )
{
	groupName.replace( QRegExp( " " ), "%20" );
	ID = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "ADG %lu ", ID );
	command += groupName.utf8() + " 0\r\n";
	socket->writeBlock( command, command.length() );
}

void KMService::renameGroup( QString groupName, QString group )
{
	groupName.replace( QRegExp( " " ), "%20" );
	ID = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "REG %lu ", ID );
	command += group;
	command += " " + groupName.utf8( ) + " 0\r\n";
	socket->writeBlock( command, command.length( ) );
}

void KMService::removeGroup( QString group )
{
	QString command;
	ID = time( ( time_t * ) NULL );
	command.sprintf( "RMG %lu ", ID );
	command += group + " \r\n";
	socket->writeBlock( command, command.length( ) );
}

/* Incoming RING command: connect to the Switchboard server and send the startChat signal */
// SessionID, Address, AuthInfo, handle, publicName     ( RNG)
void KMService::slotCreateChat( QString ID, QString address, QString auth, QString handle, QString publicName )
{
	chatService = new IMChatService();
	chatService->setHandle( myHandle );
	chatService->connectToSwitchBoard( ID, address, auth );
	emit startChat( chatService, handle );
}

/* 	Incoming XFR command: this is an result from slotStartChatSession(handle)
**	connect to the switchboard server and sen startChat signal */ 
// (XFR)
void KMService::slotCreateChat( QString address, QString auth )
{
	chatService = new IMChatService();
	chatService->setHandle( myHandle );
	chatService->msgHandle = msgHandle;
	chatService->connectToSwitchBoard( 0L, address, auth );
	emit startChat( chatService, msgHandle );
}

/*	Start a new chat session: the result is an XFR command, see above
**	**FIXME** The chat session needs an update, */
void KMService::slotStartChatSession( QString handle )
{
	/*  if(connected)
	    {
	      if( handle == myHandle)
	        return;*/
	msgHandle = handle;  //thats a problem, current implementation of chat session is a problem!!!! :(
	// **FIXME** next ugly code
	/*
	* save the handle, so class kmerlin can show the chatwindow
	*/
	theApp->calledHandle = handle;
	createChatSession( );
	// }
}
/**/
void KMService::createChatSession()
{
	ID = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "XFR %lu SB\r\n", ID );
	socket->writeBlock( command, command.length() );
}
/**/
void KMService::blockContact( const QString &handle, QString msnNick )
{
	removeContact( handle, "", "AL" ); // remove it from the al list
	addContact( handle, msnNick, "", "BL" );
}
/**/
void KMService::allowContact( const QString &handle, QString msnNick )
{
	removeContact( handle, "", "BL" );
	addContact( handle, msnNick, "", "AL" );
}
/**/
void KMService::removeContact( const QString &handle, const QString &group )
{
	removeContact( handle, group, "FL" );
}
/**/
void KMService::removeContactL( const QString &handle, const QString &list )
{
	removeContact( handle, "", list );
}
/**/
void KMService::addContact( const QString &handle, const QString &id )
{
	addContact( handle, handle, id, "FL" );
	addContact( handle, handle, "", "AL" );
}
/**/
void KMService::insertContact( const QString &handle, QString msnNick, const QString &group )
{
	addContact( handle, msnNick, group, "FL" );
}
void KMService::checkRemovedContacts()
{
	KMContact * contact;
	for ( contact = theApp->contactList.first(); contact != 0; contact = theApp->contactList.next() ) {
		if ( !contact->isReverse() && !contact->isForward() ) { // unsure
			contact->cleanUp();
		}
	}
}
void KMService::slotOpenInbox()
{
	KTempFile tmpFile( locateLocal( "tmp", "kmerlin" ), ".html" );
	QString UserID = myHandle;
	time_t now = time( ( time_t * ) NULL ) - loginTime;
	QString md5this( m_MSPAuth );
	md5this += QString( "%1" ).arg( now ) + myPassword ;
	KMD5 md5( md5this );
	m_hotmailRequest =
	    "<html>\n"
	    "<head>\n"
	    "<noscript>\n"
	    "<meta http-equiv=Refresh content=\"0; url=http://www.hotmail.com\">\n"
	    "</noscript>\n"
	    "</head>\n"
	    "<body onload=\"document.pform.submit(); \">\n"
	    "<form name=\"pform\" action=\"https://loginnet.passport.com/ppsecure/md5auth.srf?lc=1033\" method=\"POST\">\n"
	    "<input type=\"hidden\" name=\"mode\" value=\"ttl\">\n"
	    "<input type=\"hidden\" name=\"login\" value=\"" + UserID.left( UserID.find( '@' ) ) + "\">\n"
	    "<input type=\"hidden\" name=\"username\" value=\"" + UserID + "\">\n"
	    "<input type=\"hidden\" name=\"sid\" value=\"" + m_sid + "\">\n"
	    "<input type=\"hidden\" name=\"kv\" value=\"" + m_kv + "\">\n"
	    "<input type=\"hidden\" name=\"id\" value=\"2\">\n"
	    "<input type=\"hidden\" name=\"sl\" value=\"" + QString( "%1" ).arg( now ) + "\">\n"
	    "<input type=\"hidden\" name=\"rru\" value=\"/cgi-bin/HoTMaiL\">\n"
	    "<input type=\"hidden\" name=\"auth\" value=\"" + m_MSPAuth + "\">\n"
	    "<input type=\"hidden\" name=\"creds\" value=\"" + QString::fromLatin1( md5.hexDigest() ) + "\">\n"
	    "<input type=\"hidden\" name=\"svc\" value=\"mail\">\n"
	    "<input type=\"hidden\" name=\"js\" value=\"yes\">\n"
	    "</form></body>\n"
	    "</html>\n";
	//tmpFile.setAutoDelete(true);
	*tmpFile.textStream() << m_hotmailRequest;
	kapp->invokeBrowser( tmpFile.name() );
}

void KMService::authenticate()
{
	AuthDlg * auth = (AuthDlg*)sender()->parent()->parent();
	if ( auth->m_allow->isChecked() )
		addContact( auth->m_handle->text(), "0" );
	else{
		addContact( auth->m_handle->text(), "0" );
		blockContact( auth->m_handle->text(), auth->m_handle->text() );
	}
	delete auth;
}
/** sends a ping to the server- result is QNG */
void KMService::sendPing()
{
	lastPing = false;
	QString command = "PNG \r\n";
	socket->writeBlock( command, command.length() );
}
void KMService::slotPing()
{
	if ( connected ) {
		if ( lastPing ) { // ok ping again
			QTimer::singleShot( 60000, this, SLOT( slotPing() ) );
			sendPing();
			return ;
		} else { // connections lost;
			killSocket();
			return ;
		}
	}
}

void KMService::slotAuthJobDataReceived ( KIO::Job */*job*/,const  QByteArray &data)
{
	m_authData += QCString( data, data.size()+1 );
//	kdDebug(14140) << "MSNNotifySocket::slotAuthJobDataReceived: " << data << endl;
}

void KMService::slotAuthJobDone ( KIO::Job *job)
{
	kdDebug(14140) << "MSNNotifySocket::slotAuthJobDone: "<< m_authData << endl;

	if(job->error())
	{
		//FIXME: Shouldn't we say that we are the MSN plugin?
		job->showErrorDialog();
		disconnect();
	}

	if(m_kv.isNull())
	{
		QStringList cookielist=QStringList::split("\n", job->queryMetaData("setcookies") );
		QString cookies="Cookie: ";
		for ( QStringList::Iterator it = cookielist.begin(); it != cookielist.end(); ++it )
		{
			kdDebug(14140) << "MSNNotifySocket::slotAuthJobDone: cl: " << *it << endl;
			QRegExp rx("Set-Cookie: ([^;]*)");
			rx.search(*it);
			cookies+=rx.cap(1)+";";
		}
		kdDebug(14140) << "MSNNotifySocket::slotAuthJobDone: cookie: " << cookies << endl;

		//QRegExp rx("lc=([1-9]*),id=([1-9]*),tw=([1-9]*),fs=[1-9]*,ru=[1-9a-zA-Z%]*,ct=[1-9]*,kpp=[1-9]*,kv=([1-9]*),");
		QRegExp rx("lc=([0-9]*),id=([0-9]*),tw=([0-9]*),.*kv=([0-9]*),");
		rx.search(m_authData);

        // org
		//QString authURL="https://" + m_sid + "/ppsecure/post.srf?lc=" + rx.cap(1) + "&id=" +rx.cap(2) +"&tw=" +rx.cap(3) +"&cbid=" + rx.cap(2)
//			+ "&da=passport.com&login=" + m_account->accountId() + "&domain=passport.com&passwd=" + escape ( m_password ) ;

		QString authURL="https://" + m_sid + "/ppsecure/post.srf?lc=" + rx.cap(1) + "&id=" +rx.cap(2) +"&tw=" +rx.cap(3) +"&cbid=" + rx.cap(2)
			+ "&da=passport.com&login=" + myHandle + "&domain=passport.com&passwd=" +  myPassword  ;

		m_authData = QString::null;
		m_kv=rx.cap(4);
		if(m_kv.isNull()) m_kv="";

		job = KIO::get( authURL, false, false );
		job->addMetaData("cookies", "manual");
		job->addMetaData("setcookies", cookies);

		QObject::connect( job, SIGNAL(data( KIO::Job *,const QByteArray&)), this, SLOT(slotAuthJobDataReceived( KIO::Job *,const QByteArray&)) );
		QObject::connect( job, SIGNAL(result( KIO::Job *)), this, SLOT(slotAuthJobDone( KIO::Job *)) );
	}
	else
	{
		QRegExp rx(/*URL=http://memberservices.passport.net/memberservice.srf*/"\\?did=[0-9]*&(t=[0-9A-Za-z!$*]*&p=[0-9A-Za-z!$*]*)\"");

        rx.search(m_authData);
        //kdDebug() << "KMService::slotAuthJobDone: m_authData " << m_authData << endl;

		m_badPassword=true;  //if this disconnect, that mean the password was bad
		//sendCommand("USR" , "TWN S " + rx.cap(1));

        //kdDebug() << "KMService::slotAuthJobDone: rx.cap(1) " << rx.cap(1) << endl;

        QString command;
        command.sprintf( "USR 3 TWN S " + rx.cap(1)+"\r\n" );
        socket->writeBlock( command, command.length() );
        kdDebug() << "KMService::slotAuthJobDone: " << command << endl;

	}
}
