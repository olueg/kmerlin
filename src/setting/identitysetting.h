/****************************************************************************
** Form interface generated from reading ui file './identitysetting.ui'
**
** Created: Sam Jan 24 20:24:58 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IDENTITYSETTING_H
#define IDENTITYSETTING_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QSpinBox;

class IdentitySetting : public QWidget
{
    Q_OBJECT

public:
    IdentitySetting( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~IdentitySetting();

    QPushButton* btnNew;
    QPushButton* btnModify;
    QPushButton* btnRename;
    QPushButton* btnRemove;
    QPushButton* btnDefault;
    QListView* m_list;
    QCheckBox* m_auto;
    QLabel* textLabel1;
    QSpinBox* m_idle;
    QLabel* textLabel2;

    virtual void init();
    void destroy();

public slots:
    virtual void slotSelectionChanged();
    virtual void slotNew();
    virtual void slotModify();
    virtual void slotRename();
    virtual void slotRemove();
    virtual void slotDefault();

signals:
    void profileChanged();

protected:
    QGridLayout* IdentitySettingLayout;
    QVBoxLayout* layout4;
    QHBoxLayout* layout14;

protected slots:
    virtual void languageChange();
private:
    QPixmap image0;
    QPixmap image1;

};

#endif // IDENTITYSETTING_H
