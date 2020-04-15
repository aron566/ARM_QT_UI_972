//import QtQuick 2.0
//import QtQuick.VirtualKeyboard 2.3
//Item {
//    InputPanel {
//            id: inputPanel
//            z: 99
//            x: 0
//            y: window.height

//            width: window.width

//            states: State {
//                name: "visible"
//                when: inputPanel.active
//                PropertyChanges {
//                    target: inputPanel
//                    y: window.height - inputPanel.height
//                }

//            }

//            transitions: Transition {
//                from: ""
//                to: "visible"
//                reversible: true
//                ParallelAnimation {
//                    NumberAnimation {
//                        properties: "y"
//                        duration: 250
//                        easing.type: Easing.InOutQuad
//                    }
//                }
//            }

//        }
//}
