# OpenGlider
##Arduino based glider control software

This is Arduino code running a home built underwater glider.  It is currently functional and working however
there are some rough spots.  The glider states change based on a timer - the next phase of this will be 
to change states based on actual water depth by adding an external water sensor.

This runs on an Arduino Nano IOT 33 with built in wifi.  It is a new board and the libraries do not support websockets nicely yet, so I used SSE with much thanks to Oscar from his project https://github.com/ocrdu/arduino-webinterface-sse


