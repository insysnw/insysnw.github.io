+++
title = "Альтернатива"
description = "Как ещё можно передать данные по сети"
weight = 6
draft = true
+++

Ранее, в [лекции по REST](lectures/rest) мы рассмотрели, что информацию можно
передавать не в формате, предназначенном для отображения страницы, а в формате
предназначенном для передачи данных. 
Примерами таких вариантов могут служить JSON и XML.
Если мы это уже знаем, зачем нам эта лекция?
Мы пойдем по порядку, а именно, рассмотрим что же такое JSON и XML и поймем, а
есть ли что-то более подходящее для передачи данных и нужно ли?

# XML

Extensible Markup Language.
Как видно из названия - язык разметки.
С его помощью задается структура представления данных.
Мы знаем, что существует еще HTML, который так же является языком разметки.
И в том, и в другом языках разметка представлена в виде тэгов.
Однако, XML не просто язык разметки, а расширяемый язык разметки.
Это значит что у него нет жестко заданных тэгов разметки (как у HTML), а это,
в свою очередь, позволяет разработчикам создать разметку по своим требованиям.

Пример XML:

{{ include_expandable_code(file="content/lectures/alternatives/xml_example", language="xml") }}

# JSON

JavaScript Object Notation - основной формат представления данных в JavaScript.
Несмотря на тот факт, что JSON тесно связан с JavaScript, он также как и XML 
является независимым от языка.
В отличии от тэгов в XML JSON представлен в виде пар ключ-значение.
При этом, значением могут быть не только строки, числа, логические выражения, но
и массивы и объекты.

Пример JSON:

{{ include_expandable_code(file="content/lectures/alternatives/json_example", language="json") }}

Преобразование данных в какую-либо структуру для последующей передачи по сети или
для его сохранения называется сериализацией.

# Encoding & Compression

* JSON в node не более ~500 метров (не серелизуется)
* Сереализация редко является узким место
* Числи очень малоэффективны и сложны (часто заворачиваются в строки, чтобы не потерять плавающую точку)

* Binary JSON - BSON http://bsonspec.org/
* https://developers.google.com/protocol-buffers/ https://github.com/protocolbuffers/protobuf 
* https://capnproto.org/

# Higher order of things

* gRPC - framework
* GraphQL - eeehm a Query Language ?!

* Версионирование
* Нельзя удалять или добавлять обязательные поля без изменения версии
* Изменение типа

## Rest

* ошибка бизнес логики может быть обёрнута в 200, чтобы избежать ретраев
* DTO ошибки

## gRPC
* protobuf by default (json convertion to degub, log)
* json is popular
* protocol agnostic

nullable fields are not transmitted (build from defaults on the client)

## GraphQL

* Получение данных (Back -> Front)
* non nullabale null whole object if this field is null
* массив ошибок
* Тестировние? Общий юзкейс?

Apollo Server

# Code/Spec generation

Code first
Spec first

# Kafka
Abro scheme

# graceful degradation
