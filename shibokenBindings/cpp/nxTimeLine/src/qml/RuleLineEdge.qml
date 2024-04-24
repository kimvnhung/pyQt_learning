import QtQuick 2.15
import models 1.0

Column {
    id: root
    property int lineType: TimeStep.UNDEFINED
    property int value: 1//ms

    Rectangle {
        id: line
        width: 1
        height: getHeightFromType(lineType)
        color: "#b8b8b8"
        opacity: 1-parent.lineType*0.25

        Behavior on height {
            PropertyAnimation{
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }

    Text {
        width: 1
        height: 15
        text: convertMillisecondsToString(value)
        color: root.lineType != TimeStep.SMALLEST? "#b8b8b8":"transparent"
        horizontalAlignment: Text.AlignHCenter
        opacity: 1-parent.lineType*0.25

        Behavior on opacity {
            PropertyAnimation{
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }

    function getHeightFromType(type) : real {
        switch(type){
        case TimeStep.HIGHEST:
            return 15
        case TimeStep.NORMAL:
            return 11
        case TimeStep.SMALL:
            return 7
        case TimeStep.SMALLEST:
            return 3
        default:
            return 0
        }
    }

    function convertMillisecondsToString(duration) {
        // Convert milliseconds to seconds, minutes, hours, and remaining milliseconds


        var totalMilliseconds = duration % 1000;
        if(totalMilliseconds > 0)
            return totalMilliseconds + "ms";

        var totalSeconds = Math.floor((duration % 60000) / 1000);
        if(totalSeconds > 0)
            return totalSeconds + "s";

        var totalMinutes = Math.floor((duration % 3600000) / 60000);
        if(totalMinutes > 0)
            return totalMinutes + "m";

        var totalHours = Math.floor((duration%86400000)/3600000);
        if(totalHours > 0)
            return totalHours + "h";

        var totalDays = Math.floor((duration%604800000)/86400000);
        if(totalDays > 0)
            return totalDays + "d";

        var totalWs = Math.floor(duration / 604800000);
        if(totalWs > 0)
            return totalWs + "w";
    }
}
