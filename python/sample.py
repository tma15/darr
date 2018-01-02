# -*- coding: utf-8 -*-
import darr

da = darr.DoubleArray()

words = ['アップル', 'オレンジ', 'baseball', 'soccer']

v = 0
for word in words:
    v += 1.
    da.insert(word, v)

for word in words:
    v = da.get(word)
    print(word, v)
da.save('dafile')

da2 = darr.DoubleArray()
da2.load('dafile')

for word in words:
    v = da2.get(word)
    print(word, v)
