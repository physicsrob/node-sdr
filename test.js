var nodesdr = require('./build/Release/nodesdr.node')
console.log("Devices", nodesdr.getAudioDevices());
nodesdr.startRX('Built-in Microph', 'Built-in Output');
setTimeout(function() {
    console.log("Closing up2.");
    nodesdr.stopRX();
}, 30*1000);

