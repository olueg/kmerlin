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

#ifndef _KMERLINDOCK_H_
#define _KMERLINDOCK_H_

#include <qwidget.h>
#include <ksystemtray.h>
#include <qpixmap.h>


class KMerlinDock : public KSystemTray
{
  Q_OBJECT

public:
  KMerlinDock(QWidget *parent, const char *name);
  ~KMerlinDock();
public:
  void setConnecting();
  void setConnected( bool ok );
  void setEmail( const QString &text );
  void setWorking( );
  void setChat();
  void setDefault();
	void setStatus( const QString &status );
private:
  int status;
  int timer;
  bool mInverted;
  int mStep;
  QPixmap mDefaultIcon;
  QPixmap mStatusIcon;
public:
  KPopupMenu *menu;
private:
  void mousePressEvent(QMouseEvent *e);
  void timerEvent( QTimerEvent *e );
  void switchIcon( const QString &strIcon );
public:
  signals:
  void toggleApp();;
};

#endif
