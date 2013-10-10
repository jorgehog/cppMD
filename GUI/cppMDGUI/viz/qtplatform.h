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
    QtPlatform(int argc = 0, char* argv[] = NULL, MainWindow * mainWindow = NULL);
    int exec();
    void startAdvanceTimer();
    void stopAdvanceTimer();
    void setAdvanceTimerInterval(double interval);
    Sprite *createSprite(std::string imageFile);
    void drawSprite(Sprite *sprite, double x, double y, double width, double height, double rotation);
    void clear();
    void close();
    void prepareEntity(Entity *entity);
    void setEnsemble(Ensemble* ensemble){
        this->ensemble = ensemble;
    }
    void setMainMesh(MainMesh* mainMesh){
        this->mainMesh = mainMesh;
        setHeight();
        setWidth();
    }
    void setGraphicsView(QGraphicsView * graphicsView);


public slots:
    void advanceTimeout();

private:

    QTimer *timer;

    QGraphicsView *graphicsView;
    GraphicsScene *graphicsScene;

    MainWindow* mainWindow;
    MainMesh* mainMesh;

    Ensemble* ensemble;

    std::vector<std::string> images;

    std::vector<Qt::GlobalColor> colors;

    void reDraw();

    void setPixPrMeter();

    void drawFields(MeshField* mf, double size, int i);
    void drawField(MeshField* mf, double size, int i);

    double origHeight;
    double origWidth;

    void setHeight(){
        origHeight = mainMesh->shape(1)/(ENS_NY);
    }

    void setWidth() {
        origWidth = mainMesh->shape(0)/(ENS_NX);
    }

    double sizeFac = 0.25;

    void getMainMeshTopology(double &W, double &H);

    double pixPrMeter_x;
    double pixPrMeter_y;

};

#endif // QTPLATFORM_H
