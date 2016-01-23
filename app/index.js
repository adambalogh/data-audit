
var remote = require('remote'); 
var dialog = remote.require('dialog');  
var upload = require('bindings')('upload');
var verify = require('bindings')('verify');
var file_browser = require('bindings')('file_browser');

window.onload = function() {

  // Initially, show all the files available
  file_browser.get_files(function(files) {
    displayFiles(files);
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

// Displays the given list of files
function displayFiles(files) {
  var files_list = document.getElementById("files");
  while (files_list.firstChild) {
    files_list.removeChild(files_list.firstChild);
  }
  files.forEach(function(file) {
    var div = document.createElement("div");
    div.appendChild(document.createTextNode(file));
    div.onclick = function() {
      verify_file(this.innerText);
    }

    var li = document.createElement("li");
    li.appendChild(div);
    li.setAttribute("class", "file");

    files_list.appendChild(li);
  });
}

// Verifies the given file's integrity, and displays the result
function verify_file(file_name) {
  verify.verify(file_name, function(result) {
    alert(result);
  });
}
