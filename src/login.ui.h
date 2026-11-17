/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qpixmap.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kstddirs.h>
#include "kmerlin.h"
#include "kmerlinprofile.h"

QPixmap Login::getBgPixmap()
{
    KStandardDirs dir;
    return QPixmap(dir.findResource("data","kmerlin/pics/top.png") );
}


void Login::slotConnect()
{
    slotUpdateAll( true );
    if( m_login->text().isEmpty() && m_password->text().isEmpty() )
    {
	KMerlin::getInstance()->connectToMsn( m_profile->currentText() );
	return;
    }
    else
    {
	if( m_save->isChecked() )
	{
	    KMerlinProfile *profile = new KMerlinProfile( m_login->text(), m_nick->text(), m_password->text() );
	    profile->save();
			KMerlin::getInstance()->profileList.append( profile );
	    KMerlin::getInstance()->connectToMsn( m_nick->text() );
	    return;
	}
	else
	{
	    KMerlin::getInstance()->connectToMsn( m_login->text(),  m_nick->text(), m_password->text() );
	    return;
	}
    }
}


void Login::init()
{
    KMerlin *theApp = KMerlin::getInstance();
    connect( theApp, SIGNAL( profileChanged() ), this, SLOT( updateCombo()));
    connect( theApp, SIGNAL( signalConnected( bool ) ), this, SLOT( slotUpdateAll( bool ) ) );
     KStandardDirs dir;
    for( KMerlinProfile *profile = theApp->profileList.first(); profile != 0; profile = theApp->profileList.next() )
    {
	m_profile->insertItem( QPixmap(dir.findResource("data","kmerlin/pics/tux.png") ), profile->getNick() );
    }
	if( theApp->profileList.isEmpty() )
		btnConnect->setEnabled( false );
}


void Login::slotDataChanged(const QString&)
{
    if( m_login->text().isEmpty() && m_nick->text().isEmpty() && m_password->text().isEmpty() ){
	if( m_profile->count() != 0 )
	    btnConnect->setEnabled( true);
	else
	    btnConnect->setEnabled( false );
    }
    else  if( !m_login->text().isEmpty() && !m_nick->text().isEmpty() && !m_password->text().isEmpty() && m_login->text().contains("@") ) {
	btnConnect->setEnabled( true);
    }
    else
	btnConnect->setEnabled( false );
}


void Login::slotActivated( int )
{
    m_login->setText("");
    m_nick->setText("");
    m_password->setText("");
    btnConnect->setEnabled(true);
}


void Login::updateCombo()
{
     KMerlin *theApp = KMerlin::getInstance();
     KStandardDirs dir;
     m_profile->clear();
    for( KMerlinProfile *profile = theApp->profileList.first(); profile != 0; profile = theApp->profileList.next() )
    {
	m_profile->insertItem( QPixmap(dir.findResource("data","kmerlin/pics/tux.png") ), profile->getNick() );
    }
}


void Login::slotUpdateAll( bool f )
{
    btnConnect->setEnabled( !f );
    m_login->setEnabled( !f );
    m_nick->setEnabled( !f );
    m_password->setEnabled( !f );
    m_profile->setEnabled( !f );
}
