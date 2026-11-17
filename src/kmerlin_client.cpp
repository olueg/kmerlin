/*
 * Copyright (C) 2001 Olaf Lueg <olueg@olsd.de>
 */

#include <kapplication.h>
#include <dcopclient.h>
#include <qdatastream.h>
#include <qstring.h>

int main(int argc, char **argv)
{
    KApplication app(argc, argv, "kmerlin_client", false);

    // get our DCOP client and attach so that we may use it
    DCOPClient *client = app.dcopClient();
    client->attach();

    // do a 'send' for now
    QByteArray data;
    QDataStream ds(data, IO_WriteOnly);
    if (argc > 1)
        ds << QString(argv[1]);
    else
        ds << QString("http://www.kde.org");
    client->send("kmerlin", "KMerlinIface", "openURL(QString)", data);

    return app.exec();
}
