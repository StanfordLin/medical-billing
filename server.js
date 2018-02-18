var express = require('express');
const fileUpload = require('express-fileupload');

var path = require('path');
var app = express();

// Define the port to run on
app.set('port', 8080);

app.use(fileUpload());
app.use(express.static(path.join(__dirname, 'public')));

app.post('/upload', function(req, res) {
  console.log(req.files.sampleFile); // the uploaded file object
});

// Listen for requests
var server = app.listen(app.get('port'), function() {
  var port = server.address().port;
  console.log('Magic happens on port ' + port);
});

//app.post('/upload', function(req, res) {
//  if (!req.files)
//    return res.status(400).send('No files were uploaded.');
// 
//  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
//  let sampleFile = req.files.sampleFile;
// 
//  // Use the mv() method to place the file somewhere on your server
//  sampleFile.mv('.', function(err) {
//    if (err)
//      return res.status(500).send(err);
// 
//    res.send('File uploaded!');
//  });
//});