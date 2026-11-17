/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <kglobal.h>
#include <kconfig.h>
#include <klocale.h>
#include <kiconloader.h>
#include <knotifyclient.h>
#include <kfiledialog.h>


void SystemNotification::init()
{
	btnPlay->setPixmap( KGlobal::instance() ->iconLoader() ->loadIcon( "noatun" , KIcon::NoGroup , KIcon::SizeSmall ) );
	btnSound->setPixmap( KGlobal::instance() ->iconLoader() ->loadIcon( "fileopen" , KIcon::NoGroup , KIcon::SizeSmall ) );
	btnLog->setPixmap( KGlobal::instance() ->iconLoader() ->loadIcon( "fileopen" , KIcon::NoGroup , KIcon::SizeSmall ) );
	btnApp->setPixmap( KGlobal::instance() ->iconLoader() ->loadIcon( "fileopen" , KIcon::NoGroup , KIcon::SizeSmall ) );
	currentItem = 0L;

	// load pixmaps
	p_sound = KGlobal::instance() ->iconLoader() ->loadIcon( "noatun" , KIcon::NoGroup , KIcon::SizeSmall );
	p_exec = KGlobal::instance() ->iconLoader() ->loadIcon( "exec" , KIcon::NoGroup , KIcon::SizeSmall );
	p_file = KGlobal::instance() ->iconLoader() ->loadIcon( "edit" , KIcon::NoGroup , KIcon::SizeSmall );
	p_event = KGlobal::instance() ->iconLoader() ->loadIcon( "desktop" , KIcon::NoGroup , KIcon::SizeSmall );
	p_popup = KGlobal::instance() ->iconLoader() ->loadIcon( "info" , KIcon::NoGroup , KIcon::SizeSmall );

	// header 0=exec, 1=log, 2=event, 3=sound, 4 =popup
	m_List->header() ->setLabel( 0, p_exec, "    " );
	m_List->header() ->setLabel( 1, p_file, "    " );
	m_List->header() ->setLabel( 2, p_event, "    " );
	m_List->header() ->setLabel( 3, p_sound, "    " );
	m_List->header() ->setLabel( 4, p_popup, "    " );

	// create the listviewitems
	onlineItem = new QListViewItem( m_List );
	offlineItem = new QListViewItem( m_List );
	emailItem = new QListViewItem( m_List );
	messageItem = new QListViewItem( m_List );
	conversationItem = new QListViewItem( m_List );
	// set text
	onlineItem->setText( 5, i18n( "Someone from your buddy list is now online" ) );
	offlineItem->setText( 5, i18n( "Someone from your buddy list is now offline" ) );
	emailItem->setText( 5, i18n( "You have received a new e-mail" ) );
	messageItem->setText( 5, i18n( "You have received a new short message" ) );
	conversationItem->setText( 5, i18n( "A new conversation has started" ) );

	// read the settings
	KConfig * config = KGlobal::config();
	config->setGroup( "Notifications" );
	m_online_event = config->readBoolEntry( "online_event" , true ) ;
	if ( m_online_event )
		onlineItem->setPixmap( 2, p_event );
	m_online_popup = config->readBoolEntry( "online_popup" , true ) ;
	if ( m_online_popup )
		onlineItem->setPixmap( 4, p_popup );
	m_online_file = config->readBoolEntry( "online_file" , false ) ;
	if ( m_online_file )
		onlineItem->setPixmap( 1, p_file );
	m_online_sound = config->readBoolEntry( "online_sound" , false ) ;
	if ( m_online_sound )
		onlineItem->setPixmap( 3, p_sound );
	m_online_sound_file = config->readEntry( "online_sound_file" , QString::null ) ;

	m_offline_event = config->readBoolEntry( "offline_event" , true ) ;
	if ( m_offline_event )
		offlineItem->setPixmap( 2, p_event );
	m_offline_popup = config->readBoolEntry( "offline_popup" , true ) ;
	if ( m_offline_popup )
		offlineItem->setPixmap( 4, p_popup );
	m_offline_file = config->readBoolEntry( "offline_file" , false ) ;
	if ( m_offline_file )
		offlineItem->setPixmap( 1, p_file );
	m_offline_sound = config->readBoolEntry( "offline_sound" , false ) ;
	if ( m_offline_sound )
		offlineItem->setPixmap( 3, p_sound );
	m_offline_sound_file = config->readEntry( "offline_sound_file" , QString::null ) ;

	m_conversation_event = config->readBoolEntry( "conversation_event" , true ) ;
	if ( m_conversation_event )
		conversationItem->setPixmap( 2, p_event );
	m_conversation_popup = config->readBoolEntry( "conversation_popup" , true );
	if ( m_conversation_popup )
		conversationItem->setPixmap( 4, p_popup );
	m_conversation_file = config->readBoolEntry( "conversation_file" , false );
	if ( m_conversation_file )
		conversationItem->setPixmap( 1, p_file );
	m_conversation_sound = config->readBoolEntry( "conversation_sound" , false );
	if ( m_conversation_sound )
		conversationItem->setPixmap( 3, p_sound );
	m_conversation_sound_file = config->readEntry( "conversation_sound_file" , QString::null );

	m_email_event = config->readBoolEntry( "email_event" , true );
	if ( m_email_event )
		emailItem->setPixmap( 2, p_event );
	m_email_popup = config->readBoolEntry( "email_popup" , true );
	if ( m_email_popup )
		emailItem->setPixmap( 4, p_popup );
	m_email_file = config->readBoolEntry( "email_file" , false );
	if ( m_email_file )
		emailItem->setPixmap( 1, p_file );
	m_email_sound = config->readBoolEntry( "email_sound" , false );
	if ( m_email_sound )
		emailItem->setPixmap( 3, p_sound );
	m_email_sound_file = config->readEntry( "email_sound_file" , QString::null );

	m_message_event = config->readBoolEntry( "message_event" , true );
	if ( m_message_event )
		messageItem->setPixmap( 2, p_event );
	m_message_popup = config->readBoolEntry( "message_popup" , true );
	if ( m_message_popup )
		messageItem->setPixmap( 4, p_popup );
	m_message_file = config->readBoolEntry( "message_file" , false );
	if ( m_message_file )
		messageItem->setPixmap( 1, p_file );
	m_message_sound = config->readBoolEntry( "message_sound" , false );
	if ( m_message_sound )
		messageItem->setPixmap( 3, p_sound );
	m_message_sound_file = config->readEntry( "message_sound_file" , QString::null );

	m_timeout->setValue( config->readNumEntry( "notification_timeout" , 3 ) );
	m_fileName->setText( config->readEntry( "file_path" , "~/kmerlin/log/kmerlin.log" ) );
	m_animate->setChecked( config->readBoolEntry( "animate", true ) );
	
	m_List->setCurrentItem( onlineItem );
	m_List->setSelected( onlineItem, true );
	currentItem = onlineItem;

}

void SystemNotification::checkerToggled( bool f )
{
		if(currentItem == 0L) return;
	if ( sender() == m_sound ) 
		{
				if ( currentItem == onlineItem) 
				{
							m_online_sound = f;
							if ( f )
								onlineItem->setPixmap( 3, p_sound );
							else
								onlineItem->setPixmap( 3, QPixmap("") );
				}
				else if( currentItem == offlineItem )
				{
							m_offline_sound = f;
							if ( f )
								offlineItem->setPixmap( 3, p_sound );
							else
								offlineItem->setPixmap( 3, QPixmap("")  );
				}
				else if( currentItem == emailItem )
				{
							m_email_sound = f;
							if ( f )
								emailItem->setPixmap( 3, p_sound );
							else
								emailItem->setPixmap( 3, QPixmap("")  );
				}
				else if( currentItem == messageItem)
				{
							m_message_sound = f;
							if ( f )
								messageItem->setPixmap( 3, p_sound );
							else
								messageItem->setPixmap( 3, QPixmap("")  );
				}
				else if( currentItem == conversationItem ){
							m_conversation_sound = f;
							if ( f )
								conversationItem->setPixmap( 3, p_sound );
							else
								conversationItem->setPixmap( 3, QPixmap("")  );
				}
			}
	else if( sender() == m_file )
	{
				if ( currentItem == onlineItem)
				{
							m_online_file = f;
							if ( f )
								onlineItem->setPixmap( 1, p_file );
							else
								onlineItem->setPixmap( 1, QPixmap("")  );
				}
				else if( currentItem == offlineItem )
				{
							m_offline_file = f;
							if ( f )
								offlineItem->setPixmap( 1, p_file );
							else
								offlineItem->setPixmap( 1, QPixmap("")  );
				}
				else if( currentItem == emailItem )
				{
							m_email_file = f;
							if ( f )
								emailItem->setPixmap( 1, p_file );
							else
								emailItem->setPixmap( 1, QPixmap("")  );
				}
				else if( currentItem == messageItem )
				{
							m_message_file = f;
							if ( f )
								messageItem->setPixmap( 1, p_file );
							else
								messageItem->setPixmap( 1, QPixmap("")  );
				}
				else if( currentItem == conversationItem )
				{
							m_conversation_file = f;
							if ( f )
								conversationItem->setPixmap( 1, p_file );
							else
								conversationItem->setPixmap( 1, QPixmap("")  );
				}
	}
	else if( sender() ==  m_popup )
	{
				if( currentItem == onlineItem )
				{
							m_online_popup = f;
							if ( f )
								onlineItem->setPixmap( 4, p_popup);
							else
								onlineItem->setPixmap( 4, QPixmap("")  );
						}
				else if( currentItem == offlineItem)
				{
							m_offline_popup = f;
							if ( f )
								offlineItem->setPixmap( 4, p_popup);
							else
								offlineItem->setPixmap( 4, QPixmap("")  );
						}
				else if( currentItem == emailItem )
				{
							m_email_popup = f;
							if ( f )
								emailItem->setPixmap( 4, p_popup);
							else
								emailItem->setPixmap( 4, QPixmap("")  );
						}
				else if( currentItem == messageItem )
				{
							m_message_popup = f;
							if ( f )
								messageItem->setPixmap( 4, p_popup);
							else
								messageItem->setPixmap( 4, QPixmap("") );
						}
				else if ( currentItem == conversationItem )
				{
							m_conversation_popup = f;
							if ( f )
								conversationItem->setPixmap( 4, p_popup);
							else
								conversationItem->setPixmap( 4, QPixmap("")  );
				}
	}
	else if( sender() ==  m_event )
	{
				if( currentItem == onlineItem )
				{
							m_online_event = f;
							if ( f )
								onlineItem->setPixmap( 2, p_event );
							else
								onlineItem->setPixmap( 2, QPixmap("")  );
						}
				else if( currentItem == offlineItem)
				{
							m_offline_event = f;
							if ( f )
								offlineItem->setPixmap( 2, p_event );
							else
								offlineItem->setPixmap( 2, QPixmap("")  );
						}
				else if( currentItem == emailItem )
				{
							m_email_event = f;
							if ( f )
								emailItem->setPixmap( 2, p_event );
							else
								emailItem->setPixmap( 2, QPixmap("")  );
						}
				else if( currentItem == messageItem )
				{
							m_message_event = f;
							if ( f )
								messageItem->setPixmap( 2, p_event );
							else
								messageItem->setPixmap( 2, QPixmap("") );
						}
				else if ( currentItem == conversationItem )
				{
							m_conversation_event = f;
							if ( f )
								conversationItem->setPixmap( 2, p_event );
							else
								conversationItem->setPixmap( 2, QPixmap("")  );
				}
	}
}

void SystemNotification::selectionChanged( QListViewItem *item )
{
		currentItem = item;
		if( item == onlineItem)
		{
				m_sound->setChecked( m_online_sound );
				m_event->setChecked( m_online_event );
				m_popup->setChecked( m_online_popup );
				m_file->setChecked( m_online_file );
				m_soundFile->setText( m_online_sound_file );
		}
		else if ( item == offlineItem)
		{
				m_sound->setChecked( m_offline_sound );
				m_event->setChecked( m_offline_event );
				m_popup->setChecked( m_offline_popup );
				m_file->setChecked( m_offline_file );
				m_soundFile->setText( m_offline_sound_file );
		}
		else if ( item == messageItem )
		{
				m_sound->setChecked( m_message_sound );
				m_event->setChecked( m_message_event );
				m_popup->setChecked( m_message_popup );
				m_file->setChecked( m_message_file );
				m_soundFile->setText( m_message_sound_file );
		}
		else if( item == emailItem )
		{
				m_sound->setChecked( m_email_sound );
				m_event->setChecked( m_email_event );
				m_popup->setChecked( m_email_popup );
				m_file->setChecked( m_email_file );
				m_soundFile->setText( m_email_sound_file );
		}
		else if( item == conversationItem )
		{
				m_sound->setChecked( m_conversation_sound );
				m_event->setChecked( m_conversation_event );
				m_popup->setChecked( m_conversation_popup );
				m_file->setChecked( m_conversation_file );
				m_soundFile->setText( m_conversation_sound_file );
		}
}


void SystemNotification::playSound()
{
		if( !m_soundFile->text().isEmpty() )
		{
				KNotifyClient::userEvent( QString::null, 1, 1, m_soundFile->text() );
		}
}


void SystemNotification::openSound()
{
		m_soundFile->setText( KFileDialog::getOpenFileName( QString::null , //No start directory
                         "*" , //No filter
                         0 ,   //Where the dialog should be centered on
                         i18n( "Select sound file" ) ) );
		if( currentItem == onlineItem)
		{
				m_online_sound_file = m_soundFile->text();
		}
		else if ( currentItem == offlineItem)
		{
				m_offline_sound_file = m_soundFile->text();	
		}
		else if ( currentItem == messageItem )
		{
				m_message_sound_file = m_soundFile->text();	
		}
		else if( currentItem == emailItem )
		{
				m_email_sound_file = m_soundFile->text();	
		}
		else if( currentItem == conversationItem )
		{
				m_conversation_sound_file = m_soundFile->text();
		}
}


void SystemNotification::openLog()
{
		m_fileName->setText( KFileDialog::getOpenFileName( QString::null , //No start directory
                         "*" , //No filter
                         0 ,   //Where the dialog should be centered on
                         i18n( "Select log path" ) ) //Title 
													);
}

void SystemNotification::openApp()
{

}

void SystemNotification::save()
{
		KConfig *config = KGlobal::config();
		config->setGroup( "Notifications" );
		
		config->writeEntry( "online_event" , m_online_event );
		config->writeEntry( "online_popup" , m_online_popup );
		config->writeEntry( "online_file" , m_online_file );
		config->writeEntry( "online_sound", m_online_sound );
		config->writeEntry( "online_sound_file", m_online_sound_file );
		
		config->writeEntry( "offline_event" , m_offline_event );
		config->writeEntry( "offline_popup" , m_offline_popup );
		config->writeEntry( "offline_file" , m_offline_file );
		config->writeEntry( "offline_sound", m_offline_sound );
		config->writeEntry( "offline_sound_file", m_offline_sound_file );
		
		config->writeEntry( "conversation_event" , m_conversation_event );
		config->writeEntry( "conversation_popup" , m_conversation_popup );
		config->writeEntry( "conversation_file" , m_conversation_file );
		config->writeEntry( "conversation_sound", m_conversation_sound );
		config->writeEntry( "conversation_sound_file", m_conversation_sound_file );
		
		config->writeEntry( "email_event" , m_email_event );
		config->writeEntry( "email_popup" , m_email_popup );
		config->writeEntry( "email_file" , m_email_file );
		config->writeEntry( "email_sound", m_email_sound );
		config->writeEntry( "email_sound_file", m_email_sound_file );
      
		config->writeEntry( "message_event" , m_message_event );
		config->writeEntry( "message_popup" , m_message_popup );
		config->writeEntry( "message_file" , m_message_file );
		config->writeEntry( "message_sound", m_message_sound );
		config->writeEntry( "message_sound_file", m_message_sound_file );
    
		config->writeEntry( "notification_timeout" , m_timeout->value() );
  		config->writeEntry( "file_path" , m_fileName );
		config->writeEntry( "animate", m_animate->isChecked() );
		config->sync();
}


void SystemNotification::soundChanged( const QString& )
{
		if( currentItem == onlineItem)
		{
				m_online_sound_file = m_soundFile->text();
		}
		else if ( currentItem == offlineItem)
		{
				m_offline_sound_file = m_soundFile->text();	
		}
		else if ( currentItem == messageItem )
		{
				m_message_sound_file = m_soundFile->text();	
		}
		else if( currentItem == emailItem )
		{
				m_email_sound_file = m_soundFile->text();	
		}
		else if( currentItem == conversationItem )
		{
				m_conversation_sound_file = m_soundFile->text();
		}
}
