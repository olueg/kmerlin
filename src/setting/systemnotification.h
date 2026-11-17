/****************************************************************************
** Form interface generated from reading ui file './systemnotification.ui'
**
** Created: Sam Jan 24 20:24:58 2004
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.1.1   edited Nov 21 17:40 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SYSTEMNOTIFICATION_H
#define SYSTEMNOTIFICATION_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class KListView;
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListViewItem;
class QPushButton;
class QSpinBox;

class SystemNotification : public QWidget
{
    Q_OBJECT

public:
    SystemNotification( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SystemNotification();

    KListView* m_List;
    QGroupBox* groupBox1;
    QPushButton* btnPlay;
    QCheckBox* m_sound;
    QCheckBox* m_app;
    QCheckBox* m_event;
    QCheckBox* m_popup;
    QCheckBox* m_file;
    QPushButton* btnSound;
    QPushButton* btnLog;
    QPushButton* btnApp;
    QLineEdit* m_soundFile;
    QLineEdit* m_fileName;
    QLineEdit* m_appFile;
    QCheckBox* m_animate;
    QLabel* textLabel1;
    QSpinBox* m_timeout;
    QLabel* textLabel2;

    virtual void save();

public slots:
    virtual void selectionChanged( QListViewItem * item );
    virtual void playSound();
    virtual void openSound();
    virtual void openLog();
    virtual void openApp();
    virtual void soundChanged( const QString & );

protected:
    QListViewItem *currentItem;
    QPixmap p_event;
    QPixmap p_file;
    QPixmap p_popup;
    QPixmap p_sound;
    QListViewItem *messageItem;
    QString m_message_sound_file;
    bool m_message_sound;
    bool m_message_popup;
    bool m_message_event;
    bool m_message_file;
    QListViewItem *emailItem;
    QString m_email_sound_file;
    bool m_email_sound;
    bool m_email_popup;
    bool m_email_file;
    bool m_email_event;
    bool m_online_sound;
    bool m_online_event;
    bool m_online_popup;
    bool m_online_file;
    QString m_online_sound_file;
    QListViewItem *onlineItem;
    bool m_offline_sound;
    bool m_offline_event;
    bool m_offline_popup;
    bool m_offline_file;
    QString m_offline_sound_file;
    QListViewItem *offlineItem;
    bool m_conversation_event;
    bool m_conversation_file;
    bool m_conversation_popup;
    bool m_conversation_sound;
    QString m_conversation_sound_file;
    QListViewItem *conversationItem;
    QPixmap p_exec;

    virtual void init();

    QGridLayout* SystemNotificationLayout;
    QGridLayout* groupBox1Layout;
    QHBoxLayout* layout1;

protected slots:
    virtual void languageChange();

    virtual void checkerToggled( bool f );

};

#endif // SYSTEMNOTIFICATION_H
