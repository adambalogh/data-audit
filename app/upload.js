var upload = require('bindings')('upload');

window.onload = function() {

  require('electron').ipcRenderer.on('fileName', function(event, fileName) {
    var title = document.getElementById("title");
    title.appendChild(document.createTextNode("Uploading \"" + fileName + "\""));

    upload.upload(fileName, function(a) {
      alert(a);
    });

  });

}
