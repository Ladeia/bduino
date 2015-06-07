#!/usr/bin/env python2.7

import serial
import sqlite3

conn = sqlite3.connect('/home/pi/bduino/sensor_data.db')
ser = serial.Serial('/dev/ttyACM0', 9600)
c = conn.cursor()

c.execute("INSERT INTO reads VALUES ((select max(id)+1 from reads), DATETIME('now'))")
conn.commit()

while 1 :
    serial = ser.readline()

    #print(serial)

    c.execute("INSERT INTO json VALUES  ((select max(id) from reads), '"+ str(serial) +"' )")

    conn.commit()

    #conn.close()

