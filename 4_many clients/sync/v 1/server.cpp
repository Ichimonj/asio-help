#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <asio.hpp>

using namespace std;

//creating a client array
typedef shared_ptr<struct talk_to_client> client_ptr;
typedef vector<client_ptr> arr;
arr clients;

//stream synchronization
mutex clients_mutex;

//structure clients
struct talk_to_client {
    talk_to_client() : sock(context) {}

    void read_request() {
        if (sock.available()) {
            error_code ec;
            size_t length = sock.read_some(asio::buffer(buf), ec);
            if (ec)
                cout << ec.message() << endl;
            else {
                for (int i = 0; i < length; i++) {
                    cout << buf[i];
                }
                cout << endl;
            }
        }
    }

    asio::ip::tcp::socket& get_socket() { return sock; }
private:
    char buf[1024];
    asio::io_context context;
    asio::ip::tcp::socket sock;
};

//client connection function
void accept_thread() {
    asio::io_service io_service;
    asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

    while (true) {
        client_ptr new_client = make_shared<talk_to_client>();
        acceptor.accept(new_client->get_socket());
		cout << "Сlient connected\n";

        //block the stream
        lock_guard<mutex> lock(clients_mutex);
        clients.push_back(new_client);
    }
}

//read function
void read_thread() {
    while (true) {
        {
            //block the stream
            lock_guard<mutex> lock(clients_mutex);
            for (int i = 0; i < clients.size(); i++) {
                clients[i]->read_request();
            }
        }
    }
}

int main() {
    cout << 's' << endl;
    thread t1(accept_thread);
    thread t2(read_thread);
    t1.join();
    t2.join();
}