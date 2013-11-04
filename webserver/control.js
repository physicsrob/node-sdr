var audio = require('./audio');
var exec            = require('child_process').exec;
var persist = require('node-persist');
persist.initSync();


var currentState = 'idle';
var lastPing = new Date();
var vfoFreq=14e6, tuneFreq=14e6;


//audio.setFakeSource(1500.0);


function updateTuneFreq() {
    var offset = vfoFreq - tuneFreq;
    if(currentState=='RX') {
        audio.setTuner(offset);
    } else if(currentState=='TX') {
        audio.setTuner(-offset);
    }

}

function updateState(newState) {
    var config = persist.getItem('config') || {};

    // Update IQ balance
    audio.setIQBal(config.iq_tx_alpha, config.iq_tx_epsilon, config.iq_rx_alpha, config.iq_rx_epsilon);

    if(currentState==newState) { return; } // Do nothing if the state hasn't changed
    if(currentState=='RX') {
        audio.stop();
    }
    if(currentState=='TX') {
        audio.stop();
        exec("../softrock-ctrl/softrock-ctrl.py setptt off", function(err, stdout, stderr) { });
    }
    if(newState=='RX') {
        audio.setMode(audio.MODE_USB_RX);
        audio.start(config.iq_input, config.usb_output);
    }
    if(newState=='TX') {
        audio.setMode(audio.MODE_USB_TX);
        audio.start(config.usb_input, config.iq_output);
        exec("../softrock-ctrl/softrock-ctrl.py setptt on", function(err, stdout, stderr) { });
    }
    currentState = newState;
    updateTuneFreq();
}

function getFFT() {
    if(currentState=='RX') {
        return audio.getFFT(true);
    } else {
        return audio.getFFT(false);
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


