/***************************************************************************
                          imchatservice.h  -  description
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


#ifndef IMCHATSERVICE_H
#define IMCHATSERVICE_H

#include <qobject.h>
#include <kstringhandler.h>
#include <qstrlist.h>


class QSocket;

class IMChatService : public QObject
  {
    Q_OBJECT
  public:
    IMChatService();
    ~IMChatService();
    QSocket *msgSocket;
    QString msgHandle;
    KStringHandler kstr;
    QSocket *fileSocket;
    void timerEvent(QTimerEvent *ev);
    int socketTimer;

  protected:
    QString myHandle;
    QString buffer;
    // functions
    QString readLine();
    bool canReadLine();
    QString readBlock(uint len);
    QString lastFileName; //Last transfer filename recieved
  public:
    void connectToSwitchBoard(QString ID, QString address, QString auth);
    void callUser();
    void setHandle(QString handle)
    {
      myHandle = handle;
    }
    // slots
  public slots:
    void slotDataReceived();
    void slotSendMsg(QString message, QString format);
    void slotSocketClosed();
    void slotCloseSession();
    void slotInviteContact(QString handle);
    void slotTypingMsg();
    void slotSocketError(int error);

    /** No descriptions */

    // signals
  signals:
    void msgReceived(QString, QString, QString );
    void startChat(IMChatService* switchoard);
    void userTypingMsg(QString);
    void msgAcknowledgement(bool);
    void userInChat(QString);
    void chatWith(QString,bool);
    void switchBoardIsActive(bool);
    void contactReceived( const QString&, const QString&, bool);
    void logMessage(const QString &);
  };

#endif
