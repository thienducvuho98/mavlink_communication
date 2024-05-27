import QtQuick
import QtQuick.Controls

//ApplicationWindow
Window  {
    visible: true
    width: 500
    height: 200

    Rectangle {
        width: parent.width
        height: parent.height
        border.color: "black"
        border.width: 2
        Button {
            id: startStopButton
            anchors {
                left: parent.left;
                right: parent.right;
                top: parent.top;
                leftMargin: 10;
                rightMargin: 10;
                topMargin: 12;
            }

            text: qsTr("START")
            font.pixelSize: 32

            contentItem: Text {
                text: startStopButton.text
                font: startStopButton.font
                opacity: enabled ? 1.0 : 0.3
                color: startStopButton.down ? "#17a81a" : "#21be2b"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 400
                implicitHeight: 40
                opacity: enabled ? 1 : 0.3
                border.color: startStopButton.down ? "#17a81a" : "#21be2b"
                border.width: 1
                radius: 2
            }

            onClicked: {
                if (startStopButton.contentItem.text === "START") {
                    startStopButton.contentItem.text = "STOP"
                    communicationObj.startHeartbeat()
                } else {
                    startStopButton.contentItem.text = "START"
                    // textBox.text = "No data..."
                    communicationObj.stopHeartbeat()
                }
            }
        }

        TextArea {
            id: textBox
            anchors.top: startStopButton.bottom
            anchors.topMargin: 50
            verticalAlignment: Text.AlignVCenter
            color: "black"
            text: "No data...";
            opacity: (value == "" ? 1.0 : 0.0);
            font {
                italic: true
                pointSize: fontSize;
            }
            anchors {
                left: parent.left;
                right: parent.right;
                leftMargin: 10;
                rightMargin: 10;
                verticalCenter: parent.verticalCenter;
            }
            width: parent.width
            height: parent.height / 2
        }

        // Test other solution but failed
        // function handleSignalSequenceReceivedFromHeartBeat(sequence) {
        //     textBox.text = "Sequence is " + sequence
        // }

        // Connections {
        //     target: communicationObj
        //     onSignalSequenceReceivedFromHeartBeat: handleSignalSequenceReceivedFromHeartBeat
        // }

        Connections {
            target: communicationObj
            onSignalSequenceReceivedFromHeartBeat: { sequence
                textBox.text = "Sequence is " + sequence
            }
        }
    }
}
