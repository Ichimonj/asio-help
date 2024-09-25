# Эхо сервер
#### Клиент снчала отправляет данные серверу, а потом принимает данные от сервера.

```cpp
string msg;
getline(cin, msg);

asio::write(socket, asio::buffer(msg),ec);
if (ec) {
	cout << ec.message() << endl;
	return 1;
}
cout << "Write data\n";
	
vector<char> buf(100);

size_t length = socket.read_some(asio::buffer(buf), ec);
if (ec) {
	cout << ec.message() << endl;
	return 1;
}
cout << "Read data: ";

for (int i = 0; i < length; i++) {
	cout << buf[i];
}
```
Все что было использованно в этом коде уже было рассмотренно ранее