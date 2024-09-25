#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <asio.hpp>
#include <chrono>

using namespace std;

//creating a client array
typedef shared_ptr<struct talk_to_client> client_ptr;
typedef vector<client_ptr> arr;
arr clients;

asio::io_context io_context;

//disconnection func
void disconnection(talk_to_client* client);
//message sending function
void send_to(const string& msg, talk_to_client* sender);


//structure clients
struct talk_to_client {
    talk_to_client() : sock(io_context) {}

    void start() {
        read_user_name();
    }
   
    asio::ip::tcp::socket& get_socket() { return sock; }
    const string& get_user_name() { return user_name; }

private:
    //setting client name
    void read_user_name() {
        sock.async_read_some(asio::buffer(buf),
            [&](const error_code& ec, size_t length)
            {
                if (!ec) {
                    user_name = string(buf, length);
                    cout << user_name <<" connect" << endl;
                    read_request();
                }else {
                    cout << "error - " << ec.message() << endl;
                    thread(disconnection, this).detach();
                }
            });
    }

    //reading a message from a client
    void read_request() {
        sock.async_read_some(asio::buffer(buf),
            [&](const error_code& ec, size_t length)
            {
                if (!ec) {
                    //message handler
                    string msg(buf, length);
                    read_hendler(msg);
                }else {
                    cout << "error - " << ec.message() << endl;
                    thread(disconnection, this).detach();
                }
            });
    }

    //message handler
    void read_hendler(string& msg) {
        if      (msg == "-ping") ping();
        else if (msg == "-exit") thread(disconnection, this).detach();
        else if (msg.find("-change_name")!=string::npos) { change_name(msg);  }
        else                     broadcast_message(msg);
    }

    //checking the delay 
    void ping() {
        string ping("-ping");
        sock.async_write_some(asio::buffer(ping.data(),ping.length()), 
            [&](const error_code&ec,size_t/*length*/) {
                if (ec) {
                    cout << "error ping" << ec.message() << endl;
                }
            });
        read_request();
    }

    //message output
    void broadcast_message(string& msg) {
        cout << "client " << user_name << " msg - " << msg << endl;
        out_msg = user_name + " - " + msg;
        for (auto& client : clients) {
            if (client.get() != this) {
                client->get_socket().async_write_some(asio::buffer(out_msg.data(), out_msg.length()),
                    [&](const error_code& ec, size_t/*len*/) {});
            }
        }
        
        read_request();
    }

    void change_name(string& name) {
        name.erase(0, 13);

        cout << "Client " << user_name << " changed name to " << name << endl;
        this->user_name = name;

        read_request();
    }
private:
    char buf[1024];
    string user_name;

    string out_msg;
    asio::ip::tcp::socket sock;
};


void disconnection(talk_to_client* client) {
    auto it = find_if(clients.begin(), clients.end(),
        [client](const client_ptr& c) {return c.get() == client; });
    string disconn_msg;

    if (it != clients.end()) {
        disconn_msg = client->get_user_name() + " disconnected from the server";
        clients.erase(it);
    }
    cout << disconn_msg << endl;
    for (auto& client : clients) {
        asio::async_write(client->get_socket(), asio::buffer(disconn_msg.data(), disconn_msg.size()),
            [&](const error_code& ec,size_t /*length*/) {});
    }
    cout << "clients size - " << clients.size() << endl;
}

asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));
void accept_thread(const error_code& ec, client_ptr& client) {
    clients.push_back(client);
    clients[clients.size() - 1]->start();

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