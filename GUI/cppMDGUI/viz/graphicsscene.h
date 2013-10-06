#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class QtPlatform;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QtPlatform *platform = 0);

signals:
    
public slots:
    
private:

    QtPlatform *m_platform;
};

#endif // GRAPHICSSCENE_H
