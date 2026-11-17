/************************************************************************************
                          kmemotionicon.cpp  -  description
                             -------------------
    begin                : Wed Aug 21 2002
    copyright            : (C) 2000 - 2002 by Olaf Lueg
    email                : olueg@olsd.de
 *************************************************************************************/

/**************************************************************************************
 *                                                                                                         *
 *   This program is free software; you can redistribute it and/or modify          * 
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or            *
 *   (at your option) any later version.                                                        *
 *                                                                                                         *
 **************************************************************************************/

#ifndef _KMEMOTIONICON_H_
#define _KMEMOTIONICON_H_

#include <qobject.h>


class KMEmotionIcon : public QObject
  {
    Q_OBJECT

public:
    KMEmotionIcon(QString regExp, QString display, QString fileName);
    ~KMEmotionIcon();
protected:
    QString m_RegExp;
    QString m_Display;
    QString m_FileName;
    QString m_FilePath;
public:
    const QString getFileName() ;
    const QString getDisplay() ;
    const QString getRegExp() ;
    void setRegExp(QString regExp);
    void setDisplay(QString display);
    void setFileName(QString fileName);
    void setFilePath(QString path);
    const QString getFilePath();
  };

#endif
