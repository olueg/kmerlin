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
#include <qvariant.h>

#include "kmerlin.h"

#include "kmcontact.h"
#include "kmgroup.h"
#include "kmservice.h"
#include "qcategorywidget.h"
#include "kmerlinprofile.h"
#include "kmemotionicon.h"
#include "setting/settingsdialog.h"
#include "chat/kmerlinchat.h"
#include "kmtextreplace.h"
#include "imchatservice.h"
#include "kmerlinnotify.h"
#include "kmerlindock.h"
#include "kmcontactaction.h"
#include "groupdlg.h"

#include <qdragobject.h>
#include <qlineedit.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qdir.h>
#include <qcolor.h>
#include <qtimer.h>
#include <qpushbutton.h>

#include "xautolock.h"
#include <qdockwindow.h>

#include <klistview.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kfiledialog.h>
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
#include <kpopupmenu.h>




KMerlin *KMerlin::s_instance = 0;

/**/
KMerlin *KMerlin::getInstance()
{
  return s_instance;
}
KMerlinProfile *KMerlin::getProfile( const QString &name, bool f )
{
  for ( profile = profileList.first(); profile != 0; profile = profileList.next() )
  {
    if ( f )
    { // search for handle
      if ( profile->getHandle() == name )
        return profile;
    }
    else
    {
      if ( profile->getNick() == name )
        return profile;
    }
  }
  return 0L;
}

/**/
KMerlin::KMerlin()
    : KMainWindow( 0, "KMerlin" ),
    m_view( new KMerlinView( this ) ),
    m_printer( 0 )
{
  s_instance = this;
  _connected = false;
  globalFont = false;
  stopAutoConnect = false;
  //  replaceList->setAutoDelete( true );
  contactList.setAutoDelete( true );
  groupList.setAutoDelete( true );
  onlineAction.setAutoDelete( true );
  offlineAction.setAutoDelete( true );
  //  widgetList.setAutoDelete( true );
  handle = "" ;
  lastProfile = "";
  m_layout = 1;
  imService = new KMService( );
  // accept dnd
  setAcceptDrops( true );
  panel = new KMerlinDock( this, "Panel" );
  // tell the KMainWindow that this is indeed the main widget
  setCentralWidget( m_view );
  workTimer = 0L;
  connectTimer = 0L;
  idleTimer = new XAutoLock;
  connect( idleTimer, SIGNAL( activity() ), this, SLOT( setActive() ) );
  connect( idleTimer, SIGNAL( timeout() ), this, SLOT( setIdle( ) ) );
  setupActions();
  readOptions( );
  m_view->initView();
  initEmotionList();
  checkPaths();
  // and a status bar
  statusBar() ->show();

  // allow the view to change the statusbar and caption
  connect( m_view, SIGNAL( signalChangeStatusbar( const QString& ) ),
           this, SLOT( changeStatusbar( const QString& ) ) );
  connect( m_view, SIGNAL( signalChangeCaption( const QString& ) ),
           this, SLOT( changeCaption( const QString& ) ) );
  connect( this, SIGNAL( signalConnect( ) ), imService, SLOT( slotConnect() ) );
  connect( this, SIGNAL( signalConnect( ) ), this, SLOT( slotConnecting() ) );
  connect( imService, SIGNAL( startChat( IMChatService*, QString ) ), this, SLOT( slotOpenChatWindow( IMChatService*, QString ) ) );
  connect( imService, SIGNAL( statusChanged( const QString& ) ), this, SLOT( statusReceived( const QString& ) ) );
  connect( imService, SIGNAL( statusChanged( const QString&, const QString& ) ), this, SLOT( statusReceived( const QString&, const QString& ) ) );
  connect( imService, SIGNAL( newMail( const QString&, int, const QString& ) ), this, SLOT( emailReceived( const QString&, int, const QString& ) ) );
  connect( imService, SIGNAL( nickChanged( const QString& ) ), this, SLOT( nickChanged( const QString& ) ) );
  connect( imService, SIGNAL( signalStatus( const QString& ) ), this, SLOT( changeStatusbar( const QString& ) ) );
  setCaption( i18n( "Offline" ) );
  changeStatusbar( i18n( "Offline" ) );
  currentStatus = 0L;

  dw = new QDockWindow( QDockWindow::InDock, this );
  dw->setResizeEnabled( TRUE );
  //dw->setCloseMode( QDockWindow::Always );
  dw->setMovingEnabled( true );
  eventList = new KListView( dw, "EventList" );
  eventList->addColumn( "Time" );
  eventList->addColumn( "Event" );
  eventList->setAllColumnsShowFocus( true );
  // Let KMerlinNotify know about the list.
  KMerlinNotify::setEventList( eventList );
  addToolBar( dw, Qt::DockBottom );
  dw->setWidget( eventList );
  dw->setFixedExtentHeight( 100 );
  dw->setCaption( i18n( "Event List" ) );
  setDockEnabled( dw, Qt::DockTop, FALSE );
  setDockEnabled( dw, Qt::DockBottom, true );
  viewEvent->setChecked( false );
  dw->hide();
  fileStatus->setEnabled( false );
  fileMail->setEnabled( false );
  fileImport->setEnabled( false );
  fileExport->setEnabled( false );
  fileDisconnect->setEnabled( false );

  connect( imService, SIGNAL( signalStatus( const QString& ) ), m_view, SLOT( slotStatusMsg( const QString& ) ) );
  if ( defaultProfile != "" && autoConnect )
  {
    KMerlinProfile * profile = getProfile( defaultProfile, true );
    imService->setAccount( profile->getHandle(), profile->getPublicName(), profile->getPassword() );
    lastProfile = defaultProfile;
    imService->setSilent( true );
    emit signalConnected( true ); // for class Login, disables the connect button
    emit signalConnect();

  }
  fileMail->plug( panel->menu );
  ( KStdAction::quit( this, SLOT( closeKMerlin() ), actionCollection() ) ) ->plug( panel->menu );
  fileDisconnect->plug( panel->menu );
  fileStatus->plug( panel->menu );
  ( KStdAction::preferences( this, SLOT( optionsPreferences() ), actionCollection() ) ) ->plug( panel->menu );
  if ( actionToPanel->isChecked() )
  {
    addToPanel();
  }
  else
  {
    visible = true ;
  }
}
/**/
KMerlin::~KMerlin()
{
  saveOptions();
}
/**/
void KMerlin::setupActions()
{
  fileDisconnect = new KAction( i18n( "Disconnect" ), "stop", 0, this, SLOT( slotDisconnect( ) ), actionCollection(), "disconnect" );

  fileStatus = new KActionMenu( i18n( "Status" ), "go2", actionCollection(), "fileStatus" );
  fileStatus->setDelayed( false );
  statusNLN = new KToggleAction( i18n( "Online" ), "kmonline", 0, this, SLOT( setStatus() ), actionCollection(), "NLN" );
  statusBSY = new KToggleAction( i18n( "Busy" ), "kmbsy", 0, this, SLOT( setStatus() ), actionCollection(), "BSY" );
  statusBRB = new KToggleAction( i18n( "Be right back" ), "kmback", 0, this, SLOT( setStatus() ), actionCollection(), "BRB" );
  statusAWY = new KToggleAction( i18n( "Away" ), "kmaway", 0, this, SLOT( setStatus() ), actionCollection(), "AWY" );
  statusPHN = new KToggleAction( i18n( "On the phone" ), "kmphone", 0, this, SLOT( setStatus() ), actionCollection(), "PHN" );
  statusLUN = new KToggleAction( i18n( "Out to lunch" ), "kmlunch", 0, this, SLOT( setStatus() ), actionCollection(), "LUN" );
  statusHDN = new KToggleAction( i18n( "Hidden" ), "kmoffline", 0, this, SLOT( setStatus() ), actionCollection(), "HDN" );
  fileStatus->insert( statusNLN );
  fileStatus->insert( statusBSY );
  fileStatus->insert( statusBRB );
  fileStatus->insert( statusAWY );
  fileStatus->insert( statusPHN );
  fileStatus->insert( statusLUN );
  fileStatus->insert( statusHDN );
  statusNLN->setEnabled( false );
  statusBSY->setEnabled( false );
  statusBRB->setEnabled( false );
  statusAWY->setEnabled( false );
  statusPHN->setEnabled( false );
  statusLUN->setEnabled( false );
  statusHDN->setEnabled( false );
  actionToPanel = new KToggleAction( i18n( "&Add to panel" ), "panel", 0, this, SLOT( addToPanel() ), actionCollection(), "addToPanel" );
  fileImport = new KAction( i18n( "Import..." ), "fileopen", 0, this, SLOT( fileImport( ) ), actionCollection(), "fileImport" );
  fileExport = new KAction( i18n( "Export..." ), "filesave", 0, this, SLOT( fileExport( ) ), actionCollection(), "fileExport" );
  fileHide = new KAction( i18n( "Hide Window" ), "viewmag-", 0, this, SLOT( fileHide( ) ), actionCollection(), "fileHide" );
  fileMail = new KAction( i18n( "Open Hotmail" ), "hotmail", 0, this, SLOT( openHotmail( ) ), actionCollection(), "mail" );
  KStdAction::quit( this, SLOT( closeKMerlin() ), actionCollection() );
  m_toolbarAction = KStdAction::showToolbar( this, SLOT( optionsShowToolbar() ), actionCollection() );
  m_statusbarAction = KStdAction::showStatusbar( this, SLOT( optionsShowStatusbar() ), actionCollection() );
  KStdAction::keyBindings( this, SLOT( optionsConfigureKeys() ), actionCollection() );
  KStdAction::configureToolbars( this, SLOT( optionsConfigureToolbars() ), actionCollection() );
  KStdAction::preferences( this, SLOT( optionsPreferences() ), actionCollection() );

  viewEvent = new KToggleAction( i18n( "&Event View" ), "viewmag+l", 0, this, SLOT( toggleEventView() ), actionCollection(), "viewEvent" );
  viewOnline = new KToggleAction( i18n( "&View Online" ), "viewmag+l", 0, this, SLOT( toggleView() ), actionCollection(), "viewOnline" );
  viewGroup = new KToggleAction( i18n( "View &Groups" ), "viewmag+l", 0, this, SLOT( toggleView() ), actionCollection(), "viewGroup" );

  extraGroups = new KAction( i18n( "Edit Groups..." ), "desktop", 0, this, SLOT( editGroups( ) ), actionCollection(), "editGroups" );
  extraContacts = new KAction( i18n( "Edit Contacts..." ), "desktop", 0, this, SLOT( editContacts( ) ), actionCollection(), "editContacts" );
  extraNick = new KAction( i18n( "Change Nick..." ), "tux", 0, this, SLOT( editNick( ) ), actionCollection(), "editNick" );
  extraAddGroup = new KAction( i18n( "Add Group..." ), "desktop", 0, this, SLOT( addGroupDlg( ) ), actionCollection(), "addGroup" );
  extraAddContact = new KAction( i18n( "Add Contact..." ), "desktop", 0, this, SLOT( addContactDlg( ) ), actionCollection(), "addContact" );
  createGUI();

  onlineMenu = new KPopupMenu( this );


  offlineMenu = new KPopupMenu( this );
  extraNick->setEnabled( false );
  extraAddGroup->setEnabled( false );
  extraAddContact->setEnabled( false );
}

/**/
void KMerlin::closeEvent( QCloseEvent * event )
{
  if ( actionToPanel->isChecked() )
  {
    hide();
    event->ignore();  //Hidden but not destroyed
    visible = false;
  }
  else
  {
    saveOptions();
    event->accept();  //destroy
    KMainWindow::closeEvent( event );
  }
}
/**/
bool KMerlin::queryClose()
{
  saveOptions();
  return true;
}
/**/
void KMerlin::closeKMerlin()
{
  saveOptions();
  return kapp->quit();
}
/**/
void KMerlin::saveOptions( )
{
  // the 'config' object points to the session managed
  // config file.  anything you write here will be available
  // later when this app is restored
  KConfig * config = KGlobal::config();
  config->setGroup( "General" );
  config->writeEntry( "Version", 3 );
  config->writeEntry( "Visible", visible );
  config->writeEntry( "Add To Panel", actionToPanel->isChecked() );
  config->writeEntry( "Default Profile", defaultProfile );
  config->writeEntry( "Auto Connect", autoConnect );
  config->writeEntry( "Position", pos() );
  saveMainWindowSettings( config, "General" );
  config->setGroup( "Chat" );
  config->writeEntry( "Global Font", globalFont );
  config->writeEntry( "Font", fontName );
  config->writeEntry( "Font Color", fontColor );
  config->writeEntry( "Enable Logging" , enableLogging );
  config->writeEntry( "Enable Emotions", enableEmotions );
  config->writeEntry( "Log File", m_logFile );
}

void KMerlin::readOptions( )
{
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote
  // in 'saveProperties'
  KConfig * config = KGlobal::config();
  config->setGroup( "General" );
  int v = config->readNumEntry( "Version", 0 );
  defaultProfile = config->readEntry( "Default Profile", QString::null );
  autoConnect = config->readBoolEntry( "Auto Connect", false );
  visible = config->readBoolEntry( "Visible", true ) ;
  actionToPanel->setChecked( config->readBoolEntry( "Add To Panel", false ) );
  maxIdle = config->readNumEntry( "MaxIdle", 0 );
  QPoint pos = config->readPointEntry( "Position" );
  if ( !pos.isNull() )
  {
    move( pos );
  }
  setIdleTimer();
  idleTimer->stop();
  config->setGroup( "Chat" );
  globalFont = config->readBoolEntry( "Global Font", false );
  fontName = config->readEntry( "Font", QString::null );
  fontColor = config->readColorEntry( "Font Color" );
  enableLogging = config->readBoolEntry( "Enable Logging" , false );
  enableEmotions = config->readBoolEntry( "Enable Emotions", true );
  m_logFile = config->readEntry( "Log File", "%E.log" );
  config->setGroup( "Profiles" );
  profiles = config->readListEntry( "Profiles" );
  if ( v < 1 )       // old profiles convert it
  {
    QStringList list;
    for ( int k = 0 ; k < profiles.count() ; k++ )
    {
      KMerlinProfile *profile = new KMerlinProfile();
      profile->load( profiles[ k ] );
      profile->convert( profiles[ k ] );
      list.append( profile->getHandle() );
      delete profile;
    }
    profiles = list;
    config->setGroup( "General" );
    config->writeEntry( "Version", 2 );
    config->setGroup( "Profiles" );
    config->writeEntry( "Profiles", profiles );
    config->sync();
  }
  if ( profiles.count() != 0 )
  {
    //Construct menu...
    for ( int i = 0 ; i < profiles.count() ; i++ )
    {
      KMerlinProfile *profile = new KMerlinProfile();
      profile->load( profiles[ i ] );
      profileList.append( profile );
    }
  }
  if ( v < 3 )
    KMessageBox::information( this, i18n( "Please check your identity settings" ) );
  readReplaceList( config );
  // Read Settings for the notification system
  KMerlinNotify::readSettings( config );
  applyMainWindowSettings( config, "General" );
  //  resize( QSize(440, 385).expandedTo(minimumSizeHint()) );
}
void KMerlin::readReplaceList( KConfig *config )
{
  QStringList list;
  KMTextReplace *textReplace;
  config->setGroup( "Text Replacement" );
  list = config->readListEntry( "Replace List" );
  replaceList.clear();
  for ( int i = 0; i < list.count();i++ )
  {
    textReplace = new KMTextReplace( list[ i ], config->readEntry( list[ i ], QString::null ) );
    replaceList.append( textReplace );
  }
}
void KMerlin::dragEnterEvent( QDragEnterEvent * event )
{
  // accept uri drops only
  event->accept( QUriDrag::canDecode( event ) );
}

void KMerlin::dropEvent( QDropEvent * event )
{
  // this is a very simplistic implementation of a drop event.  we
  // will only accept a dropped URL.  the Qt dnd code can do *much*
  // much more, so please read the docs there
  QStrList uri;

  // see if we can decode a URI.. if not, just ignore it
  if ( QUriDrag::decode( event, uri ) )
  {
    // okay, we have a URI.. process it
    QString url, target;
    url = uri.first();

    // load in the file

  }
}

void KMerlin::optionsShowToolbar()
{
  // this is all very cut and paste code for showing/hiding the
  // toolbar
  if ( m_toolbarAction->isChecked() )
    toolBar() ->show();
  else
    toolBar() ->hide();
}

void KMerlin::optionsShowStatusbar()
{
  // this is all very cut and paste code for showing/hiding the
  // statusbar
  if ( m_statusbarAction->isChecked() )
    statusBar() ->show();
  else
    statusBar() ->hide();
}

void KMerlin::optionsConfigureKeys()
{
  KKeyDialog::configureKeys( actionCollection(), "kmerlinui.rc" );
}

void KMerlin::optionsConfigureToolbars()
{
  // use the standard toolbar editor
  KEditToolbar dlg( actionCollection() );
  if ( dlg.exec() )
  {
    // recreate our GUI
    createGUI();
  }
}

void KMerlin::optionsPreferences()
{
  // popup some sort of preference dialog, here
  SettingsDialog * l = new SettingsDialog( this );
  connect( l, SIGNAL( profileChanged() ), this, SIGNAL( profileChanged() ) );
  l->show();
}

void KMerlin::changeStatusbar( const QString & text )
{
  // display the text on the statusbar
  statusBar() ->message( text );
}

void KMerlin::changeCaption( const QString & text )
{
  // display the text on the caption
  setCaption( text );
}

/**/
void KMerlin::slotConnected( bool f )
{
  _connected = f;
  emit signalConnected( f );
  panel->setConnected( f );
  if ( f )
  {
    fileMail->setEnabled( true );
    fileDisconnect->setText( i18n( "Disconnect" ) );
    fileDisconnect->setEnabled( true );
    fileStatus->setEnabled( true );
    changeStatusbar( i18n( "Connected as: %1" ).arg( imService->getNickName() ) );
    statusNLN->setEnabled( true );
    statusBSY->setEnabled( true );
    statusBRB->setEnabled( true );
    statusAWY->setEnabled( true );
    statusPHN->setEnabled( true );
    statusLUN->setEnabled( true );
    statusHDN->setEnabled( true );
    extraNick->setEnabled( true );
    extraAddGroup->setEnabled( true );
    extraAddContact->setEnabled( true );
    if ( maxIdle != 0 )
    {
      idleTimer->start();
    }
  }
  else
  {
    fileMail->setEnabled( false );
    fileDisconnect->setText( i18n( "Disconnect" ) );
    fileDisconnect->setEnabled( false );
    fileStatus->setEnabled( false );
    idleTimer->stop();
    statusNLN->setEnabled( false );
    statusBSY->setEnabled( false );
    statusBRB->setEnabled( false );
    statusAWY->setEnabled( false );
    statusPHN->setEnabled( false );
    statusLUN->setEnabled( false );
    statusHDN->setEnabled( false );
    extraNick->setEnabled( false );
    extraAddGroup->setEnabled( false );
    extraAddContact->setEnabled( false );
    changeStatusbar( i18n( "Offline" ) );
    setCaption( i18n( "Offline" ) );
    if ( autoConnect && defaultProfile == lastProfile && !stopAutoConnect )
    {
      QTimer::singleShot( 120000, this, SLOT( slotAutoConnectTimeout() ) );
      imService->setSilent( true );
    }
  }
  m_view->updateView( f, m_layout );
}

void KMerlin::connectToMsn( const QString &handle, const QString &nick, const QString &password )
{
  imService->setAccount( handle, nick, password );
  lastProfile = handle;
  slotConnect();
  return ;
}
void KMerlin::connectToMsn( const QString &nick )
{
  KMerlinProfile * profile = getProfile( nick, false ); // search by nick
  if ( profile->getPassword().isEmpty() )
  {
    KIO::PasswordDialog dlg( i18n( "Please enter your password" ), nick );
    dlg.setUserReadOnly( true );
    if ( dlg.exec() == QDialog::Accepted )
      imService->setAccount( profile->getHandle(), nick, dlg.password() );
    else
      return ;
  }
  else
    imService->setAccount( profile->getHandle(), nick, profile->getPassword() );
  lastProfile = handle;
  slotConnect();
}
/**/
void KMerlin::slotConnect()
{
  stopAutoConnect = false;
  QTimer::singleShot( 120000, this, SLOT( slotConnectTimeout() ) );
  imService->setSilent( false );
  emit signalConnect();
}
void KMerlin::slotConnecting()
{
  fileDisconnect->setText( i18n( "Cancel" ) );
  fileDisconnect->setEnabled( true );
}

/**/
void KMerlin::slotDisconnect()
{
  stopAutoConnect = true;
  if ( fileDisconnect->text() == i18n( "Cancel" ) )
    imService->killSocket();
  else
    imService->closeService();
}
/**/
void KMerlin::initEmotionList()
{
  emotionList.append( new KMEmotionIcon( ":-\\)", ":-)", "regular" ) );
  emotionList.append( new KMEmotionIcon( ":-\\(", ":-(", "sad" ) );
  emotionList.append( new KMEmotionIcon( ";-\\)", ";-)" , "wink" ) );
  emotionList.append( new KMEmotionIcon( ":o" , ":o" , "omg" ) );
  emotionList.append( new KMEmotionIcon( ":D" , ":D" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":p" , ":p" , "tounge" ) );
  emotionList.append( new KMEmotionIcon( ":s" , ":s" , "confused" ) );
  emotionList.append( new KMEmotionIcon( ":\\|" , ":|" , "whatchutalkingabout" ) );
  emotionList.append( new KMEmotionIcon( ":'\\(", ":'(", "cry" ) );
  emotionList.append( new KMEmotionIcon( ":-\\$", ":-$", "embaressed" ) );
  emotionList.append( new KMEmotionIcon( "\\(H\\)", "(H)", "shades" ) );
  emotionList.append( new KMEmotionIcon( ":-@" , ":-@", "angry" ) );
  emotionList.append( new KMEmotionIcon( "\\(A\\)" , "(A)", "angel" ) );
  emotionList.append( new KMEmotionIcon( "\\(6\\)" , "(6)", "devil" ) );
  emotionList.append( new KMEmotionIcon( "\\(I\\)" , "(I)", "lightbulb" ) );
  emotionList.append( new KMEmotionIcon( "\\(Y\\)" , "(Y)", "thumbs_up" ) );
  emotionList.append( new KMEmotionIcon( "\\(N\\)" , "(N)", "thumbs_down" ) );
  emotionList.append( new KMEmotionIcon( "\\(B\\)" , "(B)", "beer" ) );
  emotionList.append( new KMEmotionIcon( "\\(D\\)" , "(D)", "martini" ) );
  emotionList.append( new KMEmotionIcon( "\\(X\\)" , "(X)", "girl" ) );
  emotionList.append( new KMEmotionIcon( "\\(Z\\)" , "(Z)", "boy" ) );
  emotionList.append( new KMEmotionIcon( "\\(\\{\\)", "({)", "dude_hug" ) );
  emotionList.append( new KMEmotionIcon( "\\(\\}\\)", "(})", "girl_hug" ) );
  emotionList.append( new KMEmotionIcon( ":-\\[", ":-[", "bat" ) );
  emotionList.append( new KMEmotionIcon( "\\(L\\)" , "(L)", "heart" ) );
  emotionList.append( new KMEmotionIcon( "\\(U\\)" , "(U)", "broken_heart" ) );
  emotionList.append( new KMEmotionIcon( "\\(K\\)" , "(K)", "kiss" ) );
  emotionList.append( new KMEmotionIcon( "\\(G\\)" , "(G)", "present" ) );
  emotionList.append( new KMEmotionIcon( "\\(F\\)" , "(F)", "rose" ) );
  emotionList.append( new KMEmotionIcon( "\\(W\\)" , "(W)", "wilted_rose" ) );
  emotionList.append( new KMEmotionIcon( "\\(P\\)" , "(P)", "camera" ) );
  emotionList.append( new KMEmotionIcon( "\\(T\\)" , "(T)", "phone" ) );
  emotionList.append( new KMEmotionIcon( "\\(@\\)" , "(@)", "kittykay" ) );
  emotionList.append( new KMEmotionIcon( "\\(&\\)" , "(&)", "bowwow" ) );
  emotionList.append( new KMEmotionIcon( "\\(C\\)" , "(C)", "cup" ) );
  emotionList.append( new KMEmotionIcon( "\\(s\\)" , "(s)", "moon" ) );
  emotionList.append( new KMEmotionIcon( "\\(\\*\\)", "(*)", "star" ) );
  emotionList.append( new KMEmotionIcon( "\\(8\\)", "(8)", "musical_note" ) );
  emotionList.append( new KMEmotionIcon( "\\(E\\)" , "(E)", "envelope" ) );
  emotionList.append( new KMEmotionIcon( "\\(o\\)" , "(o)", "clock" ) );
  emotionList.append( new KMEmotionIcon( "\\(M\\)" , "(M)", "messenger" ) );
  emotionList.append( new KMEmotionIcon( "\\(y\\)" , "(y)", "thumbs_up" ) );
  emotionList.append( new KMEmotionIcon( "\\(n\\)" , "(n)", "thumbs_down" ) );
  emotionList.append( new KMEmotionIcon( "\\(b\\)" , "(b)", "beer" ) );
  emotionList.append( new KMEmotionIcon( "\\(d\\)" , "(d)", "martini" ) );
  emotionList.append( new KMEmotionIcon( "\\(x\\)" , "(x)", "girl" ) );
  emotionList.append( new KMEmotionIcon( "\\(z\\)" , "(z)", "boy" ) );
  emotionList.append( new KMEmotionIcon( ":\\[" , ":[" , "bat" ) );
  emotionList.append( new KMEmotionIcon( ":\\)" , ":)" , "regular" ) );
  emotionList.append( new KMEmotionIcon( ":\\(" , ":(" , "sad" ) );
  emotionList.append( new KMEmotionIcon( ":<" , ":<" , "sad" ) );
  emotionList.append( new KMEmotionIcon( ":-<" , ":-<", "sad" ) );
  emotionList.append( new KMEmotionIcon( ";\\)" , ";)" , "wink" ) );
  emotionList.append( new KMEmotionIcon( ":O" , ":O" , "omg" ) );
  emotionList.append( new KMEmotionIcon( ":-o" , ":-o" , "omg" ) );
  emotionList.append( new KMEmotionIcon( ":-O" , ":-O" , "omg" ) );
  emotionList.append( new KMEmotionIcon( ":d" , ":d" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":-D" , ":-D" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":-d" , ":-d" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":>" , ":>" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":->" , ":->" , "teeth" ) );
  emotionList.append( new KMEmotionIcon( ":P" , ":P" , "tounge" ) );
  emotionList.append( new KMEmotionIcon( ":-p" , ":-p" , "tounge" ) );
  emotionList.append( new KMEmotionIcon( ":-P" , ":-P" , "tounge" ) );
  emotionList.append( new KMEmotionIcon( ":S" , ":S" , "confused" ) );
  emotionList.append( new KMEmotionIcon( ":-s" , ":-s" , "confused" ) );
  emotionList.append( new KMEmotionIcon( ":-S" , ":-S" , "confused" ) ) ;
  emotionList.append( new KMEmotionIcon( ":-\\|", ":-|", "whatchutalkingabout" ) );
  emotionList.append( new KMEmotionIcon( ":\\$" , ":$" , "embaressed" ) );
  emotionList.append( new KMEmotionIcon( "\\(h\\)" , "(h)", "shades" ) );
  emotionList.append( new KMEmotionIcon( ":@" , ":@" , "angry" ) );
  emotionList.append( new KMEmotionIcon( "\\(a\\)" , "(a)", "angel" ) );
  emotionList.append( new KMEmotionIcon( "\\(l\\)" , "(l)", "heart" ) );
  emotionList.append( new KMEmotionIcon( "\\(u\\)" , "(u)", "broken_heart" ) );
  emotionList.append( new KMEmotionIcon( "\\(k\\)" , "(k)", "kiss" ) );
  emotionList.append( new KMEmotionIcon( "\\(g\\)" , "(g)", "present" ) );
  emotionList.append( new KMEmotionIcon( "\\(f\\)" , "(f)", "rose" ) );
  emotionList.append( new KMEmotionIcon( "\\(w\\)" , "(w)", "wilted_rose" ) );
  emotionList.append( new KMEmotionIcon( "\\(p\\)" , "(p)", "camera" ) );
  emotionList.append( new KMEmotionIcon( "\\(t\\)" , "(t)", "phone" ) );
  emotionList.append( new KMEmotionIcon( "\\(c\\)" , "(c)", "cup" ) ),
  emotionList.append( new KMEmotionIcon( "\\(i\\)" , "(i)", "lightbulb" ) );
  emotionList.append( new KMEmotionIcon( "\\(S\\)" , "(S)", "moon" ) );
  emotionList.append( new KMEmotionIcon( "\\(e\\)" , "(e)", "envelope" ) );
  emotionList.append( new KMEmotionIcon( "\\(^\\)" , "(^)", "cake" ) );
  emotionList.append( new KMEmotionIcon( "\\(O\\)" , "(O)", "clock" ) );
  emotionList.append( new KMEmotionIcon( "\\(m\\)" , "(m)", "messenger" ) );

  KStandardDirs dir;
  QString emoDir = "kmerlin/smilies/";
  KMEmotionIcon * icon;

  KConfig *config = new KConfig( dir.findResource( "data", emoDir + "smileyrc" ) );
  config->setGroup( "Default" );

  for ( icon = emotionList.first() ; icon != 0; icon = emotionList.next() )
  {
    icon->setFilePath( dir.findResource( "data", emoDir + config->readEntry( icon->getFileName() ) ) );
  }
  delete config;
}
/* This will check the default kmerlin paths, if it not exists it will create it*/
void KMerlin::checkPaths()
{
  // path for chat logging
  if ( !QDir::setCurrent( QDir::homeDirPath() + "/kmerlin" ) )
  {
    QDir dir( QDir::homeDirPath() );
    dir.mkdir( "kmerlin" );
  }
  if ( !QDir::setCurrent( QDir::homeDirPath() + "/kmerlin/log" ) )
  {
    QDir dir( QDir::homeDirPath() + "/kmerlin" );
    dir.mkdir( "log" );
  }
  if ( !QDir::setCurrent( QDir::homeDirPath() + "/kmerlin/contacts" ) )
  {
    QDir dir( QDir::homeDirPath() + "/kmerlin" );
    dir.mkdir( "contacts" );
  }
  if ( !QDir::setCurrent( QDir::homeDirPath() + "/kmerlin/events" ) )
  {
    QDir dir( QDir::homeDirPath() + "/kmerlin" );
    dir.mkdir( "events" );
  }
}
/* ###old code - fixme!!! */
void KMerlin::slotOpenChatWindow( IMChatService * board, QString handle )
{
  if ( widgetList.isEmpty() )
  {
    KMerlinChat * chatWindow = new KMerlinChat;
    chatWindow->show(); // ### Bug?
    if( calledHandle == handle )
    {
      calledHandle = "";
      chatWindow->show();
    }
    else
      chatWindow->showMinimized(); // ### ??
    chatWindow->setBaseContact( handle );
    //Show notification
    KMerlinNotify::conversationMessage( this, handle, chatWindow->ID );
    connect( this, SIGNAL( statusChanged( const QString&, const QString& ) ), chatWindow, SLOT( statusReceived( const QString&, const QString& ) ) );
    connect( chatWindow, SIGNAL( message( QString, QString ) ), board, SLOT( slotSendMsg( QString, QString ) ) );
    connect( chatWindow, SIGNAL( closeSession() ), board, SLOT( slotCloseSession() ) );
    connect( chatWindow, SIGNAL( inviteContact( QString ) ), board, SLOT( slotInviteContact( QString ) ) );
    connect( chatWindow, SIGNAL( typingMsg() ), board, SLOT( slotTypingMsg() ) );
    connect( chatWindow, SIGNAL( createSession( QString ) ), imService, SLOT( slotStartChatSession( QString ) ) );
    connect( board, SIGNAL( msgReceived( QString, QString, QString ) ), chatWindow, SLOT( messageReceived( QString, QString, QString ) ) );
    connect( board, SIGNAL( userTypingMsg( QString ) ), chatWindow, SLOT( contactTyping( QString ) ) );
    connect( board, SIGNAL( msgAcknowledgement( bool ) ), chatWindow, SLOT( messageReceived( bool ) ) );
    connect( board, SIGNAL( switchBoardIsActive( bool ) ), chatWindow, SLOT( setActive( bool ) ) );
    connect( board, SIGNAL( contactReceived( const QString&, const QString&, bool ) ), chatWindow, SLOT( contactReceived( const QString&, const QString&, bool ) ) );
    connect( chatWindow, SIGNAL( showNotifier( QString, QString, uint ) ), this, SLOT( slotNotifyMessage( QString, QString, uint ) ) );
    widgetList.append( chatWindow );
    return ;
  }
  else
  {
    KMerlinChat *chatWindow;
    for ( chatWindow = widgetList.first(); chatWindow != 0; chatWindow = widgetList.next() )
    {
      KMChatContact * contact = chatWindow->contactList.find( handle );
      if ( contact != 0 && chatWindow->contactList.count() <= 1 )
      {
        if ( chatWindow->active() )
        {
          // kill the old switchboard connection
          chatWindow->closeSwitchBoard();
        }
        connect( this, SIGNAL( statusChanged( const QString&, const QString& ) ), chatWindow, SLOT( statusReceived( const QString&, const QString& ) ) );
        chatWindow->disconnect();
        connect( chatWindow, SIGNAL( message( QString, QString ) ), board, SLOT( slotSendMsg( QString, QString ) ) );
        connect( chatWindow, SIGNAL( closeSession() ), board, SLOT( slotCloseSession() ) );
        connect( chatWindow, SIGNAL( inviteContact( QString ) ), board, SLOT( slotInviteContact( QString ) ) );
        connect( chatWindow, SIGNAL( typingMsg() ), board, SLOT( slotTypingMsg() ) );
        connect( chatWindow, SIGNAL( createSession( QString ) ), imService, SLOT( slotStartChatSession( QString ) ) );
        connect( board, SIGNAL( msgReceived( QString, QString, QString ) ), chatWindow, SLOT( messageReceived( QString, QString, QString ) ) );
        connect( board, SIGNAL( userTypingMsg( QString ) ), chatWindow, SLOT( contactTyping( QString ) ) );
        connect( board, SIGNAL( msgAcknowledgement( bool ) ), chatWindow, SLOT( messageReceived( bool ) ) );
        connect( board, SIGNAL( switchBoardIsActive( bool ) ), chatWindow, SLOT( setActive( bool ) ) );
        connect( board, SIGNAL( contactReceived( const QString&, const QString&, bool ) ), chatWindow, SLOT( contactReceived( const QString&, const QString&, bool ) ) );
        connect( chatWindow, SIGNAL( showNotifier( QString, QString, uint ) ), this, SLOT( slotNotifyMessage( QString, QString, uint ) ) );
        return ;
      }
    }
  }
  KMerlinChat *chatWindow = new KMerlinChat;
  chatWindow->show();
  if( calledHandle == handle )
  {
    calledHandle = "";
    chatWindow->show();
  }
  else
    chatWindow->showMinimized(); // ### ??
  chatWindow->setBaseContact( handle );
  KMerlinNotify::conversationMessage( this, handle, chatWindow->ID );
  connect( this, SIGNAL( statusChanged( const QString&, const QString& ) ), chatWindow, SLOT( statusReceived( const QString&, const QString& ) ) );
  connect( chatWindow, SIGNAL( message( QString, QString ) ), board, SLOT( slotSendMsg( QString, QString ) ) );
  connect( chatWindow, SIGNAL( closeSession() ), board, SLOT( slotCloseSession() ) );
  connect( chatWindow, SIGNAL( inviteContact( QString ) ), board, SLOT( slotInviteContact( QString ) ) );
  connect( chatWindow, SIGNAL( typingMsg() ), board, SLOT( slotTypingMsg() ) );
  connect( chatWindow, SIGNAL( createSession( QString ) ), imService, SLOT( slotStartChatSession( QString ) ) );
  connect( board, SIGNAL( msgReceived( QString, QString, QString ) ), chatWindow, SLOT( messageReceived( QString, QString, QString ) ) );
  connect( board, SIGNAL( userTypingMsg( QString ) ), chatWindow, SLOT( contactTyping( QString ) ) );
  connect( board, SIGNAL( msgAcknowledgement( bool ) ), chatWindow, SLOT( messageReceived( bool ) ) );
  connect( board, SIGNAL( switchBoardIsActive( bool ) ), chatWindow, SLOT( setActive( bool ) ) );
  connect( board, SIGNAL( contactReceived( const QString&, const QString&, bool ) ), chatWindow, SLOT( contactReceived( const QString&, const QString&, bool ) ) );
  connect( chatWindow, SIGNAL( showNotifier( QString, QString, uint ) ), this, SLOT( slotNotifyMessage( QString, QString, uint ) ) );
  widgetList.append( chatWindow );
}
/**/
void KMerlin::setStatus()
{

  imService->setStatus( sender() ->name() );
  ( ( KToggleAction* ) sender() ) ->setChecked( false );
}
/**/
void KMerlin::openHotmail()
{
  if ( isConnected() )
  {
    imService->slotOpenInbox();
  }
}
/**/
void KMerlin::statusReceived( const QString & status )
{
  panel->setStatus( status );
  if ( workTimer != 0L )
  {
    killTimer( workTimer );
    workTimer = 0L;
  }
  if ( currentStatus != 0L )
  {
    currentStatus->setChecked( false );
  }
  if ( status == "IDL" )
  {
    setCaption( i18n( "Away" ) + " - " + imService->getNickName() );
  }
  else if ( status == "NLN" )
  {
    currentStatus = statusNLN;
    currentStatus->setChecked( true );
    setCaption( i18n( "Online" ) + " - " + imService->getNickName() );
  }
  else if ( status == "BSY" )
  {
    currentStatus = statusBSY;
    currentStatus->setChecked( true );
    setCaption( i18n( "Busy" ) + " - " + imService->getNickName() );
  }
  else if ( status == "BRB" )
  {
    currentStatus = statusBRB;
    currentStatus->setChecked( true );
    setCaption( i18n( "Be right back" ) + imService->getNickName() );
  }
  else if ( status == "AWY" )
  {
    currentStatus = statusAWY;
    currentStatus->setChecked( true );
    setCaption( i18n( "Away" ) + " - " + imService->getNickName() );
  }
  else if ( status == "PHN" )
  {
    currentStatus = statusPHN;
    currentStatus->setChecked( true );
    setCaption( i18n( "On the phone" ) + " - " + imService->getNickName() );
  }
  else if ( status == "LUN" )
  {
    currentStatus = statusLUN;
    currentStatus->setChecked( true );
    setCaption( i18n( "Out to lunch" ) + " - " + imService->getNickName() );
  }
  else if ( status == "HDN" )
  {
    currentStatus = statusHDN;
    currentStatus->setChecked( true );
    setCaption( i18n( "Invisible" ) + " - " + imService->getNickName() );
  }
}
void KMerlin::statusReceived( const QString & handle, const QString & status )
{
  emit statusChanged( handle, status );
  if ( status == "NLN" )
  {
    KMerlinNotify::onlineMessage( this, handle );

  }
  else if ( status == "FLN" )
  {
    KMerlinNotify::offlineMessage( this, handle );
  }
}
/**/
void KMerlin::toggleEventView()
{
  if ( viewEvent->isChecked() )
    dw->show();
  else
    dw->hide();
}
/**/
void KMerlin::toggleView()
{
  if ( ( KToggleAction* ) sender() == viewOnline )
  {
    viewOnline->setChecked( true );
    viewGroup->setChecked( false );
    m_view->updateView( imService->isConnected(), 2 );
  }
  else
  {
    viewOnline->setChecked( false );
    viewGroup->setChecked( true );
    m_view->updateView( imService->isConnected(), 1 );
  }
}
void KMerlin::toggleApp()
{
  if ( isHidden() )
  {
    visible = true;
    setShown( true );
  }
  else
  {
    visible = false;
    hide();
  }
}
void KMerlin::addToPanel()
{
  if ( actionToPanel->isChecked() )
    panel->show();
  else
    panel->hide();
}

void KMerlin::setIdleTimer()
{
  if ( maxIdle != 0 )
    idleTimer->setTimeout( maxIdle );
  else
    idleTimer->stop();
}

void KMerlin::setIdle()
{
  if ( imService->m_status == "NLN" )
  {
    // away from computer
    changeStatusbar( i18n( "System is idle, set status to Away!" ) );
    imService->setStatus( "IDL" );
  }
}

void KMerlin::setActive()
{
  if ( imService->m_status == "IDL" )
  {
    changeStatusbar( i18n( "Set status to Online!" ) );
    imService->setStatus( "NLN" );
  }
}

//-----------------------------------------------------------------------------

void KMerlin::emailReceived( const QString &sender, int count, const QString &subject )
{
  if ( sender.isEmpty() )
  { // initial message count
    fileMail->setText( i18n( "You have %1 unread e-mails" ).arg( count ) );
  }
  else
  {
    fileMail->setText( i18n( "You have %1 unread e-mails" ).arg( count ) );
    KMerlinNotify::emailMessage( this, subject, sender, "" );
    panel->setEmail( sender + " " + subject );
  }
}
//---------------------------------------------------------------------------------------
void KMerlin::editNick()
{
  GroupDlg dialog( this );
  dialog.setCaption( i18n( "Change Nick Name" ) );
  dialog.m_caption->setText( i18n( "<QT><h3>Change nick name</h3></QT>" ) );
  dialog.m_group->setText( "" );
  dialog.m_text->setText( imService->getNickName() );
  dialog.m_info->setText( i18n( "Type the new nick name" ) );
  if ( dialog.exec() == QDialog::Accepted && !dialog.m_text->text().isEmpty() )
  {
    imService->changePublicName( dialog.m_text->text() );
  }
}
void KMerlin::editGroups()
{
  // this needs a nice dialog, something like msn
}
void KMerlin::editContacts()
{}
void KMerlin::nickChanged( const QString &nick )
{
  KMerlinProfile * profile = getProfile( imService->getHandle(), true );
  profile->setPublicName( nick );
  profile->save();
  emit profileChanged();
}
void KMerlin::slotConnectTimeout()
{
  if ( isConnected() )
    return ;
  else
    imService->killSocket();
}
void KMerlin::slotAutoConnectTimeout()
{
  if ( isConnected() )
    return ;
  if ( autoConnect && lastProfile == defaultProfile && !stopAutoConnect )
  {
    emit signalConnected( true ); // for class login, disables connect button
    emit signalConnect();
  }
}
void KMerlin::slotActivateChat( unsigned int ID )
{
  KMerlinChat * chatWindow;
  for ( chatWindow = widgetList.first(); chatWindow != 0; chatWindow = widgetList.next() )
  {
    if ( chatWindow->ID == ID )
    {
      chatWindow->setShown( true );
      chatWindow->show();
      return ;
    }
  }
}
void KMerlin::addContactDlg()
{
  GroupDlg * dlg = new GroupDlg( 0, i18n( "Add Contact" ) );
  dlg->m_caption->setText( "<h3>" + i18n( "Add Contact" ) + " !</h3>" );
  dlg->m_group->setText( "" );
  dlg->m_info->setText( "<b>" + i18n( "Type in the new contact email" ) + " !</b>" );
  connect( dlg->btnOk, SIGNAL( clicked() ), this, SLOT( addContact() ) );
  dlg->show();
}
void KMerlin::addContact( )
{
  GroupDlg * dlg = ( GroupDlg* ) sender() ->parent();
  if ( !dlg->m_text->text().isEmpty() && dlg->m_text->text().contains( "@" ) )
  {
    imService->addContact( dlg->m_text->text(), "0" );
  }
  delete dlg;
}
void KMerlin::addGroupDlg()
{
  GroupDlg * dlg = new GroupDlg( 0, i18n( "Add Group" ) );
  dlg->m_caption->setText( "<h3>" + i18n( "Add Group" ) + " !</h3>" );
  dlg->m_group->setText( "" );
  dlg->m_info->setText( "<b>" + i18n( "Type in the new group name" ) + " !</b>" );
  connect( dlg->btnOk, SIGNAL( clicked() ), this, SLOT( addGroup() ) );
  dlg->show();
}
void KMerlin::addGroup()
{
  GroupDlg * dlg = ( GroupDlg* ) sender() ->parent();
  if ( !dlg->m_text->text().isEmpty() && dlg->m_text->text() != dlg->m_group->text() )
  {
    imService->addGroup( dlg->m_text->text() );
  }
  delete dlg;
}

void KMerlin::encode( QString &text )
{
  if ( text.isEmpty() )
    return ;
  QCString curl = text.utf8();
  int oldlen = curl.length();
  const QCString special( "<>#@\"&%$:,;?={}|^~[]\'`\\ \n\t\r" );
  QString newUrl;
  int newlen = 0;

  for ( int i = 0; i < oldlen ;++i )
  {
    uchar inCh = ( uchar ) curl[ i ];

    if ( inCh >= 128 || special.contains( inCh ) )
    { // inCh >=1 for complete encode (for filterd words)
      newUrl[ newlen++ ] = QChar( '%' );

      ushort c = inCh / 16;
      c += c > 9 ? 'A' - 10 : '0';
      newUrl[ newlen++ ] = c;

      c = inCh % 16;
      c += c > 9 ? 'A' - 10 : '0';
      newUrl[ newlen++ ] = c;
    }
    else
    {
      newUrl[ newlen++ ] = inCh;
    }
  }
  text = newUrl;
}

uchar KMerlin::hex_to_int( uchar c )
{
  if ( c >= 'A' && c <= 'F' )
    return c - 'A' + 10;
  if ( c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if ( c >= '0' && c <= '9')
    return c - '0';
  return 0;
}

/*!
    Decodes the string \a url \e in-place.
 
    \sa encode()
*/

void KMerlin::decode( QString& text )
{
  if ( text.isEmpty() )
    return;

  int newlen = 0;
  QCString curl = text.utf8();
  int oldlen = curl.length();

  QCString newUrl(oldlen);

  int i = 0;
  while ( i < oldlen )
  {
    uchar c = (uchar)curl[ i++ ];
    if ( c == '%' )
    {
      c = hex_to_int( (uchar)curl[ i ] ) * 16 + hex_to_int( (uchar)curl[ i + 1 ] );
      i += 2;
    }
    newUrl [ newlen++ ] = c;
  }
  newUrl.truncate( newlen );

  text = QString::fromUtf8(newUrl.data());
}
#include "kmerlin.moc"
