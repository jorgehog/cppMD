#include "qtplatform.h"

#include "qtsprite.h"
#include "../mainwindow.h"

#include <QApplication>
#include <iostream>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <math.h>

QtPlatform::QtPlatform(int argc, char* argv[], MainWindow *mainWindow) :
    Platform(argc, argv),
    timer(new QTimer),
    graphicsView(mainWindow->getGraphicsView()),
    mainWindow(mainWindow)
{

    mainWindow->setPlatform(this);

    QGLWidget *glwidget = new QGLWidget(graphicsView);
    graphicsView->setViewport(glwidget);
    graphicsView->scale(1,-1);

    //This is the magic which makes a game possible I guess?
    connect(timer, SIGNAL(timeout()), SLOT(advanceTimeout()));

    setPixPrMeter();

    graphicsScene = new GraphicsScene(this);
    graphicsScene->setSceneRect(0,0, graphicsView->width(), graphicsView->height());
    graphicsView->setScene(graphicsScene);

    for (int i = 0; i < ENS_N; ++i) {
        sprites.push_back(createSprite("defaultpicture.png"));
    }

}

/*!
  This loads everything needed to run a game and sets up the game engine
  so that it is aware about the platform.
  */
int QtPlatform::exec()
{
    try{
        graphicsView->show();
    } catch(std::exception & e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;

}

void QtPlatform::advanceTimeout()
{
    reDraw();
}

void QtPlatform::reDraw()
{
    clear();

    for(unsigned int i = 0; i < ENS_N; ++i) {

        double x = mainWindow->getEnsemble()->pos(0, i) * pixPrMeter_x;
        double y = mainWindow->getEnsemble()->pos(1, i) * pixPrMeter_y;
        double width = this->width() * pixPrMeter_x;
        double height = this->height() * pixPrMeter_y;
        drawSprite(sprites.at(i), x, y, width, height, 0);
    }
}

void QtPlatform::setPixPrMeter()
{
    double W, H;
    getMainMeshTopology(W, H);

    pixPrMeter_x = graphicsView->width()/W;
    pixPrMeter_y = graphicsView->height()/H;
}

void QtPlatform::getMainMeshTopology(double &W, double &H)
{
    W = mainWindow->getMainMesh()->shape(0);
    H = mainWindow->getMainMesh()->shape(1);
}

void QtPlatform::startAdvanceTimer()
{
    std::cout << __PRETTY_FUNCTION__ << " called" << std::endl;
    timer->start();
}

void QtPlatform::stopAdvanceTimer()
{
    std::cout << __PRETTY_FUNCTION__ << " called" << std::endl;
    timer->stop();
}

void QtPlatform::setAdvanceTimerInterval(double interval)
{
    timer->setInterval(interval * 1000);
}

Sprite* QtPlatform::createSprite(std::string spriteFile)
{
    QPixmap pixmap(QString(":/viz/images/") + QString::fromStdString(spriteFile));
    QGraphicsPixmapItem* pixItem = graphicsScene->addPixmap(pixmap);
    pixItem->setVisible(false);

    QtSprite *sprite = new QtSprite(pixItem);

    return sprite;
}

void QtPlatform::drawSprite(Sprite *sprite, double x, double y, double width, double height, double rotation)
{
    QtSprite* qtSprite = (QtSprite*)sprite;
    QGraphicsPixmapItem *spriteItem = (QGraphicsPixmapItem*)qtSprite->getGraphicsItem();
    QPixmap tmpPixmap = spriteItem->pixmap();
    spriteItem->setPixmap(tmpPixmap.scaledToHeight(height));
    spriteItem->setTransformOriginPoint(width/2, height/2);
    spriteItem->setRotation(rotation * 180 / M_PI + 180);
    spriteItem->setPos(-width/2 + x - 58, -height/2 + y - 25);
    spriteItem->setVisible(true);
}

void QtPlatform::clear()
{
}

void QtPlatform::close()
{
    graphicsView->close();
}

//Since the window is flipped upside down, all entities will appear so.
//To counter this we let the physics engine rotate the objects at creation.
void QtPlatform::prepareEntity(Entity *entity)
{
    (void) entity;
   // b2Body * body = entity->body();
   // body->SetTransform(body->GetPosition(), body->GetAngle() + M_PI);
}

