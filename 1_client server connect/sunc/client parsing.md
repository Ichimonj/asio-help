# Подключение клиента к серверу
## Разберем код client.cpp

### io_context
```cpp
asio::io_context io_context;
```
[io_context](https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio/reference/io_context.html) обеспечивает основные функции ввода и вывода в boost asio.
### endpoint
```cpp
asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);
```
[endpoint](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/ip__tcp/endpoint.html) хранит в себе адрес к которому мы будем подключаться.
[make_address](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/ip__address/make_address.html) ипользуется для создания адреса из строки.
Второй параметр - это порт, таким образом адрес подключения будет 127.0.0.1:8080.
### socket
```cpp
asio::ip::tcp::socket socket(io_context);
```
[socket](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ip__tcp/socket.html) нужен для обспечения соедения с серврером.
### connect
```cpp
socket.connect(endpoint);
```
[connect](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/basic_stream_socket/connect.html) - производим попытку соединения с сервером по адресу который хранится в ``endpoint``.