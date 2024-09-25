#include <iostream>
#include<asio.hpp>
#include<thread>
using asio::ip::tcp;
using namespace std;
mutex mt;

class client {
public:
    client(asio::io_context& context, asio::ip::tcp::endpoint& endpoint) :
        sock(context),
        context(context)
    {
        connection(endpoint);
    };

    void connection(asio::ip::tcp::endpoint& endpoint) {
        error_code ec;
        sock.connect(endpoint, ec);
        if (ec) {
            cout << "connection error" << endl;
            exit(1);
        }
        else {
            string name;
            while (true) {
                cout << "Pring ur name - ";
                getline(cin, name);
                if (!name.empty()) {
                    size_t first = name.find_first_not_of(' ');
                    size_t last = name.find_last_not_of(' ');

                    if (first != string::npos) {
                        name = name.substr(first, last - first + 1);
                        this->name = name;
                        break;
                    }
                }
            };
            sock.async_write_some(asio::buffer(name.data(), name.length()),
                [&](const error_code& ec, size_t/*length*/) {
                    if (ec) {
                        cout << "error when sending name - ";
                        cout << ec.message() << endl;
                        exit(1);
                    }
                    else {
                        cout << "You can write messages" << endl;
                        do_read();
                    }
                });
        }
    }

    void input_handler(string& command) {
        if (command == "-help") {
            conclusion = false;
            cout << "-ping (check connection)\n"
                << "-exit (disconnect from the server)\n"s
                << "-change_name <str> (change your name)\n"
                << "-clear (clean the console)\n";
            cout << "To exit, press any key or enter a command" << endl;
            
            string new_command;

            getline(cin, new_command);

            if (!new_command.empty()) {
                if (command == "-clear") {
                    system("cls");
                    write_queue();
                }
                else {
                    input_handler(new_command);
                    write_queue();
                }
            }
            else {
                write_queue();
            }
        }
        else if (command == "-ping") {
            ping(command);
        }
        else if (command == "-exit") {
            do_write(command);
            cout << "you have disconnected from the server" << endl;

            exit(0);
        }
        else if (command.find("-change_name")!=string::npos) {
            change_name(command);
        }
        else if (command == "-clear") {
            system("cls");
        }
        else {
            do_write(command);
        }
    }

    void write_queue() {
        if (!queue.empty()) {
            cout << queue;
            queue.clear();
        }
        conclusion = true;
    }

    void do_write(const string& msg) {
        sock.async_write_some(asio::buffer(msg.data(), msg.length()),
            [&](const asio::error_code& ec, size_t/*length*/) {
                if (ec) {
                    cout << "error sending message" << endl;
                }
                else {
                }
            });
    }

    asio::ip::tcp::socket& get_sock() { return sock; }

    //private class methods
private:
    void do_read() {
        sock.async_read_some(asio::buffer(buf),
            [&](const error_code& ec, size_t length) {
                if (!ec) {
                    if (conclusion) {
                        cout.write(buf, length);
                        cout << endl;
                        do_read();
                    }
                    else {
                        queue += string(buf, length);
                        queue += '\n';
                        do_read();
                    }
                }
                else {
                    cout << "Error to read" << endl;
                    if (ec == asio::error::connection_reset) {
                        cout << "The server is temporarily down" << endl;
                    }
                    sock.close();
                }
        
            });
    }
    void ping(string& msg) {
        auto start = chrono::high_resolution_clock::now();
        sock.send(asio::buffer(msg));

        char reply[1024];
        sock.async_receive(asio::buffer(reply),
            [&](const error_code&ec,size_t/*length*/) {});
        
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, std::milli> elapsed = end - start;
        cout << "Received reply: " << elapsed << "\n";
    }

    void change_name(string& comand) {
        if (comand.size() < 13) {
            cout << "you did not enter a name" << endl;
            return;
        }


        size_t pos = comand.find(' ', 12);
        if (pos == string::npos) {
            cout << "invalid command input" << endl;
            return;
        }

        string name  = comand.substr(13, comand.size());
        
        size_t first = name.find_first_not_of(' ');
        size_t last  = name.find_last_not_of (' ');
        
        if(last != string::npos){
            name = name.substr(first, last - first + 1);
            this->name = name;
            cout << "you changed your name to - " << name << endl ;
            do_write("-change_name " + name);
        }
    }

    //Class fields
private:
    string name;

    char buf[1024];
    string queue;   //messages are added to the queue when output is not possible
    asio::ip::tcp::socket sock;
    asio::io_context& context;

    bool conclusion = true;
};


int main() {
    setlocale(LC_ALL, "ru");
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080);

    client client(context,endpoint);
    thread T_context([&context]() {context.run(); });
    while (true) {
        string msg;
        getline(cin, msg);
        client.input_handler(msg);
    }
    T_context.join();
    return 0;
}