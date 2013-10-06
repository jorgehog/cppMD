#include "graphicsscene.h"

#include "qtplatform.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

GraphicsScene::GraphicsScene(QtPlatform *platform) :
    QGraphicsScene(platform)
{
    setSceneRect(0,0,400,400);
}

