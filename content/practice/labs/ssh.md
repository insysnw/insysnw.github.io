+++
title = "Удалённый доступ"
weight = 1
aliases = ["/labs/10-ssh/"]
[taxonomies]
categories = ["Лабораторная"]
+++

# [АХТУНГ!] Это черновик лабораторной

Вы этой работе мы:

* Получим доступ к персональной Виртуальной Машине(ВМ)
* Научимся решать типичные проблемы с доступом по SSH
* Попробуем проброс портов (туннелирование)
* Познакомимся с терминами JumpHost и Bastion

и не только.

Эта лабораторная требует от обучающихся предварительной подготовки - [создание пары ssh ключей](@/practice/guides/ssh-keygen.md) и передачу публичной части преподавателю.

IP адрес вам выдаст преподаватель.

Для доступа воспользуемся клиентом openssh:

```
ssh root@${IPv4Address} -i ~/.ssh/cnt
```
## Конфигурационный файл

Файл `~/.ssh/config` (`%USERPROFILE%\.ssh` для пользователей Windows)

Пример для ВМ с адресом `206.189.50.156`:

```
Host cnt-lab1
  User root
  HostName 206.189.50.156
  IdentityFile ~/.ssh/cnt
```

На данном этапе конфигурационный файл отличается от ранее приведённой команды только наличием поля `Host`.
Это имя по которому на локальной машине вы хотете обращаться к ВМ.
Используется только на локальной машине, т.е. можно выбрать то, что удобно именно вам.

## Типичные проблемы

В этом разделе мы искусственно создадим и решим одни из самых распространённых проблем при работе с OpenSSH.

* Сломать - починить права.

```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Permissions 0640 for '/home/insys/.ssh/cnt' are too open.
It is required that your private key files are NOT accessible by others.
This private key will be ignored.
Load key "/home/insys/.ssh/cnt": bad permissions
root@206.189.50.156: Permission denied (publickey).
```

* Сравнить host ключ сервера и known_host (обсудить форматы)

* Поменять ключ в known_hosts (причины поведения)
Настройка `StrictHostKeyChecking` [man ssh_config](https://linux.die.net/man/5/ssh_config)


## Копирование файлов

[scp устарел](https://www.openssh.com/txt/release-8.0)

Пример эксплуатации протокола rcp:

```
scp local-file remote:'`touch abuse-file`remote-file'
```

[source](https://lwn.net/SubscriberLink/835962/ae41b27bc20699ad/)

используем sftp, rsync

## proxy

Проверяем, с какого IP мы обращаемся.
Настраиваем браузер, curl.

```
ssh -D 8118 -N lab-cnt1
```

* `-N` Do not execute a remote command. This is useful for just forwarding ports.


```
curl ipinfo.io
```


```
curl -x socks5h://localhost:8118 ipinfo.io
```

Почему прокси плохо

## Тунелирование

### Локальное
**Локальное** (local) - проброс порта удалённой машины на локальную.

```
ssh -L 8080:localhost:80 -N cnt
```

edit `/var/www/html/index.nginx-debian.html`

### Удалённое
**Удалённое** (remote) - проброс локального порта на удалённую машину.

Для демонстрации удалённого туннелирования нам потребуется что-то, открытое на локальном порту.
Возьмём для этого веб сервер [Caddy](https://caddyserver.com/), так как он распространяется одним бинарным файлом без зависимостей и доступен для большинства платформ. 
Для этого нужно скачать файл сервера для вашей платформы на [странице загрузки](https://caddyserver.com/download).
Просто нажимаем кнопку **Download**, ведь дополнительные пакеты в этой лабораторной нам не нужны.

Так как мы будем использовать очень простую конфигурацию сервера, предлагаем создать в удобном для вас месте директорию `cnt-caddy` и дальнейшую работу с этим упражнением проводить в ней.
Именно в эту директорию положим только что загруженный файл и переименуем его в `caddy`.
Пользователям unix подобных систем стоит не забыть добавить бит исполнения - `chmod u+x caddy`

Теперь создадим в этой же директории конфигурационный файл для вэб сервера - `Caddyfile` со следующим содержанием:
```
:1620

respond "Hi, I'm caddy on YOUR_IDENTIFIER local machine
"
```

Для более простого опознавания предлагаю заменить `YOUR_IDENTIFIER` на что-то узнаваемое, например имя или никнэйм.

Теперь запустим caddy:
```
./caddy run
```

Проверим, что наш сервер работает!
Открываем в браузере `localhost:1620`

Если всё хорошо, вы увидите в браузере знакомое сообщение из конфигурационного файла.
Теперь попробует получить это же сообщение на удалённой виртуальной машине.

Начнём с проверки, что на желаемом порту виртуальной машины на данный момент ничего нет.
Открываем в браузере `206.189.50.156:1621` *(подставляем свой адрес)*.
Порт для виртуальной машины специально выбран отличный от такого на локальной, чтобы финальная команда была нагляднее.
Результат - отсутствие ответа.

Теперь откроем туннель командой
```
ssh -R 1621:localhost:1620 -N cnt
```

Снова открываем в браузере `206.189.50.156:1621`.
Результат - опять знакомая страничка.
Так мы выставили локальный для вашей машины сервис в общедоступный интернет.
Обычно это крайне сомнительная идея, но для изолированных сетей это выглядит значительно более оправдано.


## "Прыжки"

В рамках этого упражнения мы попадём на виртуальную машину, доступную только внутри изолированной сети.

![internalDroplet](../internalDroplet.png)

Первое и самое простое, попробуем получить к ней доступ напрямую:
```
ssh $InternalIP
```

* ручками, запуская клиент на бастионе - есть коннект, но всё ещё fail
* ручками, запуская клиент на бастионе с логином и паролем - win
* преподаватель отключает пароль и поясняет, чем это плохо

```
ssh root@$InternalIP -i ~/.ssh/cnt -o ProxyCommand="ssh cnt nc %h %p"
```

> **-W** Requests that standard input and output on the client be forwarded to host on port over the secure channel.

```
ssh root@$InternalIP -i ~/.ssh/cnt -o ProxyCommand="ssh cnt -W %h:%p"
```

```
ssh root@$InternalIP -i ~/.ssh/cnt -J cnt"
```

# Recap
Прямо сейчас попробуем применить новые знания к повседневной задаче!
Но для этого вам необходим профиль на github или gitlab.
Если их нет, можно смело пропустить это упражнение, ведь мы его проходили в самом начале.

## Github
Создадим новую пару ключей:

```
ssh-keygen -t ed25519 -f ~/.ssh/github
```

TODO: рекомендация по `passphrase` и его смене.

Теперь добавим только что созданный публичный ключ, `~/.ssh/github.pub`, в [settings/keys](https://github.com/settings/keys).

```
Host github
  User git
  HostName github.com
  IdentityFile ~/.ssh/github
```

Проверим наши настройки, выполнив подключение к github:
```
ssh github
```

Если всё успешно, вы увидите знакомое сообщение про принятие ключа хоста, а потом github вас поприветствует:

```
PTY allocation request failed on channel 0
Hi ejiek! You've successfully authenticated, but GitHub does not provide shell access.
Connection to github.com closed.
```

На месте `ejiek` вам должен предстать ваш username.



## Gitlab
```
ssh-keygen -t ed25519 -f ~/.ssh/gitlab
```

Теперь добавим только что созданный публичный ключ, `~/.ssh/gitlab.pub`, в [settings/SSH Keys](https://gitlab.com/-/profile/keys).

```
Host gitlab
  User git
  HostName gitlab.com
  IdentityFile ~/.ssh/gitlab
```

```
ssh gitlab
```
```
PTY allocation request failed on channel 0
Welcome to GitLab, @ejiek!
Connection to gitlab.com closed.
```

# Дополнительная информация

## Как попасть в CI pipeline?
Например, [tunshell](https://github.com/TimeToogo/tunshell)

## Бывает ли удобнее.
Бывает - [Teleport](https://gravitational.com/teleport/)
