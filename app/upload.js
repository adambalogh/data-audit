var electron = require('electron');

// This value indicates if the upload has finished and the window can be closed
window.ready = false;

window.onload = function() {

  var closeButton = document.getElementById("close");
  closeButton.onclick = function() {
    window.close();
  }

  var line = new ProgressBar.Line('#progress', {
    strokeWidth: 3,
    duration: 100
  });

  electron.ipcRenderer.on('fileName', function(event, fileName) {
    var title = document.getElementById("title");
    title.appendChild(document.createTextNode("Uploading \"" + fileName + "\"..."));
  });

  electron.ipcRenderer.on('progress', function(event, percentage) {
    line.animate(percentage / 100.0);
  });

  electron.ipcRenderer.on('finished', function(event, error) {
    if (error != null) {
      title.firstChild.textContent = error;
      title.style.color = "#FF0000";
    } else {
      title.firstChild.textContent = "Successfully Uploaded File";
      title.style.color = "#27ae60";
    }
    window.ready = true;
    var closeButton = document.getElementById("close").style.display = "inline-block";
  });

}

// Only close when file has been uploaded
window.onbeforeunload = function(e) {
  e.returnValue = window.ready;
};
