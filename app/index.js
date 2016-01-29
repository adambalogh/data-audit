var remote = require('remote'); 
var dialog = remote.require('dialog');  
var verify = require('bindings')('verify')
var file_browser = require('bindings')('file_browser')
const BrowserWindow = require('electron').remote.BrowserWindow;


window.onload = function() {

  // Initially, show all the files available
  file_browser.get_files(function(files) {
    displayFiles(files);
  });

  var uploadButton = document.getElementById("upload");
  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      if (files == undefined || files.length == 0) return;

      var uploadWin = new BrowserWindow({
        width: 540,
        height: 160,
        title: "Uploading File",
        resizable: false,
        maximizable: false,
        minimizable: false,
        fullscreen: false,
        closable: false,
      });
      uploadWin.loadURL('file://' + __dirname + '/upload.html');

      uploadWin.webContents.on('did-finish-load', function() {
        uploadWin.webContents.send('fileName', files[0]);
      });

      uploadWin.on('closed', function() {
        file_browser.get_files(function(files) {
          displayFiles(files);
        });
      });

      uploadWin.show();
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
    var icon = document.createElement("span");
    icon.setAttribute("class", "octicon octicon-file-text icon");
    
    var div = document.createElement("div");
    div.appendChild(icon);
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
