
#ifndef _CCONTACTWIDGET_H_
#define _CCONTACTWIDGET_H_

#include <qwidget.h>
#include <qsortedlist.h>

class QVBoxLayout;
class KMContactAction;

class CContactWidget : public QWidget
{
	Q_OBJECT

public:
	CContactWidget( QWidget *parent, const char *name );
	~CContactWidget();

	void addWidget( KMContactAction *action );
	void removeWidget( KMContactAction *action );
	void sort();
	void clear();
	bool isEmpty();
private:
	QVBoxLayout *layout;
	QSortedList<KMContactAction>actionList;

};

#endif
