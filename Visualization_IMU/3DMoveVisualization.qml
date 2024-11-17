import QtQuick 2.15
import QtQuick3D
import QtQuick.Controls
//import com.company.serialmanager 1.0

Rectangle{
    id: root
    Node {
        id:standScene
        Model {
            id: cubeModel
            position: Qt.vector3d(0, 0, 0)
            scale: Qt.vector3d(2, 0.5, 1)
            source: "#Cube"
            materials: [
                DefaultMaterial {
                    DefaultMaterial {
                            diffuseMap: Texture {
                            sourceItem: Item {
                            anchors.fill: parent
                            Rectangle {
                                anchors.fill: parent
                                color: "white"
                                }


                            }
                         }
                         }
            }
            ]
                        }
                      }
    View3D {
      id: view
      anchors.fill: parent
      scale: 1
      environment: SceneEnvironment {
        clearColor: "#222222"
        backgroundMode: SceneEnvironment.Color
        // No Antialiasing
        // antialiasingMode: SceneEnvironment.NoAA
        // Super Sample Antialiasing
        antialiasingMode: SceneEnvironment.SSAA
        // Multi Sample Antialiasing
        // antialiasingMode: SceneEnvironment.MSAA

        // Antialiasing Quality (Medium, High, VeryHigh)
        antialiasingQuality: SceneEnvironment.VeryHigh
      }
      importScene: standScene
      PerspectiveCamera {
        position: Qt.vector3d(0, 200, 300)
        eulerRotation.x: -30
      }

      DirectionalLight {
        eulerRotation.x: 0
        eulerRotation.y: 0
      }
    }

    Text {
        id:textRoll
        x: 55
        y: 50
        text: qsTr("Roll: ")
        color: "white"
        font.pixelSize: 20
    }
    Text {
        id:textRollVal
        x: 100
        y: 50
        text: qsTr("0")
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }
    Text {
        id:textPitch
        x: 155
        y: 50
        text: qsTr("Pitch: ")
        color: "white"
        font.pixelSize: 20
    }

    Text {
        id: textPitchVal
        x: 220
        y: 50
        text: qsTr("0")
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }
    Text {
        id:textYaw
        x: 255
        y: 50
        text: qsTr("Yaw: ")
        color: "white"
        font.pixelSize: 20
    }

    Text {
        id: textYawVal
        x: 320
        y: 50
        text: qsTr("0")
        color: "white"
        font.pixelSize: 20
        font.bold: true
    }

    function changeRoll(str: string)
    {
        textRollVal.text = qsTr(str)
        cubeModel.eulerRotation.x = str
    }
    function changePitch(str: string)
    {
        textPitchVal.text = qsTr(str)
        cubeModel.eulerRotation.y = str
    }
    function changeYaw(str: string)
    {
        textYawVal.text = qsTr(str)
        cubeModel.eulerRotation.z = str
    }
}
