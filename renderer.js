// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.



// Example code that never clears when in a renderer process
// but does clear when run in the main process

const example = require('bindings')('example');
let myTimeout;
myTimeout = function(timestamp) {
  let myBuffer = example.nextBuffer();
  setTimeout(myTimeout, 20);
}
// 20ms, ~50 times a second
setTimeout(myTimeout, 20);
