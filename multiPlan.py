# -*- coding: utf-8 -*-
"""
Created on Mon Jul  2 21:33:15 2018

@author: robot
"""


import subprocess
#import env
from env import *
#import env

if __name__ == '__main__':
    org_exe_name = 'D:\\py_code\\ComplexSystemIntelligentControl\\bin\\exc\\Debug\\MultiCover.exe'    
    conFileDir = './/data//'
    fileCfgName =  'single_STC_Cfg.txt'
    degNameCfg = conFileDir + fileCfgName
#    proOrgStatic = subprocess.Popen([org_exe_name,degNameCfg],stdin =subprocess.PIPE,stdout = subprocess.PIPE)
#    for line in proOrgStatic.stdout:
#        print(line)        
    drawPic(fileCfgName,11,'testNothing',False)
#    drawPic(fileCfgName,6,'testNothing',True)
    
    