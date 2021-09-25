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

Для этого воспользуемся [библиотечной функцией socket](https://man.archlinux.org/man/socket.3p), что вызывает [одноимённый системный вызов](https://man.archlinux.org/man/socket.2):

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

Рассмотрим подробнее функцию `socket`.
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
Дальше можно передавать файловый дескриптор во все функции, что умеют с ним работать.
В том числе самые обычные [read](https://man.archlinux.org/man/read.3p) и [write](https://man.archlinux.org/man/write.3p).

# Работа как с файлом

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

# Bind

```
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

# listen

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


# Особые sys call'ы

send/revc

man 2 send/recv

обсудить флаги

Программа чтения
программа записи

совместный запуск

# Доменные имена

# Пример с двумя писателями

В один поток/сокет читает многих

# Создание потока на каждого клиента

Пример сервера

Запуск

# IPv6?
