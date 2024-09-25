### Асинхронное чтение данных со стороны сервера 
## Разберем код

### async_read_some
```cpp
vector<char>buf(100);
void read(asio::ip::tcp::socket& socket) {
	//asynchronous socket read
	socket.async_read_some(asio::buffer(buf.data(), buf.size()),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				for (int i = 0; i < length; i++) {
					cout << buf[i];
				}
				read(socket);
			}
		});
}
```
В начале обьявляем вектор, в который будут записываться данные из сокета.
Функция `read` принимает ссылку на сокет, из которого будут читаться данные, функция [async_read_some](https://beta.boost.org/doc/libs/1_52_0/doc/html/boost_asio/reference/async_read/overload1.html) используется для асинхронного чтения из сокета, она принимает `buffer`, который был расмотрен ранее в `client parsing`. Далее функция принимает лямбда-функцию, которая используется вместо `handler`. Она принимает `error_code` и `size_t`, переменная `length` хранит в себе количество символов, которые были отправлены. В цикле выводится сообщение которое было записано в `buf`, если при чтении сообщения из сокета произошла ошибка, то сообщение не выводится и функция не вызывает себя. 
Так как `async_read_some` асинхронная функция, она не блокирует выполнение потока, и бесконечный ее вызов не помешает работе программы.