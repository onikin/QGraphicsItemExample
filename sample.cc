#include "sample.h"

#include <QtWidgets>

void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->drawPixmap(m_newRect.toRect(), m_pixmap.scaled(m_newRect.toRect().width(), 
                                                          m_newRect.toRect().height()));
  if (isSelected()) {
    // draw 4 squares in each corner
    QRectF rect = boundingRect();
    QRectF topLeft = QRectF(rect.topLeft(), rect.topLeft() += QPointF(10, 10));
    QRectF topRight = QRectF(rect.topRight(), rect.topRight()-= QPointF(10, -10));
    QRectF botLeft = QRectF(rect.bottomLeft(), rect.bottomLeft() -= QPointF(-10, 10));
    QRectF botRight = QRectF(rect.bottomRight(), rect.bottomRight() -= QPointF(10,10));
    painter->drawRect(topLeft); // top left
    painter->drawRect(botLeft); // bot left
    painter->drawRect(topRight); // top right
    painter->drawRect(botRight); // bot right

    setAcceptHoverEvents(true);
    setOpacity(1.0);
    QList<QGraphicsItem*> items = this->collidingItems();
    for (auto x : items) {
      x->setOpacity(0.25);
      x->setAcceptHoverEvents(false);
    }
  } 
  else {
    setOpacity(1.0);
    setAcceptHoverEvents(true);
  }
}

void GraphicsPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event) 
{
  if (isSelected()) {
    QPointF eventPos = event->pos();
  
    QRectF rect = boundingRect();
    QRectF topLeft = QRectF(rect.topLeft(), rect.topLeft() += QPointF(10, 10));
    QRectF topRight = QRectF(rect.topRight(), rect.topRight()-= QPointF(10, -10));
    QRectF botLeft = QRectF(rect.bottomLeft(), rect.bottomLeft() -= QPointF(-10, 10));
    QRectF botRight = QRectF(rect.bottomRight(), rect.bottomRight() -= QPointF(10,10));

    qDebug() << isSelected() << ", cursor pos: " << eventPos;

    if (topLeft.contains(eventPos) || botRight.contains(eventPos))
      setCursor(Qt::SizeFDiagCursor);
    else if (topRight.contains(eventPos) || botLeft.contains(eventPos))
      setCursor(Qt::SizeBDiagCursor);
    else if (rect.contains(eventPos))
      setCursor(Qt::SizeAllCursor);
    else
      setCursor(Qt::ArrowCursor);
  } else {
//    qDebug() << "Ignore event" << rand();
    event->ignore();
  } 
}

GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent = 0) :
  QGraphicsItem(parent),
  m_pixmap(pixmap)
{
  m_imageRect = QRectF(pixmap.rect());
  m_newRect = QRectF(pixmap.rect());
  setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);
  setFocus(Qt::MouseFocusReason);
  setAcceptHoverEvents(true);
}

void GraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QPointF eventPos = event->pos();

  QRectF rect = boundingRect();
  QRectF topLeft = QRectF(rect.topLeft(), rect.topLeft() += QPointF(10, 10));
  QRectF topRight = QRectF(rect.topRight(), rect.topRight()-= QPointF(10, -10));
  QRectF botLeft = QRectF(rect.bottomLeft(), rect.bottomLeft() -= QPointF(-10, 10));
  QRectF botRight = QRectF(rect.bottomRight(), rect.bottomRight() -= QPointF(10,10));

  if (topLeft.contains(eventPos)) { 
    m_clickPosition = rect.topLeft(); 
    m_quad = ClickQuadrant::TopLeft;
//    qDebug() << "Clicked top left..";
  } else if (topRight.contains(eventPos)) {
    m_clickPosition = rect.topRight(); 
    m_quad = ClickQuadrant::TopRight;
//    qDebug() << "Clicked top right..";
  } else if (botLeft.contains(eventPos)) {
    m_clickPosition = rect.bottomLeft(); 
    m_quad = ClickQuadrant::BotLeft;
//    qDebug() << "Clicked bottom left..";
  } else if (botRight.contains(eventPos)) {
    m_clickPosition = rect.bottomRight(); 
    m_quad = ClickQuadrant::BotRight;
//    qDebug() << "Clicked bottom right..";
  } else {
    m_clickPosition = eventPos; 
    m_quad = ClickQuadrant::None;
//    qDebug() << "Clicked none..";
  }
//  qDebug() << "Bounding Rect: " << boundingRect();
}

void GraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (!(m_quad == ClickQuadrant::None)) {
//    qDebug() << "Release resize action";
    m_quad = ClickQuadrant::None;
    update();
  } else {
//    qDebug() << "Release mouse button";
    QGraphicsItem::mouseReleaseEvent(event);
    update();
  }
}


void GraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QPointF eventPos = event->pos();

  if (m_quad != ClickQuadrant::None) { // boolean resizing flag
    prepareGeometryChange();
    QRectF rect = boundingRect();

    if (m_quad == ClickQuadrant::TopLeft) {
//      qDebug() << "Dragged top left";
//      qDebug() << "Pos: " << eventPos;
      m_newRect = QRectF(eventPos.x(), eventPos.y(), rect.right()-eventPos.x(), rect.bottom()-eventPos.y());
    } else if (m_quad == ClickQuadrant::TopRight) {
//      qDebug() << "Dragged top right";
//      qDebug() << "Pos: " << eventPos;
      m_newRect = QRectF(rect.left(), eventPos.y(), eventPos.x()-rect.left(), rect.bottom()-eventPos.y());
    } else if (m_quad == ClickQuadrant::BotLeft) {
//      qDebug() << "Dragged bottom left";
//      qDebug() << "Pos: " << eventPos;
      m_newRect = QRectF(eventPos.x(), rect.top(), rect.right()-eventPos.x(), eventPos.y()-rect.top());
    } else if (m_quad == ClickQuadrant::BotRight) {
//      qDebug() << "Dragged bottom right";
//      qDebug() << "Pos: " << eventPos;
      m_newRect = QRectF(rect.left(), rect.top(), eventPos.x()-rect.left(), eventPos.y()-rect.top());
    }
    qDebug() << "Bounding Rect: " << boundingRect();
  } else { // user is dragging the item
    QGraphicsItem::mouseMoveEvent(event);
  }
  update();
}

GraphicsScene::GraphicsScene(QObject* parent = nullptr) :
  QGraphicsScene(parent)
{
}

void GraphicsScene::ReprocessEvent(QGraphicsSceneHoverEvent* event)
{
  QPointF point = event->scenePos();
  QList<QGraphicsItem *> listItems = items(point);

  qDebug() << "Sending event2 backwards"; 
  for (int i = 1; i < listItems.size(); i++)
    this->sendEvent(listItems[i], event);
}
/*
void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) 
{
  QPointF point = event->scenePos();
  QList<QGraphicsItem *> listItems = items(point);


  QEvent* event2 = dynamic_cast<QGraphicsSceneHoverEvent*>(event);

  if (event2 != nullptr) {
    qDebug() << "Sending event2 backwards"; 
    for (int i = 1; i < listItems.size(); i++)
      this->sendEvent(listItems[i], event2);
  }


  // Forward event to the item that is on top
  QGraphicsScene::mouseMoveEvent(event);
}
*/

// Always relative to item's own coordinate frame
QRectF GraphicsPixmapItem::boundingRect() const 
{
  return m_newRect;
}

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  GraphicsScene scene;
  QWidget w;
  QHBoxLayout layout(&w);
  w.setMouseTracking(true);

  QGraphicsView view1(&scene);
  view1.setSceneRect(0,0,800,800); /* x, y, width, height */
  layout.addWidget(&view1);

  // Set various view options
  view1.setRenderHint(QPainter::Antialiasing);
  view1.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view1.setBackgroundBrush(QColor(230, 200, 167));

  // Add Exa logo to scene
  QImage image("exa.png");
  QImage image2("exaInv.png");
  GraphicsPixmapItem item(QPixmap::fromImage(image));
  GraphicsPixmapItem item2(QPixmap::fromImage(image2));
  scene.addItem(&item);
  scene.addItem(&item2);

  w.show();
  return app.exec();
}

