/****************************************************************************
** Form interface generated from reading ui file './identityeditdialog.ui'
**
** Created: Sam Jan 24 20:24:58 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef IDENTITYEDITDIALOG_H
#define IDENTITYEDITDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;

class IdentityEditDialog : public QDialog
{
    Q_OBJECT

public:
    IdentityEditDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~IdentityEditDialog();

    QTabWidget* tabWidget6;
    QWidget* tab;
    QLabel* textLabel1;
    QLineEdit* m_login;
    QLineEdit* m_password;
    QLabel* textLabel2;
    QLineEdit* m_nick;
    QLabel* textLabel3;
    QPushButton* pushButton15;
    QPushButton* pushButton14;

protected:
    QGridLayout* IdentityEditDialogLayout;
    QGridLayout* tabLayout;

protected slots:
    virtual void languageChange();
private:
    QPixmap image0;

};

#endif // IDENTITYEDITDIALOG_H
