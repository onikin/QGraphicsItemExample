#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGridLayout>
#include <QObject>
#include <QPainter>
#include <QPointF>
#include <QSlider>
#include <QTransform>

class GraphicsScene : public QGraphicsScene
{
public:
  GraphicsScene(QObject* parent);
  void ReprocessEvent(QGraphicsSceneHoverEvent* event);
protected:
//  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
private:
};


class GraphicsPixmapItem : public QGraphicsItem
{
public:
  enum ClickQuadrant { None=0, TopLeft, TopRight, BotLeft, BotRight };
 
  GraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  QRectF boundingRect() const override;
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
private:
  QRectF m_imageRect;
  QRectF m_newRect;
  QPixmap m_pixmap;
  QPointF m_clickPosition;
  ClickQuadrant m_quad = ClickQuadrant::None;
};

#endif // SAMPLE_H
