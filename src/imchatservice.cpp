/***************************************************************************
                       imchatservice.cpp  -  description
                          -------------------
 begin                : Tue Nov 27 2001
 copyright            : (C) 2001 by Olaf Lueg
 email                : olueg@olsd.de
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "imchatservice.h"
#include "imfilereceive.h"
#include <time.h> 
// qt
#include <qdatetime.h>
#include <qsocket.h> 
// kde
#include <klocale.h>
#include <kglobal.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

/**/
IMChatService::IMChatService()
{}
/**/
IMChatService::~IMChatService()
{}
/**/
void IMChatService::connectToSwitchBoard( QString ID, QString address, QString auth )
{
	QString server, port, command;
	time_t Tr_ID = time( ( time_t * ) NULL );
	if ( ID == 0L ) {
		command.sprintf( "USR %lu ", Tr_ID );
		command += myHandle + " " + auth + "\r\n";
		server = address;
		server = server.left( server.find( ":" ) );
		port = address;
		port = port.right( port.length() - port.findRev( ":" ) - 1 );
	} else {
		command.sprintf( "ANS %lu ", Tr_ID );
		command += myHandle + " " + auth + " " + ID + "\r\n";
		server = address;
		server = server.left( server.find( ":" ) );
		port = address;
		port = port.right( port.length() - port.findRev( ":" ) - 1 );
	}
	msgSocket = new QSocket();
	//	msgSocket->enableRead(true);
	connect( msgSocket, SIGNAL( readyRead() ), this, SLOT( slotDataReceived() ) );
	//	connect(msgSocket, SIGNAL(connectionClosed()),this, SLOT(slotSockedClosed()));
	connect( msgSocket, SIGNAL( error( int ) ), this, SLOT( slotSocketError( int ) ) );
	msgSocket->connectToHost( server, port.toUInt() );
	msgSocket->writeBlock( command, command.length() );
	// send active message
	kdDebug() << "New connection to SwitchBoard server established" << endl;
	emit switchBoardIsActive( true );
	socketTimer = startTimer( 1000 );
}
/**/
void IMChatService::timerEvent( QTimerEvent *ev )
{
	if ( ev->timerId() == socketTimer ) {
		if ( msgSocket->state() == QSocket::Idle ) {
			emit switchBoardIsActive( false );
			killTimer( socketTimer );
			kdDebug() << "Socket is IDLE    deleting....." << endl;
			slotSocketClosed();
			return ;
			//			delete msgSocket;
		}
		if ( msgSocket->state() == QSocket::Closing ) {
			emit switchBoardIsActive( false );
			killTimer( socketTimer );
			kdDebug() << "Socket is Closing    deleting....." << endl;
			slotSocketClosed();
			return ;
			//			delete msgSocket;
		} else {
			emit switchBoardIsActive( true );
			return ;
		}
	}
}
/* reads a line from the buffer */
QString IMChatService::readLine()
{
	QString command;
	int index = buffer.find( "\r\n" );
	if ( index != -1 ) {
		command = buffer.left( index );
		buffer = buffer.remove( 0, index + 2 );
		command.replace( QRegExp( "\r\n" ), "" );
	}
	return command;
}
/* check if a new line is in the buffer */
bool IMChatService::canReadLine()
{
	if ( buffer.contains( "\r\n" ) )
		return true;
	return false;
}
/* reads a block of data from the buffer from  ( 0 to len ) */
QString IMChatService::readBlock( uint len )
{
	QString block;
	block = buffer.left( len );
	buffer = buffer.remove( 0, len );
	return block;
}
/**/
void IMChatService::slotDataReceived()
{
	QString str, len, miss, data, format;
	char dat[ 1024 ];
	int ret;
	if ( msgSocket->canReadLine() ) {
		str = msgSocket->readLine();
	} else {
		return ;
	}
	str = QString::fromUtf8( str );

redo:
	if ( str.left( 3 ) == "NAK" ) {
		emit msgAcknowledgement( false );    // msg has not reveived
	}
	if ( str.left( 3 ) == "ACK" ) {
		emit msgAcknowledgement( true );   // msg has received
	}
	if ( str.left( 3 ) == "JOI" ) {
		// new user joins the chat, update user in chat list
		emit switchBoardIsActive( true );
		emit contactReceived( kstr.word( str, 1 ), "JOI", false );
	}
	if ( str.left( 3 ) == "IRO" ) {
		// we have joined a multi chat session- this are the users in this chat
		emit switchBoardIsActive( true );
		if ( kstr.word( str, 2 ) == kstr.word( str, 3 ) ) {
			emit contactReceived( kstr.word( str, 4 ), "IRO", true );
		} else {
			emit contactReceived( kstr.word( str, 4 ), "IRO", false );
		}
	}
	if ( str.left( 3 ) == "USR" ) {
		callUser();
	}
	if ( str.left( 3 ) == "BYE" ) {
		// some has disconnect from chat, update user in chat list
		emit contactReceived( kstr.word( str, 1 ).replace( QRegExp( "\r\n" ), "" ), "BYE", false );
	}
	if ( str.left( 3 ) == "MSG" ) {
		len = kstr.word( str, 3 );
		msgSocket->readBlock( dat, len.toUInt() );
		miss = dat;
		miss = miss.left( len.toUInt() );
		miss = QString::fromUtf8( miss );
		if ( miss.contains( "Content-Type: text/plain;" ) ) {
			format = miss.right( miss.length() - miss.findRev( "X-MMS-IM-Format:" ) );
			format = format.left( format.find( "\r\n" ) );
			format = format.right( format.length() - format.find( " " ) );
			miss = miss.right( miss.length() - miss.findRev( "\r\n\r\n" ) );
			QString handle = kstr.word( str, 1 );
			emit msgReceived( handle, miss, format ); //.replace(QRegExp("\r\n"),""));
		}
		// incoming message for File-transfer
		if ( miss.contains( "Content-Type: text/x-msmsgsinvite; charset=UTF-8" ) ) {
			// file transfer accept support


			if ( miss.contains( "Invitation-Command: ACCEPT" ) ) {
				//Get the IP we are connecting to
				QString address = miss.right( miss.length() - miss.find( "IP-Address:" ) - 12 );
				address = address.left( address.find( "\r\n" ) );
				debug( "Got Address: " + address );
				//Get the port
				QString port = miss.right( miss.length() - miss.find( "Port:" ) - 6 );
				port = port.left( port.find( "\r\n" ) );
				debug( "Got Port: " + port );
				//Get the AuthCookie
				QString cookie = miss.right( miss.length() - miss.find( "AuthCookie:" ) - 12 );
				cookie = cookie.left( cookie.find( "\r\n" ) );
				debug( "Got Cookie: " + cookie );
				//Receive the file
				IMFileReceive fileReceive;
				fileReceive.run( lastFileName, address, port, cookie, myHandle );
			} else if ( miss.contains( "Invitation-Command: INVITE" ) ) {
				//TODO: ask the user if they want to accept
				//Get the session cookie
				QString cookie = miss.right( miss.length() - miss.find( "Invitation-Cookie:" ) - 19 );
				cookie = cookie.left( cookie.find( "\r\n" ) );
				debug( "Invitiation Cookie: " + cookie );
				//Get the file name
				QString fileName = miss.right( miss.length() - miss.find( "Application-File:" ) - 18 );
				fileName = fileName.left( fileName.find( "\r\n" ) );
				lastFileName = fileName;
				QString command, message;
				time_t Tr_ID = time( ( time_t * ) NULL );
				if ( KMessageBox::questionYesNo( 0, "Accept Transfer of " + fileName, "KMerlin File Transfer" ) == KMessageBox::Yes ) {
					message = "MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nInvitation-Command: ACCEPT\r\n";
					message += "Invitation-Cookie: " + cookie + "\r\nLaunch-Application: FALSE\r\nRequest-Data: IP-Address:";
				} else {
					message = "MIME-Version: 1.0\r\nContent-Type: text/x-msmsgsinvite; charset=UTF-8\r\n\r\nInvitation-Command: CANCEL\r\n";
					message += "Invitation-Cookie: " + cookie + "\r\nCancel-Code: REJECT";
				}
				command.sprintf( "MSG %lu N %d\r\n", Tr_ID, message.length() );
				command += message;
				msgSocket->writeBlock( command, command.length() );
			}
		}
		if ( miss.contains( "MIME-Version: 1.0\r\nContent-Type: text/x-msmsgscontrol\r\nTypingUser:" ) ) {
			QString message;
			message = miss.right( miss.length() - miss.findRev( " " ) - 1 );
			message = message.replace( QRegExp( "\r\n" ), "" );
			emit userTypingMsg( message );    // changed 20.10.2001
		}
	}

	if ( msgSocket->canReadLine() ) {
		str = msgSocket->readLine();
		str = QString::fromUtf8( str );
		goto redo;
	}
}
// this sends the user is typing msg
void IMChatService::slotTypingMsg()
{
	time_t Tr_ID = time( ( time_t * ) NULL );
	QString command, message = "MIME-Version: 1.0\r\nContent-Type: text/x-msmsgscontrol\r\nTypingUser: " + myHandle + "\r\n\r\n";
	command.sprintf( "MSG %lu U %d \r\n", Tr_ID, message.length() );
	command += message;
	msgSocket->writeBlock( command, command.length() );
}
// this Invites an Contact
void IMChatService::slotInviteContact( QString handle )
{
	time_t Tr_ID = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "CAL %lu ", Tr_ID );
	command += handle + "\r\n";
	msgSocket->writeBlock( command, command.length() );
}

// this sends a short message to the server
void IMChatService::slotSendMsg( QString message, QString format )
{
	time_t Tr_ID = time( ( time_t * ) NULL );
	QString command, head;
	head = "MIME-Version: 1.0\r\nContent-Type: text/plain; charset=UTF-8\r\nX-MMS-IM-Format: ";
	//X-MMS-IM-Format: FN=MS%20Sans%20Serif; EF=; CO=0; CS=0; PF=0\r\n\r\n";
	head += format + "\r\n\r\n";
	head += message.utf8();
	command.sprintf( "MSG %lu A %d\r\n", Tr_ID, head.length() );
	command += head;
	msgSocket->writeBlock( command, command.length() );
	emit msgReceived( myHandle, message, " " + format );    // send the own msg to chat window
}
/**/
void IMChatService::slotSocketClosed()
{
	// we have lost the connection, send a message to chatwindow (this will not displayed)
	killTimer( socketTimer );
	kdDebug() << "Connection to switchboard server lost!" << endl;
	emit switchBoardIsActive( false );
	delete this;
}
/**/
void IMChatService::slotCloseSession()
{
	QString command = "OUT\r\n";
	msgSocket->writeBlock( command, command.length() );
}
/**/
void IMChatService::callUser()
{
	time_t Tr_ID = time( ( time_t * ) NULL );
	QString command;
	command.sprintf( "CAL %lu ", Tr_ID );
	command += msgHandle + "\r\n";
	msgSocket->writeBlock( command, command.length() );
}
/**/
void IMChatService::slotSocketError( int error )
{
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
			default: {
				kdDebug() << "Connection error: Socket error!" << endl;
				KMessageBox::error( 0, i18n( "Socket error" ) );
			}
	}
	emit switchBoardIsActive( false );
}

#include "imchatservice.moc"
