import QtQuick 2.15
import QtQuick.Controls.Basic

import models 1.0
import "math_utils.js" as MMath

Item {
    id: root

    Rectangle {
        id: background
        width: parent.width
        height: parent.height
        color: "black"
        clip: true

        MouseArea{
            anchors{
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                top: rule.top
            }

            hoverEnabled: true
            onPositionChanged: {
                instance.mouseX = mouseX
                if(pressed){
                    if(mouseX > hoverCursor.x)
                        hoverCursor.width = mouseX-hoverCursor.x
                    else
                    {
                        var oldWidth = hoverCursor.width
                        hoverCursor.width = hoverCursor.x-mouseX+oldWidth
                        hoverCursor.x = mouseX
                    }
                }else if(hoverCursor.width == 2)
                    hoverCursor.x = mouseX


            }

            onPressed: {
                hoverCursor.width = 2
                hoverCursor.x = mouseX
                instance.pressedX = mouseX
            }

            onEntered: {
                hoverCursor.x = mouseX
                hoverCursor.visible = true
            }

            onExited: {
                hoverCursor.visible = false
            }

            onWheel: {
                var ruleWidth = instance.ruleWidth
                var ruleX = instance.viewX

                if(wheel.angleDelta.y < 0 && ruleWidth <= parent.width){
                    return
                }

                if(wheel.angleDelta.y > 0 && instance.isMaximumScale)
                    return

                //calculate new ruleSize
                //fomule: xn = alphaN*xn_1+mouseX*(1-alphaN)
                //with: xn_1 is last x of rule
                //    : alphaN = newWidth/oldWidth
                var alpha0 = Math.pow(1.2,wheel.angleDelta.y/120)
                var wn_1 = ruleWidth
                var xn_1 = ruleX
                var wn = wn_1*alpha0
                if(MMath.differentPercent(wn,parent.width) < 15){
                    wn = 1
                    instance.ruleWidth = parent.width
                    instance.viewX = 0
                }else {//over 20%
                    var alphaN = wn/wn_1
                    instance.ruleWidth = wn
                    instance.viewX = alphaN*xn_1+mouseX*(1-alphaN)
                }

                //calculate scrollbar size
                if(wn/width > 100)
                    scrollbar.size = 0.01
                else
                    scrollbar.size = parent.width/wn

                scrollbar.position = Math.abs(instance.viewX/instance.ruleWidth)
            }
        }

        onWidthChanged: {
            instance.viewWidth = width
        }

        RuleContextCapturedView {
            id: rule
            model: instance.timeSteps
            width: parent.width
            height: 55

            anchors{
                bottom: timeZone.top
                left: parent.left
            }

            onWidthChanged: {
                instance.ruleWidth = width
                instance.viewX = 0
            }
        }

        TimeZoneRuler{
            id: timeZone
            anchors{
                bottom: scrollbar.top
                left: parent.left
            }
        }

        ScrollBar {
            id: scrollbar
            width: parent.width
            size: 1
            position: 0
            active: true
            orientation: Qt.Horizontal
            policy: ScrollBar.AlwaysOn
            anchors{
                bottom: parent.bottom
            }




            contentItem: Rectangle {
                implicitWidth: parent.width
                implicitHeight: 15
                color: scrollbar.pressed ? "#606060" : (scrollbar.hovered ? "#686868" : "#585858")
                // Hide the ScrollBar when it's not needed.
                opacity: scrollbar.policy === ScrollBar.AlwaysOn || (scrollbar.active && scrollbar.size < 1.0) ? 0.75 : 0

                // Animate the changes in opacity (default duration is 250 ms).
                Behavior on opacity {
                    NumberAnimation {}
                }

                ScrollCenterThumbnail {

                }
            }

            onPositionChanged: {
                instance.viewX = -position*instance.ruleWidth
            }

        }

        MediaCursor{
            id: runningCusor
            x: instance.relativePosition
            anchors{
                top: rule.top
                bottom: parent.bottom
            }
        }

        MediaCursor {
            id: hoverCursor

            anchors{
                top: rule.top
                bottom:  parent.bottom
            }
        }

        TimePositionBubble{
            id: positionBubble
            anchors{
                bottom: rule.top
                left: runningCusor.right
            }

            position: instance.position
        }

    }



}
