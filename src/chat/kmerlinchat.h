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

#ifndef _KMERLINCHAT_H_
#define _KMERLINCHAT_H_

#include <qwidget.h>
#include <qlist.h>
#include <qfile.h>
#include <qdict.h>
#include <kmainwindow.h>
#include <kapp.h>

#include "kmerlinchatview.h"

class QPrinter;
class KToolBar;
class KMerlin;
class KToggleAction;
class KAction;
class KActionMenu;
class SmileyWidget;
class QCategoryWidget;
class KMContact;
class KMContactAction;
class QToolBar;
class QDockWindow;
class KPopupMenu;

class KMChatContact : public QObject
  {
    Q_OBJECT
public:
    KMChatContact( QString handle, QWidget *parent);
    KMChatContact();
    ~KMChatContact();
    KMContact *contact;   
public: // Members
     KMContactAction *action;
    int timer;   
    
public: // functions
    void setWriting( bool f );
    void setEnabled( bool enable );
    
private: // functions
    void timerEvent( QTimerEvent *e );
private slots:
    void statusChanged( const QString &status );
};

class KMerlinChat : public KMainWindow
{
   Q_OBJECT

public: //Members
    KMerlinChat();
    ~KMerlinChat();
    QString chatWith;
    bool switchBoardIsActive;
    void closeSwitchBoard();
    QDict<KMChatContact> contactList;
    QFile logFile;
    uint ID;
	KPopupMenu *dummy;

public: // Members
    KToolBar *smToolBar;   // smileys
    KToolBar *coToolBar;   // contacts
    KToolBar *ivToolBar;    // invite
private: // Members   
    QDict<KMContactAction> inviteList;
    KMerlinChatView *m_view;
    QPrinter *m_printer;
    SmileyWidget *smileyWidget;
    QCategoryWidget *toolBox;
    QDockWindow *dw;
    bool _logging;    
    bool _active;
    bool pending;
    int timer;
    QString pendingMessage;
    QString pendingFormat;
    QString pendingSystemMsg;
	QString baseContact;
    
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
    KToggleAction *formatBold;
    KToggleAction *formatItalic;
    KToggleAction *formatStrike;
    KToggleAction *formatUnderline;
    
    KAction *formatFont;
    KAction *formatColor;
	
	KAction *chatAction;
    
    KToggleAction *extraSmiley;
    KToggleAction *extraTime;    
    KToggleAction *extraLog;
	
	KToggleAction *viewTool;

private: // funktions
    void setupActions();
    void changeStatusbar( const QString& text );
    bool eventFilter( QObject *o, QEvent *e );
    void timerEvent(QTimerEvent* e);
		void closeEvent( QCloseEvent * event );
public: // functions
    void setBaseContact( const QString &handle );
    bool active() { return _active; }
    bool logging() { return _logging; }    
public slots:
    void contactTyping( QString handle );
    void messageReceived( QString handle, QString message, QString format );
    void messageReceived( bool f );
    void setActive( bool f ) { _active = f; }
    void contactReceived( const QString &handle, const QString &command, bool last );
    void statusReceived( const QString &handle, const QString &status );
    
private slots:
  void optionsShowStatusbar();
  void optionsShowToolbar();
  void optionsConfigureKeys();
  void optionsConfigureToolbars();
  void sendMessage( QString msg, QString format );
  void toggleLogging();
  void setColor();
  void setFont();
  void inviteContact();
	void fileQuit(); 
	void displayChat(); 
	void visibilityChanged( bool visible );
	void toggleToolWindow();
public:
  signals:
  void closeSession();
  void message( QString, QString );
  void typingMsg();
  void inviteContact( QString );
  void createSession( QString );
};

#endif
