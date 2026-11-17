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

#ifndef _KMGROUP_H_
#define _KMGROUP_H_

#include <qobject.h>
#include <qdict.h>
class KMerlin;
class KToolBar;
class CContactWidget;
class KPopupMenu;
class KAction;
class KMContactAction;
class KMContact;



class KMGroup : public QObject
{
	Q_OBJECT
public:
	KMGroup();
	KMGroup( QString name, QString id );
	friend class KMContact;
	~KMGroup();
	KToolBar *groupBar;
	KMerlin *theApp;
	QString toolId;
	QString toolLabel;
	CContactWidget *groupContainer;
	KPopupMenu *contextMenu;
	KAction *addAction;
	KAction *renameAction;
	KAction *insertAction;
	KAction *removeAction;
	KAction *addGAction;

	void rename( QString name );
	void remove();
private slots:
	void renameGroup();
	void addContact();
	void rename();
	void add();
	void addGroup();
	void newGroup();
	void removeGroup();
	void insertContact();

public slots:
	void slotInfoChanged();
protected:
	QDict<KMContactAction>actions;
};

#endif
