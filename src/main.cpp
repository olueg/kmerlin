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

#include "kmerlin.h"
#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <config.h>

static const char *description =
  I18N_NOOP("A MSN Messenger");

static const char *version = VERSION;

static KCmdLineOptions options[] =
  {
    { "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    { 0, 0, 0 }
  };

int main(int argc, char **argv)
{
  KAboutData about("kmerlin", I18N_NOOP("KMerlin"), version, description,
                   KAboutData::License_GPL, "(C) 2001 - 2004 Olaf Lueg", 0, 0, "olueg@olsd.de");
  about.addAuthor( "Olaf Lueg", 0, "olueg@olsd.de" );
  about.addAuthor("Niels Reedijk",0,"n.reedijk@planet.nl");
  about.addCredit("Michiel Brendel",I18N_NOOP("Custom font and color selection in chat window"),"michiel@brendel.cx");
  about.addCredit("Mauritz",I18N_NOOP("Clearer KNotify messages"));
  about.addCredit("Mark Westcott",I18N_NOOP("Made it possible to receive URL's"),"mark@houseoffisch.org");
  about.addCredit("Henrik Johnson",I18N_NOOP("KDE3 porting"),"hpj@users.sourceforge.net");
  about.addCredit("César Martínez Izquierdo",I18N_NOOP("Translation to Spain and Catalan"),"dispiste@users.sourceforge.net");
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
  KApplication app;

  // register ourselves as a dcop client
  app.dcopClient()->registerAs(app.name(), false);

  // see if we are starting with session management
  if (app.isRestored())
    RESTORE(KMerlin)
    else
      {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
          {
            KMerlin *widget = new KMerlin;
            if( widget->visible )
              widget->show();
            else
              widget->hide();
          }
        else
          {
            int i = 0;
            for (; i < args->count(); i++)
              {
                KMerlin *widget = new KMerlin;
                if( widget->visible )
                  widget->show();
                else
                  widget->hide();
              }
          }
        args->clear();
      }

  return app.exec();
}
