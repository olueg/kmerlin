/**********************************************************************
** Copyright (C) 2002 Olaf Lueg.  All rights reserved.
** Copyright (C) 2002 KMerlin Developer Team.  All rights reserved.
**
** This file is part of KMerlin.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://kmerlin.olsd.com/gpl/ for GPL licensing information.
**
** Contact olueg@olsd.de if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef _KMERLINCHATVIEW_H_
#define _KMERLINCHATVIEW_H_

#include <qwidget.h>

class KTextBrowser;
class KPushButton;
class KTextEdit;
class KMerlin;

class KMerlinChatView : public QWidget
{
  Q_OBJECT

public:
  KMerlinChatView(QWidget *parent );
  ~KMerlinChatView();
  
  bool textChanged;
  
  bool CtrlDown();
  void sendText();
  void messageReceived( QString publicName, QString message, QString format );
  void systemMessage( QString message );

private: //Members
  KTextBrowser *m_browser;
  KTextEdit *m_input;
  KPushButton *m_button;
  KMerlin *theApp;
  bool CTRL_Key;
  bool m_strikeout;
  bool m_bold;
  bool m_underline;
  bool m_italic;
  bool displayTime;
  bool displaySmiley;
  bool bold, strikeout, italic, underline;
  
private: // functions
  QString htmlParser( QString message );
  QString urlParser( QString message, QString tag );
  void textReplace( QString text, int para, int index );
  void keyReleaseEvent( QKeyEvent *event );
  void keyPressEvent( QKeyEvent *event );
  QString getFormat( );
  QString parseSmiley( QString text );
  QString parseMessageHeader( QString format );
  QString createMessageFooter();
  
private slots:
  void slotTextChanged();
  void buttonClicked();
  
public slots:
  void pasteSmiley( QString smiley );
  void toggleTime();
  void toggleSmiley();
  void toggleBold();
  void toggleItalic();
  void toggleUnderline();
  void toggleStrikeout();
  void fileSave();
  void copy();
  void paste();
  void cut();
signals:
  void sendMsg( QString, QString );
  void writeMessage();
};

#endif
