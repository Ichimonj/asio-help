#include<asio.hpp>
#include<iostream>
using namespace std;

int main() {
	error_code ec;

	//I/O functionality
	asio::io_context io_context;

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.connect(endpoint, ec);
	if (ec) {
		cout << ec.message() << endl;
		return 1;
	}
	cout << "Connected to server\nPrint msg: ";

	string msg;
	getline(cin, msg);

	//write data
	asio::write(socket, asio::buffer(msg),ec);
	if (ec) {
		cout << ec.message() << endl;
		return 1;
	}
	cout << "Write data\n";

	//buffer
	vector<char> buf(100);

	//reading data
	size_t length = socket.read_some(asio::buffer(buf), ec);
	if (ec) {
		cout << ec.message() << endl;
		return 1;
	}
	cout << "Read data: ";

	//write data
	for (int i = 0; i < length; i++) {
		cout << buf[i];
	}
	return 0;
}