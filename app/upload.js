var electron = require('electron');

// This value indicates if the upload has finished and the window can be closed
window.ready = false;

window.onload = function() {

  var closeButton = document.getElementById("close");
  closeButton.onclick = function() {
    window.close();
  }

  var line = new ProgressBar.Line('#progress', {
    strokeWidth: 1,
    duration: 100,
    color: '#2980b9'
  });

  electron.ipcRenderer.on('fileName', function(event, fileName) {
    var title = document.getElementById("title");
    title.appendChild(document.createTextNode("Uploading file: " + fileName));
    window.fileName = fileName;
  });

  electron.ipcRenderer.on('progress', function(event, percentage) {
    line.animate(percentage / 100.0);
  });

  electron.ipcRenderer.on('finished', function(event, error) {
    window.ready = true;
    var closeButton = document.getElementById("close").style.display = "inline-block";
    var title = document.getElementById("title");
    title.removeChild(title.firstChild);

    if (!error) {
      var icon = document.createElement("span");
      icon.setAttribute("class", "octicon octicon-check icon");

      title.appendChild(icon);
      title.appendChild(document.createTextNode(window.fileName + " successfully uploaded"));
    } else {
      var icon = document.createElement("span");
      icon.setAttribute("class", "octicon octicon-circle-slash icon");

      title.appendChild(icon);
      title.appendChild(document.createTextNode(error));
    }
  });

}

// Only close when file has been uploaded
window.onbeforeunload = function(e) {
  //e.returnValue = window.ready;
  // TODO remove for production
  e.returnValue = true;
};
