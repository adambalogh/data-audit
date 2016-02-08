var electron = require('electron');

window.ready = false;

window.onload = function() {

  window.fileName = "";

  var closeButton = document.getElementById("close");
  closeButton.onclick = function() {
    window.close();
  }

  var line = new ProgressBar.Line('#progress', {
    strokeWidth: 2,
    duration: 100,
    color: '#f39c12'
  });

  electron.ipcRenderer.on('fileName', function(event, fileName) {
    window.fileName = fileName;
    var title = document.getElementById("title");
    title.firstChild.textContent = "Verifying \"" + window.fileName + "\"...";
  });

  electron.ipcRenderer.on('error', function(event, error) {
    finished("Error: " + error);
  });

  electron.ipcRenderer.on('result', function(event, result) {
    if (result === true) {
      title.style.color = "#27ae60";
      finished(window.fileName + " is intact");
    } else {
      title.style.color = "#FF0000";
      finished(window.fileName + " is corrupted.");
    }
  });
}

function finished(text) {
  window.ready = true;
  var closeButton = document.getElementById("close").style.display = "inline-block";
  var title = document.getElementById("title");
  title.firstChild.textContent = text;
}
