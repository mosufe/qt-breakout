#include "newslider.h"

NewSlider::NewSlider(Qt::Orientation orientation) : QSlider()
{
    slider = new QSlider(orientation);
}

void NewSlider::keyPressEvent(QKeyEvent *e) {

}
