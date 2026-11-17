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

#ifndef _KMERLINPROFILE_H_
#define _KMERLINPROFILE_H_

#include <qobject.h>

class KAction;

class KMerlinProfile : public QObject
{
Q_OBJECT
public:
  KMerlinProfile();  
  KMerlinProfile(QString handle , QString nick, const QString &passWord );
  ~KMerlinProfile();
  
private:
  QString m_handle;
  QString m_publicName;
  QString m_passWord;
  bool m_default;
  bool m_emotion;
  bool m_savePassWord;
  QString c_nick;
    
public:
  void setPublicName(const QString &publicName) ;
  QString getNick(){ return m_publicName;}
  QString getPassword(){ return m_passWord;}
  void updateData( const QString &handle, const QString &nick, const QString &password );
   void setHandle(const QString &handle);
  void setPassWord(const QString &pwd) { m_passWord = pwd; }
  void savePassWord( bool f) { m_savePassWord = f; }
  const QString& getPublicName() {return m_publicName;}
  const QString& getHandle() { return m_handle; }
  const QString& getPassWord() { return m_passWord; }
  bool savePassWord() { return m_savePassWord; }
  const QString extraNick() { return c_nick;}
  void setExtraNick( const QString &str){ c_nick = str ;}
  void setEnableEmotion( bool f ){ m_emotion = f; }
  bool enableEmotion(){ return m_emotion; }
  
  KMerlinProfile* load(const QString &profileName);
  void save();
  void remove();
  void convert( const QString &text);
  QString cryptPassword(const QString &plainPass);

public:
signals:
  void signalConnectAs( );   
};

#endif
