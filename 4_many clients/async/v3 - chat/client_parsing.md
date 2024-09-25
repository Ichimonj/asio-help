# Клиент чата 
## Начнем разбор с main функции

### main 
```cpp
int main() {
    setlocale(LC_ALL, "ru");
    asio::io_context context;
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080);

    client client(context,endpoint);
    thread T_context([&context]() {context.run(); });
    while (true) {
        string msg;
        getline(cin, msg);
        client.input_handler(msg);
    }
    T_context.join();
    return 0;
}
```
В этом фрагменте важной частью является `thread T_context([&context]() { context.run(); });`. Управление асинхронными операциями будет выполняться в отдельном потоке, что позволит нам производить чтение и вывод сообщений. Если `context` будет выполняться в том же потоке, в котором и ввод, то цикл `while` с ожиданием ввода будет блокировать поток, и, соответственно, вывод полученных сообщений не будет производиться.

### Конструктор client
```cpp
client(asio::io_context& context, asio::ip::tcp::endpoint& endpoint):
    sock(context),
    context(context)
{
    connection(endpoint);
};
```
В конструктор передаются `io_context`, который будет управлять асинхронными операциями, и `endpoint`, по которому будет производиться подключение. Далее они инициализируют соответствующие члены класса и вызывается функция `connection`.

### connection
```cpp
void connection(asio::ip::tcp::endpoint& endpoint) {
    error_code ec;
    sock.connect(endpoint, ec);
    if (ec) {
        cout << "connection error" << endl;
        exit(1);
    }
    else {
        string name;
        while (true) {
            cout << "Pring ur name - ";
            getline(cin, name);
            if (!name.empty()) {
                size_t first = name.find_first_not_of(' ');
                size_t last = name.find_last_not_of(' ');

                if (first != string::npos) {
                    name = name.substr(first, last - first + 1);
                    this->name = name;
                    break;
                }
            }
        };
        sock.async_write_some(asio::buffer(name.data(), name.length()),
            [&](const error_code& ec, size_t/*length*/) {
                if (ec) {
                    cout << "error when sending name - ";
                    cout << ec.message() << endl;
                    exit(1);
                }
                else {
                    cout << "You can write messages" << endl;
                    do_read();
                }
            });
    }
}
```
В проверке на наличие ошибок при подключении есть строка `exit(1)`, она принудительно завершает работу программы с кодом 1. Если при подключении ошибки не произошло, начинается процесс ввода имени клиента. Сначала мы проверяем, ввел ли клиент что-то, с помощью `if (!name.empty())`. Затем находим первый и последний символы, отличные от пробела. Если пользователь ввел только пробелы, то первый символ будет равен `string::npos`, и мы вернемся обратно к вводу имени. Если нет, то мы инициализируем переменную name именем без пробелов и отправляем имя на сервер.
Если при отправки имени не произошла ошибка, то вызывается функция чтения `do_read();`

### do_read
```cpp
void do_read() {
    sock.async_read_some(asio::buffer(buf),
        [&](const error_code& ec, size_t length) {
            if (!ec) {
                if (conclusion) {
                    cout.write(buf, length);
                    cout << endl;
                    do_read();
                }
                else {
                    queue += string(buf, length);
                    queue += '\n';
                    do_read();
                }
            }
            else {
                cout << "Error to read" << endl;
                if (ec == asio::error::connection_reset) {
                    cout << "The server is temporarily down" << endl;
                }
                sock.close();
            }
     
        });
}
```
Если чтение прошло без ошибок и переменная `conclusion` равна `true`, то вывод происходит в консоль. Если переменная `conclusion` равна `false`, то полученные сообщения добавляются в очередь. Это нужно для того, чтобы, когда вывод в консоль невозможен, полученные сообщения не терялись. Если при чтении произошла ошибка, выводится сообщение об ошибке, и сокет закрывается.


### Цикл ввода сообщений
```cpp
while (true) {
    string msg;
    getline(cin, msg);
    client.input_handler(msg);
}
```
После ввода сообщения с консоли вызывается функция `input_handler`, которая обрабатывает сообщение.

### input_handler
```cpp
void input_handler(string& command) {
    if (command == "-help") {
        conclusion = false;
        cout << "-ping (check connection)\n"
            << "-exit (disconnect from the server)\n"s
            << "-change_name <str> (change your name)\n"
            << "-clear (clean the console)\n";
        cout << "To exit, press any key or enter a command" << endl;
            
        string new_command;

        getline(cin, new_command);

        if (!new_command.empty()) {
            if (command == "-clear") {
                system("cls");
                write_queue();
            }
            else {
                input_handler(new_command);
                write_queue();
            }
        }
        else {
            write_queue();
        }
    }
    else if (command == "-ping") {
        ping(command);
    }
    else if (command == "-exit") {
        do_write(command);
        cout << "you have disconnected from the server" << endl;

        exit(0);
    }
    else if (command.find("-change_name")!=string::npos) {
        change_name(command);
    }
    else if (command == "-clear") {
        system("cls");
    }
    else {
        do_write(command);
    }
}
```
Команда `-help`: Если введенная команда равна `-help`, устанавливается переменная `conclusion` в `false`, соответственно чтение, разобранное в `do_read`, будет происходить в переменную `queue`, и выводится список доступных команд, объясняющих их функции. После этого объявляется переменная `new_command`, в которую будет считываться новая команда. Если введенная строка не пустая и равна `-clear`, то очищается консоль и вызывается функция `write_queue`.

#### write_queue
```cpp
void write_queue() {
    if (!queue.empty()) {
        cout << queue;
        queue.clear();
    }
    conclusion = true;
}
```
В функции `write_queue` просходит вывод очереди.
В начале мы проверяем пустая ли очередь, если очередь не пустая, то мы выводим ее в консоль и отчищаем очередь, в конце перменная в `conclusion` присваивается `true`.

Если введеная строка не пустая и не равана `-clear`, то еще раз вызывается `input_handler` с новой командой.

Если введеная команда пустая, то вызвается функция `write_queue`.

Команда `-ping`: вызывает функцию `ping`.
#### ping
```cpp
void ping(string& msg) {
    auto start = chrono::high_resolution_clock::now();
    sock.send(asio::buffer(msg));

    char reply[1024];
    sock.async_receive(asio::buffer(reply),
        [&](const error_code&ec,size_t/*length*/) {});
    
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> elapsed = end - start;
    cout << "Received reply: " << elapsed << "\n";
}
```
Функция `chrono::high_resolution_clock::now();` возвращает момент времени, представляющий текущее время.

Функции `send` и `async_receive` отправляет и получает сообщение. 
При помощи конструкции `chrono::duration<double, std::milli> elapsed = end - start;`,
мы вычитаем из времени когда мы получили сообщени время когда мы отправили сообщение, это и будет задержкой.

Команда `-exit`: отправляет строку `-exit` серверу, выводим сообщение что программа отключилась от сервера, и завершает работу программы с кодом 0.

Команда `-change_name`: вызывает функцию `change_name`

#### change_name
```cpp
void change_name(string& comand) {
    if (comand.size() < 13) {
        cout << "you did not enter a name" << endl;
        return;
    }

    size_t pos = comand.find(' ', 12);
    if (pos == string::npos) {
        cout << "invalid command input" << endl;
        return;
    }

    string name  = comand.substr(13, comand.size());
        
    size_t first = name.find_first_not_of(' ');
    size_t last  = name.find_last_not_of (' ');
        
    if(last != string::npos){
        name = name.substr(first, last - first + 1);
        this->name = name;
        cout << "you changed your name to - " << name << endl ;
        do_write("-change_name " + name);
    }
}
```
Функция сначала проверяет, меньше ли длина строки команды 13 символов. Если это так, она выводит сообщение о том, что имя не было введено, и завершает выполнение функции.
Затем функция ищет первый пробел после 12-го индекса (где должно начинаться имя). Если пробел не найден (что обозначается `string::npos`), выводится сообщение об ошибке, и функция завершается.
Функция извлекает подстроку, начиная с 13-го индекса до конца строки команды. Эта подстрока предполагается как имя.
Функция находит первый и последний непробельные символы в извлеченном имени. Это важно для удаления лишних пробелов в начале и конце.
Если найдено допустимое последнее положение (что означает, что имя не состоит только из пробелов), оно обрезается с использованием найденных индексов. Обрезанное имя затем присваивается `this->name`, что, вероятно, представляет текущее имя пользователя.
В конце выводится сообщение с подтверждением изменения имени, и вызывается функция `do_write`, чтобы, предположительно, записать или отправить команду изменения.

Команда `-clear`: отчищает консоль 

Если строка не соответствует ни одной команде, то вызывается функция `do_write`, которая отправляет сообщение серверу.