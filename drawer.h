#ifndef DRAWER_H
#define DRAWER_H

#include <QObject>
#include <QList>
#include <cmath>

enum SelectionState{
    SelectionBounds=1,SelectionCenter=2,SelectionDisabled=0
};

struct Pixel{
    int x;
    int y;
    QString color;
    int level;
    Pixel(int x=-1,int y=-1,QString color = "grey",int level = -1 ){
        this->x=x;
        this->y=y;
        this->color=color;
        this->level=level;
    }
};

class Drawer : public QObject
{
    Q_OBJECT
public:
    int size;
    QString fillColor;
    QString replacingColor;
    QString boundsColor;
    SelectionState AppSelectionState;
    explicit Drawer(QObject *parent = nullptr);
    ~Drawer();
private:
    void drawLine(int x1,int y1,int x2,int y2);
    bool nextStep();
    void prevStep();
    void clear();
    void lastStep();
    void addBound(int x,int y);
    void addCenter(int x,int y);
    void prepareFillPixel(int x,int y, QString color);
    QString** array;
    QList<Pixel> stack;
    QList<Pixel> reverseStack;
    int currentLevel;
    Pixel lastBound;
signals:
    void fillPixel(int x,int y,QString color);
public slots:
    void slotPixelSelected(int x,int y);
    void slotAddAnotherBounds();
    void slotNext();
    void slotPrev();
    void slotClear();
    void slotLast();
};

#endif // DRAWER_H
