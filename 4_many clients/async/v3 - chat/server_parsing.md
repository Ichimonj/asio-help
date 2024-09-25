# Сервер чата

## Процесс подключения 
```cpp
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
```
Процесс подключения схож с тем, который мы разбирали в `v1`.

## start
```cpp
void start() {
    read_user_name();
}
```
Функция `start` вызвает `read_user_name`.
### read_user_name
```cpp
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
```
После подключения сервер читает имя клиента. Если чтение прошло успешно, то мы присваиваем полученное имя переменной `user_name`, выводим сообщение о подключении и вызываем функцию `read_request`. Если при чтении произошла ошибка, выводится сообщение об ошибке, и в отдельном потоке вызывается функция отключения.

### disconnection
```cpp
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
```
Функция `std::find_if` для поиска клиента в векторе `clients`. Лямбда-функция сравнивает указатель `client` с каждым элементом вектора `clients`, который представляет собой умный указатель (`client_ptr`). Если указатель клиента совпадает с одним из клиентов в списке, итератор `it` указывает на этот элемент.

Если клиент найден (итератор `it` не равен `clients.end()`), создается сообщение об отключении, которое включает имя пользователя клиента, полученное с помощью метода `get_user_name()`. Затем клиент удаляется из вектора `clients` с помощью метода `erase`.

Функция проходит по всем оставшимся клиентам в векторе `clients` и отправляет им сообщение об отключении с помощью асинхронной функции `asio::async_write`. Для каждого клиента используется его сокет, чтобы отправить сообщение. Лямбда-функция, переданная в `async_write`, обрабатывает возможные ошибки, но в данном случае не делает ничего с ними.

В конце выводится текущее количество клиентов в векторе `clients`, что позволяет видеть, сколько клиентов осталось подключенными к серверу после отключения.

### read_request
```cpp
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
```
При успешном чтении создается строка `msg`, в нее присваивается полученное сообщение от клиента, и вызвается функция `read_hendler`.
Если при чтении произошла ошибка, то выводится сообщение об ошибке, и вызывается функция `disconnection`.

### read_hendler
```cpp
void read_hendler(string& msg) {
    if      (msg == "-ping") ping();
    else if (msg == "-exit") thread(disconnection, this).detach();
    else if (msg.find("-change_name")!=string::npos) { change_name(msg);  }
    else                     broadcast_message(msg);
}
```
Команда `-ping`: вызывает функцию `ping`
команда `-exit`: вызвает функцию `disconnection` в отдельном потоке.
команда `-change_name`: вызывает функцию `change_name`.
Функция `broadcast_message`: она вызывается, если полученное сообщение не соответствует ни одной команде.
### ping
```cpp
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
```
Задача функции — отправить сообщение клиенту, так как пингование со стороны клиента запускает таймер и ждет ответа. После отправки вызывается функция `read_request`.

### change_name
```cpp
void change_name(string& name) {
    name.erase(0, 13);

    cout << "Client " << user_name << " changed name to " << name << endl;
    this->user_name = name;

    read_request();
}
```
Из полученного сообщения, например, `-change_name Vlad`, удаляются первые 13 символов, то есть `-change_name `, и получается чистое имя `Vlad`. Выводится сообщение об изменении имени, имя присваивается в переменную `user_name`, и вызывается функция `read_request`.
### broadcast_message
```cpp
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
```
С начала функция выводит в консоль информацию о том, какой клиент (по имени `user_name`) отправил сообщение, после чего создается сообещние которое состоит из имени отправителя и сообщения отправителя.

Функция проходит по всем клиентам в векторе `clients`. Используется цикл `for`, который позволяет итерироваться по элементам контейнера. Условие `if (client.get() != this)` проверяет, что текущий клиент не является отправителем сообщения (то есть не равен объекту, вызвавшему функцию).

Сообщение отправляется клиенту. Эта функция принимает сокет клиента и буфер с данными для отправки. Лямбда-функция, переданная в качестве обработчика, будет вызвана после завершения операции записи, но в данном случае она не обрабатывает ошибки или длину отправленного сообщения.

После завершения цикла по клиентам вызывается функция `read_request`. 
