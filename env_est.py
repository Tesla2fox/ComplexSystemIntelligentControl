# -*- coding: utf-8 -*-
"""
Created on Fri Aug 24 00:28:12 2018

@author: stef_leonA
"""

from env import *
import plotly.plotly as py
import plotly.graph_objs as go
import plotly
import numpy as np
import plotly.io as pio

import os  
      
def file_name(file_dir):   
    for root, dirs, files in os.walk(file_dir):  
        print(root) #当前目录路径  
        print(dirs) #当前路径下所有子目录  
        print(files) #当前路径下所有非目录子文件
    return root,files


if __name__ =='__main__':
    py.sign_in('tesla_fox', 'HOTRQ3nIOdYUUszDIfgN')

    figData = []
    t_ms_oLst = []
    
    robotCfg = './/data//est//'+'_40_40_300_Outdoor_EstMakespanData.txt'
    orderCfg = './/data//est//'+'_40_40_300_Outdoor_orderData.txt'
    robCfg = Read_Cfg(robotCfg)
    ordCfg = Read_Cfg(orderCfg)
    
    
    t_ms_tLst = []
    ms_tLst = []
    ms_oLst = []
    bias_lst = []
    xx = list(range(2,11))
    print(xx)    
    for x in xx:
        ms_lst = []
        robCfg.get('makespan'+str(x),ms_lst)
        ms_tLst.append(ms_lst)
        ms_mean = np.mean(np.array(ms_lst))
        print(ms_mean)
        est_lst = []
        robCfg.get('EstMakespan'+str(x),est_lst)
        est_mean = np.mean(np.array(est_lst))
        ratio = (est_mean - ms_mean)/est_mean
        ord_ms = ordCfg.getSingleVal('makespan'+str(x))
        ms_oLst.append(ord_ms)
        bias_lst.append(ratio)
    t_ms_tLst.append(ms_tLst)
    t_ms_oLst.append(ms_oLst)
    
        
    print(len(bias_lst))
    print(x)
    markTrace = go.Scatter(mode ='lines+markers',
                                   x= xx,
                                   y= bias_lst,
                                   marker =dict(size =20),
                                   line = dict(width = 5),
#                                   ,
                                   name = 'Space I')    
    
    figData.append(markTrace)

    
#    figData = []
    robotCfg = './/data//est//'+'_40_40_301_Outdoor_EstMakespanData.txt' 
    orderCfg = './/data//est//'+'_40_40_301_Outdoor_orderData.txt'
    robCfg = Read_Cfg(robotCfg)
    ordCfg = Read_Cfg(orderCfg)
    
    
    ms_tLst = []
    bias_lst = []
    xx = list(range(2,11))    
    for x in xx:
        ms_lst = []
        robCfg.get('makespan'+str(x),ms_lst)
        ms_tLst.append(ms_lst)
        ms_mean = np.mean(np.array(ms_lst))
        est_lst = []
        robCfg.get('EstMakespan'+str(x),est_lst)
        est_mean = np.mean(np.array(est_lst))
        ratio = (est_mean - ms_mean)/est_mean
        ord_ms = ordCfg.getSingleVal('makespan'+str(x))
        ms_oLst.append(ord_ms)
        bias_lst.append(ratio)

    t_ms_tLst.append(ms_tLst)
    t_ms_oLst.append(ms_oLst)


    
    print(bias_lst)
    markTrace = go.Scatter(mode ='lines+markers',
                                   x= xx,
                                   y= bias_lst,
                                   marker =dict(size =20),
                                   line = dict(width = 5),
                                   name = 'Space II')   
    figData.append(markTrace)

#    figData.append
    layout = dict()
    layout['autosize'] = False
    layout['xaxis'] = dict(
        titlefont=dict(size=35),
        autorange=True,
        showgrid=False,
        zeroline=True,
        showline=True,
#            autotick=True,
        automargin = True,
        title ='Number of robots',
        ticks='',
        showticklabels = True)
#
##    layout['aaxis']['titleoffset'] =20
    layout['yaxis'] = dict(
        titlefont=dict(size=30),
        showline=True,
        automargin = True,
        title = 'Makespan (s)')    
    layout['font'] = dict(
            family='sans-serif',
            size=30,
            color='#000')
    layout['legend'] =   dict(font=dict(
            family='sans-serif',
            size=20,
#                color='#000'
        ))
    layout['titlefont'] = dict(size = 1)
    layout['autosize'] = True
    layout['height'] = 3000
    layout['width']= 4500
    layout['margin'] = dict(t = 30)
#    setTrace = go.Box(y=_set, name ='$    G_'+str(1)+'$',)
    fig = go.Figure(data = figData , layout = layout)
    pio.write_image(fig, 'est.pdf')

#    plotly.offline.plot(fig,filename = 'name')
#    py.image.save_as(fig,filename = 'est'+'.jpeg')
    reachNum = [1298, 1294]
    print(t_ms_tLst)
    for p in range(len(t_ms_tLst)): 
        ms_tLst = t_ms_tLst[p]
        ms_oLst = t_ms_oLst[p]
        figData = []
        lb_lst = [reachNum[p]/x for x in xx]
        markTrace = go.Scatter(mode ='lines+markers',
                                       x= xx,
                                       y= lb_lst,
                                       marker =dict(size = 6),
                                       line = dict(width = 3,
                                                   dash = 'dash'),
    #                                   ,
                                       name = 'LB')
        figData.append(markTrace)
        markTrace = go.Scatter(mode ='lines+markers',
                                       x= xx,
                                       y= ms_oLst,
                                       marker =dict(size = 6),
                                       line = dict(width = 3,
                                                   dash = 'dot'),
    #                                   ,
                                       name = 'Order')
        figData.append(markTrace)

        for i in range(len(ms_tLst)):
            x_lst = [i+2 for p in range(len(ms_tLst))] 
            print(x_lst)
            setTrace = go.Box(y = ms_tLst[i],x = x_lst, name ='n = ' + str(i + 2),
                              )
            figData.append(setTrace)
        layout.clear()
        layout['autosize'] = False
        layout['xaxis'] = dict(
            titlefont=dict(size=35),
            autorange=True,
            showgrid=False,
            zeroline=True,
            showline=True,
#            autotick=True,
            automargin = True,
            title ='Number of robots',
            ticks='',
            showticklabels = True)
    #
    ##    layout['aaxis']['titleoffset'] =20
        layout['yaxis'] = dict(
            titlefont=dict(size=30),
            showline=True,
            automargin = True,
            title = 'Makespan (s)')    
        layout['font'] = dict(
                family='sans-serif',
                size=30,
                color='#000')
        layout['legend'] =   dict(font=dict(
                family='sans-serif',
                size=20,
#                color='#000'
            ))
        layout['autosize'] = True
        layout['height'] = 3000
        layout['width']= 4500
#        layout['left'] = 100
#        layout['bottom'] = 100
        layout['margin'] = dict(t = 30)        
        fig = go.Figure(data = figData,layout = layout)
#        py.image.save_as(fig,filename = 'dec'+str(p + 1)+'.pdf')
#        pio.write_image(fig, 'fig1.pdf')
        pio.write_image(fig,'dec'+str(p+1)+'.pdf')

    
#    plotly.offline.plot(fig,filename = 'name')

