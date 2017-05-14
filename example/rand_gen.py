# -*- coding: utf-8 -*-
import random

n = 1000
l = 10
chars = 'abcdefghijklmnopqrstuvwxyz0123456789-+=*@[]/?'

for i in range(n):
    l = random.randint(5, l)
    text = ''
    for j in range(l):
        c = random.choice(chars)
        text += c
    print(text)
