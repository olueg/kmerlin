/****************************************************************************
** Form interface generated from reading ui file './groupdlg.ui'
**
** Created: Sam Jan 24 20:28:43 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef GROUPDLG_H
#define GROUPDLG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class GroupDlg : public QDialog
{
    Q_OBJECT

public:
    GroupDlg( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~GroupDlg();

    QLabel* m_info;
    QLabel* m_group;
    QLabel* m_caption;
    QLineEdit* m_text;
    QPushButton* btnOk;

    virtual QPixmap getBgPixmap();

protected:

protected slots:
    virtual void languageChange();
};

#endif // GROUPDLG_H
