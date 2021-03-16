+++
title = "HTTP"
weight = 2
transparent = true
[taxonomies]
categories = ["Лабораторная"]
+++

# Адресная строка в браузере не эквивалентна запросу
Для практического знакомства с протоколом мы сами напишем запрос.
Так как HTTP начинал свой путь как текст в TCP сессии, мы используем утилиту `telnet`, открывающую TCP сессии и позволяющую писать в и читать из двунаправленного потока.
Наша задача получить страницу по адресу [http://academy.ejiek.com/about\_http/page.html](http://academy.ejiek.com/about_http/page.html).
В качестве входных параметров `telnet` принимает адрес сервера и порт.
Адрес может быть как IP адресом, так и доменным именем.
Порт для HTTP - 80.

В командной строке/терминале:
```bash
telnet academy.ejiek.com 80
```
Сетевое общение начинается на разрешении доменного имени на стороне клиента (самим `telnet`) и более не используется.
`telnet` открывает TCP сессию.
Если сессия успешно открылась, мы увидим следующие строки:
```
Trying 188.242.22.225...
Connected to academy.ejiek.com.
Escape character is '^]'.
```
На данном этапе мы имеем TCP сессию, у которой открыты потоки в оба направления, но они пусты.
Это время для написания запроса, `telnet` ждёт ввода и отправит его по нажатию `Enter`.
```bash
GET /about_http/page.html
```
Это полноценных HTTP запрос.
Он имеет тип `GET` и запрашивает ресурс по пути `/about_http/page.html`.
Так как мы никак не указали версию протокола, сервер использует минимальную поддерживаемую **0.9**.
В ней поддерживается всего один тип ресурсов — текст.
В случае успешного запроса мы увидим:
```
<html>
  <p>In the first age</p>
  <p>In the first 18 tags</p>
  <p>When the hypertext first lenghtened</p>
  <p>One stood</p>
</html>
Connection closed by foreign host.
```
Всё, кроме последней строчки — ответ, а последняя строчка — сообщение от самого `telnet` о том, что сервер закрыл TCP сессию.
Это единственное поведение сервера в данной версии протокола.

# Способы передачи информации

Для просмотра содержимого запросов и ответов в этой лабораторной мы воспользуемся неотъемлемой частью современных браузеров — "Инструментами разработчика" (DevTools).
В Firefox, Chromium-based браузерах один из способов их открыть — клавиша `F12`.
Там нас интересует вкладка `Network`.

## Строка запроса
Исторически первый и самый часто используемый способ передачи данных — строка запроса.
Та, что в заголовке следует сразу за методом.
Посмотри на
В ранее рассмотренном примере:
```
GET /about_http/page.html HTTP/2
```
`/about_http/page.html` та самая строка запроса.
Вопреки рассмотренному примеру, она может состоять не только из пути до файла.
Рассмотрим возможные составляющий по порядку.

### Путь (path)
Изначально эта часть строки запроса соответствовала пути к файлу на сервере.
Для уже рассмотренной страницы [https://academy.ejiek.com/about\_http/page.html](https://academy.ejiek.com/about_http/page.html) на сервере дерево файлов выглядит так:
```
$ tree
.
└── about_http
    └── page.html

1 directory, 1 files
```

Но это вовсе не значит, что путь в запросе всегда соответствует файловой системе.
Так как сайты уже давно перестали быть набором статичных файлов, а превратились в сложные приложения, путь часто используется для указания желаемой страницы.
Например, группы, пользователя или проекта - [github.com/insysnw/insysnw.github.io](https://github.com/insysnw/insysnw.github.io).
Тут `insysnw` является группой, а `insysnw.github.io` - проектом.
Чаще всего запись о группе хранится в Базе Данных (БД) системы, а не директорией в файловой системе сервера.

### Запрос (query)

В [лекции по HTTP](/lectures/http) мы упоминали, что поиск был предусмотрен с самой первой версии (0.9) и имел формат:
```
address_of_index ? keywordlist
```

где `address_of_index` — путь до специального индексного файла на сервере, `?` — разделитель, `keywordlist` — список ключевых слов, разделённых символом `+`.
Проблема такого поиска в том, что это поиск по конкретному сайту и только в том случае, если специальный индексный файл был создан авторами.

Это не работающий, но корректно составленный, пример поиска `rick roll`'a на данном сайте:
```
https://insysnw.github.io/index.file?rick+roll
```
А не работает он потому, что специальный индексный файл (в данном случае `/index.file` нами не создан).

Давайте посмотрим, как будет выглядеть поисковый запрос в современном поисковике:
Для этого перейдём на [https://duckduckgo.com/](https://duckduckgo.com/)
и наберём `rick roll` и выполним поиск:

<center>

![скриншот запроса](duckduckgo.png)

</center>

Теперь обратим внимание на формат адресной строки после выполнения запроса:
```
https://duckduckgo.com/?q=rick+roll&t=h_&ia=web
```

Адрес самого поисковика остался неизменен `https://duckduckgo.com/` и путь до специального файла отсутствует, а строка поиска не соответствует ранее упомянутому формату:

```
?q=rick+roll&t=h_&ia=web
```

Теперь после `?` мы видим строку формата `Имя переменной` `=` `значение`, `&` в качестве разделителя переменных и `+` в качестве разделителя слов в рамках одной переменной.
Так последний запрос состоит из трёх переменных:

* `q` со значением `rick roll`
* `t` со значением `h_`
* `ia` со значением `web`

Это формат используемый для передачи метаданных.
В примере с поисковиком `q` очень распространённое название переменной для поискового запроса, а остальные параметры нужны поисковику.
Часто в таких переменных передают дополнительные параметры поиска: количество результатов на странице, тип сортировки, список категорий товара и многое другое.
Так как это просто переменные, их можно использовать для любых целей.

Проверим популярность переменной `q` в других поисковиках:

[ya.ru?q=rick+roll](https://ya.ru?q=rick+roll) | [google.ru?q=rick+roll](https://google.ru?q=rick+roll) | [bing.com?q=rock+roll](https://bing.com?q=rock+roll).
Каждый из них преобразует запрос к своему формату, но каждый поймёт, что мы от него хотели.

### Фрагмент

Иное название фрагментов - якорь.
В том случае, когда страница весьма масштабная якоря позволяют упростить процесс навицирования по ней, а именно - позволяют ссылаться на конкретную часть страницы.
Для этого, после запроса указывается якорь, который выглядит следующим образом `#anchor`.
Например:
[https://insysnw.github.io/practice/hw/udp-real-protocol/#rfc](https://insysnw.github.io/practice/hw/udp-real-protocol/#rfc)

В данном примере якорь `#rfc` приведет на подзаголовок страницы с соответствующим названием.
Но работает это не за счёт сходства названия фрагмента и подзаголовка, а из за свойства объкта `id`.
```
<h1 id="rfc">RFC</h1>
```

## Тело запроса

Строка запроса достаточно мощный инструмент, пригодный для создания для почти любых манипуляций с ресурсами, но не все данные удобно представлять однострочным текстом с ограничениями на многие символы.
Медиа файлы, бинарные протоколы или тот же JSON.

Именно для этого ещё в HTTP версии **1.0** тело, изначально созданное для передачи текста в ответах, было введено и для запросов.


# Коды состояния

В [лекции по HTTP](/lectures/http) мы упоминали, что с приходом HTTP версии 1.0 появились коды состояния, которые представляют из себя обратную связь от сервиса.

# 200
`OK` - запрос успешно выполнен.

**Пример**: [https://academy.ejiek.com/about\_http/page.html](https://academy.ejiek.com/about_http/page.html)

*Успешное получение страницы (может потребоваться отключить кэширование - disable cache):*

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
GET /about_http/page.html?kek HTTP/2
Host: academy.ejiek.com
User-Agent: Mozilla/5.0 Firefox/86.0
Accept: text/html,application/xhtml+xml...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Cookie: _ym_uid=160...
Upgrade-Insecure-Requests: 1
Pragma: no-cache
Cache-Control: no-cache
		</pre>
    </td>
    <td>
		<pre>
HTTP/2 200 OK
server: nginx/1.19.7
date: Thu, 11 Mar 2021 09:26:18 GMT
content-type: text/html
content-length: 135
last-modified: Tue, 02 Mar 2021 08:45:26 GMT
etag: "603dfb26-87"
strict-transport-security: max-age=15768000; includeSubDomains
x-robots-tag: noindex, nofollow, nosnippet, noarchive
accept-ranges: bytes
X-Firefox-Spdy: h2
		</pre>
    </td>
  </tr>
</table>

# 304
`Not Modified` — запрашиваемый ресурс уже присутствует на клиенте в актуальном виде.

**Пример:** [https://academy.ejiek.com/about\_http/page.html](https://academy.ejiek.com/about_http/page.html)

В примере [статуса 200](#200) в ответе есть заголовок `etag`, это метка конкретной версии ресурса.
При повторном запросе ресурса клиент отправляет заголовок `If-None-Match` со значением, полученным в `etag`.
Если это значение не изменится, то сервер ответ данным статусом без самого ресурса.
Это экономит трафик и позволяет перезагружать страницу быстрее, ведь отсутствует стадия загрузки.

* *может потребоваться включить кэширование — отключить **disable cache***

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
GET /about_http/page.html?kek HTTP/2
Host: academy.ejiek.com
User-Agent: Mozilla/5.0 ...
Accept: text/html...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Cookie: _ym_uid=...
Upgrade-Insecure-Requests: 1
If-Modified-Since: Tue, 02 Mar 2021 08:45:26 GMT
If-None-Match: "603dfb26-87"
Cache-Control: max-age=0
		</pre>
    </td>
    <td>
		<pre>
HTTP/2 304 Not Modified
server: nginx/1.19.7
date: Thu, 11 Mar 2021 09:38:24 GMT
last-modified: Tue, 02 Mar 2021 08:45:26 GMT
etag: "603dfb26-87"
strict-transport-security: max-age=15768000; includeSubDomains
x-robots-tag: noindex, nofollow, nosnippet, noarchive
X-Firefox-Spdy: h2
		</pre>
    </td>
  </tr>
</table>

# 301 & 302

`Moved Permanently` & `Found` - запрашиваемый ресурс был найден, но он находится на другом адресе (перенаправление)

**301** означает, что ресурс отныне доступен по другому адресу, что позволяет клиенту запомнить этот факт и в дальнейшем выполнять перенаправление локально.
**302** - временная мера и клиенту нужно будет за ресурсом снова обратиться по исходному адресу.

**Пример:** [http://academy.ejiek.com/about\_http/page.html](http://academy.ejiek.com/about_http/page.html)
*Обратите внимание на отсутствие `s` в `http`.*
* Если в логе видно только 200 или 304, а 301 - нет, необходимо включить `Persist logs`*

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
GET /practice HTTP/2
Host: insysnw.github.io
User-Agent: Mozilla/5.0 ...
Accept: text/html,...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Referer: https://insysnw.github.io/
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
TE: Trailers
		</pre>
    </td>
    <td>
		<pre>
HTTP/2 301 Moved Permanently
server: GitHub.com
content-type: text/html
strict-transport-security: max-age=31556952
location: https://insysnw.github.io/practice/
access-control-allow-origin: *
expires: Thu, 11 Mar 2021 10:13:28 GMT
cache-control: max-age=600
x-proxy-cache: MISS
x-github-request-id: ...
accept-ranges: bytes
date: Thu, 11 Mar 2021 10:03:28 GMT
via: 1.1 varnish
age: 0
x-served-by: cache-bma1637-BMA
x-cache: MISS
x-cache-hits: 0
x-timer: ...
vary: Accept-Encoding
x-fastly-request-id: ...
content-length: 162
X-Firefox-Spdy: h2
		</pre>
    </td>
  </tr>
</table>

*Существуют и альтернативные способы добиться данного результата.
Пример можно найти, разобрав, как работает перенаправление со [старой страницы лабораторной по удалённому доступу (/labs/10-ssh)](/labs/10-ssh).*

# 404
`Not Found` - страница отсутствует на сервере

**Пример:** [https://academy.ejiek.com/about\_http/missing_page.html](https://academy.ejiek.com/about_http/missing_page.html)

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
GET /about_http/missing_page.html HTTP/2
Host: academy.ejiek.com
User-Agent: Mozilla/5.0 ...
Accept: text/html...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Cache-Control: max-age=0
TE: Trailers
		</pre>
    </td>
    <td>
		<pre>
HTTP/2 404 Not Found
server: nginx/1.19.7
date: Thu, 11 Mar 2021 09:46:19 GMT
content-type: text/html
content-length: 153
strict-transport-security: max-age=15768000; includeSubDomains
X-Firefox-Spdy: h2
		</pre>
    </td>
  </tr>
</table>

# 418
`I'm a teapot` - отказ сварить кофе, т.к. вызываемое устройство - не кофемашина
**Пример:** [https://www.google.com/teapot](https://www.google.com/teapot)

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
HTTP/2 418 I'm a teapot
content-type: text/html; charset=UTF-8
strict-transport-security: max-age=31536000
content-encoding: br
date: Thu, 11 Mar 2021 09:50:38 GMT
server: gws
x-xss-protection: 0
x-frame-options: SAMEORIGIN
set-cookie: ...
alt-svc: h3-29=":443"; ...
X-Firefox-Spdy: h2
		</pre>
    </td>
    <td>
		<pre>
GET /teapot HTTP/2
Host: www.google.com
User-Agent: Mozilla/5.0 ...
Accept: text/html...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Cookie: ...
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
		</pre>
    </td>
  </tr>
</table>

# 502
`Bad Gateway` — сервер, к которому происходит обращение, является шлюзом и испытывает проблемы с открытием соединением к расположенному за ним ресурсу.

**Пример:** [https://academy.ejiek.com/about\_http/bad_gateway](https://academy.ejiek.com/about_http/bad_gateway)

<table>
  <tr>
    <th>
      Request
    </th>
    <th>
      Response
    </th>
  </tr>
  <tr>
    <td>
		<pre>
GET /about_http/bad_gateway HTTP/2
Host: academy.ejiek.com
User-Agent: Mozilla/5.0 ...
Accept: text/html,...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Sec-GPC: 1
Pragma: no-cache
Cache-Control: no-cache
TE: Trailers
		</pre>
    </td>
    <td>
		<pre>
HTTP/2 502 Bad Gateway
server: nginx/1.19.7
date: Thu, 11 Mar 2021 10:20:09 GMT
content-type: text/html
content-length: 157
strict-transport-security: max-age=15768000; includeSubDomains
X-Firefox-Spdy: h2
		</pre>
    </td>
  </tr>
</table>

# Заголовки

В [лекции по HTTP](/lectures/http) мы уже ознакомились с заголовками и их предназначением.
Здесь же мы рассмотрим наиболее часто используемые.

## Host

Очень важный для работы современных веб серверов и прокси.
Обязательный для версий протокола 1.1 и выше.

Проверим его наличие на странице **[https://academy.ejiek.com/about\_http/page.html](https://academy.ejiek.com/about_http/page.html)** в браузере.

`raw` заголовок запроса:

```
GET /about_http/page.html HTTP/2
Host: academy.ejiek.com
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:86.0) Gecko/20100101 Firefox/86.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Cookie: _ym_uid=1606166876113626088; _ym_d=1606166876
Upgrade-Insecure-Requests: 1
If-Modified-Since: Tue, 02 Mar 2021 08:45:26 GMT
If-None-Match: "603dfb26-87"
Cache-Control: max-age=0
TE: Trailers
```

Упущенным остаётся только протокол (`http` или `https`).

## User Agent

Сейчас заголовок чаще всего используется для снятия отпечатка устройства.
Иногда его используют для определения браузера или платформы, чтобы выдать соответствующую страницу.
Так, некоторые сервисы Google настоятельно рекомендуют использовать себя в *Google Chrome*, если открыть их в другом браузере.
Иногда они и вовсе недоступны, пока `User Agent` не будет удовлетворять их требованиям.
Часто это вызвано тем, что приложения тестируют только в определённых браузерах, а поддержку остальных совсем закрывают, чтобы сократить список возможных проблем.

Важно заметить, что сейчас вопросы отображения контента чаще всего решаются на стороне клиента, например, адаптивной вёрсткой.

Узнать из чего состоит отпечаток, собираемый рекламодателями и проверить его уникальность можно на [panopticlick.eff.org](https://panopticlick.eff.org/).

## ETag & If-None-Match

Эта пару и их взаимодействие разобраны в [статусе 304](#304).

