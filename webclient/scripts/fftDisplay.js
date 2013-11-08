function getColor(x) {
}

function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
        x: evt.clientX - rect.left,
        y: evt.clientY - rect.top
    };
}


define([], function() {
    return function(controller) {
        var self=this;
        self.$canvas = $("#fftCanvas");
        self.width=this.$canvas.width();
        self.height=this.$canvas.height();

        /* Initialize the canvas */
        var ctx = self.context = self.$canvas[0].getContext("2d");
        var width = self.width, height=self.height;
        var img = ctx.getImageData(0,0,width, height);

        // Draw default black canvas with blue stripe
        var output = ctx.createImageData(width, height);
        for(var x=0;x<width; x++) {
            for(var y=0;y<height; y++) {
                output.data[4*(y*width + x) + 0] = 0;
                output.data[4*(y*width + x) + 1] = 0;
                output.data[4*(y*width + x) + 2] = 0;
                output.data[4*(y*width + x) + 3] = 255;
                if(x==(width/2)) { output.data[4*(y*width + x) + 2] = 255; }
            }
        }
        ctx.putImageData(output, 0, 0);
        /* End canvas initialization */


        // Add timer to add rows of data
        setInterval(function() {
            $.get("/fft", function(fftData) {
                self.addRow(fftData);
            });
        }, 100);


        self.addRow = function(data) {
            var context = self.context, width=self.width, height=self.height;
            var imageData = context.getImageData(0, 0, width, height-1);
            
            if(width>data.length) { width=data.length; }
            var rowImg = context.createImageData(width, 1);

            var tunedLeft = (width/2) + width * 1000.0 * self.freqInfo.offset / self.freqInfo.sampleRate;
            var tunedRight = tunedLeft + width * 1000.0 * self.freqInfo.tunerWidth / self.freqInfo.sampleRate;

            for(var i=0;i<width;i++) {
                rowImg.data[4*i + 0] = data[i]*2.5;
                rowImg.data[4*i + 1] = data[i]*2.5;
                rowImg.data[4*i + 2] = data[i]*2.5;
                rowImg.data[4*i + 3] = 255;
                if(i>tunedLeft && i<tunedRight) {
                    rowImg.data[4*i + 0] += 10;
                    rowImg.data[4*i + 1] += 90;
                }
            }

            var c = data.length/2;
            rowImg.data[4*c + 0] = 0;
            rowImg.data[4*c + 1] = 0;
            rowImg.data[4*c + 2] = 255;
            rowImg.data[4*c + 3] = 255;

            context.putImageData(imageData, 0, 1);
            context.putImageData(rowImg, 0,0);
        }

        self.freqInfo = {
            vfo: 14050,
            sampleRate: window.sampleRate,
            offset: 0,
            tunerWidth: 3.0
        };



        // Add mouse over handler
/*        c.mousemove(function(e){
            var pos = getMousePos(c[0], e);
            console.dir(pos);
        });*/
    };
});

