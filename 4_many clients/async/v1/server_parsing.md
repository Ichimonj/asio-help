# асинхронный сервер на много клиентов v1
## Разберем код

### Подключение
``` cpp
void hadler(const error_code& ec,client_ptr& client) {
    clients.push_back(client);
    clients[clients.size() - 1]->read_request();

    cout << "number of clients - " << clients.size() << endl;
    client_ptr new_client = make_shared<talk_to_client>();
    acceptor.async_accept(new_client->get_socket(), bind(hadler, ec, new_client));

}

int main() {
    setlocale(LC_ALL, "ru");
    error_code ec;
    cout << "Start" << endl;
    client_ptr new_client = make_shared<talk_to_client>();

    acceptor.async_accept(new_client->get_socket(),bind(hadler,ec,new_client) );

    io_context.run();
	return 0;
}
```
В данном случае процесс подключения клиента начинается с создания объекта класса `client_ptr`. После его создания происходит процесс ожидания и подключения клиента с помощью `acceptor.async_accept(new_client->get_socket(), bind(handler, ec, new_client));`. Из ранее созданного экземпляра `client_ptr` мы берем сокет, чтобы подключить клиента. Функция [bind](https://ru.cppreference.com/w/cpp/utility/functional/bind) используется для вызова `handler` с параметрами `ec` и `new_client`.
В функции `handler` мы добавляем созданный экземпляр класса в `vector` и вызываем его процесс чтения, выводим количество клиентов, создаем еще один экземпляр класса и повторяем процесс ожидания и подключения клиента.
### Глобальные переменные
``` cpp
typedef shared_ptr<struct talk_to_client> client_ptr;
typedef vector<client_ptr> arr;
arr clients;

asio::io_context io_context;
```
1. Создаем псевдоним указателя `shared_ptr` на `talk_to_client` с именем `client_ptr`.
2. Создаем псевдоним на `vector<client_ptr>` с именем `arr`.
3. Обьявляем переменную `clients` типа `arr`.
4. Глобальной переменной также является `io_context`, он управляет всеми асинхронными операциями в программе.

### структура talk_to_client
```cpp
struct talk_to_client {
    talk_to_client() : sock(io_context) {}

    void read_request() {
        sock.async_read_some(asio::buffer(buf),
            [&](const error_code &ec,size_t length) 
            {
                if (!ec) {
                    cout.write(buf, length);
                }
                read_request();
            });
    }

    asio::ip::tcp::socket& get_socket() { return sock; }
private:
    char buf[1024];
    asio::ip::tcp::socket sock;
};
```
#### конструктор
`talk_to_client() : sock(io_context) {}`
Важно инициализировать сокет переменной `io_context`, если не инциализировать `socket`, то произойдет ошибка(socket не имеет конструктора по умолчянию)

Функция чтения нам уже знакома.
Функция `get_socket` используется для подключения.
`char buf[1024];` используется для чтения.
`asio::ip::tcp::socket sock;` для обмена данными.


