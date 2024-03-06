
  var Socket;
  
  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event) {
      processCommand(event);
    };
  }
  
  function processCommand(event) {
		var obj = JSON.parse(event.data);
  	var type = obj.type;
		if(type.localeCompare(\"cmd_time\") == 0) { 
			var val = parseInt(obj.value);
			document.getElementById('conttime').innerHTML = val;
		}
		else {
			console.log(type);
		}
  }
  window.onload = function(event) {
    init();
  }