#include<asio.hpp>
#include<iostream>
using namespace std;

//write func
void out(asio::ip::tcp::socket& socket, string& msg) {
	//asynchronous socket write
	asio::async_write(socket, asio::buffer(msg.data(), msg.length()),
		[&](const error_code& ec, size_t length) {
			if (ec) {
				cout << ec.message();
			}
			else {
				cout << "out - " << length << " byte" << endl;
			}

		});
}

int main() {
	setlocale(LC_ALL, "ru");
	//I/O functionality
	asio::io_context io_context;

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.async_connect(endpoint,
		[&](error_code ec)
		{
			if (ec)
			{
				cout << ec.message() << endl;
			}
			else {
				cout << "Connect\n";
				string msg;
				for (;;) {
					getline(cin, msg);
					out(socket, msg);
				}
			}
		});

	//asynchronous socket reasing
	io_context.run();
	return 0;
}