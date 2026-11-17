/***************************************************************************
                          imfilereceive.h - File Receive class
                             -------------------
    begin                : Friday 4 October 2002
    copyright            : (C) 2002 by Marc Brooker
    email                : mbrooker@smuts.uct.ac.za
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef IM_FILERECEIVE
#define IM_FILERECEIVE

#include <qobject.h>
#include <kstringhandler.h>
#include <qstrlist.h>
#include <qthread.h>
#include <ksock.h>


class AsyncReceiver: public QThread
{
 public:
  AsyncReceiver(FILE *outFile, int inSocket, int fileSize);
  virtual void run();
 private:
  FILE *outFile;
  int inSocket;
  int fileSize;
};

class IMFileReceive: public QObject
{
 public:
  IMFileReceive();
  void run(QString fileName, QString address, QString port, QString cookie, QString passport);
 private:
  int socketReadLine(int inSocket, char *buffer, int bufsize);
  
  bool receiveData(QString fileName, int fileSize);
  void disconnect();
  KSocket *recvSocket;
  
};

#endif
