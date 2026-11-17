/****************************************************************************
** Form interface generated from reading ui file './connectwidget.ui'
**
** Created: Sam Jan 24 20:28:42 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class ConnectWidget : public QWidget
{
    Q_OBJECT

public:
    ConnectWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ConnectWidget();


protected:

protected slots:
    virtual void languageChange();
};

#endif // CONNECTWIDGET_H
