const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;

var mainWindow = null;

app.on('window-all-closed', function() {
  app.quit();
});

app.on('ready', function() {
  mainWindow = new BrowserWindow({
    width: 900,
    height: 500,
    resizable: false,
    fullscreen: false,
    fullscreenable: false,
    title: "File Check",
  });
  mainWindow.loadURL('file://' + __dirname + '/index.html');

  // TODO disable for Release
  //mainWindow.webContents.openDevTools();
  mainWindow.on('closed', function() {
    mainWindow = null;
  });
});
