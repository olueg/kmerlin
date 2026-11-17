
#include "CContactWidget.h"
#include "kmcontactaction.h"

#include <kdebug.h>

#include <qlayout.h>


CContactWidget::CContactWidget(QWidget *parent, const char *name)
  : QWidget(parent, name)
{
	layout = new QVBoxLayout( this );
	actionList.setAutoDelete( false );
	
}


CContactWidget::~CContactWidget()
{
	if( actionList.count() != 0 )
		actionList.clear();
}

void CContactWidget::addWidget( KMContactAction *action )
{
	actionList.append( action );
	if( actionList.count() >= 2 ){
		kdDebug() << "Sorting contacts" << endl;
		actionList.sort();
		kdDebug() << "finished" << endl;
	}
	delete layout;
	layout = new QVBoxLayout( this );
	for( action = actionList.first(); action != 0; action = actionList.next() ){
		layout->addWidget( action );
		action->show();
	}
	QSpacerItem* spacer = new QSpacerItem( 20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout->addItem( spacer );
	update();
	repaint();
}

void CContactWidget::removeWidget( KMContactAction *action )
{
	if( actionList.count() != 0 )
	{
		int pos = actionList.find( action );
		if( pos != -1 ){
			actionList.remove( pos );
			if( actionList.count() >= 2)
				actionList.sort();
		}
	}
	delete layout;
	layout = new QVBoxLayout( this );
	for( action = actionList.first(); action != 0; action = actionList.next() ){
		layout->addWidget( action );
		action->show();
	}
	QSpacerItem* spacer = new QSpacerItem( 20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout->addItem( spacer );
	update();
	repaint();
}

void CContactWidget::sort()
{
	if( actionList.count() >= 2 )
		actionList.sort();
	delete layout ;
	layout = new QVBoxLayout( this );
	for( KMContactAction *action = actionList.first(); action != 0; action = actionList.next() ){
		layout->addWidget( action );
		action->show();
	}
	QSpacerItem* spacer = new QSpacerItem( 20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding );
	layout->addItem( spacer );
	update();
	repaint();
}

void CContactWidget::clear()
{
	if( actionList.count() != 0 )
		actionList.clear();
}
bool CContactWidget::isEmpty()
{
	return actionList.isEmpty();
}

