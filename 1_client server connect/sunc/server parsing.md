# Принятие подключения сервером клиента
## Разберем код
### acceptor
```cpp
asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoin(asio::ip::make_address_v4("127.0.0.1"), 8080));
```
[acceptor](https://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/reference/ip__tcp/acceptor.html) используется для принятия входящих соединений.
### accept
```cpp
acceptor.accept(socket);
```
Оператор [accept](https://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/reference/basic_socket_acceptor/accept.html) ипользуется для ожидания подключения и записывает "клиента " в сокет.
 
Остальной код, в том числе и ``endpoin``, который используется в ``acceptor``, был разобрал в ``clien parsing.md``

