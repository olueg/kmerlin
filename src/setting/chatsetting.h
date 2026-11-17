/****************************************************************************
** Form interface generated from reading ui file './chatsetting.ui'
**
** Created: Sam Jan 24 20:24:58 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CHATSETTING_H
#define CHATSETTING_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class KColorButton;
class KFontCombo;
class QCheckBox;
class QLabel;
class QLineEdit;

class ChatSetting : public QWidget
{
    Q_OBJECT

public:
    ChatSetting( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ChatSetting();

    KColorButton* m_color;
    KFontCombo* m_font;
    QCheckBox* m_defaultFont;
    QLabel* textLabel4;
    QLabel* textLabel3;
    QLineEdit* m_logFile;
    QLabel* PixmapLabel1;
    QLabel* textLabel6;
    QCheckBox* m_emotions;
    QCheckBox* m_logging;

    virtual void init();
    virtual void destroy();
    void save();

protected:
    QGridLayout* ChatSettingLayout;
    QGridLayout* layout2;
    QGridLayout* layout3;

protected slots:
    virtual void languageChange();
private:
    QPixmap image0;

};

#endif // CHATSETTING_H
