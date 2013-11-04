var audio = require('./audio');
var _ = require('underscore');
var persist = require('node-persist');
persist.initSync();

module.exports = function(app) {
    app.get('/config', function(req, res) {
        var config = persist.getItem('config') || {};
        res.render('config', {
            _: _,
            audio_devices: audio.getAudioDevices(),
            config: config
        });
    });
    app.post('/config', function(req, res) {
        var config = req.body;
        persist.setItem('config', config);
        audio.setIQBal(config.iq_tx_alpha, config.iq_tx_epsilon, config.iq_rx_alpha, config.iq_rx_epsilon);
        res.send("OK", 200);
    });
};


