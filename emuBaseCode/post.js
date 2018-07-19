function loadDoc() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      console.log(xhttp);
    }
  };
  xhttp.open("POST", "/?x=" + joystick.deltaX() + "&y=" + joystick.deltaY(), true);
  xhttp.send();
}
