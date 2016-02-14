var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var request = require('request');

var sockets = [], ips = [];

server.listen(3000);
console.log('listening on :3000');

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/client-example.html');
});

io.on('connection', function (socket) {
	socket.ip = socket.handshake.address;

	// socket disconnect event
	socket.on('disconnect', function() {
		// remove from sockets array
		sockets.splice(sockets.indexOf(socket), 1);

		// remove from ip array (if no other socket from this ip exists)
		var otherSocketFromSameIp = false;
		for (var i = 0; i < sockets.length; i++) {
			if (sockets[i].ip === socket.ip) {
				otherSocketFromSameIp = true;
				break;
			}
		}

		if (!otherSocketFromSameIp) {
			ips.splice(ips.indexOf(socket.ip), 1);
		}

		emitVisitors();
	});

	// add socket to array
	sockets.push(socket);

	// add ip address (if not existing yet)
	if (ips.indexOf(socket.ip) === -1) {
		ips.push(socket.ip);
	}

	emitVisitors();
});

function getVisitorsObject() {
	return { 
		'n': ips.length 
	};
}

function emitVisitors() {
	process.stdout.write("\r    ");
	process.stdout.write("\r" + ips.length.toString());

	// send update to all sockets
	io.sockets.emit('visitors', getVisitorsObject());
	notifyArduino();
}

function notifyArduino() {
	try {
		var options = {
			url: 'http://192.168.0.113/' + ips.length.toString(),
			headers: {
				'User-Agent': 'request'
			}
		};

		request(options, function() {});
	}
	catch(e) {}
}
