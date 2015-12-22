window.onload = function() {

  var remote = require('remote'); 
  var dialog = remote.require('dialog');  
  var upload = require('bindings')('upload');

  var uploadButton = document.getElementById("upload");
  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      upload.upload(files[0], function(a) {
        alert(a);
      });
    });
  }

}

