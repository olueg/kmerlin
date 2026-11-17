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

#ifndef _SMILEYWIDGET_H_
#define _SMILEYWIDGET_H_

#include <qwidget.h>

class SmileyWidget : public QWidget
{
  Q_OBJECT

public:
  SmileyWidget(QWidget *parent, const char *name);
  ~SmileyWidget();
  
public slots:
  void slotClicked();

signals:
  void signalClicked( QString );
};

#endif
