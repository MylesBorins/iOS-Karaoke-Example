// Core Libs
var path = require('path');
var util = require('util');
// lodash for map function
var _ = require('lodash');
// q for promises
var q = require('q');
// level for db
var level = require('level');
var db = level('./.mydb', {valueEncoding : 'json'});
// multiparty for multipar forms
var multiparty = require('multiparty');

var addFile = function (req) {
  'use strict';
  var deferred = q.defer();
  var form = new multiparty.Form();
  var ws = db.createWriteStream();
  
  ws.on('error', function (err) {
    deferred.reject(err);
  });
  ws.on('close', function () {
    deferred.resolve();
  });
  
  form.parse(req, function (err, fields, files) {
    var soundFile = {
/*      name: fields.name[0],*/
/*      udid: fields.udid[0],*/
/*      long: fields.long[0],*/
/*      lat: fields.lat[0],*/
/*      likes: 0,*/
      path: files.soundfile[0].originalFilename,
      pub_date: new Date().toISOString()
/*      description: fields.description[0]*/
    };

    ws.write({key: soundFile.name, value: soundFile});
    ws.end();
  });
  return deferred.promise;
};

exports.addFile = addFile;

var getFiles = function () {
  'use strict';
  var deferred = q.defer();
  var files = [];
  db.createReadStream().on('data', function (data) {
    files.push(data.value);
  })
  .on('end', function () {
    var count = 0;
    files = _.map(files, function (file) {
      file = {
        pk: count,
        model: 'soundshare.sound',
        fields: file
      };
      count++;
      return file;
    });
    
    deferred.resolve(files);
  });
  return deferred.promise;
};

exports.getFiles = getFiles;