# Отправка сообщения клиентом серверу
## Разберем код

### connect
```cpp
socket.connect(endpoint,ec);
if (ec) {
	cout << ec.message();
	return 1;
}
```
В этом примере в функцию подключения передается второй параметр `ec`, в который будет записан итог попытки подключения к серверу. Далее идет проверка: если `ec` не равен нулю, то выведется сообщение об ошибке, и программа завершит свою работу с кодом 1.
### string
```cpp
string msg = "hello asio";
```
Создаем сообщение которое мы будем отправлять серверу.
### write
```cpp
asio::write(socket, asio::buffer(msg.data(), msg.length()), ec);
if (ec) {
	cout << ec.message();
	return 1;
}
```
Функция [write](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/write.html) используется для отправки сообщения, сообщение отправляется через `socket` и его принимает сервр который слушает этот порт.

[buffer](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/buffer.html) используется для создания буфера, в который будет скопировано сообщение из строки `msg`, и который будет использован для отправки. 

Последним параметром передается `ec`, который затем будет использован для проверки завершения операции, так же как и в функции `connect`.