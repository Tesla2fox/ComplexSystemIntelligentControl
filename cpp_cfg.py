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
import networkx as nx
import matplotlib.pyplot as plt



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

def getNeighbor(envMat,lst = (0,0),row =20, col =20):
    resLst = []
    #left
    lstLeft = (lst[0]-1,lst[1])
    if(lstLeft[0]>=0):
        if(envMat[lstLeft[0]][lstLeft[1]]==1):
            resLst.append(lstLeft)
    #right
    lstRight = (lst[0]+1,lst[1])
    if(lstRight[0]<row):
        if(envMat[lstRight[0]][lstRight[1]]==1):
            resLst.append(lstRight)    
    #top
    lstTop = (lst[0],lst[1]+1)
    if(lstTop[1]<col):
        if(envMat[lstTop[0]][lstTop[1]]==1):
            resLst.append(lstTop)    
    #bottom    
    lstBottom = (lst[0],lst[1]-1)
    if(lstBottom[1]>=0):
        if(envMat[lstBottom[0]][lstBottom[1]]==1):
            resLst.append(lstBottom)
    
    return resLst

if __name__ =='__main__':
    
    row = 20
    col = 20
    
    mat = zeros((row,col),dtype=int)
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
    
#    testNei = getNeighbor(envMat =mat,lst = [0,0],row =row,col=col)
#    print(testNei)
    
    sPntx = []
    sPnty = []
    tPntx = []
    tPnty = []
    G = nx.Graph()
    for i in range(row):
        for j in range(col):
            centre = (i,j)
            G.add_node((i,j))
            if(mat[i][j] == 0):
                continue            
            neiLst =getNeighbor(envMat = mat, lst = centre,row = row, col =col)
            for unit in neiLst:
#                print('')
                sPntx.append(i + 0.5)
                sPnty.append(j + 0.5)
                tPntx.append(unit[0] + 0.5)
                tPnty.append(unit[1] + 0.5)
                G.add_edge(centre,unit)
#    component2 = nx.connected_components(G)
#    print(component2)
    component = list(nx.connected_components(G))
    print(component)
    reachComponentLst = []
    unReachCompLst = [n for n in range(len(component))]
    for i in range(robNum):
        for j in range(len(component)):
            if(reachComponentLst.count(j) !=1):
                if((robRowLst[i],robColLst[i]) in component[j]):
                    reachComponentLst.append(j)
                    unReachCompLst.remove(j)
#                    print(reachComponentLst)            
                                
    print(reachComponentLst)
    print(len(component[0]))
                
#    print(00)
#    print(component)
#    print(len(component))
#    len(list())
    nx.draw(G)
    #nx.draw_random(G)
    #nx.draw_spectral(G)
    plt.show()

                
    
    conFileDir = './/data//'
    conFileCfg = conFileDir + str(robNum)+'_'+str(row)+'_'+str(col)+'_'+str(obNum)+'_Outdoor_Cfg.txt'
    f_con = open(conFileCfg , 'w')
    
    f_con.write('time '+ datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")+'\n')
    writeConf(f_con,'row',[row])
    writeConf(f_con,'col',[col])
    writeConf(f_con,'robRow',robRowLst)
    writeConf(f_con,'robCol',robColLst)
    robReachRowLst = []
    robReachColLst = []
    for unit in reachComponentLst:
        for gridUnit in component[unit]:
            robReachRowLst.append(gridUnit[0])
            robReachColLst.append(gridUnit[1])    
    writeConf(f_con,'robReachRowLst',robReachRowLst)
    writeConf(f_con,'robReachColLst',robReachColLst)

    robUnReachRowLst = []
    robUnReachColLst = []
    for unit in unReachCompLst:
        for gridUnit in component[unit]:
            robUnReachRowLst.append(gridUnit[0])
            robUnReachColLst.append(gridUnit[1])
    
    writeConf(f_con,'robUnReachRowLst',robUnReachRowLst)
    writeConf(f_con,'robUnReachColLst',robUnReachColLst)
                     
#==============================================================================
#   test the edge add function 
#==============================================================================
    
#    writeConf(f_con,'sPntx',sPntx)
#    writeConf(f_con,'sPnty',sPnty)
#    writeConf(f_con,'tPntx',tPntx)
#    writeConf(f_con,'tPnty',tPnty)    
# =============================================================================
#  end the test the edge add fucntion 
# =============================================================================
    grid = []    
    for x,y in ndindex(mat.shape):
        grid.append(int(mat[x][y]))
    writeConf(f_con,'grid',grid)
        
    writeConf(f_con,'obRow',obRowLst)
    writeConf(f_con,'obCol',obColLst)
        
    f_con.close()
    
