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
#ifndef _KMCONTACT_H_
#define _KMCONTACT_H_

#include <qobject.h>
#include <qlist.h>
#include <qstringlist.h>
#include <qiconset.h>

class KMContactAction;
class KPopupMenu;
class KToolBar;
class KMGroup;
class KMerlin;
class KAction;
class QPixmap;

class KMContact : public QObject
{
	Q_OBJECT
public:
	KMContact();
	KMContact( QString handle, QString msnNick );
	~KMContact();
public:
	void init( );
	void setHandle( QString handle )
	{
		m_handle = handle;
	}
	void setMsnNick( QString nick )
	{
		m_msnNick = nick;
	}
	void setNick( QString nick )
	{
		m_nick = nick;
	}

	QString getHandle()
	{
		return m_handle;
	}
	QString getMsnNick()
	{
		return m_msnNick ;
	}
	QString getNick();
	void cleanUp();

public:
	/*!
	* changes the current nick name, if update is true the contact actions will be updated, too.
	* If no status changes follows after this, set update to true.
	*/
	void setNickName( const QString &nick, bool update );
	/*!
	* changes the status
	*/
	void setStatus( const QString &status );
	/*!
	* @param nick  The Nick name
	* @param group The Grouplist
	*/
	void setForward( QString nick, QString group );
	/*! returns the full status text
	*/
	QString getFullStatus();
	void addToGroup( QString group );
	
	void setAllow( bool f ) { _allowed = f; }
	void setBlocked( bool f );
	void setReverse( bool f ) { _reverse = f ;}
	bool isReverse() { return _reverse; }
	bool isForward() { return _forward; }
	void removed( const QString &handle, const QString &list, const QString &group );
	const QIconSet& iconSet( );
	QString nickName() { return m_msnNick; }
	bool available(); // return true if is not FLN or blocked
	QString getStatus( ) {return m_status; }
	QString getParsedNick(){ return parsedNick;}

private:
	QStringList groupList;
	QString m_handle;
	QString m_msnNick;  // comes from msn
	QString m_nick; // set by KMerlin
	QString m_status;
	QString _strStatus;
	QIconSet _iconSet;
	QString _toolTip;
	QString _statusTip;
	QPtrList<KMContactAction>actionList;
	QPtrList<KMGroup>_groupList;
	bool _forward, _reverse, _blocked, _allowed;
	KMerlin *theApp;
	QString sort;
	QString parsedNick;
	int picCount;

	// context menu
	KPopupMenu *contextMenu;
	KAction *chatAction;
	KAction *removeAction;
	KAction *blockAction;
	KAction *allowAction;
	KAction *infoAction;
	KAction *nickAction;
private: //functions
	void parseNick( QString nick );
	void updateActions();
public:
signals:
	void infoChanged( );
	void nickChanged( QString );
	void statusChanged( const QString& );
private slots:
	void displayMenu( );
	void sendMessage( );
	void allowContact();
	void blockContact();
	void removeContact();
	void changeNick();

};

#endif
