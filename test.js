var nodesdr = require('./build/Release/nodesdr.node')
console.log("Devices", nodesdr.getAudioDevices());
nodesdr.startRX('Built-in Input', 'Built-in Output');

setInterval(function() {
    console.log("FFT", nodesdr.getFFT());
},500.0);

setTimeout(function() {
    console.log("Closing up2.");
    nodesdr.stopRX();
}, 30*1000);

