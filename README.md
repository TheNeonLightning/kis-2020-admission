# kis-2020-admission 
Еременко Илья, группа Б05 - 831
# Задача С004
Реализуйте многопоточный RPC-демон и библиотеку RPC-клиента поверх системных вызовов.
# Реализация
Реализуется асинхронный сервер, взаимодействие с клиентом осуществляется с помощью сокетов. Для асинхронности используется механизм epoll. Таким образом сервер работает на одном потоке и выполняет поочерёдную обработку клиентов, не блокируясь в случае, если какой-то клиент пока не готов взаимодействовать. Также чтение и запись - неблокирующие. Всё вышеупомянутое направлено на то, чтобы не тратить ресурсы во время ожидания клиента и чтобы постоянно обрабатывать готовых клиентов.

Для RPC-вызовов на сервере используется обёртка syscall, ей от клиентов передаётся номер системного вызова, количесвто аргументов вызова, аргументы вызова.

Однозначно необходимо переделать механизм передачи аргументов: настроить передачу аргументов используя только один системный вызов (например передавать одним массивом char-ов и затем парсить). Также пока не обрабатываются возможные ошибки.

С точки зрения реализации сервера: идеальным вариантом для данной задачи будет распределение клиентов по нескольким потокам. Но не один поток на одного клиента (так при большом количестве клиентов будет создано много потоков, планировщих начнёт сам по себе сильно нагружать процессор, клиенты тольком не будут обрабатываться синхронно), а по группе клиентов на один поток, с описанной реализацией внутри одного потока (то есть с механизмом очереди epoll).
