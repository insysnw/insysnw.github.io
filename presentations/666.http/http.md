---
title: Hypertext Transfer Protocol  
author: Алексюк Артём Олегович, Зорин Арсений Геннадьевич, Петров Владислав Дмитриевич
type: slide
slideOptions:
  transition: slide
---

# Recap

. . . 

До этого в курсе были рассмотрены самодостаточные протоколы прикладного уровня

# Hypertext Transfer Protocol

. . .

Зачем?

## 0.9 - Тёмные времена

##

```
➜ telnet academy.ejiek.com 80
Trying 188.242.22.225...
Connected to academy.ejiek.com.
GET /about_http/page
Hello, I'm pre html text.

Isn't it easy to read?
No mark up nonsense! The question is "How to make a Hypertext out of it"?
The fact it, I don't know what the idea was in HTTP 0.9 (which seemd to be developed before HTML)

Bye.
Connection closed by foreign host.
```

## 

`address_of_index ? keywordlist`

. . .

`http://cernvm/FIND/?sgml+cms`


# 1.0

. . .

Переход от простого отображения к взаимодействию

. . .

Так появились типы запросов
POST GET PUT DELETE (others)

. . .

обратная связь от сервера

# Status codes

* 1XX
* 2XX
* 3XX
* 4XX
* 5XX

# Составные страницы

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

# 1.1

* Keep-alive стал поведением по умолчанию

. . .

* Появился заголовок `Host`

## AJAX

Позволяет обновить часть страницы.

## Single Page Application

* Сокращает нагрузку на сеть, сервер, клиент
* Может даже работать offline

# HTTP 2

* Мультиплексирование 
* Бинарный заголовок

# HTTP 3

QUIC

##

* Установление сессий связи и шифрования объединены
* В этот процесс завезли кэширование
* В рамках одной сессии существуют несколько потоков

## Недостатки 

UDP является набором пакетов

# Открываем Dev Tools
