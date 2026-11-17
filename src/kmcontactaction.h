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

#ifndef _KMCONTACTACTION_H_
#define _KMCONTACTACTION_H_

#include <qwidget.h>
#include <qiconset.h>

class KMActionButton;

class KMContactAction : public QWidget
{
	Q_OBJECT

public:
	KMContactAction( QWidget *parent, const char *name );
	int operator==(KMContactAction &ac){ return ac._sort == _sort; }
	int operator<(KMContactAction &ac){ return ac._sort > _sort; } // QLayout inserts the last at first
	~KMContactAction();
	KMActionButton *action;

private:  // members
	QString _text;
	QString _toolTip;
	QString _statusTip;
	QIconSet _iconSet;
	QString _sort;

public:  //functions
	void setIconSet( const QIconSet &icon );
	const QIconSet& iconSet();
	const QString& text();
	const QString& toolTip();
	const QString& statusTip();
	void setSort( const QString &sort );
	void setParsedNick( QString nick );
public slots:
	void setText( const QString &text );
	void setToolTip( const QString &tip );
	void setStatusTip( const QString& );
signals:
	void activated();
};

#endif
