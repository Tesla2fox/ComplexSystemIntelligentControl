# -*- coding: utf-8 -*-
"""
Created on Fri Jun  8 17:34:12 2018

@author: robot
A simple gui to setup environment
"""

import tkinter as tk

n = 30
m = 20
width = 50

state_empty = 0
state_ob    = 1
state_agent = 2

btn = []
btn_text = []
btn_state = []
win = tk.Tk()

win.title('Configuration space')

def print_state():
    k = 0
    s = ''
    for i in range(n):
        for j in range(m):
            s = s+ str(btn_state[k])+'  '
            k = k + 1
        s += '\n'
    print(s)

def to_index(row, col):
    return row*m + col

def from_index(index):
    row = int(index/m)
    col = index % m
    return row, col

def btn_cmd(index):
    #row, col = from_index(index)
    btn_state[index] = (btn_state[index] + 1) % 2
    if btn_state[index] != 0:
        btn[index].configure(bg = 'blue')
    else:
        btn[index].configure(bg = 'gray')
        

for i in range(n):
    for j in range(m):
        index = to_index(i, j)
        txt = tk.StringVar()
        one = tk.Button(win, width = 15,height = 15,bitmap = 'gray12',
                        textvariable = txt, bg='gray', command = lambda x=index: btn_cmd(x))
        one.grid(row = i, column = j)
        txt.set('   ')
        btn.append(one)
        btn_text.append(txt)
        btn_state.append(0)

win.mainloop()
