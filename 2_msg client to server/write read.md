# sunc
```cpp
asio::write(socket, asio::buffer(buf.data(), length), ec);
socket.write_some(asio::buffer(buf.data(), length),ec);
```

1. asio::write: Этот метод гарантирует, что все данные будут отправлены. Он будет продолжать вызывать системные функции отправки до тех пор, пока все данные не будут переданы или не произойдет ошибка. Это означает, что если вы передаете 100 байт, asio::write будет пытаться отправить все 100 байт, даже если это требует нескольких вызовов к системным функциям.
2. socket.write_some: Этот метод отправляет данные, но не гарантирует, что все данные будут отправлены. Он может вернуть меньше байт, чем вы передали, и это нормально. Например, если вы пытаетесь отправить 100 байт, write_some может вернуть 50 байт, и вам нужно будет повторно вызвать его, чтобы отправить оставшиеся данные.

```cpp
size_t length = socket.read_some(asio::buffer(buf.data(), buf.size()), ec); 
size_t length = asio::read(socket, asio::buffer(buf.data(), buf.size()), ec);
```
1. socket.read_some: Этот метод пытается прочитать данные из сокета, но не гарантирует, что все запрашиваемые байты будут прочитаны. Он может вернуть меньше байт, чем вы указали, и это нормально. Например, если вы запрашиваете 100 байт, read_some может вернуть 50 байт, и вам нужно будет повторно вызывать его, чтобы получить оставшиеся данные.
2. asio::read: Этот метод гарантирует, что все запрашиваемые данные будут прочитаны. Он будет продолжать вызывать системные функции чтения до тех пор, пока не будет прочитано указанное количество байт или не произойдет ошибка. Если вы запрашиваете 100 байт, asio::read будет пытаться прочитать все 100 байт.

# asunc

```cpp
asio::async_read(socket, asio::buffer(buf.data(), buf.size()),
	[&](const error_code& ec, size_t length) {
		for (int i = 0; i < length; i++) {
			cout << buf[i];
		}
		read(socket);
	});

socket.async_read_some(asio::buffer(buf.data(), buf.size()),
	[&](const error_code& ec, size_t length) {
		for (int i = 0; i < length; i++) {
			cout << buf[i];
		}
		read(socket);
	});
```
1. asio::async_read: Этот метод гарантирует, что будет прочитано указанное количество байт. Он будет продолжать вызывать системные функции чтения до тех пор, пока не будет прочитано нужное количество байт или не произойдет ошибка. Это означает, что если вы запрашиваете 100 байт, async_read будет пытаться прочитать все 100 байт, даже если это требует нескольких вызовов.
2. socket.async_read_some: Этот метод пытается прочитать данные из сокета, но не гарантирует, что все запрашиваемые байты будут прочитаны. Он может вернуть меньше байт, чем вы указали, и это нормально. Например, если вы запрашиваете 100 байт, async_read_some может вернуть 50 байт, и вам нужно будет повторно вызывать его, чтобы получить оставшиеся данные.
```cpp
asio::async_write(socket,asio::buffer(msg),
	[&](const error_code& ec, size_t length) {
		if (!ec) {
			cout << "2out - " << length << endl;
		}
	});

socket.async_write_some(asio::buffer(msg),
	[&](const error_code& ec, size_t length) {
		if (!ec) {
			cout << "1out - " << length << endl;
		}
	});
```
1. asio::async_write: Этот метод гарантирует, что все данные, которые вы хотите записать, будут отправлены. Он будет продолжать вызывать системные функции записи до тех пор, пока не будет записано указанное количество байт или не произойдет ошибка. Это означает, что если вы передаете 100 байт, async_write будет пытаться записать все 100 байт, даже если это требует нескольких вызовов.
2. socket.async_write_some: Этот метод пытается записать данные в сокет, но не гарантирует, что все запрашиваемые байты будут записаны. Он может вернуть меньше байт, чем вы указали, и это нормально. Например, если вы пытаетесь записать 100 байт, async_write_some может вернуть 50 байт, и вам нужно будет повторно вызывать его, чтобы записать оставшиеся данные.
