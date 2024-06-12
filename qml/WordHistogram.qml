import QtQuick

Item {
    Canvas {
        id: histogram
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var wordCountList = fileProcessor.wordCountList;
            if (wordCountList.length === 0)
                return;

            var barWidth = width / wordCountList.length;
            var barHeightS = (height - 48) / wordCountList[0].count;

            for (var i = 0; i < wordCountList.length; i++) {
                var word = wordCountList[i].word;
                var count = wordCountList[i].count.toString();
                var barHeight = count * barHeightS;

                ctx.fillStyle = "black";
                ctx.fillRect(i * barWidth, height - barHeight, barWidth - 5, barHeight);

                ctx.font = "8px sans-serif";
                ctx.fillText(count, i * barWidth + barWidth / 2 - ctx.measureText(count).width / 2, height - barHeight - 10);
                ctx.font = "bold 12px sans-serif";
                ctx.fillText(word, i * barWidth + barWidth / 2 - ctx.measureText(word).width / 2, height - barHeight - 25);
            }
        }

        Connections {
            target: fileProcessor
            onWordCountChanged: {
                histogram.requestPaint();
            }
        }
    }
}
