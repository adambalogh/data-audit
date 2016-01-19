window.onload = function() {

  var remote = require('remote'); 
  var dialog = remote.require('dialog');  
  var upload = require('bindings')('upload');
  var verify = require('bindings')('verify');
  var file_browser = require('bindings')('file_browser');

  file_browser.get_files(function(files) {
    displayFiles(files);
   
    var file_buttons = document.getElementsByClassName("file");
    for (var i = 0; i < file_buttons.length; i++) {
      file_buttons[i].onclick = function(e) {
        var fileName = this.innerText;
        verify.verify(fileName, function(result) {
          alert(result);
        });
      }
    }

  });

  var uploadButton = document.getElementById("upload");

  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      upload.upload(files[0], function(a) {
        alert(a);
      });
    });
  }

  var searchBar = document.getElementById("search");
  searchBar.oninput = function() {
    file_browser.get_files(this.value, function(files) {
      displayFiles(files);
    });
  }

}

function displayFiles(files) {
  var files_list = document.getElementById("files");
  while (files_list.firstChild) {
    files_list.removeChild(files_list.firstChild);
  }
  files.forEach(function(file) {
    var div = document.createElement("div");
    div.appendChild(document.createTextNode(file));

    var li = document.createElement("li");
    li.appendChild(div);
    li.setAttribute("class", "file");

    files_list.appendChild(li);
  });
}

function filter(query) {
  alert(query);
}

