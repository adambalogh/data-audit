var remote = require('remote'); 
var dialog = remote.require('dialog');  
var native_module = require('bindings')('native_module')
const BrowserWindow = require('electron').remote.BrowserWindow;


window.onload = function() {

  native_module.hasToLogin(function(need_login) {
    if (!need_login) {
      main();
    } else {

      native_module.getAuthorizeUrl(function(url) {
          remote.require('child_process').exec("open \"" + url + "\"");
        });

        swal({
          title: "Log In",
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
  });
}

// Main called after user is logged in - if needed
function main() {
    
  //Initially, show all the files available
  native_module.getFiles(function(files) {
    var spinner = document.getElementById("spinner");
    spinner.style.display = "none";
    displayFiles(files);
  });

  var uploadButton = document.getElementById("upload");
  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      if (files == undefined || files.length == 0) return;

      var uploadWin = new BrowserWindow({
        width: 540,
        height: 150,
        title: "Uploading File",
        resizable: false,
        maximizable: false,
        minimizable: false,
        fullscreen: false,
        closable: false,
      });
      uploadWin.loadURL('file://' + __dirname + '/upload.html');
      uploadWin.show();

      var file = files[0];
      uploadWin.webContents.on('did-finish-load', function() {
        native_module.uploadAsync(
          file,
          function(progress) {
            uploadWin.webContents.send('progress', progress);

          },
          function(error) {
            uploadWin.webContents.send('finished', error);
          }
        );
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
    var fileName = file[0];
    var fileSize = file[1];

    var infoSpan = document.createElement("span");
    infoSpan.appendChild(document.createTextNode(fileSize));
    infoSpan.setAttribute("class", "icon-info");
  
    var verifyButton = document.createElement("span");
    verifyButton.setAttribute("class", "verify-button");
    verifyButton.setAttribute("data-file", fileName); 
    verifyButton.appendChild(document.createTextNode("Verify"));
    verifyButton.onclick = function() {
      verify_file(this.getAttribute("data-file"));
    }
    
    var icon = document.createElement("span");
    icon.setAttribute("class", "octicon octicon-file-text icon");

    var fileNameLabel = document.createElement("span");
    fileNameLabel.setAttribute("class", "file-name-label");
    fileNameLabel.appendChild(icon);
    fileNameLabel.appendChild(document.createTextNode(fileName));

    var div = document.createElement("div");
    div.appendChild(fileNameLabel);
    div.appendChild(infoSpan);
    div.appendChild(verifyButton);

    var li = document.createElement("li");
    li.appendChild(div);
    li.setAttribute("class", "file");

    files_list.appendChild(li);
  });
}

// Verifies the given file's integrity, and displays the result
function verify_file(file_name) {
  var verifyWin = new BrowserWindow({
    width: 500,
    height: 150,
    title: "Verify " + file_name,
    resizable: false,
    maximizable: false,
    minimizable: false,
    fullscreen: false,
    closable: false,
  });
  verifyWin.loadURL('file://' + __dirname + '/verify.html');
  verifyWin.show();

  verifyWin.webContents.on('did-finish-load', function() {
    verifyWin.webContents.send('fileName', file_name);

    native_module.verifyAsync(file_name, function(percentage) {}, function(result, error) {
      if (error) {
        verifyWin.webContents.send('error', error);
      } else {
        verifyWin.webContents.send('result', result);
      }
    });
  });
}
