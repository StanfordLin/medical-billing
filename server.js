var express = require('express');
const fileUpload = require('express-fileupload');

var path = require('path');
var app = express();

// Define the port to run on
app.set('port', 8080);

app.use(fileUpload());
app.use(express.static(path.join(__dirname, 'public')));

//app.post('/upload', function(req, res) {
//  console.log(req.files.sampleFile.name); // the uploaded file object
//});

app.post('/upload', function(req, res) {
  res.redirect('/updone.html');
  if (!req.files)
    return res.status(400).send('No files were uploaded.');

  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
  let sampleFile = req.files.sampleFile;

  // Use the mv() method to place the file somewhere on your server
  console.log(req.files.sampleFile.name); // the uploaded file object
  sampleFile.mv(__dirname + '/uploads/'+req.files.sampleFile.name, function(err) {
    if (err)
      return res.status(500).send(err);

//    res.send('File uploaded!');

  });
});
//
//app.post('/upload', function(req, res) {
//  // The name of the input field (i.e. "sampleFile") is used to retrieve the uploaded file
//   var startup_image = req.files.foo;
//   var fileName = req.body.fileName;
//   console.log(req.body.filename);
//   // Use the mv() method to place the file somewhere on your server
//   startup_image.mv(__dirname + '/images/' + fileName + '.jpg' , function(err) {
//     if(err){
//       console.log(err);
//     }else{
//    console.log("uploaded");
//}
//   });
// });

// Listen for requests
var server = app.listen(app.get('port'), function() {
  var port = server.address().port;
  console.log('Magic happens on port ' + port);
});
