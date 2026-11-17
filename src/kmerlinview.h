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

#ifndef _KMERLINVIEW_H_
#define _KMERLINVIEW_H_

#include <qwidget.h>
#include <kparts/part.h>
#include "kmerliniface.h"
#include <qscrollview.h>

class QPainter;
class Login;
class KMerlin;


class KMerlinView : public QWidget, virtual public KMerlinIface
{
	Q_OBJECT
public:
	/**
	 * Default constructor
	 */
	KMerlinView( QWidget *parent );

	/**
	 * Destructor
	 */
	virtual ~KMerlinView();
	KMerlin *theApp;
	void initView();
	/**
	 * Print this view to any medium -- paper or not
	 */
	void print( QPainter *, int height, int width );
	void updateView( bool f, int layout );

private:  // Members
	QWidget *currentPage;

private:  // functions


signals:
	/**
	 * Use this signal to change the content of the statusbar
	 */
	void signalChangeStatusbar( const QString& text );

	/**
	 * Use this signal to change the content of the caption
	 */
	void signalChangeCaption( const QString& text );

private slots:
	void slotSetTitle( const QString& title );
public slots:
	void slotStatusMsg( const QString &msg );

private:
	Login *w;

};

#endif // _KMERLINVIEW_H_
