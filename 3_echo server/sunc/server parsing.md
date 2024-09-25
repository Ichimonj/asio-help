# Эхо сервер
#### Эхо сервер сначала принмает сообщение от клиента, а потом отправляет это сообщение обратно клиенту.

```cpp
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
		
asio::write(socket, asio::buffer(buf, length), ec);
if (ec) {
	cout << ec.message() << endl;
	return 1;
}
cout << "\nWrite data\n";
```
Все что было использованно в этом коде уже было рассмотренно ранее