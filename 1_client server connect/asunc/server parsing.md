# Принятие подключения сервером клиента
## Разберем код


### async_accept
```cpp
acceptor.async_accept(socket,
	[&](const error_code& ec) {
		if (ec)
		{
			cout << ec.message() << endl;
		}
		else {
			cout << "Connect\n";
		}
	});
```
[async_accept](https://www.boost.org/doc/libs/1_39_0/doc/html/boost_asio/reference/basic_socket_acceptor/async_accept/overload1.html) используется для асинхронного ожидания подключения. `acceptor` хранит адрес, по которому мы ждем подключения. Второй параметр — это сокет, в который будет записан "клиент". Третий параметр — это лямбда-функция, которая выступает в роли `accept_handler`. Она принимает `error_code` и обрабатывает ошибку, которая могла возникнуть при подключении клиента.
### run
```cpp
io_context.run();
```
[run](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/io_context/run.html) используется для запуска цикла обработки событий. Так как асинхронные операции не блокируют выполнение программы, она немедленно завершится. Чтобы этого предотвратить, нужно запустить цикл обработки событий.

Остальные функции были разобраны в папке sunc