// http for server
var http = require('http');
// consumer is library that has helper function addFile and getFiles
// both of these functions return promises
var consumer = require('./lib/consumer.js');

var server = http.createServer(function (req, res) {
  'use strict';
  if (req.method === 'POST' && req.url === '/karaoke/song') {
    consumer.addFile(req)
    .then(function () {
      res.end();
    });
  }
  else if (req.method === 'GET' && req.url === '/feed') {
    consumer.getFiles()
    .then(function (files) {
      res.writeHead(200, {'Content-Type': 'application/json'});
      res.end(JSON.stringify(files));
    });
  }
});

server.listen(8080, '0.0.0.0');