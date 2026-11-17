/****************************************************************************
** Form interface generated from reading ui file './authdlg.ui'
**
** Created: Sam Jan 24 20:28:42 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef AUTHDLG_H
#define AUTHDLG_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QFrame;
class QLabel;
class QPushButton;

class AuthDlg : public QWidget
{
    Q_OBJECT

public:
    AuthDlg( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~AuthDlg();

    QFrame* frame5;
    QLabel* textLabel4;
    QPushButton* btnOK;
    QCheckBox* m_block;
    QCheckBox* m_allow;
    QLabel* m_handle;

    virtual QPixmap getBgPixmap();

protected:

protected slots:
    virtual void languageChange();
};

#endif // AUTHDLG_H
