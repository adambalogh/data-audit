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
	
	// Set up menu highlight mechanism
	var menuItems = document.getElementsByClassName("menu-item");
	Array.prototype.forEach.call(menuItems, function(e) {
		e.addEventListener('click', function() {
			selectMenu(e.id)
		});
	});
	
	var recentFilesButton = document.getElementById("recent-files");
	recentFilesButton.addEventListener('click', function() {
		document.getElementById("main").style.display = "block";
		document.getElementById("settings-window").style.display = "none";
		
		native_module.getRecentFiles(function (files) {
			displayFiles(files);
		});
	});
	
	var allFilesButton = document.getElementById("all-files");
	allFilesButton.addEventListener('click', function() {
		document.getElementById("main").style.display = "block";
		document.getElementById("settings-window").style.display = "none";
	
		native_module.getFiles(function (files) {
			displayFiles(files);
		});
	});
	
	var settingsButton = document.getElementById("settings");
	settingsButton.addEventListener("click", function() {
		document.getElementById("main").style.display = "none";
		document.getElementById("settings-window").style.display = "block";
	});

  var uploadButton = document.getElementById("upload");
	
  uploadButton.onclick = function() {
    dialog.showOpenDialog(function(files) {
      if (files == undefined || files.length == 0) return;
      var file = files[0];
			
  		swal({
  			title:"", 
				text: "Uploading " + file + "<br><br><div id='progress'></div>", 
				showConfirmButton: false,
				animation: 'false',
				html: true,
  		});		
			
		  var line = new ProgressBar.Line('#progress', {
		    strokeWidth: 1,
		    duration: 100,
		    color: '#2980b9'
		  });

      native_module.uploadAsync(
        file,
        function(progress) {
		      line.animate(progress / 100.0);
        },
        function(error) {
					if (error) {
			  		swal({
							title: "",
							text: "Failed to upload " + file + "<br>" + error, 
							animation: 'false',
							confirmButtonColor: '#3498db',
							type: "error",
			  		});	
					} else {
			  		swal({
							title: "",
							text: file + " successfully uploaded", 
							animation: 'false',
							confirmButtonColor: '#3498db',
							type: "success",
			  		});	
					}	
	      	native_module.refreshFiles(function(files) {
	        	displayFiles(files);
	      	});
				}
      );

    }); // showopendialog
  } // onclick

  var searchBar = document.getElementById("search");
  searchBar.oninput = function() {
		selectMenu("all-files");
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

  var index = 0;
  files.forEach(function(file) {
    index += 1;

    var fileName = file[0];
    var fileSize = file[1];

    var indexSpan = document.createElement("span");
    indexSpan.appendChild(document.createTextNode(index));
    indexSpan.setAttribute("class", "index");
    indexSpan.setAttribute("id", "file_" + index);
    indexSpan.setAttribute("data-name", fileName);

    var infoSpan = document.createElement("span");
    infoSpan.appendChild(document.createTextNode(fileSize));
    infoSpan.setAttribute("class", "icon-info");
  
    var verifyButton = document.createElement("span");
    verifyButton.setAttribute("class", "verify-button");
    verifyButton.setAttribute("data-file", fileName); 
    verifyButton.appendChild(document.createTextNode("Verify"));
    verifyButton.onclick = function() {
			verifyButton.className += " under-verification";
      verify_file(this.getAttribute("data-file"), function() {
        verifyButton.classList.remove("under-verification");
      });
    }
    
     var fileNameLabel = document.createElement("span");
    fileNameLabel.setAttribute("class", "file-name-label");
    fileNameLabel.appendChild(indexSpan);
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
function verify_file(file_name, callback) {
  native_module.verifyAsync(file_name, function(percentage) {}, function(result, error) {
		callback();
    if (error) {
      alert(error);
    } else {
    	if (result == true) {
    		swal({
    			title:"", 
					text: file_name + " successfully verified", 
					type: "success",
					animation: 'false',
					confirmButtonColor: '#3498db'
    		});
    	} else {
    		swal({
    			title:"", 
					text: file_name + " is corrupted", 
					type: "error",
					animation: 'false'
    		});
    	}
    }
  });
}

function selectMenu(id) {
	var selected = document.getElementsByClassName("selected");
	Array.prototype.forEach.call(selected, function(e) {
		e.classList.remove("selected");
	});
	var menu = document.getElementById(id);
	menu.classList.add("selected");
}

function isInt(n){
    return Number(n) === n && n % 1 === 0;
}

// Taken from http://stackoverflow.com/a/12014376/3430265
var Small = {
    'zero': 0,
    'one': 1,
    'two': 2,
    'three': 3,
    'four': 4,
    'five': 5,
    'six': 6,
    'seven': 7,
    'eight': 8,
    'nine': 9,
    'ten': 10,
    'eleven': 11,
    'twelve': 12,
    'thirteen': 13,
    'fourteen': 14,
    'fifteen': 15,
    'sixteen': 16,
    'seventeen': 17,
    'eighteen': 18,
    'nineteen': 19,
    'twenty': 20,
    'thirty': 30,
    'forty': 40,
    'fifty': 50,
    'sixty': 60,
    'seventy': 70,
    'eighty': 80,
    'ninety': 90
};

var Magnitude = {
    'thousand':     1000
};

var a, n, g;

function text2num(s) {
    a = s.toString().split(/[\s-]+/);
    n = 0;
    g = 0;
    a.forEach(feach);
    return n + g;
}

function feach(w) {
    var x = Small[w];
    if (x != null) {
        g = g + x;
    }
    else if (w == "hundred") {
        g = g * 100;
    }
    else {
        x = Magnitude[w];
        if (x != null) {
            n = n + g * x
            g = 0;
        }
        else { 
            throw("Unknown number: "+w); 
        }
    }
};
