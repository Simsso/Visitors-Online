var socket = io.connect('http://timodenk.dd-dns.de:3000');

socket.on('visitors', function(data) {
	// number of visitors has changed
	var visitorsOnline = data.n;

	if (typeof window.visitorsOnlineChanged !== 'undefined') {
		window.visitorsOnlineChanged(visitorsOnline);
	}
});