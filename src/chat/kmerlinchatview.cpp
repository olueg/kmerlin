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

#include "kmerlinchatview.h"
#include "kmerlinchat.h"
#include "../kmtextreplace.h"
#include "../kmemotionicon.h"
#include "../kmerlin.h"

#include <kdebug.h>
#include <klocale.h>
#include <kglobal.h>
#include <kpushbutton.h>
#include <ktextbrowser.h>
#include <ktextedit.h>
#include <kmessagebox.h>
#include <kaction.h>
#include <kstddirs.h>
#include <kstringhandler.h>
#include <kfiledialog.h>

#include <qlayout.h>
#include <qsize.h>
#include <qregexp.h>
#include <qnamespace.h>
#include <qdatetime.h>

/**/
KMerlinChatView::KMerlinChatView(QWidget *parent)
    : QWidget(parent, "chat")
{
  QGridLayout *l = new QGridLayout(this, 1, 1, 0, 0, "Layout");
  m_button = new KPushButton( i18n( "Send" ),this );
  m_button->setMaximumSize( QSize( 50, 50 ) );
  l->addWidget( m_button, 1,1 );
  m_input = new KTextEdit( this );
  m_input->setMaximumSize( QSize( 32767, 50 ) );
  m_input->setReadOnly( false );
  l->addWidget( m_input, 1, 0 );
  m_browser = new KTextBrowser( this );
  m_browser->setTextFormat( KTextBrowser::AutoText );
  l->addMultiCellWidget(m_browser, 0, 0, 0, 1);
  m_input->installEventFilter( (KMerlinChat*)parent );
  theApp = KMerlin::getInstance();
  connect( m_input, SIGNAL( textChanged() ), this, SLOT( slotTextChanged() ) );
  connect( m_button, SIGNAL( clicked() ), this, SLOT( buttonClicked() ) );
  CTRL_Key = false;
  displaySmiley = true;
  displayTime = false;
  m_button->setEnabled( false );
  textChanged = false;
  m_bold = false;
  m_strikeout = false;
  m_italic = false;
  m_underline = false;
}
/**/
KMerlinChatView::~KMerlinChatView()
{}
/**/
void KMerlinChatView::pasteSmiley( QString smiley )
{
  m_input->insert( smiley );
}

/**/
QString KMerlinChatView::htmlParser(QString message)
{
  message = message.replace(QRegExp("<"),"&lt;");
  message = message.replace(QRegExp(">"),"&gt;");
  return message;
}
/**/
QString KMerlinChatView::urlParser(QString message,QString tag )
{
  // if viewSmileys=true  urlParser works not correctly
  QChar c;
  c=message.at(0);
  while (c == '\n' || c == '\r' || c == '\0')
    {
      message.remove(0,1);  //removes prefixing \n \r and \0 so this function can work
      c=message.at(0);
    }

  int i = message.find(tag);

redo:
  QString str= "";
  if(i != -1)
    {
      if(i == 0)
        {
          message.insert(i,"<a href=\"");
          i = 1;
        }
      else
        {
          if(QString(message.at(i-1)) == " ")
            {
              message.insert(i,"<a href=\"");
              i = i+1;

            }
          else
            {
              kdDebug() << " no valid url" << endl;
              goto nodata;
            }
        }
      i = i+8;

      do
        {
          str += message.at(i);
          i++;
        }
      while((c= message.at(i)) != " " & i != message.length());
      message.insert(i,"\">" +str +"</a>");
    }
nodata:
  if( i != -1)
    {

      i = message.find(tag,i+str.length()+1);
      if(i != -1)
        {
          goto redo;
        }
    }
  return message;
}
/**/
void KMerlinChatView::textReplace(QString text, int para, int index)
{
  // replace the text, some inspiration from x-chat
  KMTextReplace *replace;
  QString str,st;
  for( replace = theApp->replaceList.first();replace !=0 ;replace = theApp->replaceList.next() )
    {
      if( text.find(replace->orgTxt) == 0 )
        {
          if( text == replace->orgTxt)// for example : u is replaced with you
            {
              m_input->setSelection ( para, index-text.length()-1, para, index-1, 1 ); // select the text and replace it
              m_input->removeSelectedText(1);
              m_input->insertAt(replace->newTxt,para,index-text.length()-1);
              m_input->removeSelection(1);
              m_input->setCursorPosition(para,index - text.length() + replace->newTxt.length() );
            }
          if((QChar)text.at(replace->orgTxt.length() ) == "'")// example: u'r is replaced with your , u''r -> you'r
            {
              QString st = text.right(text.length() - replace->orgTxt.length() -1);
              str = replace->newTxt;
              str += st;
              m_input->setSelection ( para, index-text.length()-1, para, index-1, 1 );
              m_input->removeSelectedText(1);
              m_input->insertAt(str,para,index-text.length()-1);
              m_input->removeSelection(1);
              m_input->setCursorPosition(para,index - text.length() + str.length() );
            }
        }
    }
}
/**/
void KMerlinChatView::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    {
      CTRL_Key = true;
    }
}
/**/
void KMerlinChatView::keyReleaseEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    {
      CTRL_Key = false;
    }
}
/**/
bool KMerlinChatView::CtrlDown()
{
  return CTRL_Key;
}
/**/
void KMerlinChatView::sendText()
{
  if( m_input->text().isEmpty() ) return;
  if(CTRL_Key)
    {
      //insert a new line at the current pos,
      m_input->insert("\n");
      return;
    }
  QString text = m_input->text();
  if(!text.isEmpty())
    {
      if(text.length() >400)
        {
          KMessageBox::error(0,"Your message is too long!\nYou canot use more than 400 characters.");
          return;
        }
      else
        {
          for(int i =0; i < text.length(); i++)
            {
              if(text.at(i) == '\n' && text.at(i+1) == '\n')
                {
                  text.replace(i,1," ");
                }
            }
          text = text.replace( QRegExp( "\n" ), "\r\n" );
          emit sendMsg( text, getFormat() );
          m_input->setText("");
          textChanged = false;
        }
    }
}
/**/
QString KMerlinChatView::getFormat()
{
  QString format, ef,co, fontName;
  format = "FN=" + theApp->fontName.replace(QRegExp(" "),"%20") +";";
  if(m_bold)
    ef = "B";
  if(m_strikeout)
    ef += "S";
  if(m_underline)
    ef += "U";
  if(m_italic)
    ef += "I";
  format += " EF=" + ef + ";";
  co = theApp->fontColor.name();
  co = co.right(co.length() -1);
  format += " CO=" + co +";" ;
  format += " CS=0; PF=22";
  fontName = theApp->fontName.replace(QRegExp("%20")," ") ;
  return format;
}
/**/
void KMerlinChatView::messageReceived( QString publicName, QString message, QString format )
{
  QString strTime;
  if(displayTime)
    {
      strTime=KGlobal::locale()->formatTime( QTime::currentTime(), false );
      strTime = QString( "[ %1 ]").arg(strTime) ;
    }
		if( ( (KMerlinChat* )parent() )->logging() )
    {
      QTextStream stream( &( (KMerlinChat* )parent() )->logFile );
      stream << strTime << i18n("%1 says:").arg( publicName ) << " " << message<< "\n";
      ( (KMerlinChat* )parent() )->logFile.flush();
    } 
	message = htmlParser(message);
  message = urlParser(message,"http:");
  message = urlParser(message,"mailto:");
  message = urlParser(message,"ftp:");
  publicName = publicName.replace(QRegExp("<"),"&lt;");
  publicName = publicName.replace(QRegExp(">"),"&gt;");
  if(theApp->enableEmotions) // enable emotion icons in publicName
    {
      publicName = parseSmiley(publicName);
    }

  if(displaySmiley)
    {
      message = parseSmiley(message);
    }


  message = message.replace( QRegExp("\r\n"),"<br>");
  message = strTime + parseMessageHeader( format ) + " " + i18n( "%1 says:").arg( publicName ) +"<br>" + message + createMessageFooter() ;
  m_browser->append(message );
  m_browser->scrollToBottom();
}
/**/
void KMerlinChatView::systemMessage( QString message )
{
  if( ( (KMerlinChat* )parent() )->logging() )
    {
      QTextStream stream( &( (KMerlinChat* )parent() )->logFile );
      stream << "-----------------------------------------\n" ;
      stream << i18n("System message:") << " " << message<< "\n";
      stream << "-----------------------------------------\n" ;
      ( (KMerlinChat* )parent() )->logFile.flush();
    }
  KStandardDirs dir;
  QString str = QString("<table><tr><td witdh=\"120\"><img src=\"%1\"></td><td><p color=red size=+1>%2</p></td></tr></table>").arg( dir.findResource( "data", "kmerlin/pics/user.png" ) ).arg( message );
  m_browser->append(message );
  m_browser->scrollToBottom();
}  
/**/
QString KMerlinChatView::parseSmiley( QString text )
{
  KMEmotionIcon *icon;
  QString t = text;
  
  if( !text.isEmpty() ) // no text - no parsing
    {
      for( icon = theApp->emotionList.first(); icon != 0; icon = theApp->emotionList.next() )
        {
          if( t.left(icon->getDisplay().length()+1 ) == icon->getDisplay()+" "  ) // start with a emo
            {
              t = t.right( t.length() - icon->getDisplay().length() );
              t.prepend( "<img src=\""+icon->getFilePath()+"\"height=\"16\" width=\"16\" align=\"middle\">&nbsp;" );
            }
          if( t.right(icon->getDisplay().length()+1 ) == " " + icon->getDisplay()  ) // end with a emo
            {
              t = t.left( t.length() - icon->getDisplay().length() );
              t = t +  "<img src=\""+icon->getFilePath()+"\"height=\"16\" width=\"16\" align=\"middle\">" ;
            }
          int i = 0;
          if( (i = t.contains( icon->getDisplay() ) ) > 0  )
            {
              t= t.replace(QRegExp(icon->getRegExp() )," <img src=\""+ icon->getFilePath() + "\"height=\"16\" width=\"16\" align=\"middle\">&nbsp;");
            }
        }
    }
  return t;
}
/**/
void KMerlinChatView::toggleTime()
{
  KToggleAction* action = (KToggleAction*)sender();
  displayTime = action->isChecked();
}
/**/
void KMerlinChatView::toggleSmiley()
{
  KToggleAction* action = (KToggleAction*)sender();
  displaySmiley = action->isChecked();
}
/**/
void KMerlinChatView::toggleBold()
{
  KToggleAction* action = (KToggleAction*)sender();
  m_bold = action->isChecked();
}
/**/
void KMerlinChatView::toggleItalic()
{
  KToggleAction* action = (KToggleAction*)sender();
  m_italic = action->isChecked();
}
/**/
void KMerlinChatView::toggleStrikeout()
{
  KToggleAction* action = (KToggleAction*)sender();
  m_strikeout = action->isChecked();
}
/**/
void KMerlinChatView::toggleUnderline()
{
  KToggleAction* action = (KToggleAction*)sender();
  m_underline = action->isChecked();
}
/**/
void KMerlinChatView::slotTextChanged()
{
  // << new implementation for text replacement, this search in the current paragraph for a complete typed word. 
  // A word ends witch a whitespace
  int para;
  int index;
  m_input->getCursorPosition(&para,&index);
  QString str = m_input->text(para);
  if( index >= 2)
    {
      if( (QChar)str.at(index -1 ) == " ")
        {
          QString s,s1;
          bool b= true;
          uint i =1;
          while(b)
            {
              i++;
              s1 = str.at(index -i) ;
              if(s1 == " " || i == str.length())
                {
                  if( i == str.length()-1 && str.left(1) != " " )
                    {
                      s1 = str.left(1);
                      s = s1 +s;
                    }
                  b = false; // s contains the complete word
                }
              else
                {
                  s = s1 +s;
                }
            }
          textReplace(s,para,index); // search for text replacement
        }
    }
  if( m_input->text().isEmpty() || m_input->text() == "\n" )
    {
      m_button->setEnabled(false);
      textChanged = false;
    }
  else
    {
      m_button->setEnabled(true);
      textChanged = true;
    }
  if( m_input->text().length() == 1) // typing has start
    {
      emit writeMessage();
    }
}
/**/
void KMerlinChatView::buttonClicked()
{
  sendText();
}
QString KMerlinChatView::parseMessageHeader(QString format)
{
  KStringHandler kstr;
  bold = strikeout = italic = underline = false;
  QString header,font,color,effects;
  if( KMerlin::getInstance()->globalFont )
    {
      font = KMerlin::getInstance()->fontName;
    }
  else
    {
      font = kstr.word(format,1);
      font = font.right(font.length() -3);
      font = font.left(font.length() -1);
      font = font.replace( QRegExp( "%20" ), " " );
    }
  color = kstr.word(format,3);
  color = color.right(color.length() -3);
  color = color.left(color.length() -1);
  color = "#" +color;

  header = "<font face=\"" + font +"\" color=\"" + color + "\" >";
  effects = kstr.word(format,2);
  if(effects.contains("b",false) )
    {
      bold = true;
      header += "<b>";
    }
  if(effects.contains("i",false ))
    {
      italic = true;
      header += "<i>";
    }
  if(effects.contains("s",false) )
    {
      strikeout = true;
      header += "<strike>";
    }
  if(effects.contains("u",false) )
    {
      underline = true;
      header += "<u>";
    }
  return header;
}
/**/
QString KMerlinChatView::createMessageFooter()
{
  QString footer;
  if(bold) footer += "</B>";
  if(strikeout) footer += "</strike>";
  if(underline) footer += "</u>";
  if(italic) footer += "</i>";
  return footer;
}
/**/
void KMerlinChatView::fileSave()
{
  QString saveFileName = KFileDialog::getSaveFileName( QString::null , //No start directory
                         "*.txt" , //No filter
                         this ,   //Where the dialog should be centered on
                         i18n( "Save conversation" ) ); //Title
  if ( saveFileName == QString::null )
    return;
  QFile f( saveFileName );
  if (f.open( IO_WriteOnly ) )
    {
      QTextStream t( &f );
      t << m_browser->text();
      f.close();
    }
  else
    KMessageBox::error( 0 , //No parent
                        i18n("Save failed") , //Short!
                        i18n("Error while saving") ); //Caption
}
/**/
void KMerlinChatView::copy()
{
  if( m_browser->hasFocus())
  {
    if( m_browser->hasSelectedText())
    {
      m_browser->copy();
    }
  }
  if( m_input->hasFocus())
  {
    m_input->copy();
  }
}
/**/
void KMerlinChatView::paste()
{
  if( m_input->hasFocus())
  {
    m_input->paste();
  }  
}
/**/
void KMerlinChatView::cut()
{
  if( m_input->hasFocus())
  {
    m_input->cut();
  }
}

