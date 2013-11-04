function getColor(x) {
}

function addRow(context, data, width, height) {
    var imageData = context.getImageData(0, 0, width, height-1);
    
    if(width>data.length) { width=data.length; }
    var rowImg = context.createImageData(width, 1);
    for(var i=0;i<width;i++) {
        rowImg.data[4*i + 0] = data[i]*2.5;
        rowImg.data[4*i + 1] = data[i]*2.5;
        rowImg.data[4*i + 2] = data[i]*2.5;
        rowImg.data[4*i + 3] = 255;
    }

    context.putImageData(imageData, 0, 1);
    context.putImageData(rowImg, 0,0);
}

define([], function() {
    var c=$("#fftCanvas");
    var ctx=c[0].getContext("2d");
    var width = c[0].width, height=c[0].height;
    var img = ctx.getImageData(0,0,width, height);
    var output = ctx.createImageData(width, height);
    for(var x=0;x<width; x++) {
        for(var y=0;y<height; y++) {
            output.data[4*(y*width + x) + 0] = 0;
            output.data[4*(y*width + x) + 1] = 0;
            output.data[4*(y*width + x) + 2] = 0;
            output.data[4*(y*width + x) + 3] = 255;
        }
    }
    ctx.putImageData(output, 0, 0);


    setInterval(function() {
        $.get("/fft", function(fftData) {
            addRow(ctx, fftData, width, height);
        });
    }, 100);

//ctx.putImageData(imgData,0,0);
    
//    ctx.fillStyle="#FF0000";
//   ctx.fillRect(0,0,150,75);i

});

