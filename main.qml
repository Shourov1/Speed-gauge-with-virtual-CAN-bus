import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.1

Window {

    id: window
    width: 800
    height: 800
    visible: true

    property bool accelerating
    property int speed

    Gauge {


        value: speed //accelerating ? maximumValue : 0
        maximumValue: 250
        anchors {
            fill: parent
            margins: window.height * 0.2
        }

        Component.onCompleted: forceActiveFocus()

        Behavior on value { NumberAnimation { to:speed; duration: 10000 }}
    }

    Button {
        id: exitButton
        text: 'Quit'

        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            //bottomCenter:  parent.bottomCenter
        }

        onClicked: {
            window.close();
        }
    }
}
