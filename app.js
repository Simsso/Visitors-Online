var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

var sockets = [];

server.listen(3000);

app.get('/', function (req, res) {
  res.send(JSON.stringify(getVisitorsObject()));
});

io.on('connection', function (socket) {
	// socket disconnect event
	socket.on('disconnect', handleDisconnect, socket);

	// add socket to array
  sockets.push(socket);

  io.sockets.emit('visitors', getVisitorsObject());
});

function handleDisconnect(socket) {
	// remove from sockets array
	sockets.splice(sockets.indexOf(socket), 1);

	// send update to all sockets
	io.sockets.emit('visitors', getVisitorsObject());
}

function getVisitorsObject() {
	return { 
		'n': sockets.length 
	};
}