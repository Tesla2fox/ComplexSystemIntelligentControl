# -*- coding: utf-8 -*-
"""
Created on Sat May 12 20:47:33 2018

@author: robot

用于生成C++的cfg
CPP 指代coverage path planning 
"""
import time 
import datetime
import random
import math
import random
from numpy import *
from copy import *
import copy



def writeConf(fcon = open('text.txt', 'w'),name = '0-0',data = []):
    fcon.write(name+' ')
    for unit in data:
        fcon.write(' '+ str(unit))
    fcon.write('\n')

class MultiCPPcfg:
    def __init__(self,row = 20,col = 20,robNum =5, obNum = 80):
        self.row = row
        self.col = col
        self.robNum = robNum
        self.obNum = obNum
    def constructCfg(self):
        row = self.row
        col = self.col        
        mat = zeros((row,col))
        
        for i in range(row):
            for j in range(col):
                mat[i][j] = 1
    # zero means the obstacle pnt
    # one means the way pnt
    
        robNum = self.robNum
        random.seed(100)
        
        obNum = self.obNum
        
        robRowLst = []
        robColLst = []
        while len(robRowLst)<robNum:
            robRow = random.randint(0,row - 1)
            robCol = random.randint(0,col - 1)
            reasonable = True
            for i in range(len(robRowLst)):
                if robRow == robRowLst[i] and robCol == robColLst[i]:
                    reasonable = False
                    break
            if reasonable:
                robRowLst.append(robRow)
                robColLst.append(robCol)
                
        obRowLst = []
        obColLst = []
        while len(obRowLst)<obNum :
            obRow = random.randint(0,row - 1)
            obCol = random.randint(0,col - 1)
            reasonable = True
            for i in range(len(obRowLst)):
                if obRow == obRowLst[i] and obCol == obColLst[i]:
                    reasonable = False
                    break
            if reasonable:
                mat[obRow][obCol] = 0
                obRowLst.append(obRow)
                obColLst.append(obCol)
        
        conFileDir = './/data//'
        conFileCfg = conFileDir + str(robNum)+'_'+str(row)+'_'+str(col)+'_'+str(obNum)+'_Outdoor_Cfg.txt'
        f_con = open(conFileCfg , 'w')
        
        f_con.write('time '+ datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")+'\n')
        writeConf(f_con,'row',[row])
        writeConf(f_con,'col',[col])
        writeConf(f_con,'robRow',robRowLst)
        writeConf(f_con,'robCol',robColLst)
        
        grid = []    
        for x,y in ndindex(mat.shape):
            grid.append(int(mat[x][y]))
        writeConf(f_con,'grid',grid)
            
        writeConf(f_con,'obRow',obRowLst)
        writeConf(f_con,'obCol',obColLst)
            
        f_con.close()



if __name__ =='__main__':
    
    row = 19
    col = 19
    
    mat = zeros((row,col))
    
    for i in range(row):
        for j in range(col):
            mat[i][j] = 1
# zero means the obstacle pnt
# one means the way pnt

    robNum = 5
    random.seed(100)
    
    obNum = 80
    
    robRowLst = []
    robColLst = []
    while len(robRowLst)<robNum:
        robRow = random.randint(0,row - 1)
        robCol = random.randint(0,col - 1)
        reasonable = True
        for i in range(len(robRowLst)):
            if robRow == robRowLst[i] and robCol == robColLst[i]:
                reasonable = False
                break
        if reasonable:
            robRowLst.append(robRow)
            robColLst.append(robCol)
            
    obRowLst = []
    obColLst = []
    while len(obRowLst)<obNum :
        obRow = random.randint(0,row - 1)
        obCol = random.randint(0,col - 1)
        reasonable = True
        for i in range(len(obRowLst)):
            if obRow == obRowLst[i] and obCol == obColLst[i]:
                reasonable = False
                break
        if reasonable:
            mat[obRow][obCol] = 0
            obRowLst.append(obRow)
            obColLst.append(obCol)
    
    conFileDir = './/data//'
    conFileCfg = conFileDir + str(robNum)+'_'+str(row)+'_'+str(col)+'_'+str(obNum)+'_Outdoor_Cfg.txt'
    f_con = open(conFileCfg , 'w')
    
    f_con.write('time '+ datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")+'\n')
    writeConf(f_con,'row',[row])
    writeConf(f_con,'col',[col])
    writeConf(f_con,'robRow',robRowLst)
    writeConf(f_con,'robCol',robColLst)
    
    grid = []    
    for x,y in ndindex(mat.shape):
        grid.append(int(mat[x][y]))
    writeConf(f_con,'grid',grid)
        
    writeConf(f_con,'obRow',obRowLst)
    writeConf(f_con,'obCol',obColLst)
        
    f_con.close()
    
