#ifndef UAMP_APP_SRC_STARRATING_H_
#define UAMP_APP_SRC_STARRATING_H_

#include <QPainter>
class StarRating
{
 public:
  enum class EditMode { Editable, ReadOnly };

  explicit StarRating(int starCount = 1, int maxStarCount = 5);

  void paint(QPainter *painter, const QRect &rect,
             const QPalette &palette, EditMode mode) const;
  QSize sizeHint() const;
  int starCount() const { return myStarCount; }
  int maxStarCount() const { return myMaxStarCount; }
  void setStarCount(int starCount) { myStarCount = starCount; }
  void setMaxStarCount(int maxStarCount) { myMaxStarCount = maxStarCount; }

 private:
  QPolygonF starPolygon;
  QPolygonF diamondPolygon;
  int myStarCount;
  int myMaxStarCount;
};

Q_DECLARE_METATYPE(StarRating)

#endif //UAMP_APP_SRC_STARRATING_H_
