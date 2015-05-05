var b = require('bonescript');

var b1=true;
var b2=true;
var b3=true;
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

var num;
var b;
var fibonacci;
var i;
var anterior;
num = 7;
b = 1;
fibonacci = 0;
i = 1;
while (i <= num)
{
  anterior = b;
  b = fibonacci;
  fibonacci = anterior + b;
  console.log("Fibonacci: " + fibonacci);
  i = i + 1;
}
