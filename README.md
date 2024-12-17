# lib_ioshield
LCD16x2 IO Shield (WiringPi)

wiringPi install(wiki) : https://wiki.odroid.com/odroid-c4/application_note/gpio/wiringpi

```
// add apt-source list for wiringPi (ubuntu 24.01-odroidc4)
root@odroid:/etc/apt/sources.list.d$ cat ppa-linuxfactory-or-kr.list 
deb [trusted=yes] http://ppa.linuxfactory.or.kr noble main

root@odroid:/etc/apt/sources.list.d$ apt update
root@odroid:/etc/apt/sources.list.d$ apt install odroid-wiringpi
root@odroid:/etc/apt/sources.list.d$ apt install libwiringpi-dev
```

```
Usage: ./lib_ioshield [-xymctsr]

  -x --lcd_x         lcd x (COL) position. (default 0).
  -y --lcd_y         lcd y (ROW) position. (default 0).
  -m --msg           lcd display msg
  -c --lcd_clear     lcd clear line.(default -1, all clear)
  -t --show_time     current time display (offset)
  -s --led_set       led 1 ~ 7 on (D1 ~ D7)
  -r --led_clear     led 1 ~ 7 off(D1 ~ D7)
```

```
// LCD Display
./lib_ioshield -x 0 -y 0 -m 'hello odroid!!'
// Time Display & Offset
./lib_ioshield -t 9
// LED Control (D1 ~ D7), D1 On, D2 off
./lib_ioshield -s 1 -r 2
```
