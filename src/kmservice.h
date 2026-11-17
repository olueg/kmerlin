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

#ifndef _KMSERVICE_H_
#define _KMSERVICE_H_

// kde include
#include <kstringhandler.h>
#include <kio/job.h>
// qt include
#include <qobject.h>
#include <qsocket.h>
// other
#include <time.h>

class KMerlin;
class IMChatService;
class KMContact;

class KMService : public QObject
  {
    Q_OBJECT
  public:
    KMService();
    ~KMService();
    QString m_status;
    QString getHandle( )
    {
      return myHandle ;
    }
    QString getNickName( )
    {
      return myNick;
    }
    void setAccount( QString handle, QString nick, QString passwd )
    {
      myHandle = handle;
      myPassword = passwd;
	  myNick = nick;
    }

    void changePublicName( QString publicName );
    void removeContact( const QString &handle, const QString &group, const QString  &list );
    void addContact( const QString &handle, QString publicName, const QString &group, const QString &list );
    void addGroup(QString groupName);
    void renameGroup( QString groupName, QString group);
    void removeGroup( QString group);
    void setStatus( QString status );
    void insertContact( const QString &handle, QString msnNick, const QString &group );
    void addContact( const QString &handle, const QString &id );
    void removeContactL( const QString &handle, const QString &list );
    void removeContact( const QString &handle, const QString &group );
    void allowContact( const QString &handle, QString msnNick );
    void blockContact( const QString &handle, QString msnNick );
    void closeService();
    const QString& getStatus() { return m_status; }
    bool isConnected() { return connected ; }
    void killSocket();
	void setSilent( bool f){ _silent = f;}
    

  private: // members
    QString _serial;
	QString receivedNick;
    int mailCount;
    QString myHandle;
    QString myNick;
    QString myPassword;
    QString hashRes;
    QString msgHandle;
    bool _silent;
    time_t ID;
	time_t loginTime;
    time_t md5_tr;
    QSocket *socket;
    bool connected;
    KStringHandler kstr;
    KMerlin *theApp;
    IMChatService *chatService;
	bool autoRename;
	QString m_sid, m_kv, m_MSPAuth, m_hotmailRequest;
	bool lastPing;

    QString m_account;
    QString m_password;
    QString m_badPassword;
    QString m_loginTime;
    QString m_authData;


  private: // function
    void sendProtocol();
    void newConnect( QString data );
    void sendServerPolicy( QString cmd );
    void sendInitialInfo();
    void sendResponseInfo();
    void sendSerial();
    void sendCVR();
    void sendFinalAuthentication( QString res );
    void cleanUp();
    void parseCommand( QString txt);

    void groupReceived( QString txt );
    void statusReceived( QString txt );
    void contactReceived( QString txt );
    void createChatSession();
	void checkRemovedContacts();
	void changeNickname( const QString &handle, QString nick );
	void sendPing();
    
  private slots:
    void slotDataReceived();
    void slotSocketConnected();
    void slotSocketClosed();
    void slotSocketError( int error );
    void slotCreateChat(QString ID, QString address, QString auth, QString handle, QString publicName);
    void slotCreateChat( QString address, QString auth);
	/*! Called from AuthDlg
	* Allow or blocks a new Contact*/
	void authenticate();
	void slotPing();
	void slotAuthJobDataReceived ( KIO::Job *, const QByteArray &data);
	void slotAuthJobDone ( KIO::Job *);

    
  public slots:
    void slotConnect();
    void slotStartChatSession(QString handle);
	void slotOpenInbox();

  signals:
    void signalConnected( bool );
    void nickChanged( const QString&  );
    void signalStatus( const QString& );
    void newMail( const QString&, int, const QString& );
    void statusChanged( const QString& );
    void statusChanged( const QString&, const QString& );
    void startChat(IMChatService*,QString);

  };

#endif
