---
title: Маленький протокол, который смог
author: Алексюк Артём Олегович, Зорин Арсений Геннадьевич, Петров Владислав Дмитриевич
type: slide
slideOptions:
  transition: slide
---

# Recap

. . . 

Самодостаточные протоколы прикладного уровня

# Hypertext Transfer Protocol

. . .

Зачем?

# HTTP 0.9

Передача и отображение HyperText

##

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

## 

`address_of_index ? keywordlist`

. . .

`http://academy.ejiek.com/about_http/index?key+words`


# HTTP 1.0

. . .

Переход от простого отображения к взаимодействию

## HTTP 1.0

Так появились типы запросов
POST UPDATE PUT DELETE (others)

. . .

обратная связь от сервера

## Status codes

* 1XX
* 2XX
* 3XX
* 4XX
* 5XX

## Составные страницы

* В 0.9 на каждый запрос открывались новое соединение
* Страницы начали содержать не только текст
* потребовался способ сообщить серверу, что не нужно закрывать соединение.

## Где хранить?

* тип запросв
* код состояния
* просьбу не закрывать соединение

## Заголовок

. . .

* Request
* Response
* Entity
* General

. . .

`Connection: keep-alive`

# HTTP 1.1

. . .

* Keep-alive — поведение по умолчанию

. . .

* Появился заголовок `Host`

## AJAX

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
