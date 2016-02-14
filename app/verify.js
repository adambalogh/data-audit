var electron = require('electron');

window.ready = false;

window.onload = function() {

  window.fileName = "";

  var closeButton = document.getElementById("close");
  closeButton.onclick = function() {
    window.close();
  }

  electron.ipcRenderer.on('fileName', function(event, fileName) {
    window.fileName = fileName;
    var title = document.getElementById("title");
    title.appendChild(document.createTextNode("Verifying " + window.fileName));
  });

  electron.ipcRenderer.on('error', function(event, error) {
    finished("Error during verification: " + error, false);
  });

  electron.ipcRenderer.on('result', function(event, result) {
    var progress = document.getElementById("progress-loader");
    progress.style.display = 'none';
    if (result === true) {
      finished(window.fileName + " successfully verified.", true);
    } else {
      finished(window.fileName + " is corrupted.", false);
    }
  });
}

function finished(text, successful) {
  window.ready = true;
  var closeButton = document.getElementById("close").style.display = "inline-block";
  var title = document.getElementById("title");
  title.removeChild(title.firstChild);

  if (successful) { 
    var icon = document.createElement("span");
    icon.setAttribute("class", "octicon octicon-check icon");

    title.appendChild(icon);
    title.appendChild(document.createTextNode(text));
  } else {
    var icon = document.createElement("span");
    icon.setAttribute("class", "octicon octicon-circle-slash icon");

    title.appendChild(icon);
    title.appendChild(document.createTextNode(text));

  }
}
