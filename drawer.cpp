#include "drawer.h"
#include <QDebug>
Drawer::Drawer(QObject *parent) : QObject(parent)
{
    size = 32;
    fillColor = "yellow";
    replacingColor = "grey";
    boundsColor = "red";
    AppSelectionState = SelectionState::SelectionBounds;
    array = new QString*[size];
    for(int i = 0; i < size; i++){
        array[i] = new QString[size];
        for(int j = 0; j < size; j++){
            array[i][j] = replacingColor;
        }
    }
    stack = QList<Pixel>();
    reverseStack = QList<Pixel>();
    currentLevel = 0;
    lastBound = Pixel();
}

Drawer::~Drawer(){
    for(int i = 0; i < size; i++){
        delete[] array[i];
    }
    delete [] array;
}

void Drawer::slotPixelSelected(int x,int y){
    switch (AppSelectionState) {
    case SelectionDisabled:break;
    case SelectionBounds:this->addBound(x,y);break;
    case SelectionCenter:this->addCenter(x,y);break;
    }
}

void Drawer::slotAddAnotherBounds(){
    this->AppSelectionState=SelectionBounds;
}

void Drawer::clear(){
    this->stack.clear();
    for(int i = 0; i < size; i++){
        delete[] array[i];
    }
    delete [] array;
    array = new QString*[size];
    for(int i = 0; i < size; i++){
        array[i] = new QString[size];
        for(int j = 0; j < size; j++){
            prepareFillPixel(j,i,replacingColor);
        }
    }
    currentLevel = 0;
    AppSelectionState = SelectionState::SelectionBounds;
}

void Drawer::addBound(int x, int y){
    if(lastBound.x!=-1){
        this->drawLine(x,y,lastBound.x,lastBound.y);
    }
    if(array[y][x]==boundsColor){
        this->AppSelectionState=SelectionCenter;
        this->lastBound=Pixel();
    }
    else{
        lastBound.x=x;
        lastBound.y=y;
        this->prepareFillPixel(x,y,boundsColor);
    }
}

void Drawer::prepareFillPixel(int x, int y, QString color){
    this->array[y][x]=color;
    emit this->fillPixel(x,y,color);
}

void Drawer::addCenter(int x, int y){
    this->AppSelectionState=SelectionDisabled;
    this->stack.push_back(Pixel(x,y,"blue",currentLevel));
    this->reverseStack.push_back(Pixel(x,y,"blue",currentLevel));
    this->prepareFillPixel(x,y,"blue");
}

void Drawer::drawLine(int x1, int y1, int x2, int y2){
     int i, L;
     float dX, dY, x[1000], y[1000];
     L = abs(x1-x2) > abs(y1-y2)?abs(x1-x2):abs(y1-y2);
     dX = (x2-x1) / (static_cast<float>(L));
     dY = (y2-y1) / (static_cast<float>(L));
     i = 0;
     x[i] = x1;
     y[i] = y1;
     i++;
     while (i < L)
     {
      x[i] = x[i-1] + dX;
      y[i] = y[i-1] + dY;
      i++;
     }
     x[i] = x2;
     y[i] = y2;
     i = 0;
     while (i++ < L)
     {
      this->prepareFillPixel(static_cast<int>(roundf(x[i])), static_cast<int>(roundf(y[i])),this->boundsColor);
     }
}

void Drawer::slotClear(){
    this->clear();
}

void Drawer::slotNext(){
    this->nextStep();
}

void Drawer::slotPrev(){
    this->prevStep();
}

void Drawer::slotLast(){
    this->lastStep();
}

bool Drawer::nextStep(){
    if(stack.isEmpty()){
        return false;
    }
    while(stack.first().level==currentLevel){
        Pixel curPix = stack.first();
        stack.pop_front();
        if(curPix.color!=this->fillColor&&curPix.color!=boundsColor){
            this->reverseStack.push_back(curPix);
            this->prepareFillPixel(curPix.x,curPix.y,fillColor);
        }
        if(curPix.x!=size-1&&array[curPix.y][curPix.x+1]!=this->fillColor && array[curPix.y][curPix.x+1]!=this->boundsColor){
            stack.push_back(Pixel(curPix.x+1,curPix.y,replacingColor,currentLevel+1));
        }
        if(curPix.x!=0&&array[curPix.y][curPix.x-1]!=this->fillColor && array[curPix.y][curPix.x-1]!=this->boundsColor){
            stack.push_back(Pixel(curPix.x-1,curPix.y,replacingColor,currentLevel+1));
        }
        if(curPix.y!=size-1&&array[curPix.y+1][curPix.x]!=this->fillColor && array[curPix.y+1][curPix.x]!=this->boundsColor){
            stack.push_back(Pixel(curPix.x,curPix.y+1,replacingColor,currentLevel+1));
        }
        if(curPix.y!=0&&array[curPix.y-1][curPix.x]!=this->fillColor && array[curPix.y-1][curPix.x]!=this->boundsColor){
            stack.push_back(Pixel(curPix.x,curPix.y-1,replacingColor,currentLevel+1));
        }
        if(stack.length()==0){
            break;
        }
    }
    currentLevel++;
    return !stack.isEmpty();
}

void Drawer::lastStep(){
    while(nextStep()){}
}

void Drawer::prevStep(){
    if(currentLevel>0){
        while(reverseStack.last().level==currentLevel-1){
            this->prepareFillPixel(reverseStack.last().x,reverseStack.last().y,reverseStack.last().color);
            this->stack.push_front(this->reverseStack.last());
            reverseStack.pop_back();
            if(reverseStack.isEmpty()){
                break;
            }
        }
        while(stack.last().level==currentLevel){
            stack.pop_back();
            if(stack.isEmpty()){
                break;
            }
        }
        currentLevel--;
    }
}
