window.onload = function() {

  var remote = require('remote'); 
  var dialog = remote.require('dialog');  
  var upload = require('bindings')('upload');
  var verify = require('bindings')('verify');
  var file_browser = require('bindings')('file_browser');

  var files_list = document.getElementById("files");
  file_browser.show_files(function(files) {
    files.forEach(function(file) {
      var li = document.createElement("li");
      li.appendChild(document.createTextNode(file));
      files_list.appendChild(li);
    });
  });

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

