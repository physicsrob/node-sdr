define(['fftDisplay', 'controlBar'], function(fftDisplay, controlBar) {
    var currentMode = 'idle'; 
    var currentMode = 'idle';
    var vfoFreq = 14000, tuneFreq=14010;

    $("#configButton").click(function() {
        window.open("/config", "_blank", "width=400,height=500");
    });
    function tune() {
        $.post("/tune", {vfo:vfoFreq, tune:tuneFreq}, function() {
        });
    }
    function updateMode() {
        $.post("/run", {mode:currentMode});
    }

    var controller = {
        changeMode: function(newMode) {
            currentMode = newMode;
            updateMode();
        },
        changeVFO: function(newVFO) {
            tuneFreq += newVFO - vfoFreq; // Shift the tune by the same amount
            vfoFreq = newVFO;
            if(controller.fftDisplay) {
                controller.fftDisplay.freqInfo.vfo = newVFO;
            }
            tune();
        },
        changeTune: function(newTune) {
            tuneFreq = newTune;
            if(controller.fftDisplay) {
                controller.fftDisplay.freqInfo.offset = tuneFreq - vfoFreq;
            }
            tune();
        }
    };

        

    controller.fftDisplay = new fftDisplay(controller);
    controller.controlBar = new controlBar(controller);

    setInterval(function() {
        updateMode();
    }, 1000);

});

