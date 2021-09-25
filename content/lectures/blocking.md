+++
title = "Типовые блокирующие решения"
description = "Архитектура сервера для работы со множеством клиентов"
weight = 2
+++

{{ youtube(id="MilImqovonE") }}

Ранее, обсуждая сокеты мы договорились, что это абстракция для сетевого взаимодействия, выдающая себя за файл.
Начнём с проверки того, насколько сокет похож на файл.

**TODO:** объяснить, почему мы используем C

Первое отличие: сокет не получится найти где угодно на просторах файловой системы.
Это особый файл, поэтому и создавать его придётся особым методом.

Для этого воспользуемся [библиотечной функцией socket][socket3], что вызывает [одноимённый системный вызов][socket2]:

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
С наибольшей вероятностью у вас эта программа, как и у нас, выведет **Yay**, что означает успешне создание сокета.

Рассмотрим подробнее функцию [socket][socket3].
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

Первым делом с новым сокетом попробуем [функцию чтение файлов][read3].

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

**TODO:** не можем прочитать, так как сокет не знает, откуда читать
**TODO**: меняем на UDP

```c
  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
```

Все работает

# Bind

[bind](https://man.archlinux.org/man/bind.3p)

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

**TODO**: Пишем 4 байта (почему 4, когда символа 3)

Очень важная особенность работы [функции read][read3] скрыта в возвращаемом значении.
Она возвращает количество прочитанных байт.
Последний аргумент функции - это именно размер буфера, т.е. верхнее ограничение.
Функция за раз не вернёт больше информации, но может легко вернуть меньше.


# listen

[listen](https://man.archlinux.org/man/listen.3p)

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

# accept()

[accept](https://man.archlinux.org/man/accept.3p)

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

# Пример с двумя писателями

В один поток/сокет читает многих

Соединения принимаются для многих, но программа на это не реагирует, а ждёт посылки от первого клиента (попавшего в accept).
[Пример поведения в записи](https://youtu.be/MilImqovonE?t=3762).

# Особые sys call'ы

[recv][recv3]
[send][send3]


обсудить флаги

Программа чтения
программа записи

совместный запуск

# Создание потока на каждого клиента

Пример сервера

Запуск


# Допы

## Errno

## Сборка и запуск

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
