// Entry point to the WebSocket interface.
var gateway = `ws://${window.location.hostname}/ws`;

// WebSocket??
var websocket;

// Event Listeners.
window.addEventListener('load', onLoad);

// Initializes a WebSocket connection on the gateway and registers callbacks.
function initWebSocket() {
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen    = onOpen;
  websocket.onclose   = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
}

function onClose(event) {
  console.log('Connection closed');
  // If the connectionn closses, reattempt after 2 seconds.
  setTimeout(initWebSocket, 2000);
}

// Handle the message from the webserver.
function onMessage(event) {
  let data = JSON.parse(event.data);
  for (const property in data) {
    document.getElementById(property).value = data[property];
  }
}

function onLoad(event) {
  // Initialize a WebSocket connection with the server.
  initWebSocket();

  var els = document.querySelectorAll("input, select");
  for (var i = 0; i < els.length; i++) 
    els[i].setAttribute( "onchange", "handleInputChange(this)" );
}

function handleInputChange(elem) {
  let obj = {};
  obj[elem.id] = elem.value;
  let str = JSON.stringify(obj)
  console.log(str);
  websocket.send(str);
}