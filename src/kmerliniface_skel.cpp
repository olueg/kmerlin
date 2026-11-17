/****************************************************************************
**
** DCOP Skeleton created by dcopidl2cpp from kmerliniface.kidl
**
** WARNING! All changes made in this file will be lost!
**
*****************************************************************************/

#include "./kmerliniface.h"

#include <kdatastream.h>


static const char* const KMerlinIface_ftable[1][3] = {
    { 0, 0, 0 }
};

bool KMerlinIface::process(const QCString &fun, const QByteArray &data, QCString& replyType, QByteArray &replyData)
{
	return DCOPObject::process( fun, data, replyType, replyData );
}

QCStringList KMerlinIface::interfaces()
{
    QCStringList ifaces = DCOPObject::interfaces();
    ifaces += "KMerlinIface";
    return ifaces;
}

QCStringList KMerlinIface::functions()
{
    QCStringList funcs = DCOPObject::functions();
    for ( int i = 0; KMerlinIface_ftable[i][2]; i++ ) {
	QCString func = KMerlinIface_ftable[i][0];
	func += ' ';
	func += KMerlinIface_ftable[i][2];
	funcs << func;
    }
    return funcs;
}


