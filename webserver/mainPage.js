var audio = require('./audio');

module.exports = function(app) {
    app.get('/', function(req, res) {
        res.render('index', {sampleRate: audio.getSampleRate()});
    });
};


