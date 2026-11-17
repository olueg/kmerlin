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

#include "kmcontactaction.h"
#include <kdebug.h>
#include <qtoolbutton.h>
#include <qtoolbar.h>
#include <qlayout.h>
#include <qscrollview.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qobjectlist.h>
#include <qapplication.h>
#include <qwidgetlist.h>
#include <qstylefactory.h>
#include <qtooltip.h>
#include <qsimplerichtext.h>

static QStyle *windowsStyle = 0;

class KMActionButton : public QToolButton
  {
  public:
    KMActionButton( QWidget *parent, const char *name ) :
        QToolButton( parent, name ), selected( FALSE )
    {
      setBackgroundMode( PaletteLight );
    }

    void setSelected( bool b )
    {
      selected = b;
      update();
    }
    friend class KMContactAction;

  protected:
    QIconSet _iconSet;
    void drawButton( QPainter * );
    void setToolTip( const QString &tip );
	QSimpleRichText *doc;
	QString parsedNick;

  private:
    bool selected;
  };
/**/
void KMActionButton::setToolTip( const QString &tip )
{
  QToolTip::add
    ( this, tip );
}
void KMActionButton::drawButton( QPainter *p )
{/*
      if ( selected ) {
  	QFont f( p->font() );
  	f.setBold( TRUE );
  	p->setFont( f );
      }
   
      int d = 20 + height() - 3;
   
      QPointArray a( 7 );
      a.setPoint( 0, -1, height() + 1 );
      a.setPoint( 1, -1, 1 );
      a.setPoint( 2, width() - d, 1 );
      a.setPoint( 3, width() - 20, height() - 2 );
      a.setPoint( 4, width() - 1, height() - 2 );
      a.setPoint( 5, width() - 1, height() + 1 );
      a.setPoint( 6, -1, height() + 1 );
   
   
      if ( selected )
  	p->setBrush( colorGroup().light() );
      else
  	p->setBrush( colorGroup().brush( QColorGroup::Background ) );
   
      p->setPen( colorGroup().mid().dark( 150 ) );
      p->drawPolygon( a );
      p->setPen( colorGroup().light() );
      p->drawLine( 0, 2, width() - d, 2 );
      p->drawLine( width() - d - 1, 2, width() - 21, height() - 1 );
      p->drawLine( width() - 20, height() - 1, width(), height() - 1 );
      p->setBrush( NoBrush );
   
      p->setPen( colorGroup().buttonText() );
      if ( p->fontMetrics().width( text() ) < width() - d - 5 ) {
  	p->drawText( 2, 2, width(), height() - 2, AlignVCenter | AlignLeft, text() );
      } else {
  	QString s = text().left( 1 );
  	int ew = p->fontMetrics().width( "..." );
  	int i = 1;
  	while ( p->fontMetrics().width( s ) + ew + p->fontMetrics().width( text()[i] )  < width() - d - 5 )
  	    s += text()[i++];
  	s += "...";
  	p->drawText( 2, 2, width(), height() - 2, AlignVCenter | AlignLeft, s );
      }*/
	  doc = new QSimpleRichText( parsedNick ,p->font() );
	doc->setWidth(width() );
  const QPixmap &pic =  _iconSet.pixmap();
  int x = 2;
  if( !pic.isNull() )
    x = pic.width() +4;
  QStyle::SCFlags controls = QStyle::SC_ToolButton;
  QStyle::SCFlags active = QStyle::SC_None;

  if (isDown())
    active |= QStyle::SC_ToolButton;
  QStyle::SFlags flags = QStyle::Style_Default;
  if (isEnabled())
    flags |= QStyle::Style_Enabled;
  if (hasFocus())
    flags |= QStyle::Style_HasFocus;
  if (isDown())
    flags |= QStyle::Style_Down;
  if (isOn())
    flags |= QStyle::Style_On;
  if (autoRaise())
    {
      flags |= QStyle::Style_AutoRaise;
      if (uses3D())
        {
          flags |= QStyle::Style_MouseOver;
          if (! isOn() && ! isDown())
            flags |= QStyle::Style_Raised;
        }
    }
  else if (! isOn() && ! isDown())
    flags |= QStyle::Style_Raised;

  style().drawComplexControl(QStyle::CC_ToolButton, p, this, rect(), colorGroup(),
                             flags, controls, active,
                             QStyleOption());
  p->setPen( colorGroup().buttonText() );
  QRect r;
  r.setRect( 0, 0, width(), height() );
  if ( p->fontMetrics().width( text() ) < width() - x - 15 )
    {
      if( !pic.isNull() ) 
        p->drawPixmap(0,2, pic);
		doc->draw(p, x + r.x(), r.y(), r, colorGroup(), 0 );
      //p->drawText( x, 2, width(), height() - 2, AlignVCenter | AlignLeft, text() );
    }
  else
    {
      QString s = text().left( 1 );
      int ew = p->fontMetrics().width( "..." );
      int i = 1;
      while ( p->fontMetrics().width( s ) + ew + p->fontMetrics().width( text()[i] )  < width() - x - 15 )
        s += text()[i++];
      s += "...";
      if( !pic.isNull() )
        p->drawPixmap(0,2,pic );
		doc->draw(p, x + r.x(), r.y(), r, colorGroup(), 0 );
      //p->drawText( x, 2, width(), height() - 2, AlignVCenter | AlignLeft, s );
    }
	delete doc;
}

/**/
KMContactAction::KMContactAction(QWidget *parent, const char *name)
    : QWidget(parent, name)
{
  QVBoxLayout *layout = new QVBoxLayout( this );
  action = new KMActionButton( this, name );
  connect( action, SIGNAL( clicked() ), this, SIGNAL( activated() ) );
  _text = name ;
  action->setAutoRaise( true );
  if ( !windowsStyle )
    windowsStyle = QStyleFactory::create("Windows");;
  action->setStyle( windowsStyle );
  action->setText( name );
  layout->addWidget( action );
	kdDebug() << "Item created : " << name << " in: " << this->parent()->name() << endl;
}

/**/
KMContactAction::~KMContactAction()
{
	kdDebug() << "Item removed : " << name() << " from: " << parent()->name() << endl;
}

/**/
void KMContactAction::setIconSet( const QIconSet &icon )
{
  _iconSet = icon;
  action->_iconSet = _iconSet ;
  action->update();
}

/**/
const QIconSet& KMContactAction::iconSet()
{
  return _iconSet;
}
/**/
void KMContactAction::setText( const QString &text )
{
  action->setText( text );
  _text = text;
  update();
}
/**/
const QString& KMContactAction::text()
{
  return _text; // allways return the complete text,
}
/**/
void KMContactAction::setToolTip( const QString &tip )
{
  _toolTip = tip;
  action->setToolTip( _toolTip );
}
/**/
const QString& KMContactAction::toolTip()
{
  return _toolTip;
}
/**/
void KMContactAction::setStatusTip( const QString& )
{
}
/**/
const QString& KMContactAction::statusTip()
{
  return "";
}

void KMContactAction::setSort( const QString &sort )
{
	_sort = sort ;
}

void KMContactAction::setParsedNick( QString nick )
{
	action->parsedNick = nick;
}
