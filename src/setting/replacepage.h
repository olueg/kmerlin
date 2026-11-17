/****************************************************************************
** Form interface generated from reading ui file './replacepage.ui'
**
** Created: Sam Jan 24 20:24:57 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef REPLACEPAGE_H
#define REPLACEPAGE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class KPushButton;
class QLineEdit;
class QListView;
class QListViewItem;

class ReplacePage : public QWidget
{
    Q_OBJECT

public:
    ReplacePage( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ReplacePage();

    QLineEdit* newTxt;
    QLineEdit* orgTxt;
    KPushButton* KPushButton1;
    QListView* List;
    KPushButton* btnRemove;

public slots:
    virtual void orgTxt_textChanged( const QString & text );
    virtual void newTxt_textChanged( const QString & text );
    virtual void KPushButton1_pressed();
    virtual void List_selectionChanged( QListViewItem * item );

protected:
    QGridLayout* ReplacePageLayout;
    QGridLayout* Layout1;
    QGridLayout* Layout2;

protected slots:
    virtual void languageChange();
};

#endif // REPLACEPAGE_H
