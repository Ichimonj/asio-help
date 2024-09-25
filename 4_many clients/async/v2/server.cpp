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

asio::io_context io_context;

void disconnection(talk_to_client* client);
//structure clients
struct talk_to_client {
    talk_to_client() : sock(io_context) {}

    void read_request() {
        sock.async_read_some(asio::buffer(buf),
            [&](const error_code& ec, size_t length)
            {
                if (!ec) {
                    cout.write(buf, length);
                    read_request();
                }if (ec) {
                    cout << "error - " <<ec.message()<< endl;
                    thread t1(disconnection, this);
                    t1.detach();
                }
            });
    }

    asio::ip::tcp::socket& get_socket() { return sock; }
private:
    char buf[1024];
    asio::ip::tcp::socket sock;
};

asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));

void disconnection(talk_to_client* client) {
    cout << "start find" << endl;
    auto it = find_if(clients.begin(), clients.end(),
        [client](const client_ptr& c) {return c.get() == client; });

    if (it != clients.end()) {
        clients.erase(it);
    }
    cout << "clients size - " << clients.size() << endl;
}
void accept_thread(const error_code& ec, client_ptr& client) {
    clients.push_back(client);
    clients[clients.size() - 1]->read_request();

    cout << "number of clients - " << clients.size() << endl;
    client_ptr new_client = make_shared<talk_to_client>();
    acceptor.async_accept(new_client->get_socket(), bind(accept_thread, ec, new_client));

}

int main() {
    setlocale(LC_ALL, "ru");
    error_code ec;
    cout << "Start" << endl;
    client_ptr new_client = make_shared<talk_to_client>();

    acceptor.async_accept(new_client->get_socket(), bind(accept_thread, ec, new_client));

    io_context.run();
    return 0;
}