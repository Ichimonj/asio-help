 # Подключение клиента к серверу
## Разберем код client.cpp
### async_connect
```cpp
	socket.async_connect(endpoint,
		[&](const error_code ec)
		{
			if (ec)
			{
				cout << ec.message() << endl;
			}
			else {
				cout << "Connect\n";
			}
		});
```
[async_connect](https://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/reference/basic_socket/async_connect.html) используется для асинхронного подключения к сервреру. Первым параметром он принимает `endpoint`, в котором хранится IP-адрес для подключения. Вторым параметром передается лямбда-функция, которая выступает в роли `connect_handler`. Она принимает `error_code` и обрабатывает ошибку, котороая могла возникнуть при подключении.
### run
```cpp
io_context.run();
```
[run](https://www.boost.org/doc/libs/1_86_0/doc/html/boost_asio/reference/io_context/run.html) используется для запуска цикла обработки событий. Так как асинхронные операции не блокируют выполнение программы, она немедленно завершится. Чтобы этого предотвратить, нужно запустить цикл обработки событий.

Остальные функции были разобраны в папке sunc