/*

This file is part of KMerlin.
Copyright 2002 Niels Reedijk.
This file is distributed under the GNU GPL version 2 as found in the COPYING file in this distribution.

*/

#include "kmerlinnotify.h"
#include <qpixmap.h>
#include <qdatetime.h>
#include <kconfig.h>
#include <kglobal.h>
#include <klocale.h>
#include <klistview.h>
#include <kstddirs.h>
#include <krun.h>
#include <kmessagebox.h>
#include <knotifyclient.h>

#include "kmerlin.h"
#include "chat/kmerlinchat.h"
#include "kmservice.h"

KMerlinNotify::KMerlinNotify( QWidget *parent )
            : KPassivePopup( parent )
{
  //Do nothing
}

void KMerlinNotify::readSettings( KConfig *config )
{
  config->setGroup( "Notifications" );
  m_online_log               = config->readBoolEntry( "online_log" , true );
  m_online_popup             = config->readBoolEntry( "online_popup" , true );
  m_online_file              = config->readBoolEntry( "online_file" , true );
	m_online_sound             = config->readBoolEntry( "online_sound", false );
	m_online_sound_file        = config->readEntry( "online_sound_file", QString::null );
  m_offline_log              = config->readBoolEntry( "offline_log" , true );
  m_offline_popup            = config->readBoolEntry( "offline_popup" , true );
  m_offline_file             = config->readBoolEntry( "offline_file" , true );
	m_offline_sound            = config->readBoolEntry( "offline_sound", false );
	m_offline_sound_file       = config->readEntry( "offline_sound_file", QString::null );
  m_conversation_log         = config->readBoolEntry( "conversation_log" , true );
  m_conversation_popup       = config->readBoolEntry( "conversation_popup" , true );
  m_conversation_file        = config->readBoolEntry( "conversation_file" , true );
	m_conversation_sound       = config->readBoolEntry( "conversation_sound", false );
	m_conversation_sound_file  = config->readEntry( "conversation_sound_file", QString::null );
  m_email_log                = config->readBoolEntry( "email_log" , true );
  m_email_popup              = config->readBoolEntry( "email_popup" , true );
  m_email_file               = config->readBoolEntry( "email_file" , true );
	m_email_sound              = config->readBoolEntry( "email_sound", false );
	m_email_sound_file         = config->readEntry( "email_sound_file", QString::null );
  m_message_log              = config->readBoolEntry( "message_log" , true );
  m_message_popup            = config->readBoolEntry( "message_popup" , true );
  m_message_file             = config->readBoolEntry( "message_file" , true );
	m_message_sound            = config->readBoolEntry( "message_sound", false );
	m_message_sound_file       = config->readEntry( "message_sound_file", QString::null );
  m_notification_timeout     = ( config->readNumEntry( "notification_timeout" , 3 ) * 1000 );
  
  if ( m_saving_to_file == true )
    KMerlinNotify::cleanup();
  
  if ( m_online_file == true || m_offline_file == true || m_conversation_file == true ||
       m_email_file == true || m_message_file == true )
  {
    m_logfile.setName( config->readEntry( "file_path" , "kmerlin.log" ) );
    bool ret = m_logfile.open( IO_ReadWrite | IO_Append );
    if ( ret == false )
    {
      KMessageBox::error( 0 , "The file you specified for logging events couldn't be opened" );
      return;
    }
    m_logstream.setDevice( &m_logfile );
    m_saving_to_file = true;
  }
}
  

void KMerlinNotify::setEventList( KListView *list )
{
  m_eventlist = list;
}

void KMerlinNotify::cleanup()
{
  //Close the current file
  if ( m_saving_to_file == true )
    m_logfile.close();
}

KMerlinNotify * KMerlinNotify::emailMessage( QWidget *parent , QString subject , QString sender , QString messageUrl )
{
  KMerlinNotify *pop = new KMerlinNotify(parent);
  
  //Send to eventlog
  if ( m_eventlist !=0  && m_email_log == true )
  {
    new KListViewItem( m_eventlist , 
                       KGlobal::locale()->formatTime( QTime::currentTime() , true ) , 
                       i18n( "New mail from %1" ).arg( sender ) );
  }
  
  if ( m_email_file == true && m_saving_to_file == true )
    m_logstream << KGlobal::locale()->formatTime( QTime::currentTime() , true ) << " " << i18n( "New mail from %1" ).arg( sender ) << endl;

	if( m_email_sound ) // play the sound
		KNotifyClient::userEvent( QString::null, 1,-1, m_email_sound_file );
  if ( m_email_popup == true )
  {
    KStandardDirs dirs;
    pop->setView( i18n( "New Mail" ) , 
                  i18n( "Subject: %1\nFrom: %2" ).arg(subject).arg(sender) , 
                  QPixmap( dirs.findResource( "data" , "kmerlin/pics/mail.png" ) ) );
    pop->setTimeout( m_notification_timeout );
    pop->m_link = "http://pv2fd.pav2.hotmail.msn.com" + messageUrl; // this can be changed to use the new openhotmail slot
		                                                                // for later use, implement open this email
    connect( pop , SIGNAL( leftClicked() ) , KMerlin::getInstance()->imService , SLOT( slotOpenInbox() ) ); // call it in imservice
    pop->show();
    return pop;
  }
}


KMerlinNotify * KMerlinNotify::onlineMessage( QWidget *parent , QString name )
{
  KMerlinNotify *pop = new KMerlinNotify(parent);

  //Send to eventlog
  if ( m_eventlist !=0 && m_online_log == true )
  {
    new KListViewItem( m_eventlist , 
                       KGlobal::locale()->formatTime( QTime::currentTime() , true ) , 
                       i18n( "%1 is online" ).arg( name ) );
  }

  if ( m_online_file == true && m_saving_to_file == true )
    m_logstream << KGlobal::locale()->formatTime( QTime::currentTime() , true ) << " " << i18n( "%1 is online" ).arg( name )  << endl;

	if( m_online_sound )
		KNotifyClient::userEvent( QString::null, 1,-1, m_online_sound_file );
  if ( m_online_popup == true )
  {
    KStandardDirs dirs;
    pop->setView( i18n( "Status Change" ) ,
                  i18n( "%1 is online" ).arg(name) ,
                  QPixmap( dirs.findResource( "data" , "kmerlin/pics/penguin.png" ) ) );
    pop->setTimeout( m_notification_timeout );
    connect( pop, SIGNAL( leftClicked() ) , pop , SLOT( hide() ) );
    pop->show();
    return pop;
  }
}

KMerlinNotify * KMerlinNotify::offlineMessage( QWidget *parent , QString name )
{
  KMerlinNotify *pop = new KMerlinNotify(parent);

  //Send to eventlog
  if ( m_eventlist !=0 && m_offline_log == true )
  {
    new KListViewItem( m_eventlist , 
                       KGlobal::locale()->formatTime( QTime::currentTime() , true ) , 
                       i18n( "%1 went offline" ).arg( name ) );
  }

  if ( m_offline_file == true && m_saving_to_file == true )
    m_logstream << KGlobal::locale()->formatTime( QTime::currentTime() , true ) << " " << i18n( "%1 went offline" ).arg( name ) << endl;

	if( m_offline_sound )
		KNotifyClient::userEvent( QString::null, 1,-1, m_offline_sound_file );
		
  if ( m_offline_popup == true )
  {
    KStandardDirs dirs;
    pop->setView( i18n( "Status Change" ) ,
                  i18n( "%1 just went offline" ).arg(name) ,
                  QPixmap( dirs.findResource( "data" , "kmerlin/pics/penguin.png" ) ) );
    pop->setTimeout( m_notification_timeout );
    connect( pop, SIGNAL( leftClicked() ) , pop , SLOT( hide() ) );
    pop->show();
    return pop;
  }
}


KMerlinNotify * KMerlinNotify::conversationMessage( QWidget *parent , QString name , unsigned int identifier )
{
  KMerlinNotify *pop = new KMerlinNotify(parent);
  
  //Send to eventlog
  if ( m_eventlist !=0  && m_conversation_log == true )
  {
    new KListViewItem( m_eventlist , 
                       KGlobal::locale()->formatTime( QTime::currentTime() , true ) , 
                       i18n( "Started conversation with %1" ).arg( name ) );
  }

  if ( m_conversation_file == true && m_saving_to_file == true )
    m_logstream << KGlobal::locale()->formatTime( QTime::currentTime() , true ) << " " << i18n("You are in conversation with %1" ).arg(name) <<endl;
	
	if( m_conversation_sound )
		KNotifyClient::userEvent( QString::null, 1,-1, m_conversation_sound_file );
  if ( m_conversation_popup == true )
  {
    KStandardDirs dirs;
    pop->setView( i18n( "Conversation" ) ,
                  i18n( "You are in conversation with %1" ).arg(name) ,
                  QPixmap( dirs.findResource( "data" , "kmerlin/pics/chat.png" ) ) );
    pop->setTimeout( m_notification_timeout );
    connect( pop , SIGNAL( leftClicked() ) , pop , SLOT( slotShowWindow() ) );
    connect( pop , SIGNAL( showWindow( unsigned int ) ) , parent , SLOT( slotActivateChat( unsigned int ) ) );
    pop->m_identifier = identifier;
    pop->show();
    return pop;
  }
}

KMerlinNotify * KMerlinNotify::message( QWidget *parent , QString name , QString message )
{
  KMerlinNotify *pop = new KMerlinNotify(parent);
	pop->m_identifier = ( ( KMerlinChat* )parent)->ID;
  //Send to eventlog
  if ( m_eventlist !=0  && m_message_log == true )
  {
    new KListViewItem( m_eventlist , 
                       KGlobal::locale()->formatTime( QTime::currentTime() , true ) , 
                       i18n( "Message from %1: %2" ).arg( name ).arg(message.simplifyWhiteSpace()) );
  }
  
  if ( m_message_file == true && m_saving_to_file == true )
    m_logstream << KGlobal::locale()->formatTime( QTime::currentTime() , true ) << " " << i18n( "Message from %1: %2" ).arg( name ).arg(message.simplifyWhiteSpace()) << endl;
	
	if( m_message_sound )
		KNotifyClient::userEvent( QString::null, 1,-1, m_message_sound_file );
		
  if ( m_message_popup == true )
  {
    KStandardDirs dirs;
    pop->setView( i18n( "Message from %1" ).arg( name ) ,
                  message.simplifyWhiteSpace() ,
                  QPixmap( dirs.findResource( "data" , "kmerlin/pics/chat.png" ) ) );
    pop->setTimeout( m_notification_timeout );
    connect( pop , SIGNAL( leftClicked() ) , pop , SLOT( slotShowWindow() ) );
    connect( pop , SIGNAL( showWindow( unsigned int ) ) , KMerlin::getInstance() , SLOT( slotActivateChat( unsigned int ) ) );
    pop->show();
    return pop;
  }
}

void KMerlinNotify::mouseReleaseEvent( QMouseEvent *e )
{
  if (e->button() == Qt::LeftButton ) // On left button emit this one -> leads to openhotmail
    emit leftClicked();
  else //All others just emit these signals (and automagically hide)
  {
    emit clicked();
    emit clicked( e->pos() );
  }
}

// not longer needed
void KMerlinNotify::slotOpenHotmail()
{
  KRun *run = new KRun( m_link ); //Open link that is made in static KMerlinNotify::emailMessage
  hide();
}


void KMerlinNotify::slotShowWindow()
{
  emit showWindow( m_identifier );
  hide();
}

bool KMerlinNotify::m_online_log = false;
bool KMerlinNotify::m_online_popup = false;
bool KMerlinNotify::m_online_file = false;
bool KMerlinNotify::m_online_sound = false;
QString KMerlinNotify::m_online_sound_file = QString::null;
bool KMerlinNotify::m_offline_log = false;
bool KMerlinNotify::m_offline_popup = false;
bool KMerlinNotify::m_offline_file = false;
bool KMerlinNotify::m_offline_sound = false;
QString KMerlinNotify::m_offline_sound_file = QString::null;
bool KMerlinNotify::m_conversation_log = false;
bool KMerlinNotify::m_conversation_popup = false;
bool KMerlinNotify::m_conversation_file = false;
bool KMerlinNotify::m_conversation_sound = false;
QString KMerlinNotify::m_conversation_sound_file = QString::null;
bool KMerlinNotify::m_email_log = false;
bool KMerlinNotify::m_email_popup = false;
bool KMerlinNotify::m_email_file = false;
bool KMerlinNotify::m_email_sound = false;
QString KMerlinNotify::m_email_sound_file = QString::null;
bool KMerlinNotify::m_message_log = false;
bool KMerlinNotify::m_message_popup = false;
bool KMerlinNotify::m_message_file = false;
bool KMerlinNotify::m_message_sound = false;
QString KMerlinNotify::m_message_sound_file = QString::null;

int  KMerlinNotify::m_notification_timeout = 0;
QFile KMerlinNotify::m_logfile;
QTextStream KMerlinNotify::m_logstream;
bool KMerlinNotify::m_saving_to_file = false;

KListView *KMerlinNotify::m_eventlist = 0;

#include "kmerlinnotify.moc"
