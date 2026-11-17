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

#include "kmerlinprofile.h"
#include "kmerlin.h"
#include "kmservice.h"

// kde
#include <kglobal.h>
#include <kconfig.h>
#include <kaction.h>
#include <kdebug.h>

/**/
KMerlinProfile::KMerlinProfile( QString handle, QString nick, const QString &passWord )
		: QObject()
{
	m_passWord = passWord;
	m_handle = handle;
	m_publicName = nick;
	m_emotion = false;
	m_default = false;
	KMerlin::getInstance() ->profiles.append( handle );
}
/**/
KMerlinProfile::KMerlinProfile()
		: QObject()
{
	m_passWord = "";
	m_handle = "";
	m_publicName = "";
	m_emotion = false;
	m_default = false;
}

/**/
KMerlinProfile::~KMerlinProfile()
{
}

/* This loads a profile from disk and return a pointer to it */
KMerlinProfile * KMerlinProfile::load( const QString &profileName )
{
	KConfig * config = KGlobal::config();
	config->setGroup( "General" );
	int v = config->readNumEntry( "Version", 0 );
	config->setGroup( "Profile_" + profileName );
	m_handle = config->readEntry( "name" , QString::null );
	if ( v < 1 )  // old profiles
	{
		m_passWord = config->readEntry( "password" , QString::null ) ;
	} else {
		m_passWord = cryptPassword( config->readEntry( "password" , QString::null ) );
	}
	m_publicName = config->readEntry( "publicname" , QString::null );
	if( m_publicName.isEmpty() )
		m_publicName = m_handle;
	return this;
}

/* save the profile to disk */
void KMerlinProfile::save()
{
	KConfig * config = KGlobal::config();
	config->setGroup( "Profile_" + m_handle );
	config->writeEntry( "name", m_handle );
	config->writeEntry( "publicname", m_publicName );
	config->writeEntry( "password", cryptPassword( m_passWord ) );
	if ( !KMerlin::getInstance() ->profiles.contains( m_handle ) )
		KMerlin::getInstance() ->profiles.append( m_handle );
	config->setGroup( "Profiles" );
	config->writeEntry( "Profiles", KMerlin::getInstance() ->profiles );
	config->sync();
}

/* removes a profile from the disk */
void KMerlinProfile::remove()
{
	KConfig * config = KGlobal::config();
	config->deleteGroup( "Profile_" + m_handle, true );
	KMerlin::getInstance() ->profiles.remove( m_handle );
	config->setGroup( "Profiles" );
	config->writeEntry( "Profiles", KMerlin::getInstance() ->profiles );
	config->sync();
}

/* simple pass crypt, loan from kinkatta */
QString KMerlinProfile::cryptPassword( const QString &plainPass )
{
	QString crypted;
	int len = plainPass.length();
	for ( int i = 0; i < len; i ++ ) {
		int temp = plainPass[ i ].latin1();
		if ( ( temp >= 'a' ) && ( temp <= 'm' ) )
			temp += 13;
		else if ( ( temp >= 'A' ) && ( temp <= 'M' ) )
			temp += 13;
		else if ( ( temp >= 'n' ) && ( temp <= 'z' ) )
			temp -= 13;
		else if ( ( temp >= 'N' ) && ( temp <= 'Z' ) )
			temp -= 13;
		else if ( ( temp >= '0' ) && ( temp <= '4' ) )
			temp += 5;
		else if ( ( temp >= '5' ) && ( temp <= '9' ) )
			temp -= 5;
		crypted += ( char ) temp;
	}
	return crypted;
}
/**/
void KMerlinProfile::convert( const QString &text )
{
	KConfig * config = KGlobal::config();
	config->deleteGroup( "Profile_" + text, true ); // delete the old profile;
	config->setGroup( "Profile_" + m_handle );
	config->writeEntry( "name", m_handle );
	config->writeEntry( "publicname", m_publicName );
	config->writeEntry( "password", cryptPassword( m_passWord ) );
	config->sync();
}

/**/
void KMerlinProfile::setPublicName( const QString &publicName )
{
	m_publicName = publicName;
}
/**/
void KMerlinProfile::setHandle( const QString &handle )
{
	m_handle = handle;
}

void KMerlinProfile::updateData( const QString &handle, const QString &nick, const QString &password )
{
	remove();
	m_handle = handle;
	m_publicName = nick;
	m_passWord = password;
	save();
}
