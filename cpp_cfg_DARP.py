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


def stc2base(lst = (0,0)):
    resLst = []
#    1
    resLst.append((lst[0]*2,lst[1]*2))
#    2
    resLst.append((lst[0]*2 + 1,lst[1]*2))
#    3
    resLst.append((lst[0]*2,lst[1]*2  + 1))
#    4
    resLst.append((lst[0]*2 + 1,lst[1]*2 + 1))
    
    return resLst


if __name__ =='__main__':
    
    
    stc_row = 32
    stc_col = 32
    
    stc_mat = zeros((stc_row,stc_col),dtype=int)
    for i in range(stc_row):
        for j in range(stc_col):
            stc_mat[i][j] = 1
    
    base_row = stc_row *2
    base_col = stc_col *2
# zero means the obstacle pnt
# one means the way pnt

    robNum = 20
    random.seed(100)
    
    obNum = 300
    
    
    robRowLst = []
    robColLst = []
    while len(robRowLst)<robNum:
        robRow = random.randint(0,stc_row - 1)
        robCol = random.randint(0,stc_col - 1)
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
        obRow = random.randint(0,stc_row - 1)
        obCol = random.randint(0,stc_col - 1)
        reasonable = True
        for i in range(len(obRowLst)):
            if obRow == obRowLst[i] and obCol == obColLst[i]:
                reasonable = False
                break
        for i in range(len(robRowLst)):
            if obRow == robRowLst[i] and obCol == robColLst[i]:
                reasonable = False
                break
        if reasonable:
            stc_mat[obRow][obCol] = 0
            obRowLst.append(obRow)
            obColLst.append(obCol)
    
            

    
#    testNei = getNeighbor(envMat =mat,lst = [0,0],row =row,col=col)
#    print(testNei)
    
    sPntx = []
    sPnty = []
    tPntx = []
    tPnty = []
    G = nx.Graph()
    for i in range(stc_row):
        for j in range(stc_col):
            centre = (i,j)
            G.add_node((i,j))
            if(stc_mat[i][j] == 0):
                continue            
            neiLst =getNeighbor(envMat = stc_mat, lst = centre,row = stc_row, col =stc_col)
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

                
    
    conFileDir = './data//DARP_data//'
    conFileCfg = conFileDir + str(robNum)+'_'+str(base_row)+'_'+str(base_col)+'_'+str(obNum)+'_Outdoor_Cfg.txt'
    f_con = open(conFileCfg , 'w')
    
    f_con.write('time '+ datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")+'\n')
    writeConf(f_con,'row',[base_row])
    writeConf(f_con,'col',[base_col])
#   change the rob stc pos to the base position
    lst = robRowLst * 2
    b_robRowLst = [elment*2 for elment in robRowLst]
    b_robColLst = [elment*2 for elment in robColLst]    
    writeConf(f_con,'robRow',b_robRowLst)
    writeConf(f_con,'robCol',b_robColLst)
    robReachRowLst = []
    robReachColLst = []
    for unit in reachComponentLst:
        for gridUnit in component[unit]:
            robReachRowLst.append(gridUnit[0])
            robReachColLst.append(gridUnit[1])
    r_baseRowLst = []
    r_baseColLst = []
    for i in range(len(robReachRowLst)):
        baseLst = stc2base((robReachRowLst[i],robReachColLst[i]))
        for unit in baseLst:
            r_baseRowLst.append(unit[0])
            r_baseColLst.append(unit[1])
        
    writeConf(f_con,'robReachRowLst',r_baseRowLst)
    writeConf(f_con,'robReachColLst',r_baseColLst)

    robUnReachRowLst = []
    robUnReachColLst = []
    for unit in unReachCompLst:
        for gridUnit in component[unit]:
            robUnReachRowLst.append(gridUnit[0])
            robUnReachColLst.append(gridUnit[1])

    ur_baseRowLst = []
    ur_baseColLst = []
    for i in range(len(robUnReachRowLst)):
        baseLst = stc2base((robUnReachRowLst[i],robUnReachColLst[i]))
        for unit in baseLst:
            ur_baseRowLst.append(unit[0])
            ur_baseColLst.append(unit[1])
    
    writeConf(f_con,'robUnReachRowLst',ur_baseRowLst)
    writeConf(f_con,'robUnReachColLst',ur_baseColLst)
    
    b_obRowLst = ur_baseRowLst
    b_obColLst = ur_baseColLst
     
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
    base_mat = zeros((base_row,base_col),dtype=int)
    for i in range(base_row):
        for j in range(base_col):
            base_mat[i][j] = 1
    
    for i in range(len(ur_baseRowLst)):
        base_mat[ur_baseRowLst[i]][ur_baseColLst[i]] = 0
        
    grid = []    
    for x,y in ndindex(base_mat.shape):
        grid.append(int(base_mat[x][y]))
    writeConf(f_con,'grid',grid)
        
    writeConf(f_con,'obRow',b_obRowLst)
    writeConf(f_con,'obCol',b_obColLst)
    
    b_obNum = len(b_obRowLst)
    b_rNum = len(r_baseRowLst)
    
    b_gridNum = base_row *base_col
    print('obNum',b_obNum)
    print('b_rNum',b_rNum)
    print(b_gridNum)
    lowBound = b_rNum/robNum 
    writeConf(f_con,'lowBound',[lowBound])
    print('lowBound',lowBound)
    
    
    print(conFileCfg)

    f_con.close()
    
    conFileDir = './data//DARP_data//'
    
    conFileCfg = conFileDir + '_java_'+str(robNum)+'_'+str(stc_row)+'_'+str(stc_col)+'_'+str(obNum)+'CPP_Cfg.txt'
    
    print(conFileCfg)
    f_ja = open(conFileCfg , 'w')
    
    for i in range(len(robRowLst)):
        f_ja.write('EnvironmentGrid['+str(robRowLst[i])+']['+str(robColLst[i])+'] = 2;\n')
    
    for i in range(len(obRowLst)):
#        s_ind0  = math.floor((ob_x[i]+5)/20)
#        s_ind1  = math.floor((ob_y[i]+5)/20)
        f_ja.write('EnvironmentGrid['+str(obRowLst[i])+']['+str(obColLst[i])+'] = 1;\n')
        
    f_ja.close()
    
#    conFileCfg = conFileDir +'_java_'+str(robNum) +
