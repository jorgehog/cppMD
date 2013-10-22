#include "qtplatform.h"

#include "qtsprite.h"
#include "../mainwindow.h"

#include <QApplication>
#include <iostream>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <math.h>
#include <exception>

QtPlatform::QtPlatform(int argc, char* argv[], MainWindow *mainWindow) :
    Platform(argc, argv),
    timer(new QTimer),
    mainWindow(mainWindow)
{

    std::string superPath;
    if (argc > 1) {
        superPath = std::string(argv[1]);
    } else {
        superPath = "./";
    }

    if (mainWindow != NULL){
        mainWindow->setPlatform(this);

        setEnsemble(mainWindow->getEnsemble());
        setMainMesh(mainWindow->getMainMesh());

        setGraphicsView(mainWindow->getGraphicsView());

    }


    //This is the magic which makes a game possible I guess?
    connect(timer, SIGNAL(timeout()), SLOT(advanceTimeout()));

    colors.push_back(Qt::black);
    colors.push_back(Qt::blue);
    colors.push_back(Qt::red);

    std::string imagePath = superPath + "GUI/cppMDGUI/viz/images/";

//    images.push_back(imagePath + "LithiumAtom.png");
//    images.push_back(imagePath + "Stylised_Lithium_Atom.png");
//    sizeFac = 1;

    images.push_back(imagePath + "ball.png");
    images.push_back(imagePath + "defaultpicture.png");

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

        double x = ensemble->pos(0, i) * pixPrMeter_x;
        double y = ensemble->pos(1, i) * pixPrMeter_y;
        double width = sizeFac*origWidth* pixPrMeter_x;
        double height = sizeFac*origHeight * pixPrMeter_y;
        drawSprite(createSprite(images.at(i%ensemble->nSpecies)), x, y, width, height, 0);
    }

    drawFields(mainMesh, 6, 0);

}

void QtPlatform::setPixPrMeter()
{
    double W, H;
    getMainMeshTopology(W, H);

    pixPrMeter_x = graphicsView->width()/W;
    pixPrMeter_y = graphicsView->height()/H;
}

void QtPlatform::drawFields(MeshField *mf, double size, int i)
{
    drawField(mf, size, i);
    for (MeshField* smf : mf->getSubfields()){
        drawFields(smf, 0.5*size, i+1);
    }
}

void QtPlatform::drawField(MeshField *mf, double size, int i)
{
    QPen pen(colors.at(i%colors.size()));
    if (i > 0) {
        pen.setStyle(Qt::DotLine);
    }

    pen.setWidthF(size);
    QRect *rect = new QRect(mf->topology(0, 0)*pixPrMeter_x,
                            mf->topology(1, 0)*pixPrMeter_y,
                            mf->shape(0)*pixPrMeter_x,
                            mf->shape(1)*pixPrMeter_y);
    graphicsScene->addRect(*rect, pen);

    int k = 0;
    for (Event* event : mf->getEvents()){
        if (event->notSilent()) {
            QGraphicsSimpleTextItem * simpleTextItem1 = graphicsScene->addSimpleText(QString::number(event->getMeasurement())
                                                                                     + QString::fromStdString(event->getUnit()));
            if (i != 0) {
                simpleTextItem1->setPos(30 + rect->left() + k*100, rect->bottom());
            } else {
                simpleTextItem1->setPos(30 + rect->left() + k*100, rect->top() - size*4);
            }
            k++;
        }
    }
}

void QtPlatform::getMainMeshTopology(double &W, double &H)
{
    W = mainMesh->shape(0);
    H = mainMesh->shape(1);
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

    if (mainWindow != NULL){
        mainWindow->stop();
    }

}

void QtPlatform::setAdvanceTimerInterval(double interval)
{
    timer->setInterval(interval * 1000);
}

Sprite* QtPlatform::createSprite(std::string imageFile)
{
    QPixmap pixmap(QString::fromStdString(imageFile));
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
    spriteItem->setRotation(rotation * 180 / M_PI);
    spriteItem->setPos(-width/2 + x, -height/2 + y);
    spriteItem->setVisible(true);
}

void QtPlatform::clear()
{
   graphicsScene->clear();
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

void QtPlatform::setGraphicsView(QGraphicsView *graphicsView)
{
       this->graphicsView = graphicsView;

       QGLWidget *glwidget = new QGLWidget(graphicsView);
       graphicsView->setViewport(glwidget);
//       graphicsView->scale(1,-1);

       setPixPrMeter();

       graphicsScene = new GraphicsScene(this);
       graphicsScene->setSceneRect(0,0, graphicsView->width(), graphicsView->height());
       graphicsView->setScene(graphicsScene);

}

