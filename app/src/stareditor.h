#ifndef UAMP_APP_SRC_STAREDITOR_H_
#define UAMP_APP_SRC_STAREDITOR_H_

#include <QWidget>
#include <QStyledItemDelegate>

#include "starrating.h"

class StarEditor : public QWidget
{
  Q_OBJECT
 public:
  StarEditor(QWidget *parent = nullptr);

  QSize sizeHint() const override;
  void setStarRating(const StarRating &starRating) {
    myStarRating = starRating;
  }
  StarRating starRating() { return myStarRating; }

  signals:
      void editingFinished();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

 private:
  int starAtPosition(int x) const;

  StarRating myStarRating;
};

#endif //UAMP_APP_SRC_STAREDITOR_H_
