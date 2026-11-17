/**********************************************************************
** Copyright (C) 2002 Olaf Lueg.  All rights reserved.
** Copyright (C) 2002 KMerlin Developers Team.  All rights reserved.
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <qwidget.h>
#include <kdialogbase.h>

class KMerlin;
class ChatSetting;
class ReplacePage;
class IdentitySetting;
class SystemNotification;

class SettingsDialog : public KDialogBase
{
	Q_OBJECT
public:
	SettingsDialog( QWidget *parent = 0, const char *name = 0 );
	~SettingsDialog();
	KMerlin *theApp;

	void init();
private:
	IdentitySetting *identitySetting;
	SystemNotification *systemNotifications;
	ChatSetting *chatSetting;
	ReplacePage *replacePage;


	QStringList replaceList;
	QStringList deletedReplace;
	void readConfig();
	void saveConfig();

private slots:
	void slotOk();
	void slotCancel();
	void slotRemoveReplaceText();

signals:
	void closed();
	void profileChanged();
};

#endif

