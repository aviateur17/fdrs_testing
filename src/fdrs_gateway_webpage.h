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
  <p>Compile debug level: <span id=debugLevel></span></p>
  <p>Runtime debug level:</p>
  <input type="radio" id="debugNone" name="debugNone" value="Debug Off">
  <label for="debugNone">Debug Off</label><br>
  <input type="radio" id="debug0" name="debug0" value="Debug Level 0">
  <label for="debug0">Debug Level 0</label><br>
  <input type="radio" id="debug1" name="debug1" value="Debug Level 1">
  <label for="debug1">Debug Level 1</label><br>
  <input type="radio" id="debug2" name="debug2" value="Debug Level 2">
  <label for="debug2">Debug Level 2</label>
</form>
	<br>
  <br>
<form>
  <fieldset style="width:550px;">
    <legend>Time Set:</legend>
    PC Time: <span id='pctime'></span>
    <br>
    Controller Time: <span id='ctrlrtime'></span>
    <br><br>
	  <input type='button' id='butsettime' name='butsettime' value='Set Time' onclick='btnTimePressed()'>
  </fieldset>
</form>
  <br><br>
<form>
  <fieldset style="width:550px; height:600px;">
    <legend>Debug Messages</legend>
    <dl id="debug"><dt></dt></dl>
  </fieldset>
</form>
  <br><br>
<form>
  <fieldset style="width:550px;">
    <legend>Adjustable Parameters</legend>
    <label for="printTime">Print Time Interval:</label>
    <input type="text" id="printTime" name="printTime" size="10" required min="0" max="5000">[0 - 5000 Mins]
    <input type="submit" value="Submit"><br>
    <label for="espNowNbr1">ESP-NOW Neighbor 1:</label>
    <input type="text" id="espNowNbr1" name="espNowNbr1" size="10" required min="0x00" max="0xFF">[0x00 - 0xFF]
    <input type="submit" value="Submit"><br>
    <label for="espNowNbr2">ESP-NOW Neighbor 2:</label>
    <input type="text" id="espNowNbr2" name="espNowNbr2" size="10" required min="0x00" max="0xFF">[0x00 - 0xFF]
    <input type="submit" value="Submit"><br>
    <label for="loraNbr1">LoRa Neighbor 1:</label>
    <input type="text" id="loraNbr1" name="loraNbr1" size="10" required min="0x00" max="0xFF">[0x00 - 0xFF]
    <input type="submit" value="Submit"><br>
    <label for="loraNbr2">LoRa Neighbor 2:</label>
    <input type="text" id="loraNbr2" name="loraNbr2" size="10" required min="0x00" max="0xFF">[0x00 - 0xFF]
    <input type="submit" value="Submit"><br>
  </fieldset>
</form>

<script>
  var Socket;
  var espTime;

  document.getElementById('debugNone').addEventListener('click', function() { debug_changed(-1) });
  document.getElementById('debug0').addEventListener('click', function() { debug_changed(0) });
  document.getElementById('debug1').addEventListener('click', function() { debug_changed(1) });
  document.getElementById('debug2').addEventListener('click', function() { debug_changed(2) });

  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + '/ws');
    Socket.onmessage = function(event) { processCommand(event); };
  }
  
  function debug_changed(debug_level) {
    switch(debug_level) {
      case -1:
        document.getElementById('debugNone').checked = true;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = false;
        break;
      case 0:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = true;
        document.getElementById('debug1').checked = false;
        document.getElementById('debug2').checked = false;
        break;
      case 1:
        document.getElementById('debugNone').checked = false;
        document.getElementById('debug0').checked = false;
        document.getElementById('debug1').checked = true;
        document.getElementById('debug2').checked = false;
        break;
      case 2:
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
      // console.log(obj.value); 
			espTime = new Date(obj.value);
			document.getElementById('ctrlrtime').innerHTML = espTime;
		}
    if(type.localeCompare("printTime") == 0) {
      document.getElementById('printTime').value = obj.value;
    } 
    if(type.localeCompare("espNowNbr1") == 0) {
      document.getElementById('espNowNbr1').value = obj.value.toString(16);
    } 
    if(type.localeCompare("espNowNbr2") == 0) {
      document.getElementById('espNowNbr2').value = obj.value;
    } 
    if(type.localeCompare("loraNbr1") == 0) {
      document.getElementById('loraNbr1').value = obj.value;
    } 
    if(type.localeCompare("loraNbr2") == 0) {
      document.getElementById('loraNbr2').value = obj.value;
    } 
    if(type.localeCompare("cmd_mac") == 0) {
     	document.getElementById('ctrlrmac').innerHTML = obj.value;
    }
    if(type.localeCompare("cmd_ip") == 0) {
     	document.getElementById('ctrlrip').innerHTML = obj.value;
    }
    if(type.localeCompare("cmd_dbg_level") == 0) {
      // console.log(obj.value);
      document.getElementById('debugLevel').innerHTML = obj.value;
      switch(obj.value) {
        case "-1":
          document.getElementById('debugNone').checked = true;
          document.getElementById('debug0').disabled = true;
          document.getElementById('debug1').disabled = true;
          document.getElementById('debug2').disabled = true;
          break;
        case "0":
          document.getElementById('debug0').checked = true;
          document.getElementById('debug1').disabled = true;
          document.getElementById('debug2').disabled = true;
          break;
        case "1":
          document.getElementById('debug1').checked = true;
          document.getElementById('debug2').disabled = true;
          break;
        case "2":
          document.getElementById('debug2').checked = true;
          break;
        default:
          document.getElementById('debug0').disabled = true;
          document.getElementById('debug1').disabled = true;
          document.getElementById('debug2').disabled = true;
          document.getElementById('debugNone').disabled = true;
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
    if(ms > 1710377241) {
      espTime.setTime(ms + 1000);
      document.getElementById('ctrlrtime').innerHTML = espTime;
    }
  }
  function btnTimePressed() {
  	let l_time = Date.now();
  	// console.log(l_time);
    l_time = l_time/1000;
  	var msg = {type: "cmd_time", value: l_time.toString()};
  	Socket.send(JSON.stringify(msg));
  }
  function showDebug(msg) {
    // console.log(msg);
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