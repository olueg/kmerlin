/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../kmerlin.h"
#include "../kmerlinprofile.h"
#include "identityeditdialog.h"

#include <klocale.h>
#include <qlineedit.h>

void IdentitySetting::slotSelectionChanged( )
{
		if( m_list->currentItem() == 0L ) return;
    btnModify->setEnabled( true);
    btnRename->setEnabled( true);
    btnRemove->setEnabled( true);
    if( m_list->currentItem()->text(1) == KMerlin::getInstance()->defaultProfile )
	btnDefault->setEnabled( false);
    else
	btnDefault->setEnabled( true );
}


void IdentitySetting::slotNew()
{
   // create a new account
    IdentityEditDialog dialog( this, i18n( "New Account" ) );
    if( dialog.exec() == QDialog::Accepted ){
	// do something
	KMerlinProfile *profile = new KMerlinProfile( dialog.m_login->text(), dialog.m_nick->text(), dialog.m_password->text() ); 
	profile->save();
	KMerlin::getInstance()->profileList.append( profile );
	new QListViewItem(m_list, profile->getNick() , profile->getHandle() );
	emit profileChanged();
    }
}

void IdentitySetting::slotModify()
{
    IdentityEditDialog dialog( this );
    KMerlinProfile *profile = KMerlin::getInstance()->getProfile( m_list->currentItem()->text( 1 ), true );
    if( profile == 0L){
	// error
#ifdef DEBUG
	debug( "IdentitySetting : Profile not found");
#endif
	return;
    }
    dialog.m_login->setText( profile->getHandle() );
    dialog.m_nick->setText( profile->getNick() );
    dialog.m_password->setText( profile->getPassword() );
    dialog.setCaption( i18n("Edit Identity %1").arg( profile->getHandle() ) );
    if( dialog.exec() == QDialog::Accepted ){
	// do something
	profile->updateData( dialog.m_login->text(), dialog.m_nick->text(), dialog.m_password->text() );  
	if( KMerlin::getInstance()->defaultProfile == dialog.m_login->text() )
	    m_list->currentItem()->setText(0, dialog.m_nick->text() +" (" + i18n( "Standard") +" )" );
	else
	    m_list->currentItem()->setText(0, dialog.m_nick->text() );
	m_list->currentItem()->setText(1, dialog.m_login->text() );
	emit profileChanged();
    }
}


void IdentitySetting::slotRename()
{
    IdentityEditDialog dialog( this );
    KMerlinProfile *profile = KMerlin::getInstance()->getProfile( m_list->currentItem()->text( 1 ), true );
    if( profile == 0L){
	// error
#ifdef DEBUG
	debug( "IdentitySetting : Profile not found");
#endif
	return;
    }
    dialog.m_login->setText( profile->getHandle() );
    dialog.m_nick->setText( profile->getNick() );
    dialog.m_password->setText( profile->getPassword() );
    dialog.setCaption( i18n("Edit Identity %1").arg( profile->getHandle() ) );
    if( dialog.exec() == QDialog::Accepted ){
	// do something
	profile->updateData( dialog.m_login->text(), dialog.m_nick->text(), dialog.m_password->text() );    
	emit profileChanged();
    }
}

void IdentitySetting::slotRemove()
{
    KMerlinProfile *profile = KMerlin::getInstance()->getProfile( m_list->currentItem()->text( 1 ), true );
    if( profile == 0L){
	// error
#ifdef DEBUG
	debug( "IdentitySetting : Profile not found");
#endif
	return;
    }
    else{
	profile->remove();
	KMerlin::getInstance()->profileList.remove( profile );
	delete profile;
	m_list->removeItem( m_list->currentItem() );
	btnModify->setEnabled( false );
	btnRename->setEnabled( false );
	btnRemove->setEnabled( false );
	btnDefault->setEnabled( false );
	emit profileChanged();
    }
}

void IdentitySetting::slotDefault()
{
    KMerlin::getInstance()->defaultProfile = m_list->currentItem()->text(1);
    m_list->clear();
    init();
}

void IdentitySetting::init()
{
    KMerlin *theApp = KMerlin::getInstance();
    for( KMerlinProfile *profile = theApp->profileList.first(); profile != 0; profile = theApp->profileList.next() )
    {
	if( theApp->defaultProfile == profile->getHandle() )
	    (void ) new QListViewItem(m_list, profile->getNick() + " " + i18n( "Standard"), profile->getHandle() );
	else
	    (void ) new QListViewItem(m_list, profile->getNick() , profile->getHandle() );
    }
    btnModify->setEnabled( false );
    btnRename->setEnabled( false );
    btnRemove->setEnabled( false );
    btnDefault->setEnabled( false );
    m_auto->setChecked( theApp->autoConnect );
}


void IdentitySetting::destroy()
{
    KMerlin::getInstance()->autoConnect = m_auto->isChecked();
}
