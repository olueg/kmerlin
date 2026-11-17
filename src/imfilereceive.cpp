/***************************************************************************
                          imfilereceive.cpp  - file recieve class
                             -------------------
    begin                : Friday 4 Oct 2002
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


#include "imfilereceive.h"
#include <qsocket.h>
#include <kfiledialog.h>
#include <time.h>
#include <kdebug.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
//Timeout in seconds
#define TRANSFER_TIMEOUT 30

AsyncReceiver::AsyncReceiver(FILE *outFile, int inSocket, int fileSize)
{
  this->outFile = outFile;
  this->inSocket = inSocket;
  this->fileSize = fileSize;
}

void AsyncReceiver::run()
{
  unsigned int gotData = 0, waitData = 0;  
  unsigned char *buffer = new unsigned char[4];
  int lastReceived = time(NULL);    
  //Read data from socket
  while (gotData < fileSize)
    {
      //Check for a timeout
      if ((time(NULL) - lastReceived) > TRANSFER_TIMEOUT)
	{
	  printf("Timeout on file receive");
	  break;
	}
      if (waitData == 0)
	{	  
	  //We have a header
	  if (read(inSocket, buffer, 3) == 0) 
	    {
	      printf("Read error on socket\n");
	      break;
	    }
	  //Check validity
	  if (buffer[0] != 0)
	    {
	      printf("Data Stream Malformed\n");
	      break;
	    }
	  waitData = buffer[1] + buffer[2]*256;	  
	  lastReceived = time(NULL);
	}
      else if (waitData > 0)
	{	  
	  char *transBuffer  = new char[waitData+1];
	  //We have data	  
	  unsigned int gotDataPacket = read(inSocket, transBuffer, waitData);
	  //Put it in the file	  
	  fwrite(transBuffer, gotDataPacket, 1, outFile);
	  //Calculate how much data we still need to wait for
	  gotData += gotDataPacket;
	  waitData -= gotDataPacket;
	  lastReceived = time(NULL);
	  delete[] transBuffer;
	}
    }
  printf("Done\n");
  write(inSocket, "BYE 16777989\r\n", 15); 
  delete[] buffer;
  fclose(outFile);
  close(inSocket);
}
  

IMFileReceive::IMFileReceive()
{
  
}

//Read a line from a socket with timeout
int IMFileReceive::socketReadLine(int inSocket, char *buffer, int bufSize)
{
  int status, i = 0, start = time(NULL);
  buffer[0] = 0;
  
  do
    { 
      if ((time(NULL) - start) > TRANSFER_TIMEOUT) 
	{
	  debug("socketReadLine transfer timeout\n");
	  return -1;
	}
      status = read(inSocket, &(buffer[i]), 1);
      if (status == 1)
	{
	  i++;	
	  if ((buffer[i-1] == '\n') || (i == bufSize))
	    return i;
	}
      if ((status == -1) && (errno == EAGAIN))
	status = 0;	
    }
  while (status != -1);
  printf("socketReadLine failed: %d\n", errno);
  return -1;
}
      

/* Start and run the file transfer */
void IMFileReceive::run(QString fileName, QString address, QString port, QString cookie, QString passport)
{  
  debug("File Transfer started");
  QString saveName = KFileDialog::getSaveFileName("~/"+fileName, "*.*");
  if (saveName == QString::null)
    return;
  debug("Save Name: " + saveName);
  //Make a socket
  unsigned short portNumber = port.toUShort();
  recvSocket = new KSocket(address.latin1(), portNumber);
  //Check we are connected
  if (recvSocket->socket() <= -1)
    {
      debug("Error Connecting to " + address);
      delete recvSocket;
      return;
    }
  //Connected
  debug("Connected");
  
  //Do the handshake  
  char result[2048];  
  int outSocket = recvSocket->socket();
  
  int len = sprintf(result, "VER MSNFTP\r\n");
  write(outSocket, result, len);  
  socketReadLine(outSocket, result, 2048);
  printf("VEResult %s\n", result);
  if (strstr(result, "VER") == NULL)
    {
      printf("Wrong response from server: %s (expected VER)\n", result);
      disconnect();
      delete recvSocket;
      return;
    }

  len = snprintf(result, 2048, "USR %s %s\r\n", passport.latin1(), cookie.latin1());
  printf("%s", result);
  write(outSocket, result, len);
  socketReadLine(outSocket, result, 2048);
  
  if (strstr(result, "USR") != NULL)
    socketReadLine(outSocket, result, 2048);
  if (strstr(result, "FIL") == NULL)
    {
      printf("Wrong response from server: %s (expected FIL)\n",result);
      disconnect();
      delete recvSocket;
      return;
    }

  //Get the filesize
  int fileSize;
  QString qresult(result);
  QString fSize = qresult.right(qresult.length() - qresult.find("FIL")-4);
  fileSize = fSize.left(fSize.find("\r\n")).toInt();
  //Send the TFR
  len = sprintf(result, "TFR\r\n");
  write(outSocket, result, len);
  debug("Handshake Done. Got Filesize "+fSize);
  
  //Open the output file
  FILE *outFile = fopen(saveName.latin1(), "w");
  if (outFile == NULL)
    {
      printf("Unable to Open %s.", saveName.latin1());
      return;
    }
  AsyncReceiver receiver(outFile, recvSocket->socket(), fileSize);
  debug("Forking");
  receiver.start();
  receiver.wait(1000);
  debug("Forked");
}

void IMFileReceive::disconnect()
{
  write(recvSocket->socket(), "BYE 16777989\r\n", 15); 
}

