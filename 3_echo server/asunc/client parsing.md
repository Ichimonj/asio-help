# Эхо сервер
#### Клиент снчала отправляет данные серверу, а потом принимает данные от сервера.
## Разберем код
### startWrite
```cpp
void startWrite(asio::ip::tcp::socket& soket,string msg) {
	asio::async_write(soket, asio::buffer(msg.data(),msg.size()),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				cout << "write data" << endl;
				startRead(soket);
			}
			else {
				cout << ec.message() << endl;
			}
		});
}
```
В функции `main`, после подключения и ввода строки клиентом из консоли, вызывается функция отправки сообщения `startWrite`. Функцию асинхронной отправки сообщения мы уже разбирали в папке `msg client to server/async/client parsing`, она нам знакома. Если сообщение было успешно отправлено, выводится сообщение `write data`, и вызывается функция `startRead`.
### startRead
```cpp
void startRead(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(buf),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				for (int i = 0; i < length; i++) {
					cout << buf[i];
				}
			}
			else {
				cout << ec.message() << endl;
			}

		});
}
```
Функцию асинхронной чтении сообщения мы уже разбирали в папке `msg client to server/async/server parsing`