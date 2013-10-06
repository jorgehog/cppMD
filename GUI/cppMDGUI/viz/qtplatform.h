#ifndef QTPLATFORM_H
#define QTPLATFORM_H

#include "platform.h"
#include "graphicsscene.h"
#include "../../../src/MeshField/MainMesh/mainmesh.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include <QObject>

class MainWindow;

class QtPlatform : public QObject, public Platform
{
    Q_OBJECT
public:
    QtPlatform(int argc, char* argv[], MainWindow * mainWindow);
    int exec();
    void startAdvanceTimer();
    void stopAdvanceTimer();
    void setAdvanceTimerInterval(double interval);
    Sprite *createSprite(std::string spriteFile);
    void drawSprite(Sprite *sprite, double x, double y, double width, double height, double rotation);
    void clear();
    void close();
    void prepareEntity(Entity *entity);

public slots:
    void advanceTimeout();

private:

    QTimer *timer;

    QGraphicsView *graphicsView;
    GraphicsScene *graphicsScene;

    MainWindow* mainWindow;

    std::vector<Sprite*> sprites;

    void reDraw();

    int height(){
        return 1;
    }

    int width() {
        return 1;
    }

    void getMainMeshTopology(double &W, double &H);

    double pixPrMeter;

};

#endif // QTPLATFORM_H
