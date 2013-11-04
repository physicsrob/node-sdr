
/**
 * Module dependencies.
 */

var express         = require('express');
var path            = require("path");
var fs              = require("fs");

var app = module.exports = express.createServer();


app.configure('development', function() {
    //app.use(express.logger({ format: ':method :url' }));
    app.use(express.errorHandler({ dumpExceptions: true, showStack: true }));
    app.use(express.static(__dirname + '/../webclient'));
    app_mode = "dev";
    buildver = "";
});
app.configure(function() {
    app.set('views', __dirname + '/templates');
    app.set('view engine', 'ejs');
    app.use(express.bodyParser());
    app.use(express.methodOverride());
    app.use(express.cookieParser());
    app.use(express.favicon(__dirname + '/public/favicon.ico'));
    app.use(app.router);
});


process.on('uncaughtException', function (err) {
  console.log('Caught unhandled exception: ', err);
  console.log("Stack: ", err.stack);
});


require ('./pages')(app);
require ('./control')(app);

app.listen(5555);
console.log("Express server listening on port %d in %s mode", app.address().port, app.settings.env);

module.exports = app;


