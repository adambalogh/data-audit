var remote = require('remote'); 
var dialog = remote.require('dialog');  
var native_module = require('bindings')('native_module')
const BrowserWindow = require('electron').remote.BrowserWindow;


window.onload = function() {

  native_module.getAuthorizeUrl(function(url) {
    remote.require('child_process').exec("open \"" + url + "\"");
  });

  swal({
    title: "Authentication",
    text: "Please enter the code you received from Dropbox",
    type: "input",
    showCancelButton: false,
    closeOnConfirm: true,
    inputPlaceholder: "Code"
  }, function(code){
    if (code === false) return false;
    if (code === "") {
      swal.showInputError("Please enter the code you received from Dropbox");
      return false;
    }

    native_module.exchangeCodeForToken(code, function() {
      main();
    });
  });

}

function main() {
    
  //Initially, show all the files available
  native_module.getFiles(function(files) {
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

      var file = files[0];
      native_module.uploadAsync(
          file,
          function(progress) {
            uploadWin.webContents.send('progress', progress);

          },
          function(error) {
            uploadWin.webContents.send('finished', error);
          }
      );

      uploadWin.show();

      uploadWin.webContents.on('did-finish-load', function() {
        uploadWin.webContents.send('fileName', file);
      });

      uploadWin.on('closed', function() {
        native_module.refreshFiles(function(files) {
          displayFiles(files);
        });
      });

    });
  }

  var searchBar = document.getElementById("search");
  searchBar.oninput = function() {
    native_module.getFiles(this.value, function(files) {
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
  native_module.verifyAsync(file_name, function(percentage) {}, function(result, error) {
    if (error != null) {
      alert(error);
    }
    alert(result);
  });
}
