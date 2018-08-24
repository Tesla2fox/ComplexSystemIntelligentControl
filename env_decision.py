# -*- coding: utf-8 -*-
"""
Created on Fri Aug 24 00:28:12 2018

@author: stef_leonA
"""

from env import *
import plotly.plotly as py
import plotly.graph_objs as go
import plotly

if __name__ =='__main__':
    py.sign_in('tesla_fox', 'HOTRQ3nIOdYUUszDIfgN')
    figData = []
    robotCfg = './/data//'+'cfg2data.txt'
    robCfg = Read_Cfg(robotCfg)
    _set = []
    robCfg.get('makeSpan',_set)
    figData =[]
#    figData.append
    layout = dict()
    layout['xaxis'] = dict(
        titlefont=dict(size=1),
        autorange=True,
        showgrid=False,
        zeroline=False,
        showline=True,
        autotick=True,
        ticks='',
        showticklabels = True)
    layout['yaxis'] = dict(
            titlefont=dict(size=30),
        scaleanchor = "x",
        autorange=True,
        showgrid=False,
        zeroline=False,
        showline=True,
        autotick=True,
        ticks='',
        title ='Makespan (s)',
        showticklabels = True)
    layout['autosize'] = False
    layout['height'] = 1000
    layout['width']= 1000
    layout['showlegend'] = False 
    layout['font'] = dict(
            family='sans-serif',
            size=20,
            color='#000')
    setTrace = go.Box(y=_set, name ='$    G_'+str(1)+'$',)
    figData.append(setTrace)
    fig = go.Figure(data = figData , layout = layout)
    plotly.offline.plot(fig,filename = 'name')
    
#    py.image.save_as(fig,filename = '0-0'+'.jpeg')
