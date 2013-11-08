define(['fftDisplay', 'controlBar'], function(fftDisplay, controlBar) {
    $("#configButton").click(function() {
        window.open("/config", "_blank", "width=400,height=500");
    });
});

