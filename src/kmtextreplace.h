
#ifndef _KMTEXTREPLACE_H_
#define _KMTEXTREPLACE_H_

#include <qobject.h>

/**
 * 
 * Olaf Lueg
 **/
class KMTextReplace : public QObject
{
public:
  KMTextReplace(QString m_org,QString m_new);
  ~KMTextReplace();
  QString orgTxt;
  QString newTxt;
};

#endif
