var nodesdr = require('./build/Release/nodesdr.node')
console.log("Devices", nodesdr.getAudioDevices());
nodesdr.testFilter(0, -20000.0, 20000.0);

