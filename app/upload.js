window.onload = function() {

  var remote = require('remote'); 
  var dialog = remote.require('dialog');  
  var upload = require('bindings')('upload');
  var verify = require('bindings')('verify');

  var uploadButton = document.getElementById("upload");
  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      upload.upload(files[0], function(a) {
        alert(a);
      });
    });
  }

  var verifyButton = document.getElementById("verify");
  
  verifyButton.onclick = function() {
    var fileName = document.getElementById("input").value;
    verify.verify(fileName, function(result) {
      alert(result);
    });
  }

}

