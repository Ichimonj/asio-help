# сервер на много клиентов v1
## Разберем код

Программа разделена на два потока, в первом потоке происходит подключение клиента, создание указателя на структуру talk_to_client и добавления ее в вектор, второй поток читает сообщения полученные от клиентов.
### vector
```cpp
typedef shared_ptr<struct talk_to_client> client_ptr;
typedef vector<client_ptr> arr;
arr clients;
```
1. Создаем псевдоним для умного уазателя ```shared_ptr``` под назаванием ```client_ptr``` на структуру talk_to_client.
2. Создаем псевдоним для вектора умных указатьелей ```client_ptr``` под назаванием ```arr```.
3. Обьявляем вектор умных указателей ```arr``` под назавание ```clients```.
Вся работа в серверной части будет происходить в этим вектором.
### mutex
```cpp
mutex clients_mutex;
```
В данном случае процесс синхронизации работает в момент добавления клиента в вектор и чтения из вектора сообщений.
### talk_to_client

```cpp
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
```
##### Начнем с приватных полей:
1. ```char buf[1024];``` обьявляем массив ```char``` который будет использоваться как буфер для вывода данных полученных от клиента.
2. ```asio::io_context context;``` обьявляем ```io_context``` для инициализации его в сокет при создании экземпляра структуры. 
3. ```asio::ip::tcp::socket sock;``` обьявляем сокет.

##### Конструктор:
```talk_to_client() : sock(context) {}``` при создании экземпляра структуры сокет инициализируется переменной context.

##### Фкнция чтения: 
```cpp
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
```
1. ```if (sock.available())```Функция начинается с проверки есть ли в сокете данные которые можно было бы прочитать, если таких данных нет, то логика внутри конструкии if не выполняется.
2. Если в сокете есть данные для чтения, то выполняется чтение из сокета, проверка и вывод данных.

```asio::ip::tcp::socket& get_socket() { return sock; }``` функция которая возвращает сылку на сокет, в дальнейшем она нам поднадобится для инициализации сокета.
### accept_thread

```cpp
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
```
В этой функции есть уже знакомые уонструкции, начнем разбор с цикла:
1. ```client_ptr new_client = make_shared<talk_to_client>();``` создаем умный укзаатель с именем ``new_client`` с помощию оператора ``make_shared``.
2. ```acceptor.accept(new_client->get_socket());``` производим подключение клиента к серверу и передаем параметром сокет который находится в ранее созданной ``new_client``. 
3. ```lock_guard<mutex> lock(clients_mutex);``` блокируем поток, он будет заблокирован до конца выполнения функции.
4. ```clients.push_back(new_client);``` добавляем уже созданный указатель на структуру ``talk_to_client`` в вектор ``clients``.

### accept_thread
```cpp
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
```
Это довольно простая функция. В начале с помощью строки ``lock_guard<mutex> lock(clients_mutex);`` мы блокируем поток. Далее обходим всех подключенных клиентов и вызываем у них ранее рассмотренную функцию ``read_request``.
