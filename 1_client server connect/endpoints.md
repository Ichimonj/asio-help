# v1
```cpp
asio::ip::tcp::resolver resolver(io_context);
asio::ip::tcp::resolver::results_type endpoint = resolver.resolve("127.0.0.1", "8080");
```
При создании ``endpoint`` при помощью ``results_type`` процесс подключения происходит при помощи следущих конструкций
```cpp
asio::connect(socket, endpoint, ec);
```

```cpp
asio::async_connect(socket, endpoint,
	[](std::error_code ec, asio::ip::tcp::endpoint)
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
# v2
```cpp
asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);
```
При создании ``endpoint`` при помощью ``endpoint`` процесс подключения происходит при помощи следущих конструкций

```cpp
socket.connect(endpoint, ec);
```

```cpp
socket.async_connect(endpoint,
     [&](asio::error_code ec)
     {
         if (ec) {
             std::cout << ec.message() << std::endl;
         }
         else {
             std::cout << "Connect\n";
         }
     });
```