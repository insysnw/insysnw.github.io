% Технологии компьютерных сетей
% Алексюк Артём Олегович, Зорин Арсений Геннадьевич, Петров Владислав Дмитриевич

# Отладка

# ifconfig

В мире Windows - `ipconfig`

# iproute2

## 

* **ip**
* **ss**
* arpd
* bridge
* ctstat
* devlink
* genl
* ifcfg
* ifstat
* lnstat
* nstat
* rdma
* routef
* routel
* rtacct
* rtmon
* rtpr
* rtstat
* tc
* tipc

# Address

```
> ip a
...
> ip address
```
## 

```
❯ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host
       valid_lft forever preferred_lft forever
2: enp0s25: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether f0:de:f1:9f:aa:8a brd ff:ff:ff:ff:ff:ff
    inet 10.1.99.67/24 brd 10.1.99.255 scope global dynamic noprefixroute enp0s25
       valid_lft 8485sec preferred_lft 8485sec
    inet6 fe80::a3ae:8b04:3734:7d7a/64 scope link noprefixroute
       valid_lft forever preferred_lft forever
3: wlp3s0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc mq state DOWN group default qlen 1000
    link/ether be:5f:b7:55:f6:a4 brd ff:ff:ff:ff:ff:ff permaddr 08:11:96:ca:0f:d4
```

## Модификация конфигурации

```
ip a add addr 192.168.88.1/24 dev wlan0
```


# Route

```
❯ ip r
...
❯ ip route
```

##

```
❯ ip r
default via 10.1.99.1 dev enp0s25 proto dhcp metric 100
10.1.99.0/24 dev enp0s25 proto kernel scope link src 10.1.99.67 metric 100
```

## 

```
❯ sudo ip r change default via 188.242.16.1 dev enp0s25 proto dhcp metric 200
```

# Neighbour (ARP) 

```
> ip n
...
> ip neighbour
```

## 

```
❯ ip neigh
10.1.99.1 dev enp0s25 lladdr 00:40:f4:6e:7b:52 REACHABLE
192.168.15.15 dev wlan0 lladdr dc:68:eb:00:c5:a8 STALE
192.168.15.12 dev wlan0  FAILED
```

# SocketS (ss)

Как netstat, но быстрее и вкуснее

. . .

Цветочек - тюлпан

. . .

`ss -tulpan`

## 

```
❯ ss -tulpn
Netid        State         Recv-Q        Send-Q               Local Address:Port                  Peer Address:Port        Process
udp          UNCONN        0             0                          0.0.0.0:51820                      0.0.0.0:*
tcp          LISTEN        0             128                        0.0.0.0:22                         0.0.0.0:*            users:(("sshd",pid=347,fd=3))
```

## Root or Not

# Ping

`ECHO_REQUEST`

## Pong

`ECHO_RESPONSE`

## Запись маршрута

Флаг `-R`

Лимитированное количество записей

Фильтруется Firewall'ом

# traceroute

Альтернативы - `tracepath`

# netcat
<!-- Prove it -->

## index.html

```
<html>
        <head>
                <title>NetCat page</title>
        </head>
        <body>
                <h1>Это целая страница</h1>
                <p>Выдаётся один раз, но из netcat</p>
        </body>
</html>
```

## Not A Web Server

```
printf 'HTTP/1.1 200 OK\n\n%s' "$(cat index.html)" | netcat -l 8080
```

# Traffic Control (tc)

Всё ещё часть `iproute2`

# WireShark

* CLI
* GUI
* Termshark

# Альтернативы

* iptraf-ng
