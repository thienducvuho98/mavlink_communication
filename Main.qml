import QtQuick
import QtQuick.Controls

//ApplicationWindow
ApplicationWindow  {
    visible: true
    width: 500
    height: 300
    title: qsTr("Send/get Mavlink heartbeat message")

    property color borderColor: "red"
    property int borderwidth: 0

    Column {
        anchors.centerIn: parent
        spacing: 10  // Khoảng cách giữa các thành phần trong cột
        Button {
            id: startStopButton
            text: qsTr("START")
            font.pixelSize: 32
            width: 460
            height: 80

            contentItem: Text {
                text: startStopButton.text
                font: startStopButton.font
                opacity: enabled ? 1.0 : 0.3
                color: "#17a81a"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            onClicked: {
                if (startStopButton.contentItem.text === "START")
                {
                    startStopButton.contentItem.text = "STOP"
                    borderwidth = 0;
                    communicationObj.startHeartbeat(textBoxIP.text)
                }
                else
                {
                    startStopButton.contentItem.text = "START"
                    communicationObj.stopHeartbeat()
                }
            }
        }

        Label {
            text: "IP Address: (Default will be localhost if not entered)"
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextArea {
            id: textBoxIP
            verticalAlignment: Text.AlignVCenter
            color: "black"
            text: "Please enter IP Adress";
            wrapMode: Text.Wrap
            width: parent.width
            height: 40

            Rectangle {
                id: border
                anchors.fill: parent
                color: "transparent"
                border.color: borderColor
                border.width: borderwidth
            }
        }

        Label {
            text: "Sequence of message heartbeat:"
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextArea {
            id: textBox
            // anchors.top: textBoxIP.bottom
            // anchors.topMargin: 50
            verticalAlignment: Text.AlignVCenter
            color: "black"
            text: "No data...";
            width: parent.width
            height: 40
        }
    }

    Connections {
        target: communicationObj
        onSignalSequenceReceivedFromHeartBeat:
        { sequence
            console.log("onSignalSequenceReceivedFromHeartBeat << sequence: ", sequence)
            textBox.text = "Sequence is " + sequence
        }
        onSignalIP_AddressIsInvalid:
        {
            console.log("onSignalIP_AddressIsInvalid")
            startStopButton.contentItem.text = "START"
            borderwidth = 1;
        }
    }
}
