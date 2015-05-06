var b = require('bonescript');


var PB_1 = 'P8_12';
var PB_2 = 'P8_14';
var PB_3 = 'P8_16';

var SERVO_WAIST = 'P9_14';
var SERVO_SHOULDER = 'P8_13';
var SERVO_ELBOW = 'P9_21';

b.pinMode(PB_1, b.INPUT);
b.pinMode(PB_2, b.INPUT);
b.pinMode(PB_3, b.INPUT);

b.pinMode(SERVO_WAIST, b.OUTPUT);
b.pinMode(SERVO_SHOULDER, b.OUTPUT);
b.pinMode(SERVO_ELBOW, b.OUTPUT);

function rotateWaist(position) {
    var duty_cycle = (position*0.005555*0.075) +  0.045;
    b.analogWrite(SERVO_WAIST, duty_cycle, 50);

    console.log("Duty Cycle Base: " +
    parseFloat(duty_cycle*100).toFixed(1) + " %");
}
function rotateShoulder(position) {
    var duty_cycle = (position*0.005555*0.075) +  0.045;
    b.analogWrite(SERVO_SHOULDER, duty_cycle, 50);

    console.log("Duty Cycle Brazo: " +
    parseFloat(duty_cycle*100).toFixed(1) + " %");
}
function rotateElbow(position) {
    var duty_cycle = (position*0.005555*0.06) + 0.06;
    b.analogWrite(SERVO_ELBOW, duty_cycle, 50);

    console.log("Duty Cycle Codo: " +
    parseFloat(duty_cycle*100).toFixed(1) + " %");
}
function sleep(milliSeconds){
  var startTime = new Date().getTime(); // get the current time
  while (new Date().getTime() < startTime + milliSeconds); // hog cpu
}

///////////// MAIN CODE ///////////////

var b1;
var b2;
var b3;

function mov1() {
  rotateWaist(120);
  sleep(1000);
  rotateWaist(60);
}

function mov2() {
  rotateShoulder(120);
  sleep(1000);
  rotateShoulder(60);
}

function mov3() {
  rotateElbow(30);
  sleep(1000);
  rotateElbow(120);
}
while (true)
{
  b1 = b.digitalRead(PB_1);
  b2 = b.digitalRead(PB_2);
  b3 = b.digitalRead(PB_3);
  if (b1 == false)
  {
    mov1();
    console.log("se presiono boton 1");
  }
  while (b1 == false)
  {
    b1 = b.digitalRead(PB_1);
  }
  if (b2 == false)
  {
    mov2();
    console.log("se presiono boton 2");
  }
  while (b2 == false)
  {
    b2 = b.digitalRead(PB_2);
  }
  if (b3 == false)
  {
    mov3();
    console.log("se presiono boton 3");
  }
  while (b3 == false)
  {
    b3 = b.digitalRead(PB_3);
  }
}
