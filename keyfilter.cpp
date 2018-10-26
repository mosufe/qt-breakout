#include "KeyFilter.h"

bool KeyFilter::eventFilter( QObject *dist, QEvent *e )
{
 if( e->type() == QEvent::KeyPress )
 {

     // refuse up and down arrow keys,
      // since those move the paddles
      QKeyEvent *castKeyEvent = static_cast<QKeyEvent*>( e );
      if((castKeyEvent->key() == Qt::Key_Up)||
        (castKeyEvent->key() == Qt::Key_Down))
        return true;

     //return true;
 }
 return false;
}

