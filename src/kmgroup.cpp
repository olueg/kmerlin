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

#include "kmerlin.h"
#include "kmservice.h"
#include "kmgroup.h"
#include "qcategorywidget.h"
#include "CContactWidget.h"
#include "groupdlg.h"
#include "kmcontactaction.h"
#include "kmcontact.h"

#include <ktoolbar.h>
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kpopupmenu.h>
#include <kaction.h>
#include <kmessagebox.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>


/**/
KMGroup::KMGroup()
		: QObject()
{}
/**/
KMGroup::KMGroup( QString name, QString id )
		: QObject()
{
	theApp = KMerlin::getInstance();
	contextMenu = new KPopupMenu( 0 );
	addAction = new KAction( i18n( "Add Contact" ), "edit", 0, this, SLOT( addContact( ) ) );
	insertAction = new KAction( i18n( "Insert Contact" ), "edit", 0, this, SLOT( insertContact( ) ) );
	insertAction->setEnabled( false );
	connect( addAction, SIGNAL( activated() ), this, SLOT( addContact() ) );
	connect( insertAction, SIGNAL( activated() ), this, SLOT( insertContact() ) );
	
	
	renameAction = new KAction( i18n( "Rename Group" ), "edit", 0, this, SLOT( renameGroup( ) ) );
	removeAction = new KAction( i18n( "Remove Group" ), "edit", 0, this, SLOT( removeGroup( ) ) );
	addGAction = new KAction( i18n( "Add Group" ), "edit", 0, this, SLOT( addGroup( ) ) );
	
	connect( renameAction, SIGNAL( activated() ), this, SLOT( renameGroup() ) );
	connect( removeAction, SIGNAL( activated() ), this, SLOT( removeGroup() ) );
	connect( addGAction, SIGNAL( activated() ), this, SLOT( addGroup() ) );
	
	addGAction->plug( contextMenu );
	renameAction->plug( contextMenu );
	removeAction->plug( contextMenu );
	contextMenu->insertSeparator();
	addAction->plug( contextMenu );
	insertAction->plug( contextMenu );
	
	groupBar = new KToolBar( theApp, theApp->groupWidget, false, name, false, false );
	groupBar->setEnableContextMenu( false );
	groupBar->setFrameStyle( QFrame::NoFrame );
	groupBar->setOrientation( Qt::Vertical );
	theApp->groupWidget->addCategory( name, groupBar, contextMenu, KGlobal::instance()->iconLoader()->loadIcon( "2downarrow" ,KIcon::NoGroup , KIcon::SizeSmall ), KGlobal::instance()->iconLoader()->loadIcon( "2leftarrow" ,KIcon::NoGroup , KIcon::SizeSmall )  );
	groupContainer = new CContactWidget( groupBar, "Group Container " + id );
	groupBar->insertWidget( 1, 1, groupContainer, -1 );
	toolLabel = name;
	toolId = id;
	theApp->groupList.append( this );
}
/**/
KMGroup::~KMGroup()
{
	groupContainer->clear();
	delete contextMenu;
}
/**/
void KMGroup::slotInfoChanged()
{
}
void KMGroup::rename( QString name )
{
	// rename the category
	toolLabel = name;
	KMerlin::getInstance()->groupWidget->setGroupLabel( name );
}
/**/
void KMGroup::rename( )
{
	GroupDlg *dlg = (GroupDlg*)sender()->parent();
	if( !dlg->m_text->text().isEmpty() && dlg->m_text->text() != dlg->m_group->text() )
	{
		KMerlin::getInstance()->imService->renameGroup( dlg->m_text->text(), toolId );
	}
	delete dlg;
}
/**/
void KMGroup::remove()
{
	KMerlin::getInstance()->groupWidget->removeCategory();
	theApp->groupList.remove( this );
}
void KMGroup::renameGroup()
{
	GroupDlg *dlg = new GroupDlg( 0, i18n( "Rename Group" ) );
	dlg->m_caption->setText( "<h3>" + i18n( "Rename Group" ) + " !</h3>" );
	dlg->m_group->setText( toolLabel );
	dlg->m_info->setText( "<b>" + i18n("Type in the new group name") +" !</b>" );
	connect( dlg->btnOk, SIGNAL( clicked() ), this, SLOT( rename() ) );
	dlg->show();
}
void KMGroup::addContact()
{
	GroupDlg *dlg = new GroupDlg( 0, i18n( "Add Contact" ) );
	dlg->m_caption->setText( "<h3>" + i18n( "Add Contact" ) + " !</h3>" );
	dlg->m_group->setText( toolLabel );
	dlg->m_info->setText( "<b>" + i18n("Type in the new contact email") +" !</b>" );
	connect( dlg->btnOk, SIGNAL( clicked() ), this, SLOT( add() ) );
	dlg->show();
}
void KMGroup::add( )
{
	GroupDlg *dlg = (GroupDlg*)sender()->parent();
	if( !dlg->m_text->text().isEmpty() && dlg->m_text->text().contains("@") )
	{
		KMerlin::getInstance()->imService->addContact( dlg->m_text->text(), toolId );
	}
	delete dlg;
}
void KMGroup::addGroup()
{
	GroupDlg *dlg = new GroupDlg( 0, i18n( "Add Group" ) );
	dlg->m_caption->setText( "<h3>" + i18n( "Add Group" ) + " !</h3>" );
	dlg->m_group->setText( toolLabel );
	dlg->m_info->setText( "<b>" + i18n("Type in the new group name") +" !</b>" );
	connect( dlg->btnOk, SIGNAL( clicked() ), this, SLOT( newGroup() ) );
	dlg->show();
}
void KMGroup::newGroup()
{
	GroupDlg *dlg = (GroupDlg*)sender()->parent();
	if( !dlg->m_text->text().isEmpty() && dlg->m_text->text() != dlg->m_group->text() )
	{
		KMerlin::getInstance()->imService->addGroup( dlg->m_text->text() );
	}
	delete dlg;
}
void KMGroup::removeGroup()
{
	if( groupContainer->isEmpty() )
		KMerlin::getInstance()->imService->removeGroup( toolId );
	else
		KMessageBox::error( 0, i18n("Group not empty"));
	
}
void KMGroup::insertContact()
{
}
