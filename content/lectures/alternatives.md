+++
title = "Альтернатива"
description = "Как ещё можно передать данные по сети"
weight = 5
draft = true
+++

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
