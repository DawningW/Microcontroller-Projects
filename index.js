// Import
import "./pure-min.css";
import "./main.css";
import ReconnectingWebSocket from 'reconnecting-websocket';

// WebSocket Client
const urlProvider = function() {
    return document.getElementById("url").value;
};
const options = {
    maxRetries: 3,
    maxEnqueuedMessages: 0,
    startClosed: true
};
window.rws = new ReconnectingWebSocket(urlProvider, ["arduino"], options);
rws.addEventListener("open", function() {
    var date = new Date().toLocaleString();
    console.log("Connected at " + date);
    printLog("已于 " + date + " 成功与 " + rws.url + " 建立连接!");
});
rws.addEventListener("error", function(error) {
    console.log("WebSocket error: ", error);
    printLog("错误: " + error);
});
rws.addEventListener("message", function(msg) {
    console.log("Received message: ", msg.data);
    printLog("接收: " + msg.data);
});
window.send = function(msg) {
    console.log("Send message: " + msg);
    printLog("发送: " + msg);
    rws.send(msg);
}

// Console
var logCount = 0;
window.printLog = function(msg) {
    if (++logCount > 50) {
        document.getElementById("console").innerHTML = "";
        logCount = 0;
    }
    document.getElementById("console").innerHTML += msg + "\n";
}
window.sendCommand = function() {
    var cmd = document.getElementById("command").value;
    if (cmd != undefined && cmd != "") {
        document.getElementById("command").value = "";
        send(cmd);
    }
}

// Auth
window.auth = function() {
    var code = document.getElementById("code").value;
    if (code != undefined && code != "") {
        send("auth " + code);
    }
}

// Wifi
window.connectWifi = function() {
    var ssid = document.getElementById("wifi-ssid").value;
    var password = document.getElementById("wifi-pw").value;
    if (ssid != undefined && ssid != "") {
        send("connect " + ssid + " " + password);
    }
}

// Events
document.onkeydown = function(e) {
    var theEvent = window.event || e;
    var code = theEvent.keyCode || theEvent.which || theEvent.charCode;
    if (code == 13) {
        sendCommand();
    }
}
