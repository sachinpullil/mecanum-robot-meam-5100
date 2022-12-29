const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html>
<body>
<button type="button" onclick="hitForward()"> Forward </button>
<button type="button" onclick="hitBackward()"> Backward </button>
<button type="button" onclick="hitLeft()"> Left </button>
<button type="button" onclick="hitRight()"> Right </button>
<button type="button" onclick="hitClockwise()"> Clockwise </button>
<button type="button" onclick="hitCounterClockwise()"> CounterClockwise </button>
<button type="button" onclick="hitStop()"> Stop </button>
<p> Follow wall: </p> <button type="button" onclick="hitFollowWall()"> Activate </button>
<p> Grab beacon: </p> <button type="button" onclick="hitGrabBeacon()"> Activate </button>
<p> Push police car: </p> <button type="button" onclick="hitPushPoliceCar()"> Activate </button>
<p> Vive location: </p>
<span id="vivelabel"> </span>
<p> Current action: </p>
<span id="outputlabel"> </span>
</body>
<script>

function hitForward() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitForward", true);
  xhttp.send();
}

function hitBackward() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitBackward", true);
  xhttp.send();
}

function hitLeft() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitLeft", true);
  xhttp.send();
}

function hitRight() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitRight", true);
  xhttp.send();
}

function hitClockwise() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitClockwise", true);
  xhttp.send();
}

function hitCounterClockwise() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitCounterClockwise", true);
  xhttp.send();
}

function hitStop() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitStop", true);
  xhttp.send();
}


function hitFollowWall() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitFollowWall", true);
  xhttp.send();
}

function hitGrabBeacon() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitGrabBeacon", true);
  xhttp.send();
}

function hitPushPoliceCar() {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "hitPushPoliceCar", true);
  xhttp.send();
}

setInterval(viveLabel,300);
function viveLabel()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if(this.readyState == 4 && this.status == 200) {
      document.getElementById("vivelabel").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "vivelocation", true);
  xhttp.send();
}

setInterval(updateLabel,1000);
function updateLabel()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if(this.readyState == 4 && this.status == 200) {
      document.getElementById("outputlabel").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "output", true);
  xhttp.send();
}

</script>
</html>
)===";
