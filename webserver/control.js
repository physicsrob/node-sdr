var nsdr = require('./nodesdr');
var config = require('./config');
var exec            = require('child_process').exec;

var currentState = 'idle';
var lastPing = new Date();
var vfoFreq=14e6, tuneFreq=14e6;


//nsdr.setFakeSource(1500.0);

function updateTuneFreq() {
    var offset = vfoFreq - tuneFreq;
    if(currentState=='RX') {
        nsdr.setTuner(offset);
    } else if(currentState=='TX') {
        nsdr.setTuner(-offset);
    }
}

function updateState(newState) {
    if(currentState==newState) { return; } // Do nothing if the state hasn't changed
    if(currentState=='RX') {
        nsdr.stop();
    }
    if(currentState=='TX') {
        nsdr.stop();
        exec("./softrock-ctrl.py setptt off", function(err, stdout, stderr) { });
    }
    if(newState=='RX') {
        nsdr.setMode(nsdr.MODE_USB_RX);
        nsdr.start(config.iq_input_device, config.usb_output_device);
    }
    if(newState=='TX') {
        nsdr.setMode(nsdr.MODE_USB_TX);
        nsdr.start(config.usb_input_device, config.iq_output_device);
        exec("./softrock-ctrl.py setptt on", function(err, stdout, stderr) { });
    }
    currentState = newState;
    updateTuneFreq();
}

function getFFT() {
    if(currentState=='RX') {
        return nsdr.getFFT(true);
    } else {
        return nsdr.getFFT(false);
    }
}


setInterval(function() {
    if((new Date() - lastPing) > 5000) {
        // No state update in greater than 5 seconds.  Decay to idle
        updateState('idle');
    }
}, 1000);

module.exports = function(app) {
    app.post('/run', function(req, res) {
        lastPing = new Date();
        updateState(req.body.mode);
        res.send("OK", 200);
    });
    app.post('/tune', function(req, res) {
        vfoFreq= req.body.vfo * 1000.0;
        tuneFreq = req.body.tune * 1000.0;
        updateTuneFreq();
        console.log("Tuning");
        exec("../softrock-ctrl/softrock-ctrl.py setfreq " + vfoFreq, function(err, stdout, stderr) { console.log("Tuning out: ", stdout); });
        res.send("OK", 200);
    });

    app.get('/fft', function(req, res) {
        res.json(getFFT(),200);
    });
};


