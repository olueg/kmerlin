/*

This file is part of KMerlin.
Copyright 2002 Niels Reedijk.
This file is distributed under the GNU GPL version 2 as found in the COPYING file in this distribution.

*/

#ifndef KMerlinNotify_DEFINED
#define KMerlinNotify_DEFINED
#include <kpassivepopup.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qstring.h>

class KListView;
class KConfig;
/*
  This class can't really be seen as a class. It is more a collection of methods
  that make the event notification system. The objects of the class represent
  kpassivepopups. Naturally only popups are made when the user asks for them.

  OO wise this design might be bad, but it works, so don't sweat about it. :-)
  
*/ 

class KMerlinNotify : public KPassivePopup
{
  Q_OBJECT
public:
  KMerlinNotify( QWidget *parent ); //Reimplemented
  //(Re)Read settings
  static void readSettings( KConfig *config );
  static void setEventList( KListView * );
  static void cleanup();
  //Takes an emailmessage and make sure that on left click hotmail is opened
  static KMerlinNotify *emailMessage( QWidget *parent , QString subject , QString sender , QString messageUrl );
  static KMerlinNotify *onlineMessage( QWidget *parent , QString name );
  static KMerlinNotify *offlineMessage( QWidget *parent , QString name );
  static KMerlinNotify *conversationMessage( QWidget *parent , QString name , unsigned int identifier );
  static KMerlinNotify *message( QWidget *parent , QString name , QString message );
signals:
  void leftClicked();
  void showWindow( unsigned int );
protected:
  virtual void mouseReleaseEvent( QMouseEvent *e );
protected slots:
  void slotOpenHotmail();
  void slotShowWindow();
private:
  QString m_link;//used by emailMessage
  unsigned int m_identifier;
  static KListView  *m_eventlist;
  static bool        m_online_log;
  static bool        m_online_popup;
  static bool        m_online_file;
	static bool        m_online_sound;
	static QString     m_online_sound_file;
  static bool        m_offline_log;
  static bool        m_offline_popup;
  static bool        m_offline_file;
	static bool        m_offline_sound;
	static QString     m_offline_sound_file;
  static bool        m_conversation_log;
  static bool        m_conversation_popup;
  static bool        m_conversation_file;
	static bool        m_conversation_sound;
	static QString     m_conversation_sound_file;
  static bool        m_email_log;
  static bool        m_email_popup;
  static bool        m_email_file;
	static bool        m_email_sound;
	static QString     m_email_sound_file;
  static bool        m_message_log;
  static bool        m_message_popup;
  static bool        m_message_file;
	static bool        m_message_sound;
	static QString     m_message_sound_file;
  static int         m_notification_timeout;
  static QFile       m_logfile;
  static QTextStream m_logstream;
  static bool        m_saving_to_file;
};
#endif

