/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include "../kmerlin.h"
#include <kglobal.h>

void ChatSetting::init()
{
    KMerlin *theApp = KMerlin::getInstance();
    m_defaultFont->setChecked( theApp->globalFont );
    m_emotions->setChecked( theApp->enableEmotions );
    m_logging->setChecked( theApp->enableLogging );
    m_logFile->setText( theApp->m_logFile );
    m_color->setColor( theApp->fontColor );
    for ( int j = 0; j < m_font->count(); j++ ) {
	if ( m_font->text( j ) == theApp->fontName ) {
	    m_font->setCurrentItem( j );
	}
    }
}

void ChatSetting::destroy()
{
   
}


void ChatSetting::save()
{
    debug( "Save chat Setting" );
    KMerlin *theApp = KMerlin::getInstance();
    theApp->globalFont = m_defaultFont->isChecked();
    theApp->enableEmotions = m_emotions->isChecked();
    theApp->enableLogging = m_logging->isChecked();
    theApp->m_logFile = m_logFile->text();
    theApp->fontColor = m_color->color();
    theApp->fontName = m_font->currentText();
}
