/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void ReplacePage::orgTxt_textChanged( const QString & text )
{
    if(List->currentItem() != 0){
	List->currentItem()->setText(0,text);
    }
}


void ReplacePage::newTxt_textChanged( const QString &text )
{
    if(List->currentItem() != 0){
	List->currentItem()->setText(1,text);
    }
}


void ReplacePage::KPushButton1_pressed()
{
    (void*) new QListViewItem(List,"NEW","EDIT ME");
}


void ReplacePage::List_selectionChanged( QListViewItem *item )
{
    orgTxt->setText(item->text(0));
    newTxt->setText(item->text(1));
}
