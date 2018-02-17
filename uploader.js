//var form = document.getElementById('file-form');
//var fileSelect = document.getElementById('file-select');
//var uploadButton = document.getElementById('upload-button');
//
//form.onsubmit = function(event) {
//  event.preventDefault();
//
//  // Update button text.
//  uploadButton.innerHTML = 'Uploading...';
//
//  // The rest of the code will go here...
//}
//
//// Get the selected files from the input.
//var files = fileSelect.files;
//
//// Create a new FormData object.
//var formData = new FormData();
//
//// Loop through each of the selected files.
//for (var i = 0; i < files.length; i++) {
//  var file = files[i];
//
//  // Check the file type.
//  if (!file.type.match('image.*')) {
//    continue;
//  }
//
//  // Add the file to the request.
//  formData.append('photos[]', file, file.name);
//}
//
//// Files
//formData.append(name, file, filename);
//
//// Blobs
//formData.append(name, blob, filename);
//
//// Strings
//formData.append(name, value);    
var xhr = new XMLHttpRequest();
xhr.onreadystatechange = function () {
  if (xhr.readyState === 4) {
    document.getElementById('ajax').innerHTML = xhr.responseText;
  }
};
function getAJAXrequest() {
    xhr.open('get', 'test.html');
    xhr.send();
}
function postAJAXrequest() {
    
}