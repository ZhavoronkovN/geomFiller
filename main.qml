import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import Drawer 1.0

Window {
    visible: true
    width: grid.width;
    height: 900
    title: qsTr("Ind zavd 3")
    GridView {
        id: grid
        boundsBehavior: Flickable.StopAtBounds
        width: 25*32
        height: 25*32
        cellWidth: 25
        cellHeight: 25
        model: 32*32
        delegate: Rectangle{
            width: 24
            height: 24
            color: "grey"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    drawer.slotPixelSelected(index%32,(index-(index%32))/32);
                }
            }
        }
    }
    Rectangle{
        id:rect_tools
        anchors.top: grid.bottom
        width: parent.width
        height: 100
        Button{
            id:but_prev
            width: 150
            height: 80
            anchors.margins: 10
            anchors.left: parent.left
            text: "<-"
            font.pixelSize: 20
            onClicked: drawer.slotPrev();
        }
        Button{
            id:but_next
            width: 150
            height: 80
            anchors.margins: 10
            anchors.left: but_prev.right
            text: "->"
            font.pixelSize: 20
            onClicked: drawer.slotNext();
        }
        Button{
            id:but_last
            width: 150
            height: 80
            anchors.margins: 10
            anchors.left: but_next.right
            text: "End"
            font.pixelSize: 20
            onClicked: drawer.slotLast();
        }
        Button{
            id:but_clear
            width: 150
            height: 80
            anchors.margins: 10
            anchors.left: but_last.right
            text: "Clear"
            font.pixelSize: 20
            onClicked: drawer.slotClear();
        }
        Button{
            id:but_anotherBound
            width: 150
            height: 80
            anchors.margins: 10
            anchors.left: but_clear.right
            text: "New Bound"
            font.pixelSize: 20
            onClicked: drawer.slotAddAnotherBounds();
        }
    }

    Drawer{
        id:drawer
        onFillPixel: {grid.itemAt(grid.x+x*grid.cellWidth,grid.y+y*grid.cellHeight).color=color;}
        Component.onCompleted: fillWithTestData();
    }
    function fillWithTestData(){
        drawer.slotPixelSelected(4,4);
        drawer.slotPixelSelected(4,26);
        drawer.slotPixelSelected(20,26);
        drawer.slotPixelSelected(28,18);
        drawer.slotPixelSelected(21,4);
        drawer.slotPixelSelected(21,8);
        drawer.slotPixelSelected(10,8);
        drawer.slotPixelSelected(10,4);
        drawer.slotPixelSelected(4,4);
        drawer.slotAddAnotherBounds();
        drawer.slotPixelSelected(10,12);
        drawer.slotPixelSelected(10,20);
        drawer.slotPixelSelected(17,20);
        drawer.slotPixelSelected(21,16);
        drawer.slotPixelSelected(21,12);
        drawer.slotPixelSelected(10,12);
        drawer.slotPixelSelected(14,21);
    }
}
