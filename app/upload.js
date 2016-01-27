var upload = require('bindings')('upload');

window.ready = false;

window.onload = function() {

  var closeButton = document.getElementById("close");
  closeButton.onclick = function() {
    window.close();
  }

  require('electron').ipcRenderer.on('fileName', function(event, fileName) {
    var title = document.getElementById("title");
    title.appendChild(document.createTextNode("Uploading \"" + fileName + "\""));

    upload.uploadAsync(fileName, function(stats, error) {
      if (error != null) {
        alert(error);
      } else {
        alert(new Stats(stats).String());
      }
      window.ready = true;
      var closeButton = document.getElementById("close").style.display = "inline-block";
    });


  }); // on fileName

}

// Only close when file has been uploaded
window.onbeforeunload = function(e) {
  e.returnValue = window.ready;
};

var Stats = function (properties) {
  this.fileSize = properties[0];
  this.fileTagSize = properties[1];
  this.blockTagsSize = properties[2];
  this.String = function() {
    return "File Size: " + MBs(this.fileSize) + " MB\n" +
           "File Tag Size: " + MBs(this.fileTagSize) + " MB\n" +
           "Block Tag Size: " + MBs(this.blockTagsSize) + " MB\n"; 
  }
}

// Returns number of Megabytes the given bytes are equal to, rounded to 4
// decimal places
function MBs(bytes) {
  return Math.round(bytes / 1000 / 1000 * 1000) / 1000;
}
