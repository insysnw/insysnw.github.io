---
title: История HTTP
author: Зорин Арсений Геннадьевич, Петров Владислав Дмитриевич
type: slide
slideOptions:
  transition: slide
---

# Recap?

. . .

DNS, DHCP, NTP, SNMP, TFTP

. . .

Самописные

## Где они в TCP/IP?
::: notes

Какое место в стеке занимают упомянутые выше протоколы?
Прикладной уровень?

:::

| Прикладной        | Application      |
| --                | --               |
| ~~Представления~~ | ~~Presentation~~ |
| ~~Сеансовый~~     | ~~Session~~      |
| Транспортный      | Transport        |
| Сетевой           | Network          |
| Канальный         | Data link        |
| Физический        | Physical         |

# Маленький протокол, который смог

Hypertext Transfer Protocol 0.9

## Hypertext

Картинка связности текстов (документ указывающий на другой и так по кругу)

## HTML
::: notes
Убедиться, что в тегах были картинки, добавить картинку
:::

```html
<html>
  <p>In the first age</p>
  <p>In the first 18 tags</p>
  <p>When the hypertext first lenghtened</p>
  <p>One stood</p>
</html>
```

## Программное обеспечение
::: notes
до того, как arm и x86 захватили рынок пользовательской электроники, обилие систем, остутсвие браузеров
:::

```
$ telnet academy.ejiek.com 80
Trying 188.242.22.225...
Connected to academy.ejiek.com.
Escape character is '^]'.
GET /about_http/page.html
<html>
  <p>In the first age</p>
  <p>In the first 18 tags</p>
  <p>When the hypertext first lenghtened</p>
  <p>One stood</p>
</html>
Connection closed by foreign host.
```

## Перввая интерактивность

`address_of_index ? keywordlist`

. . .

`http://academy.ejiek.com/about_http/index?key+words`

# HTTP 1.0

Простое отображение -> Взаимодействие

## Типы запросов

POST UPDATE PUT DELETE ...

. . .

```
$ telnet academy.ejiek.com 80
Trying 188.242.22.225...
Connected to academy.ejiek.com.
Escape character is '^]'.
GET /about_http/page.html
...
```

## Status codes

* 1XX
* 2XX
* 3XX
* 4XX
* 5XX

## Усложнение страниц

* В 0.9 на каждый запрос открывались новое соединение
* Страницы начали содержать не только текст
* потребовался способ сообщить серверу, что не нужно закрывать соединение.

## Где хранить?

* тип запросв
* код состояния
* просьбу не закрывать соединение

## Заголовок

TODO: пример заколовка (формат запроса и ответа)

## Типы заколовков

* Request
* Response
* Entity
* General

# HTTP 1.1

. . .

* Keep-alive — поведение по умолчанию

. . .

* Появился заголовок `Host`

## Использование заголовка Host

TODO: картинка с примером "virtual host"
TODO: картинка с примером reverse proxy

## Оптимизация

TODO: картинка сайти с шапкой, футером и меню
TODO: картинка сайти с подсвеченнгой разницей между страницами

## AJAX
::: notes
Пример запроса и передаваемых данных

Основная проблема - передаётся сформерованная для отображения часть страницы.
:::

Asynchronous JavaScript and XML

## SPA

Single Page Application

# HTTP 2

* Мультиплексирование 
* Бинарный заголовок

# HTTP 3

QUIC

## QUIC

* Установление сессий связи и шифрования объединены
* В этот процесс завезли кэширование
* В рамках одной сессии существуют несколько потоков

## QUIC == Silver bullet?

# Recap

. . .

Путь из передачи текста в транспорт

. . .

https://insysnw.github.io/lectures/http/
