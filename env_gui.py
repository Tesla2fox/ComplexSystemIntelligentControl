# -*- coding: utf-8 -*-
"""
Created on Fri Jun  8 17:34:12 2018

@author: robot
A simple gui to setup environment
"""

import tkinter as tk
from cpp_cfg import *



g_row = 40
g_col = 40

mat = ones((g_row,g_col),dtype=int)




# =============================================================================
#  here is for GUI
# =============================================================================
n = g_row
m = g_col
#width = 50

state_empty = 0
state_ob    = 1
state_agent = 2

btn = []
btn_text = []
btn_state = []
win = tk.Tk()

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

def form_index(index):
    row = int(index/m)
    col = index % m
    return row, col

def btn_cmd(index):
    #row, col = from_index(index)
    btn_state[index] = (btn_state[index] + 1) % 3
    if btn_state[index] == 0:
        btn[index].configure(bg = 'blue')
    if btn_state[index] == 1:
        btn[index].configure(bg = 'white')
    if btn_state[index] == 2:
        btn[index].configure(bg = 'black')
        

for i in range(n):
    for j in range(m):
        index = to_index(i, j)
        txt = tk.StringVar()
        one = tk.Button(win, textvariable = txt, bg='white', command = lambda x=index: btn_cmd(x))
        one.grid(row = i, column = j)
        txt.set('   ')
        btn.append(one)
        btn_text.append(txt)
        btn_state.append(1)

win.mainloop()
print_state()

robRowLst = []
robColLst = []
obRowLst = []
obColLst = []

robUnReachRowLst = []
robUnReachColLst = []
robReachRowLst = []
robReachColLst = []
#for state in btn_state:
for i in range(len(btn_state)):
    row,col = form_index(i)        
    if btn_state[i] == 0:
        robRowLst.append(row)
        robColLst.append(col)
    if btn_state[i] == 1:
        mat[row][col] = 1
        robReachRowLst.append(row)
        robReachColLst.append(col)        
        #        
#   case obstacle
    if btn_state[i] == 2:
        mat[row][col] = 0 
        obRowLst.append(row)
        obColLst.append(col)
        robUnReachRowLst.append(row)
        robUnReachColLst.append(col)        

        
print('change_complete')
robNum = len(robRowLst)
obNum = len(obRowLst)


conFileDir = './/data//'
conFileCfg = conFileDir + str(robNum)+'_'+str(g_row)+'_'+str(g_col)+'_'+str(obNum)+'_office_Cfg.txt'
f_con = open(conFileCfg , 'w')

f_con.write('time '+ datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")+'\n')
writeConf(f_con,'row',[g_row])
writeConf(f_con,'col',[g_col])
writeConf(f_con,'robRow',robRowLst)
writeConf(f_con,'robCol',robColLst)
writeConf(f_con,'robReachRowLst',robReachRowLst)
writeConf(f_con,'robReachColLst',robReachColLst)
writeConf(f_con,'robUnReachRowLst',robUnReachRowLst)
writeConf(f_con,'robUnReachColLst',robUnReachColLst)
                 
grid = []    
for x,y in ndindex(mat.shape):
    grid.append(int(mat[x][y]))
writeConf(f_con,'grid',grid)
    
writeConf(f_con,'obRow',obRowLst)
writeConf(f_con,'obCol',obColLst)
    
f_con.close()

drawPic(conFileCfg,1,'testNothing',True)
    
#print('wtf')