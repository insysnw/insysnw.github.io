

## Минимально необходимая настройка
Перед использованием презентации на паре необходимо заполнить последний слайд - сгенерировать и вставить ссылку на телеграм канал в форме QR кода.

Рекомендуется делать закрытый канал и добавлять к нему чат, чтобы основные объявления и материалы можно было легко найти в общем обсуждение.

Для генерации QR можно в корневой директории данной презентации (`0.TCP`) выполнить:
``` bash
qrencode 'https://t.me/joinchat/AAAAAuniq2gibberish9' -s 9 -o images/tme.png
```

## Сборка презентации

This presentation is mostly written in Markdown markup language and intended to be build into revealjs site with pandoc.

To build:
```
pandoc -t revealjs --slide-level 2 -s tcp-presentation.md -o tcp-presentation.html
```
This builds html file which is dependant on revealjs sources from the internet.

`-s/--standalone` flag makes resulting html a link reveal.js in `reveal.js` directory next to html file.

`--self-contained` flags resulting presentation contain all of the necessary resources.

## Экспорт в PDF

**TODO**: проверить как это работает в chrome/chromium и чём-нибудь ещё и описать

## Поделиться результирующей со студентами

Влад считает, что для этого подходит html, собранный с опцией `--self-contained` и PDF. 
