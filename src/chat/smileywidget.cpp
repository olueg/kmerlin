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

#include "smileywidget.h"
#include "../kmerlin.h"
#include "../kmemotionicon.h"

#include <qtoolbutton.h>
#include <qlayout.h>

#include <kdebug.h>
#include <qstylefactory.h>

static QStyle *windowsStyle = 0;

/**/
SmileyWidget::SmileyWidget(QWidget *parent, const char *name)
    : QWidget(parent, name)
{
  if ( !windowsStyle )
    windowsStyle = QStyleFactory::create("Platinum");
  KMerlin *theApp = KMerlin::getInstance();
  KMEmotionIcon *icon;
  QToolButton *button;
  icon = theApp->emotionList.first();
  QGridLayout *grid = new QGridLayout(this, 11,4,0,4);
  for( int i =0; i < 11; i++)
    {
      for( int j = 0; j < 4; j++ )
        {
          button = new QToolButton( this, icon->getDisplay() );
          button->setTextLabel( icon->getDisplay() );
          button->setIconSet( QPixmap( icon->getFilePath() ) );
          button->setMaximumSize( QSize( 24, 24 ) );
          button->setStyle( windowsStyle );
          connect( button, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
          grid->addWidget( button, i, j );
          icon = theApp->emotionList.next();
        }
    }
}
/**/
SmileyWidget::~SmileyWidget()
{}
/**/
void SmileyWidget::slotClicked()
{
  emit signalClicked( sender()->name() );
}
