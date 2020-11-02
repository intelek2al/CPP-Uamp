#include "stareditor.h"
#include "starrating.h"
#include "loggingcategories.h"

#include <QtWidgets>

StarEditor::StarEditor(QWidget *parent)
    : QWidget(parent)
{
    qDebug(logDebug()) << "StarEditor::StarEditor";
  setMouseTracking(true);
  setAutoFillBackground(true);
}

QSize StarEditor::sizeHint() const
{
    qDebug(logDebug()) << "StarEditor::sizeHint";
  return myStarRating.sizeHint();
}

void StarEditor::paintEvent(QPaintEvent *)
{
    qDebug(logDebug()) << "StarEditor::paintEvent";
  QPainter painter(this);
  myStarRating.paint(&painter, rect(), palette(),
                     StarRating::EditMode::Editable);
}


void StarEditor::mouseMoveEvent(QMouseEvent *event)
{
    qDebug(logDebug()) << "StarEditor::mouseMoveEvent";
  const int star = starAtPosition(event->x());

  if (star != myStarRating.starCount() && star != -1) {
    myStarRating.setStarCount(star);
    update();
  }
  QWidget::mouseMoveEvent(event);
}

void StarEditor::mouseReleaseEvent(QMouseEvent * /* event */)
{
    qDebug(logDebug()) << "StarEditor::mouseReleaseEvent";
  emit editingFinished();
}

int StarEditor::starAtPosition(int x) const
{
    qDebug(logDebug()) << "StarEditor::starAtPosition";
  int star = (x / (myStarRating.sizeHint().width()
      / myStarRating.maxStarCount())) + 1;
  if (star <= 0 || star > myStarRating.maxStarCount())
    return -1;

  return star;
}

