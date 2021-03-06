# -*- coding: utf-8 -*-
"""
Created on Mon Jun 11 21:43:14 2018

@author: robot
"""

import plotly.plotly as py
import plotly.graph_objs as go
import plotly
import random
from numpy import *
from copy import *
import copy
from read_cfg import Read_Cfg
from IPython.display import HTML,display 
import colorlover as cl
import plotly.io as pio

#import read_cfg 


class Pnt:
    def __init__(self,x=0,y=0):
        self.x = x
        self.y = y
    def pnt2dict(self):
        dic = dict(x = x,y= y)
        return dic
    def display(self):
        print('x = ',self.x,'y = ',self.y)

class Circle:
    def __init__(self,pnt = Pnt(),rad = 0):
        self.x = pnt.x
        self.y = pnt.y
        self.rad = rad
        self.x0 = self.x  - self.rad
        self.y0 = self.y  - self.rad
        self.x1 = self.x  + self.rad
        self.y1 = self.y  + self.rad
    def circle2dict(self):
        dic = dict()
        dic['type'] = 'circle'
        dic['xref'] = 'x'
        dic['yref'] = 'y'
        dic['x0'] = self.x0
        dic['y0'] = self.y0
        dic['x1'] = self.x1
        dic['y1'] = self.y1        
        dic['line'] = dict(color = 'rgba(50, 171, 96, 1)')
        return dic
class Line:
    def __init__(self,pnt0 =Pnt(),pnt1=Pnt()):
        self.x0 = pnt0.x
        self.y0 = pnt0.y
        self.x1 = pnt1.x
        self.y1 = pnt1.y
    def line2dict(self):
        dic= dict()
        dic['type']='line'
        dic['x0'] =self.x0
        dic['y0'] =self.y0
        dic['x1'] =self.x1
        dic['y1'] =self.y1
        dic['line'] = dict(color = 'rgb(128, 0, 128)')
        return dic
class Rect:
    def __init__(self,pnt =Pnt(),width =0,height =0):
        self.x0 = pnt.x
        self.y0 = pnt.y
        self.x1 = self.x0 + width
        self.y1 = self.y0 + height
    def rect2dict(self):
        dic = dict()
        dic['type']='rect'
        dic['x0'] = self.x0
        dic['y0'] = self.y0
        dic['x1'] = self.x1
        dic['y1'] = self.y1
        dic['line'] = dict(color = 'rgb(128, 0, 128)')
        return dic

def getLevelColor(level):
    strcolor = 'rgba('
    for i in range(3):
        strcolor = strcolor + str(level*50)+','
    strcolor = strcolor + str(1/level) +')'
    return strcolor    

colorLst = ['white','black']

class Env:
    def __init__(self, mat = zeros((2,2))):
        self.mat = mat
        self.shapeLst = []
        self.drawData = []
        self.annotations = []
    def addTest(self):
        
        pathTrace = go.Scatter(x = [5],
                    y = [5],
                    mode= 'lines',
                    line = dict(shape = 'spline',
                                width = 4),
                    name = 'Path_'+str(1))
        self.drawData.append(pathTrace)                    
    def addgrid(self):
        g_color = 'blue'
        row = len(self.mat)        
        for i in range(row):
            for j in range(len(self.mat[i])):
                pnt = Pnt(i,j)
                rect = Rect(pnt,1,1)
                rectDic = rect.rect2dict()
                rectDic['line']['color'] = g_color
                rectDic['line']['width'] = 0.1
#                rectDic['opacity'] =  1/(int(self.mat[i][j])+1)                
#                rectDic['fillcolor'] = colorLst[int(self.mat[i][j])]
                if(int(self.mat[i][j])==1):
                    rectDic['fillcolor'] = 'black'                
#                if(int(self.mat[i][j])==0):
#                    rectDic['fillcolor'] = colorLst[int(self.mat[i][j])]
#                getLevelColor(mat[i][j])
                self.shapeLst.append(copy.deepcopy(rectDic))
        print(len(self.shapeLst))
    def addRobotStartPnt(self,lst= []):
        for i in range(len(lst[0])):
            lst[0][i] = lst[0][i] + 0.5
            lst[1][i] = lst[1][i] + 0.5
            startTrace = go.Scatter(x =[lst[0][i]], y = [lst[1][i]],mode ='markers',marker = dict(symbol = 'cross-dot',size = 20),
                                    name = 
#                                    'start')
                                    'Robot_'+ str(i+1))
            self.drawData.append(startTrace)
    def addRobotTerminalPnt(self,lst= []):
        terminalTrace = go.Scatter(x =[lst[0]], y = [lst[1]],mode ='markers',marker = dict(symbol = 'circle-open-dot',size = 20),
                                    name = 'terminal')
        self.drawData.append(terminalTrace)
    def addEdges(self,lst= []):
        mark_x = []
        mark_y = []     
        for p in range (len(lst[0])):
            pnt0 = Pnt(lst[0][p] + 0.5,lst[1][p]+ 0.5)
            pnt1 = Pnt(lst[2][p] + 0.5,lst[3][p]+ 0.5)
            mark_x.append(pnt0.x)
            mark_x.append(pnt1.x)
            mark_y.append(pnt0.y)
            mark_y.append(pnt1.y)
            line = Line(pnt0,pnt1)
            lineDic = line.line2dict()
#                print(randColor())
            lineDic['line']['color'] = 'rgba(15,15,15,0.5)'
            lineDic['line']['width'] = 3
            self.shapeLst.append(copy.deepcopy(lineDic))
        markTrace = go.Scatter(mode ='markers',
                                   x= mark_x,
                                   y= mark_y,
                                   marker =dict(size =3),
                                   name = 'Spanning-Tree')
        self.drawData.append(markTrace)
    def addEdgeInPnt(self, lst = []):
        mark_x = []
        mark_y = []
        for  p in range(len(lst[0])):
            pnt0 = Pnt(lst[0][p],lst[1][p])
            pnt1 = Pnt(lst[2][p],lst[3][p])
            mark_x.append(pnt0.x)
            mark_x.append(pnt1.x)
            mark_y.append(pnt0.y)
            mark_y.append(pnt1.y)
            line = Line(pnt0,pnt1)
            lineDic = line.line2dict()
            lineDic['line']['color'] = 'red'
            lineDic['line']['width'] = 5
            self.shapeLst.append(copy.deepcopy(lineDic))
        markTrace = go.Scatter(mode ='markers',
                                   x= mark_x,
                                   y= mark_y,
                                   marker =dict(size =12),
                                   name = 'Spanning-Tree')
        self.drawData.append(markTrace)
    def addMultiEdgeInPnt(self, robNum = 5,lst =[]):
        for i in range(robNum):            
            mark_x = []
            mark_y = []
            for  p in range(len(lst[i][0])):                
                pnt0 = Pnt(lst[i][0][p],lst[i][1][p])
                pnt1 = Pnt(lst[i][2][p],lst[i][3][p])
                mark_x.append(pnt0.x)
                mark_x.append(pnt1.x)
                mark_y.append(pnt0.y)
                mark_y.append(pnt1.y)
                line = Line(pnt0,pnt1)
                lineDic = line.line2dict()
                lineDic['line']['color'] = 'black'
                lineDic['line']['width'] = 0.4
                self.shapeLst.append(copy.deepcopy(lineDic))
            markTrace = go.Scatter(mode ='markers',
                                       x= mark_x,
                                       y= mark_y,
                                       marker =dict(size =12),
                                       name = 'Spanning Tree_' + str(i +1))
            self.drawData.append(markTrace)
    def addGraph(self,robNum  = 0, lst = [], txtType = False):
        g_color = 'blue'
#        bupu = cl.scales[str(6)]['seq']['BuPu']
        print(robNum)
        bupu =  cl.scales[str(robNum)]['div']['PRGn']
        print(bupu)
        for i in range(robNum):
            for j in range(len(lst[2*i])):
                pnt = Pnt(int(lst[2*i][j]),int(lst[2*i +1][j]))
                rect = Rect(pnt,1,1)
                rectDic = rect.rect2dict()
                rectDic['line']['color'] = g_color
                rectDic['line']['width'] = 0.5
                rectDic['fillcolor'] = bupu[i]
                rectDic['opacity'] = 1
                if(False):
                    if(txtType):
                        self.annotations.append(dict(showarrow = False,
                                             x = pnt.x + 0.5 ,y = pnt.y + 0.5,
                                             text = str(i)))
                    else:
                        self.annotations.append(dict(showarrow = False,
                                             x = pnt.x + 0.5 ,y = pnt.y + 0.5,
                                             text = str(pnt.x)+'-'+str(pnt.y)))
                self.shapeLst.append(copy.deepcopy(rectDic))
    def addPath(self,robNum = 0, lst  = [],txtType = False):
        for i in range(robNum):
            x = lst[2*i]
            y = lst[2*i+1]
            markTrace = go.Scatter(mode ='lines',
                                   x= x,
                                   y= y,
#                                   marker =dict(size =8),
                                   line = dict(width = 3),
#                                   ,
                                   name = 'Path_' + str(i+1))
            if(txtType):
                for i in range(len(x)):
                    self.annotations.append(dict(showarrow = False,
                                             x = x[i] ,y = y[i],
                                             text = str(i)))                    
            self.drawData.append(markTrace)
    def addSTCGraph(self,robNum  = 0, lst = [], txtType = False):
        g_color = 'blue'
        bupu = cl.scales[str(robNum)]['seq']['BuPu']
        print(bupu)
        for i in range(robNum):
            for j in range(len(lst[2*i])):
                pnt = Pnt(int(lst[2*i][j])*2,int(lst[2*i +1][j])*2)
                rect = Rect(pnt,2,2)
                rectDic = rect.rect2dict()
                rectDic['line']['color'] = g_color
                rectDic['line']['width'] = 1
                rectDic['fillcolor'] = bupu[i]
                rectDic['opacity'] = 0.6
                if(txtType):
                    self.annotations.append(dict(showarrow = False,
                                             x = pnt.x + 1 ,y = pnt.y + 1,
                                             text = str(i)))
                else:
                    self.annotations.append(dict(showarrow = False,
                                             x = pnt.x + 1 ,y = pnt.y + 1,
                                             text = str(int(lst[2*i][j]))+'-'+
                                             str(int(lst[2*i +1][j]))))                    
                self.shapeLst.append(copy.deepcopy(rectDic))
    def addSTCVert(self,lst = []):
        for i in range(len(lst[0])):
            self.annotations.append(dict(showarrow = False,
                                             x = lst[0][i],y = lst[1][i],
                                             text = str(i)))
            
    def addNeiGraph(self,robNum  = 0, lst = []):
        g_color = 'blue'
        bupu = cl.scales[str(robNum)]['seq']['BuPu']
        print(bupu)
        for i in range(robNum):
            for j in range(len(lst[2*i])):
                pnt = Pnt(int(lst[2*i][j]),int(lst[2*i +1][j]))
                rect = Rect(pnt,1,1)
                rectDic = rect.rect2dict()
                rectDic['line']['color'] = g_color
                rectDic['line']['width'] = 0.5
                rectDic['fillcolor'] = bupu[i]
                rectDic['opacity'] = 1
                self.shapeLst.append(copy.deepcopy(rectDic))                
    def drawPic(self,name ='env',fileType = True):
        layout = dict()
        layout['shapes'] = self.shapeLst
        layout['xaxis'] = {'range':[0,len(self.mat[0])]}
        layout['yaxis'] = {'range':[0,len(self.mat)]}
        layout['xaxis'] = dict(
        autorange=True,
        showgrid=False,
        zeroline=False,
        showline=False,
#        autotick=True,
        ticks='',
        showticklabels = False)
        layout['yaxis'] = dict(
        scaleanchor = "x",
        autorange=True,
        showgrid=False,
        zeroline=False,
        showline=False,
#        autotick=True,
        ticks='',
        showticklabels = False)
        layout['font'] = dict(
            family='sans-serif',
            size=25,
            color='#000'
        )
        layout['legend'] =   dict(font=dict(
            family='sans-serif',
            size=25,
            color='#000'
        ))
        layout['autosize'] = False
        layout['height'] = 1000
        layout['width']= 1000
        layout['annotations'] = self.annotations
#        print(layout)
        layout['margin'] = dict(t = 15, l= 15,r = 20,b = 0)
        layout['showlegend'] = False
        fig = dict(data = self.drawData ,layout = layout)
        if(fileType):
            plotly.offline.plot(fig,filename = name + '.html',validate=False)
        else:
            pio.write_image(fig, name+ '.pdf')
#            py.image.save_as(fig,filename = name+'.jpeg')
            
        
def drawPic(cfgFileName = '5_20_20_80_Outdoor_Cfg.txt',drawType = 1,
            fileName = 'nothing',
            fileType = False ):
    py.sign_in('tesla_fox', 'HOTRQ3nIOdYUUszDIfgN')
    conFileDir = './/data//'
    degNameCfg = conFileDir + cfgFileName
    
    readCfg = Read_Cfg(degNameCfg)
    
    
    data = []
    readCfg.get('row',data)
    row = int(data.pop())

    readCfg.get('col',data)
    col = int(data.pop())
        
    mat = zeros((row,col))
    
    obRowLst = []
    obColLst = []
    readCfg.get('obRow',obRowLst)
    readCfg.get('obCol',obColLst)
    
    for i in range(len(obRowLst)):
        obRow = int(obRowLst[i])
        obCol = int(obColLst[i])
        mat[obRow][obCol] = 1 


    robRowLst = []
    robColLst = []
    readCfg.get('robRow',robRowLst)
    readCfg.get('robCol',robColLst)
        
    #case 1 draw Environment
    if(drawType == 1):
        env = Env(mat)
        env.addgrid()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)
        env.drawPic('./png/env_'+cfgFileName,fileType)
    #case 2 draw Environment with edges
    if(drawType == 2):
        env = Env(mat)
        edgeNameCfg = conFileDir +'obmapDeg.txt'
        edgeCfg = Read_Cfg(edgeNameCfg)
        edgeData = []
        edgeUnit = []
        edgeCfg.get('sRow',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('sCol',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))

        edgeUnit = []
        edgeCfg.get('tRow',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('tCol',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        
        env.addgrid()
        env.addEdges(edgeData)
        env.addTest()
        env.drawPic('./png/edges',fileType = False)
    #case 3 draw Envirionment with edges in pnt 
    if(drawType  == 3):
        env  = Env(mat)
#        edgeNameCfg = conFileDir +'obmapDeg.txt'        
        edgeNameCfg = conFileDir +'auctionSTCDeg.txt'
        edgeCfg = Read_Cfg(edgeNameCfg)
        edgeData = []
        edgeUnit = []
        edgeCfg.get('sPntx',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('sPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))

        edgeUnit = []
        edgeCfg.get('tPntx',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('tPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        
        env.addgrid()
        env.addEdgeInPnt(edgeData)
        env.addTest()
        env.drawPic('./png/edgesInPnt',fileType = fileType)
# case 4 draw Envirionment with graph
    if(drawType == 4):
        env = Env(mat)
        graphNameCfg = conFileDir +'auctionDeg.txt'
        graphNameCfg = conFileDir +'auctionSTCDeg.txt'
        graphCfg = Read_Cfg(graphNameCfg)
        robNum = int(graphCfg.getSingleVal('robNum'))
        graphData = []
        print('row',row)
        print('col',col)
        for i in range(robNum):
            graphUnit = []
            graphCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            graphCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addgrid()
        env.addTest()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addGraph(robNum,graphData,True)        
        env.addRobotStartPnt(robLst)
        env.drawPic('./png/robGraph',fileType = fileType)
#   case 5 
    if(drawType == 5):   
        env = Env(mat)        
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)
        env.addgrid()
        graphNameCfg = conFileDir +'auctionDeg.txt'
        graphCfg = Read_Cfg(graphNameCfg)
        robNum = int(graphCfg.getSingleVal('robNum'))
        graphData = []
        for i in range(robNum):
            graphUnit = []
            graphCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            graphCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addGraph(robNum,graphData)        
        graphData = []
        
        for i in range(robNum):
            graphUnit = []
            graphCfg.get('neiRow'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            graphCfg.get('neiCol'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        print(graphData)
        env.addNeiGraph(robNum,graphData)                    
        env.addTest()        
        env.drawPic('./png/robNei&Graph',fileType = fileType)
# case 4 draw Envirionment with graph
    if(drawType == 6):
        env = Env(mat)
        graphNameCfg = conFileDir +'auctionDeg.txt'
        graphNameCfg = conFileDir +'auctionSTCDeg.txt'
        graphCfg = Read_Cfg(graphNameCfg)
        robNum = int(graphCfg.getSingleVal('robNum'))
        graphData = []
        print('row',row)
        print('col',col)
        for i in range(robNum):
            graphUnit = []
            graphCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            graphCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addgrid()
        env.addTest()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addGraph(robNum,graphData,False)        
        env.addRobotStartPnt(robLst)
        env.drawPic('./png/robGraph&RC',fileType = fileType)

#   case 5 draw graph with STC
    if(drawType == 7):   
        env = Env(mat)        
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        graphNameCfg = conFileDir +'auctionSTCDeg.txt'
        graphCfg = Read_Cfg(graphNameCfg)
        robNum = int(graphCfg.getSingleVal('robNum'))
        graphData = []
        for i in range(robNum):
            graphUnit = []
            graphCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            graphCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addSTCGraph(robNum,graphData)        
        graphData = []
        
#        for i in range(robNum):
#            graphUnit = []
#            graphCfg.get('neiRow'+str(i),graphUnit)
#            graphData.append(copy.deepcopy(graphUnit))
#            graphUnit = []
#            graphCfg.get('neiCol'+str(i),graphUnit)
#            graphData.append(copy.deepcopy(graphUnit))
#        print(graphData)
#        env.addNeiGraph(robNum,graphData)                    
        env.addTest()
        env.addRobotStartPnt(robLst)
        env.addgrid()        
        env.drawPic('./png/robNei&Graph',fileType = fileType)
    
    
#  case 8  draw environment and the GA_pattern path    
    if(drawType == 8):
        env = Env(mat)
        env.addgrid()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)
        pathNameCfg = conFileDir +'_decode_Cfg.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        pathData = []
        for i in range(robNum):
            path_x = []
            pathCfg.get('path_x'+str(i),path_x)
            pathData.append(copy.deepcopy(path_x))
            path_y = []
            pathCfg.get('path_y'+str(i),path_y)
            pathData.append(copy.deepcopy(path_y))
        env.addPath(robNum = robNum, lst = pathData)
#            graphData.append(copy.deepcopy(graphUnit))
        env.drawPic('./png/env_'+cfgFileName,fileType)
        
# case 9 draw environment and test the python graph function
    if(drawType == 9):
        env  = Env(mat)
        edgeNameCfg = conFileDir +'5_20_20_80_Outdoor_Cfg.txt'
        edgeCfg = Read_Cfg(edgeNameCfg)
        edgeData = []
        edgeUnit = []
        edgeCfg.get('sPntx',edgeUnit)
        print('edgeLength',len(edgeUnit))
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('sPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))

        edgeUnit = []
        edgeCfg.get('tPntx',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        edgeCfg.get('tPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))        
        env.addgrid()
        env.addEdgeInPnt(edgeData)
        env.addTest()
        env.drawPic('./png/edgesInPntTestGraph',fileType = fileType)
#case 10 draw multi path
    if(drawType == 10):
        env = Env(mat)
        env.addgrid()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)
        pathNameCfg = conFileDir +'auctionSTCEstDegend.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        
        graphData = []
        print('row',row)
        print('col',col)
        for i in range(robNum):
            graphUnit = []
            pathCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            pathCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addGraph(robNum,graphData,False)        
        
        edgeData = []
        edgeUnit = []
        pathCfg.get('sPntx',edgeUnit)
        print('edgeLength',len(edgeUnit))
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        pathCfg.get('sPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))

        edgeUnit = []
        pathCfg.get('tPntx',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        pathCfg.get('tPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))        
        env.addgrid()
        env.addEdgeInPnt(edgeData)
        
        STCData =  []
        STCUnit = []
        pathCfg.get('STCPntx',STCUnit)
        STCData.append(copy.deepcopy(STCUnit))
        STCUnit =[]
        pathCfg.get('STCPnty',STCUnit)
#        print(STCUnit)
        STCData.append(copy.deepcopy(STCUnit))        
#        env.addgrid()
#        env.addSTCVert(STCData)
#        env.addEdgeInPnt(edgeData)
        
        pathData = []
        for i in range(robNum):
            path_x = []
            pathCfg.get('path_x'+str(i),path_x)
            pathData.append(copy.deepcopy(path_x))
            path_y = []
            pathCfg.get('path_y'+str(i),path_y)
            pathData.append(copy.deepcopy(path_y))
#        env.addPath(robNum = robNum, lst = pathData,txtType = False)
#            graphData.append(copy.deepcopy(graphUnit))
        
        env.drawPic('./png/env_'+cfgFileName+'path',fileType)
#   for the single stc
    if(drawType == 11):
        env = Env(mat)
        env.addgrid()
        robLst = []
        robLst.append(robRowLst)
        robLst.append(robColLst)
        
        pathNameCfg = conFileDir +'singleSTC.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        
        graphData = []
        print('row',row)
        print('col',col)
        print(robNum)
        for i in range(robNum):
            graphUnit = []
            pathCfg.get('row'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
            graphUnit = []
            pathCfg.get('col'+str(i),graphUnit)
            graphData.append(copy.deepcopy(graphUnit))
        env.addGraph(robNum,graphData,False)        
        terminalPnt = []
        pathData = []
        for i in range(robNum):
            path_x = []
            pathCfg.get('path_x'+str(i),path_x)
            pathData.append(copy.deepcopy(path_x))
            terminalPnt.append(path_x[len(path_x)- 1])
            path_y = []
            pathCfg.get('path_y'+str(i),path_y)
            pathData.append(copy.deepcopy(path_y))
            terminalPnt.append(path_y[len(path_y)- 1])
        env.addPath(robNum = robNum, lst = pathData,txtType = False)

        env.addRobotStartPnt(robLst)
        env.addRobotTerminalPnt(terminalPnt)
        
        edgeData = []
        edgeUnit = []
        pathCfg.get('sPntx',edgeUnit)
        print('edgeLength',len(edgeUnit))
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        pathCfg.get('sPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))

        edgeUnit = []
        pathCfg.get('tPntx',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))
        edgeUnit =[]
        pathCfg.get('tPnty',edgeUnit)
        edgeData.append(copy.deepcopy(edgeUnit))        
        env.addgrid()
        env.addEdgeInPnt(edgeData)
        

#            graphData.append(copy.deepcopy(graphUnit))        
        env.drawPic('./png/env_'+cfgFileName+'path',fileType)
#        print('wtf')
    #case 10 draw multi path
    if(drawType == 12):
        env = Env(mat)
        env.addgrid()
        robLst = []
        pathNameCfg = conFileDir +'auctionSTCEstDeg.txt'
        pathNameCfg = conFileDir +'CfgDeg2.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        
        
        pathData = []
        for i in range(robNum):
            path_x = []
            pathCfg.get('path_x'+str(i),path_x)
            pathData.append(copy.deepcopy(path_x))
            path_y = []
            pathCfg.get('path_y'+str(i),path_y)
            pathData.append(copy.deepcopy(path_y))
        env.addPath(robNum = robNum, lst = pathData,txtType = False)
#            graphData.append(copy.deepcopy(graphUnit))
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)        
        env.drawPic('./png/env_'+cfgFileName+'JustPath',fileType)
    if(drawType == 13):
        env = Env(mat)
        spanningData = []
        pathNameCfg = conFileDir +'auctionSTCEstDeg.txt'
        pathNameCfg = conFileDir +'CfgDeg2.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        for i in range(robNum):
            edgeData = []
            edgeUnit = []
            pathCfg.get('sPntx'+str(i),edgeUnit)
            print('edgeLength',len(edgeUnit))
            edgeData.append(copy.deepcopy(edgeUnit))
            edgeUnit =[]
            pathCfg.get('sPnty'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit))
    
            edgeUnit = []
            pathCfg.get('tPntx'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit))
            edgeUnit =[]
            pathCfg.get('tPnty'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit)) 
            spanningData.append(copy.deepcopy(edgeData))
        env.addMultiEdgeInPnt(robNum = robNum, lst =  spanningData)
        env.drawPic('./png/env_'+cfgFileName+'spanningTree',fileType)
    if(drawType == 14):
        env = Env(mat)
        env.addgrid()
        robLst = []
        print('0-0-',len(cfgFileName))
        cfgFileNameStri = cfgFileName.split('.')
        pathNameCfg = conFileDir +'GAcomp//'+ cfgFileNameStri[0]+'auctionSTCEstDeg.txt'
        pathCfg = Read_Cfg(pathNameCfg)
        robNum = int(pathCfg.getSingleVal('robNum'))
        
        
        pathData = []
        for i in range(robNum):
            path_x = []
            pathCfg.get('path_x'+str(i),path_x)
            pathData.append(copy.deepcopy(path_x))
            path_y = []
            pathCfg.get('path_y'+str(i),path_y)
            pathData.append(copy.deepcopy(path_y))        
        env.addPath(robNum = robNum, lst = pathData,txtType = False)
#            graphData.append(copy.deepcopy(graphUnit))
        spanningData = []
        for i in range(robNum):
            edgeData = []
            edgeUnit = []
            pathCfg.get('sPntx'+str(i),edgeUnit)
            print('edgeLength',len(edgeUnit))
            edgeData.append(copy.deepcopy(edgeUnit))
            edgeUnit =[]
            pathCfg.get('sPnty'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit))
    
            edgeUnit = []
            pathCfg.get('tPntx'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit))
            edgeUnit =[]
            pathCfg.get('tPnty'+str(i),edgeUnit)
            edgeData.append(copy.deepcopy(edgeUnit)) 
            spanningData.append(copy.deepcopy(edgeData))
        env.addMultiEdgeInPnt(robNum = robNum, lst =  spanningData)
        robLst.append(robRowLst)
        robLst.append(robColLst)
        env.addRobotStartPnt(robLst)        
        env.drawPic('./png/env_'+cfgFileName+'JustPath',fileType)
        
if __name__ == '__main__':

    
    conFileDir = './/data//'
#    cfgFileName  = '5_20_20_80_Outdoor_Cfg.txt'
#    degNameCfg = conFileDir + cfgFileName
#    
#    readCfg = Read_Cfg(degNameCfg)
#    
#    
#    data = []
#    readCfg.get('row',data)
#    row = int(data.pop())
#
#    readCfg.get('col',data)
#    col = int(data.pop())
#        
#    mat = zeros((row,col))
#    
#    obRowLst = []
#    obColLst = []
#    readCfg.get('obRow',obRowLst)
#    readCfg.get('obCol',obColLst)
#    
#    for i in range(len(obRowLst)):
#        obRow = int(obRowLst[i])
#        obCol = int(obColLst[i])
#        mat[obRow][obCol] = 1 
#
#
#    robRowLst = []
#    robCowLst = []
#    readCfg.get('robRow',robRowLst)
#    readCfg.get('robCol',robColLst)
#    
#    
#    
#    drawType = 1
#    #case 1 draw Environment
#    if(drawType == 1):
#        env = Env(mat)
#        env.addgrid()
#        robLst = []
#        robLst.append(robRowLst)
#        robLst.append(robColLst)
#        env.addRobotStartPnt(robLst)
#        env.drawPic('./png/env_'+cfgFileName,fileType = False)
#    #case 2 draw Environment
#    if(drawType == 2):
#        env = Env(mat)
#        edgeNameCfg = conFileDir +'obmapDeg.txt'
#        edgeCfg = Read_Cfg(edgeNameCfg)
#        edgeData = []
#        edgeUnit = []
#        edgeCfg.get('sRow',edgeUnit)
#        edgeData.append(copy.deepcopy(edgeUnit))
#        edgeUnit =[]
#        edgeCfg.get('sCol',edgeUnit)
#        edgeData.append(copy.deepcopy(edgeUnit))
#
#        edgeUnit = []
#        edgeCfg.get('tRow',edgeUnit)
#        edgeData.append(copy.deepcopy(edgeUnit))
#        edgeUnit =[]
#        edgeCfg.get('tCol',edgeUnit)
#        edgeData.append(copy.deepcopy(edgeUnit))
#        
#        env.addgrid()
#        env.addEdges(edgeData)
#        env.addTest()
#        env.drawPic('./png/edges',fileType = False)