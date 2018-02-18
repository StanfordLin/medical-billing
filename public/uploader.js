var xhr = new XMLHttpRequest();
var post = new XMLHttpRequest();
var formData = new FormData();
//post.onreadystatechange = function () {
//    if (post.readyState === 4) {
//      document.getElementById()
//    }
//}
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
    // Loop through each of the selected files.
    for (var i = 0; i < files.length; i++) {
      var file = files[i];
      // Check the file type.
      if (!file.type.match('image.*')) {
        continue;
      }
      // Add the file to the request.
      formData.append('photos[]', file, file.name);
    }
    // opening with false makes call blocking and prevents race condition
    post.open('POST', 'handler.php', false);
    post.send(formData);
}
post.onload = function () {
    if (post.status === 200) {
        document.getElementById('postButton').innerHTML = 'upload';
    } else {
        alert('blah');
    }
}