# Echo Server Client
###### Лабораторная работа по технологиям разработки ПО

В программе реализовано приложение *эхо-сервер*, который возвращает клиенту то, что тот ему написал. Сервер может работать с любым числом клиентов. Когда подключается новый клиент, он запрашивает в консоли в бесконечном цикле у пользователя сообщение и отправляет его серверу. Сервер получает сообщение целиком и посылает его обратно. Сигналом окончания взаимодействия и закрытия соединения служит сообщение пользователя ***"exit"***.

Мини-проект содержит два отдельно запускаемых приложения, реализующих клиент ***Client_application*** и сервер ***Server_application***, взаимодействующих по сети посредством протокола ***tcp***. Как клиент, так и сервер являются полностью асинхронными. При этом сервер однопоточный, а на клиенте его взаимодействие с сервером вынесено в отдельный поток (от общения с пользователем в консольном интерфейсе) для обеспечения полной асинхронности.

Для сетевого соединения была использована библиотека ***boost asio***.

##### Принцип работы сервера:

* Сервер слушает на отдельном порту ***8001*** соединения от клиентов (*WaitForClient*).
* При подключении (***async_accept***) очередного клиента создается новая сессия ***Session*** со своим сокетом *socket* и буфером *buffer* для хранения сообщений клиента.
* Сервер читает (***async_read_until***) из сокета в буфер сообщение от клиента. Признаком конца сообщения является символ преноса строки '\n'.
* По окончании чтения сервер пишет (***async_write***) сообщение, полученное от клиента, из буфера в сокет и чистит буфер (*buffer.consume(length)*) для недопущения его переполнения.
* Далее он снова читает следующее сообщение и продолжает это в бесконечном цикле, пока клиент не закроет соединение.

##### Принцип работы клиента:

* Клиентпытается подключиться (***async_connect***) к серверу на порту ***8001*** с адресом ***127.0.0.1*** и стартует в отдельном потоке.
* Клиент запрашивает сообщение у пользователя в консоли.
* Полученное сообщение клиент пишет (***async_write***) в сокет для сервера, добавляя признак конца - символ преноса строки '\n'.
* Клиент читает (***async_read_until***) сообщение, полученное от сервера из сокета в буфер и выводит его в консоль для пользователя.
* Последовательность действий продолжается в бесконечном цикле, пока пользователь не ввдет сообщение ***"exit"***, означающее закрытие соединения между клиентом и сервером.