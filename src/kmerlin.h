/**
**********************************************************************
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
*********************************************************************
*/
#ifndef _KMERLIN_H_
#define _KMERLIN_H_

#ifdef HAVE_CONFIG_H
//  #include <config.h>
#endif

#include <kapp.h>
#include <kmainwindow.h>
#include <qlist.h>
#include <qdict.h>

#include "kmerlinview.h"

class QPrinter;
class QColor;
class KToggleAction;
class KURL;
class KMContact;
class KMGroup;
class QCategoryWidget;
class KMEmotionIcon;
class KMerlinProfile;
class KMService;
class KAction;
class KActionMenu;
class KMTextReplace;
class IMChatService;
class KMerlinChat;
class KListView;
class QDockWindow;
class KMContactAction;
class KMerlinDock;
class XAutoLock;
class CContactWidget;
class KPopupMenu;

/*!
 * The base class for KMerlin application windows. It sets up the main
 * window and reads the config file as well as providing a menubar, toolbar
 * and statusbar. An instance of KMerlinView creates your center view, which is connected
 * to the window's Doc object.
 * KMerlinApp reimplements the methods that KMainWindow provides for main window handling and supports
 * full session management as well as using KActions.
 * @see KMainWindow
 * @see KApplication
 * @see KConfig
 *
 * @author (c) The KMerlin Team.
 */
class KMerlin : public KMainWindow
{
  Q_OBJECT
public:
  QPtrList<KMContact>contactList; // ###update this to QPtrDict
  QPtrList<KMGroup>groupList;
  QPtrList<KMEmotionIcon> emotionList;
  QPtrList<KMTextReplace>replaceList;
  QPtrList<KMerlinChat>widgetList;  // ### please fixme
  QDict<KMContactAction>onlineAction;
  QDict<KMContactAction>offlineAction;
  QStringList profiles;     //List of profiles
  QPtrList<KMerlinProfile> profileList;
  KToolBar *onlineBar, *offlineBar;
  static KMerlin *getInstance();
  KMerlinProfile * getProfile( const QString &name, bool f );
  KMService *imService;
  QCategoryWidget *groupWidget;
  QCategoryWidget *onlineWidget;
  QDockWindow *dw;
  KListView *eventList;
  QColor fontColor;
  QString autoProfile;
  QString fontName;
  QString m_logFile;
  /*! If true the chat logs all trafic */
  bool enableLogging;
  bool enableEmotions;
  bool globalFont;
  bool visible;
  bool autoConnect;
  XAutoLock *idleTimer;
  int maxIdle;
  int workTimer;
  /** Default Constructor */
  KMerlin();
  CContactWidget *onlineContainer;
  CContactWidget *offlineContainer;
  virtual ~KMerlin();
  void setIdleTimer();
  KPopupMenu *onlineMenu;
  KPopupMenu *offlineMenu;
  QString defaultProfile;
  bool isConnected( )
  {
    return _connected ;
  }
  void toggleApp();
  void connectToMsn( const QString &handle, const QString &nick, const QString &password);
  void connectToMsn( const QString &nick );
  /*! System Tray widget */
  KMerlinDock *panel;
  void readReplaceList( KConfig *config );
  QString calledHandle; // displays the new chatwindow
  static void encode( QString &text );
  static void decode( QString &text );
protected:
  /**
   * Overridden virtuals for Qt drag 'n drop (XDND)
   */
  virtual void dragEnterEvent( QDragEnterEvent *event );
  virtual void dropEvent( QDropEvent *event );

protected:
  /**
   * This function is called when it is time for the app to save its
   * properties for session management purposes.
   */
  void saveOptions( );

  /**
   * This function is called when this app is restored.  The KConfig
   * object points to the session management config file that was saved
   * with @ref saveProperties
   */
  void readOptions();
  bool queryClose();
private slots:
  void optionsShowToolbar();
  void optionsShowStatusbar();
  void optionsConfigureKeys();
  void optionsConfigureToolbars();
  void optionsPreferences();

  void changeStatusbar( const QString& text );
  void changeCaption( const QString& text );
  /*! Opens the Hotmail inbox in the default browser */
  void openHotmail();
  void slotConnect();
  void slotDisconnect();
  void setStatus();
  void statusReceived( const QString &status );
  void statusReceived( const QString &handle, const QString &status );
  void toggleEventView();
  void toggleView();
  void closeKMerlin();
  /*! Toggles the system tray icon */
  void addToPanel();
  void setActive();
  void setIdle();

  void emailReceived( const QString &sender, int count, const QString &subject );
  /*! Opens the change nick dialog and sends the new nick name to the server */
  void editNick();
  /*! Open the edit contacts dialog */
  void editContacts();
  /*! Opens the edit groups dialog */
  void editGroups();
  /*! Saves the new received nick */
  void nickChanged( const QString &nick );
  void slotAutoConnectTimeout();
  void slotConnectTimeout();
  void slotConnecting();

  void addContactDlg();
  void addContact();
  void addGroupDlg();
  void addGroup();


private:
  void setupAccel();
  void setupActions();
  void initEmotionList();
  static uchar hex_to_int( uchar c );

private:
  KMerlinView *m_view;
  QPrinter *m_printer;
  KToggleAction *m_toolbarAction;
  KToggleAction *m_statusbarAction;
  KToggleAction *actionToPanel;
  KAction *fileImport;
  KAction *fileExport;
  KAction *fileHide;
  KAction *fileMail;
  KAction *extraGroups;
  KAction *extraContacts;
  KAction *extraNick;
  KAction *extraAddGroup;
  KAction *extraAddContact;
  KActionMenu *fileStatus;
  KToggleAction *statusNLN;
  KToggleAction *statusBSY;
  KToggleAction *statusBRB;
  KToggleAction *statusAWY;
  KToggleAction *statusPHN;
  KToggleAction *statusLUN;
  KToggleAction *statusHDN;
  KToggleAction *currentStatus;
  KAction *fileDisconnect;
  KToggleAction *viewEvent;
  KToggleAction *viewOnline;
  KToggleAction *viewGroup;
  static KMerlin *s_instance;
  KMerlinProfile *profile;
  bool _connected;
  uint m_layout;
  QString lastProfile;
  bool stopAutoConnect;
  QString handle;
  int connectTimer;

private:   //funktionen
  void checkPaths();
  void closeEvent( QCloseEvent *e );

signals:
  void signalConnect();
  void signalConnected( bool );
  void statusChanged( const QString&, const QString& );
  void profileChanged();

public slots:
  void slotConnected( bool f );
  void slotOpenChatWindow( IMChatService *board, QString handle );
  void slotActivateChat( unsigned int ID );

};

#endif // _KMERLIN_H_
