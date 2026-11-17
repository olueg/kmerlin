/****************************************************************************
** Form interface generated from reading ui file './login.ui'
**
** Created: Sam Jan 24 20:28:42 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef LOGIN_H
#define LOGIN_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QComboBox;
class QFrame;
class QLabel;
class QLineEdit;
class QPushButton;

class Login : public QWidget
{
    Q_OBJECT

public:
    Login( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~Login();

    QFrame* m_frame;
    QLabel* textLabel1;
    QComboBox* m_profile;
    QLineEdit* m_login;
    QLineEdit* m_password;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLabel* textLabel3_2;
    QLineEdit* m_nick;
    QCheckBox* m_save;
    QPushButton* btnConnect;

    virtual QPixmap getBgPixmap();
    virtual void init();

public slots:
    virtual void slotConnect();
    virtual void slotDataChanged( const QString & );
    virtual void slotActivated( int );
    void updateCombo();
    void slotUpdateAll( bool f );

protected:
    QGridLayout* LoginLayout;

protected slots:
    virtual void languageChange();
};

#endif // LOGIN_H
