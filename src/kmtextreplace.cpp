
#include "kmtextreplace.h"


KMTextReplace::KMTextReplace(QString m_org,QString m_new)
  : QObject()
{
  orgTxt = m_org;
  newTxt =m_new;
}


KMTextReplace::~KMTextReplace()
{
}
