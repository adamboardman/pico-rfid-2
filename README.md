# Raspberry Pi Pico RFID 2 Library

This library lets you use the WS18505 and MFRC522 chips via an i2c
connection from a RPi Pico.

It's modified from the upstream:
https://github.com/m5stack/M5Stack/tree/master/examples/Unit/RFID_RC522

You can read the ID's from many cards that support the ISO 14443A
interface, I found most of my Credit Cards, a Library Card and my Oyster
Cards were all readable. It also supports more features including
writing and key handling with Mifare or NTAG203 cards.

The M5Stack RFID Unit 2 has printed upon it 5V, but the upstream data
sheet for the WS1850S claims that it works from 2.2V to 5.5V so its fine
with the Pico's 3.3V.

See https://github.com/adamboardman/rfid-security for an example of a
project using this library.
