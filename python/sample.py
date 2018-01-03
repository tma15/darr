# -*- coding: utf-8 -*-
import darr

da = darr.DoubleArray()

words = ['くるま', 'く', 'くる', 'りんご', 'オレンジ', 'baseball', 'soccer']

v = 0
for word in words:
    v += 1.
    da.insert(word, v)

print('### common prefix search ###')
ret = da.common_prefix_search('くるまで')
for w in ret:
    print(w)

print('### get values ###')
for word in words:
    v = da.get(word)
    print(word, v)

da.save('dafile')

print('### load dumped double array ###')
da2 = darr.DoubleArray()
da2.load('dafile')

for word in words:
    v = da2.get(word)
    print(word, v)
