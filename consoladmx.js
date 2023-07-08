var http = require('http').createServer(handler); 
var fs = require('fs'); 
var io = require('socket.io')(http)

const { SerialPort } = require('serialport');
const baud = 9600; 

const port = new SerialPort
(
  { 
  path: '/dev/tty.usbmodem141301', 
  baudRate: baud 
  }, 
  function (err) 
  {
    if (err) 
    {
    return console.log('Error: ', err.message)
    }
  }
);

redRGB = 0,
greenRGB = 0,
blueRGB = 0, 
fader = 0,
delay = 0,
preset = 0;



http.listen(8080); 

function handler (req, res) { 
  fs.readFile(__dirname + '/public/index.html', function(err, data) { 
    if (err) {
      res.writeHead(404, {'Content-Type': 'text/html'}); 
      return res.end("404 Not Found");
    }
    res.writeHead(200, {'Content-Type': 'text/html'}); 
    res.write(data); //
    return res.end();
  });
}

io.sockets.on('connection', function (socket) {
    socket.on('rgbLed', function(data) { 
      console.log(data);

      redRGB=parseInt(data.red);
      greenRGB=parseInt(data.green);
      blueRGB=parseInt(data.blue);
      fader=parseInt(data.fader);
      delay=parseInt(data.delay);
      preset=parseInt(data.preset);

  
      port.write(`${fader},${redRGB},${greenRGB},${blueRGB},${delay},${preset}\n`, (err) => {
      if (err) {
        return console.log('Error al enviar los datos al Arduino:', err.message);
      }
      console.log('Datos enviados al Arduino correctamente.');

      });
    });
  });

  process.on('SIGINT', function () { 
  ledRed.digitalWrite(0); 
  ledGreen.digitalWrite(0);
  ledBlue.digitalWrite(0); 
  process.exit(); 
});