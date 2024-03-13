const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>FDRS Gateway</title>
</head>
<body style='background-color: #EEEEEE;'>

<span style='color: #003366;'>

<h1>FDRS Gateway 0x<span id="ctrlrmac"></span></h1>
IP: <span id='ctrlrip'></span>

<form>
  <div>
  <p>Debug Level:</p>
  <input type="radio" id="debugNone" name="debugNone" value="Debug Off">
  <label for="debugNone">Debug Off</label><br>
  <input type="radio" id="debug0" name="debug0" value="Debug Level 0">
  <label for="debug0">Debug Level 0</label><br>
  <input type="radio" id="debug1" name="debug1" value="Debug Level 1">
  <label for="debug1">Debug Level 1</label><br>
  <input type="radio" id="debug2" name="debug2" value="Debug Level 2">
  <label for="debug2">Debug Level 2</label>
	<br>
  <br>
  <fieldset>
    <legend>Time Set:</legend>
    PC Time: <span id='pctime'></span>
    <br>
    Controller Time: <span id='ctrlrtime'></span>
    <br>
	<input type='button' id='butsettime' name='butsettime' value='Set Time' onclick='btnTimePressed()'>
    </fieldset>
    <br><br>
   <fieldset>
    <legend>Debug Messages</legend>
    <dl id="debug"><dt></dt></dl>
  </fieldset>
  </div>
</form>

<script>
  var Socket;
  var espTime = new Date();

  document.getElementById('debugNone').addEventListener('click', function() { debug_changed(0) });
  document.getElementById('debug0').addEventListener('click', function() { debug_changed(1) });
  document.getElementById('debug1').addEventListener('click', function() { debug_changed(2) });
  document.getElementById('debug2').addEventListener('click', function() { debug_changed(3) });

  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + '/ws');
    Socket.onmessage = function(event) { processCommand(event); };
  }
  
  function debug_changed(debug_level) {
    switch(debug_level) {
      case 0:
        document.getElementById('debugNone').checked = true;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = false;
        break;
      case 1:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = true;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = false;
        break;
      case 2:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = true;
        document.getElementById('debug2').checked = false;
        break;
      case 3:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = true;
        break;
      default:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = false;
        break;
    }
    // console.log(debug_level);
    var msg = { type: "cmd_dbg_level", value: debug_level };
	  Socket.send(JSON.stringify(msg)); 
  }

  function processCommand(event) {
		var obj = JSON.parse(event.data);
  	var type = obj.type;
		if(type.localeCompare("cmd_time") == 0) {
      obj.value = obj.value * 1000; 
			espTime = Date(obj.value);
			document.getElementById('ctrlrtime').innerHTML = espTime;
		}
    if(type.localeCompare("cmd_mac") == 0) {
     	document.getElementById('ctrlrmac').innerHTML = obj.value;
    }
    if(type.localeCompare("cmd_ip") == 0) {
     	document.getElementById('ctrlrip').innerHTML = obj.value;
    }
    if(type.localeCompare("cmd_dbg_level") == 0) {
      switch(obj.value) {
        case 0:
          document.getElementById('debugNone').checked = true;
          document.getElementById('debug0').disabled = true;
          document.getElementById('debug1').disabled = true;
          document.getElementById('debug2').disabled = true;
          break;
        case 1:
          document.getElementById('debug0').checked = true;
          document.getElementById('debug1').disabled = true;
          document.getElementById('debug2').disabled = true;
          break;
        case 2:
          document.getElementById('debug1').checked = true;
          document.getElementById('debug2').disabled = true;
          break;
        case 3:
          document.getElementById('debug2').checked = true;
          break;
        default:
          document.getElementById('debug2').checked = true;
          break;
      }
    }
    if(type.localeCompare("cmd_dbg") == 0) {
      let msg = obj.value
      showDebug(msg);
    }
		else {
			console.log(type);
		}
  }

  setInterval(pcDate,1000); // Update PC time every second
  setInterval(ctrlrDate,1000); // Update Controller time every second
  
  function pcDate() {
    document.getElementById('pctime').innerHTML = Date();
  }
  function ctrlrDate() {
    const ms = espTime.valueOf();
    if(ms > 19000000) {
      espTime.setTime(ms + 1000);
      document.getElementById('ctrlrtime').innerHTML = espTime;
    }
  }
  function btnTimePressed() {
  	let l_time = Date.now();
  	console.log(l_time);
    l_time = l_time/1000;
  	var msg = {type: "cmd_time", value: l_time.toString()};
  	Socket.send(JSON.stringify(msg));
  }
  function showDebug(msg) {
    console.log(msg);
    if(document.getElementById("debug").childElementCount > 20) {
      const list = document.getElementById("debug");
      list.removeChild(list.firstElementChild);      
    }
    const node = document.createElement("dt");
    const textnode = document.createTextNode(msg);
    node.appendChild(textnode);
    document.getElementById("debug").appendChild(node);
  }
  
  window.onload = function(event) {
      init();
    }
</script>
</body>
</html>
)=====";