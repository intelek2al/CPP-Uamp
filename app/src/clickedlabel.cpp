#include "clickedlabel.h"
#include "loggingcategories.h"

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent) {

}

ClickedLabel::~ClickedLabel() { }

void ClickedLabel::mouseDoubleClickEvent(QMouseEvent *event) {
    emit doubleClicked();
}