+++
title = "Типовые блокирующие решения"
description = "Архитектура сервера для работы со множеством клиентов"
weight = 2
+++

{{ youtube(id="MilImqovonE", class="youtuber") }}

В данной лабораторной мы научимся взаимодействовать с сокетами.
Попробуем взаимодействовать с ними, как с обычными файлами и найдём специфичные для них поведения.
Для более детального знакомства с тем как сокеты представляет Операционная Система, мы используем язык C.
Это вызвано тем, что библиотечные функции в C почти полностью повторяют интерфейс системных вызовов.
Более того, часто в описании системных вызовов приведены примеры на C.

Ранее, обсуждая сокеты мы договорились, что это абстракция для сетевого взаимодействия, выдающая себя за файл.
Начнём с проверки того, насколько сокет похож на файл.

Первое отличие от обычного файла - сокет не получится найти где угодно на просторах файловой системы.
Это особый файл, поэтому и создавать его придётся особым способом.

Для этого воспользуемся [библиотечной функцией socket][socket3], что внутри вызывает [одноимённый системный вызов][socket2]:

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

 printf("Yay\n");

}
```

Всё, что делает данная программа - создаёт сокет, убеждается в корректности его создания и выводит **Yay** в случае успешного создания.
С наибольшей вероятностью у вас эта программа, как и у нас, выведет **Yay**, что означает успешное создание сокета.

Рассмотрим подробнее [функцию socket][socket3].
У неё есть три аргумента:

* domain - семейство абстрагируемых каналов связи (это может быть как [IR], bluetooth, так и IPv4 стэк)
* type - бывает потоковый, датаграммный и SEQPACKET (но он нам в данном курсе не интересен)
* protocol - конкретный протокол для реализации вышеуказанного **типа** общения

В качестве домена мы используем "адресное семейство" `AF_INET` соответствующее IPv4.
Это семейство поддерживает только потоковый и датаграммный типы сокетов.
И каждому типу сокета соответствует только один протокол: TCP для потоковых, UDP для датаграммных.
Это приводит к тому, что последнее поле - protocol, можно не заполнять, так как вариант по умолчанию и есть единственный доступный.

Ещё один важный аспект этой функции - её возвращаемое значение - файловый дескриптор.
Вот оно, сходство с файлом!
Дальше можно передавать файловый дескриптор во все системные вызовы, что умеют с ним работать.
В том числе самые обычные [read][read2] и [write][write2].

# Работа как с файлом

Первым делом с новым сокетом попробуем [функцию чтения файлов][read3].

На вход она принимает файловый дескриптор, указатель на буфер и размер этого буфера.

```c
#include <errno.h> //new
#include <stdint.h> // new
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h> // new

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);

  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

// < new

  uint8_t buf[16];
  int bytes_read = read(socketfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(1);
  }

// new >
}
```

В таком виде данный пример не заработает.
Наш тип сокета ещё не готов к работе.
Отсутствует возможность к нему обратиться, так у него нет ни порта, ни адреса.
Поэтому и прочитать из него нельзя.

# Bind и первое чтение

С помощью [функции bind][bind3] мы присваиваем сокету свой адрес и порт на локальной машине.
На вход данная функция принимает файловый дескриптор, структуру представляющую собой описание сокета и ее размер.

```c
#include <arpa/inet.h> //new
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

// < new
  struct sockaddr_in sin;
  printf("Port before bind %d\n", ntohs(sin.sin_port));
  socklen_t len = sizeof(sin);

  int bindr = bind(socketfd, (struct sockaddr *) &sin, len);

  printf("Port after bind %d\n", ntohs(sin.sin_port));
  if (bindr == -1) {
    printf("Unable to bind: %d\n", errno);
    exit(1);
  }
// new >

  uint8_t buf[16];
  int bytes_read = read(socketfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(1);
  }
}
```

Мы создали **TCP** сокет, но у нас ещё ни с кем не установлена сессия.
А, следовательно, не создан и поток из которого можно читать.

Если заменить TCP сокет на UDP, то программа успешно запускается и "зависает".

```c
  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
```

На самом деле она [функцией read][read3] передаёт управление ядру до тех пор, пока на данный порт не прилетят какие-либо данные.
Как только данные появляются, ядро записывает их в указанный нами буфер и возвращает управление программе.

Именно такое поведение называется **блокирующим**, так как исполнение программы останавливается.

Утилитой `ss` с флагами `-tulpn` ищем занятый порт созданным UDP сокетом.

Утилитой `netcat` подключаемся к сокету и отправляем тестовое сообщение `kek`.
В результате программа выдаст **Bytes read: 4** и завершится.
Почему? Символа 3, соответственно и прочитано должно быть 3 байта.

Все дело в особенности работы `netcat`. При отправке сообщения к нему добавляется
символ конца строки ("\n") и увеличивает размер на 1 байт.

Очень важная особенность работы [функции read][read3] скрыта в возвращаемом значении.
Она возвращает количество прочитанных байт.
Последний аргумент функции - это именно размер буфера, т.е. верхнее ограничение.
Функция за раз не вернёт больше информации, но может легко вернуть меньше.

# Listen

UDP отработал успешно, так как в один порт могут без дополнительных ограничений приходить разные "клиенты".
TCP отличается тем, что с каждым клиентом нужно установить отдельное соединение.
Сокет инициализирующий соединение обычно работает всего с одним сетевым узлом (и называется активным).

Однако, если вы делаете сервер, вам нужно на одном порту общаться сразу с несколькими клиентами.

[Функция listen][listen3] позволяет перевести сокет в "слушающий режим" (такой сокет называется пассивным).
В нём ядро по определённому порту начинает "слушать" входящие соединения.

На вход функция принимает уже привычный файловый дескриптор и backlog 0.o
Backlog является верхним ограничением по количеству принимаемых соединений.

Что может мотивировать нас искусственно устанавливать такое ограничение?
Только ли это акт копирования примера из интернетов?

Давайте сначала чётко определим, что это за backlog.
В нём хранятся соединения, что мы ещё не приняли, т.е. принять и иметь в активном состоянии мы можем ощутимо большее количество сооединений.

Некоторые имплементации при передаче **0**, выставляют минимальное доступное значение, при котором возможна работа (так говорит документация, мы не виноваты).
Некоторые - [игнорируют](https://pubs.opengroup.org/onlinepubs/9699919799/functions/listen.html) это значение.
В С максимальное значение доступно в константе `SOMAXCONN`.

```c
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

  struct sockaddr_in sin;
  printf("Port before bind %d\n", ntohs(sin.sin_port));
  socklen_t len = sizeof(sin);

  int bindr = bind(socketfd, (struct sockaddr *) &sin, len);

  printf("Port after bind %d\n", ntohs(sin.sin_port));
  if (bindr == -1) {
    printf("Unable to bind: %d\n", errno);
    exit(2);
  }

// < new
  int listenr = listen(socketfd, 0);

  if (listenr == -1) {
    printf("Unable to listen: %d\n", errno);
    exit(3);
  }
// new >

  uint8_t buf[16];
  int bytes_read = read(socketfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(4);
  }
}
```

`listen` не является блокирующей функцией, что приведет к немедленной попытке чтения из еще не установленного соединения.

# Accept

[Функция accept][accept3] принимает соединение от клиента, который к нам подключается.
Так как функция является блокирующей, мы будем ожидать клиента, пока он к нам не придет.
Аргументами функции являются: файловый дескриптор сокета к которому будет организовано подключение, структура его описывающая и размер структуры.

В случае успешной организации соединения возвращаемым значением будет файловый дескриптор нового сокета.

```c
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd == -1) {
    printf("Unable to open a socket\n");
    exit(1);
  }

  struct sockaddr_in sin;
  printf("Port before bind %d\n", ntohs(sin.sin_port));
  socklen_t len = sizeof(sin);

  int bindr = bind(socketfd, (struct sockaddr *) &sin, len);

  printf("Port after bind %d\n", ntohs(sin.sin_port));
  if (bindr == -1) {
    printf("Unable to bind: %d\n", errno);
    exit(2);
  }

  int listenr = listen(socketfd, 3);

  if (listenr == -1) {
    printf("Unable to listen: %d\n", errno);
    exit(3);
  }
// < new
  int acceptfd = accept(socketfd, (struct sockaddr *) &sin, &len);

  if (acceptfd == -1) {
    printf("Unable to accept: %d\n", errno);
    exit(4);
  }
  printf("Accepted port: %d\n", ntohs(sin.sin_port));
// new >

  uint8_t buf[16];
  int bytes_read = read(acceptfd, (void *)buf, 16);

  printf("Bytes read: %d\n", bytes_read);
  if (bytes_read == -1) {
    printf("Unable to read: %d\n", errno);
    exit(5);
  }
}
```

Здесь нужно отметить, что мы пытаемся прочитать не из ранее созданного нами сокета (как мы это делали в предыдущих примерах), а из нового, полученного от функции `accept`.

И, наконец-то, наш пример позволил нам прочитать сообщение от клиента и вывел заветные **Bytes read: 4**.

# Пример с двумя писателями

В один поток/сокет читает многих

Соединения принимаются для многих, но программа на это не реагирует, а ждёт посылки от первого клиента (попавшего в accept).
[Пример поведения в записи](https://youtu.be/MilImqovonE?t=3762).

# Особые sys call'ы

Иногда работы с сокетами как с файлами недостаточно.
Для более гранулярной работы с сетевой природой сокетов существуют специальные системный вызовы.

[recv][recv2] - аналог [read][read2], но поддерживает указание дополнительных флагов.
Без них функция ведёт себя в точности как [read][read2].

Флаги позволяют позволяют изменить стандартное поведение.
Три ярких примера:

* Подсматривать в данные (читать без помечания данных прочитанными), т.е. следующее чтение всё ещё вернёт эти данные
* Блокировать исполнение, до тех пор, пока не будет прочитан указанный объём данных (или не произойдёт ошибка)
* Организовать неблокирующую работу в рамках вызова, а не сокета целиком

[send][send2] относится к [write][write2], так же как и recv к read.


Программа чтения
программа записи

совместный запуск

# Создание потока на каждого клиента

Пример сервера

Запуск


# Допы

## Errno

[errno](https://man.archlinux.org/man/errno.3p) - номер ошибки возникшей во время выполнения системного вызова или некоторых функций.

Значение `errno` глобальное.

## Сборка и запуск

Здесь все примеры написаны на языке `C`.
Для их сборки достаточно выполнить команду `gcc` и указать путь к файлу с исходным кодом.
Без указания флага `-o` код будет скомпилирован в исполняемый файл`a.out`.
Который можно сразу запустить: `./a.out`.

## Доменные имена

## IPv6?

[socket2]: https://man.archlinux.org/man/socket.2
[socket3]: https://man.archlinux.org/man/socket.3p
[read2]: https://man.archlinux.org/man/read.2
[read3]: https://man.archlinux.org/man/read.3p
[write2]: https://man.archlinux.org/man/write.2
[write3]: https://man.archlinux.org/man/write.3p
[listen2]: https://man.archlinux.org/man/listen.2
[listen3]: https://man.archlinux.org/man/listen.3p
[bind2]: https://man.archlinux.org/man/bind.2
[bind3]: https://man.archlinux.org/man/bind.3p
[accept2]: https://man.archlinux.org/man/accept.2
[accept3]: https://man.archlinux.org/man/accept.3p
[recv2]: https://man.archlinux.org/man/recv.2
[recv3]: https://man.archlinux.org/man/recv.3p
[send2]: https://man.archlinux.org/man/send.2
[send3]: https://man.archlinux.org/man/send.3p
