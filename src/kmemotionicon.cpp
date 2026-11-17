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

#include "kmemotionicon.h"

/* Constructs a new emotion icon */
KMEmotionIcon::KMEmotionIcon(QString regExp, QString display, QString fileName)
    : QObject()
{
  m_RegExp = regExp;
  m_Display = display;
  m_FileName = fileName;
}
/* Destructor */
KMEmotionIcon::~KMEmotionIcon()
{}

/* Return the fileName */
const QString KMEmotionIcon::getFileName()
{
  return m_FileName;
}

/* Return the regExp */
const QString KMEmotionIcon::getRegExp()
{
  return m_RegExp;
}

/* return the display */
const QString KMEmotionIcon::getDisplay()
{
  return m_Display;
}

/* set the fileName */
void KMEmotionIcon::setFileName(QString fileName)
{
  m_FileName = fileName;
}

/* set the RegExp */
void KMEmotionIcon::setRegExp(QString regExp)
{
  m_RegExp = regExp;
}

/* set the display */
void KMEmotionIcon::setDisplay(QString display)
{
  m_Display = display;
}

/* */
const QString KMEmotionIcon::getFilePath()
{
  return m_FilePath;
}

/**/
void KMEmotionIcon::setFilePath(QString path)
{
  m_FilePath = path;
}

#include "kmemotionicon.moc"

