# ComplexSystemIntelligentControl
ComplexSystemIntelligentControl


![Image text](https://raw.githubusercontent.com/Tesla2fox/ComplexSystemIntelligentControl/master/png/FITEE.png)



#Multi-robot coverage path planning 

version 2.0




#research map 

### must complete the STC algorithm ###
### need some comparitive experiment such as the 2008 ###
### i need write the paper by the 8.5 ###

现在需要建立一个map，来构建STC vert 和 boost graph 之间的对应关系

>- STC节点直接的链接关系仍然存在问题，需要等待进一步修复。
>- 完成STC节点关系生成之后，写在通过生成树获取路径。


7-30：
仍然需要把STC拍卖的框架写出来。
8-1
标记特殊的节点

8-3
拍卖的过程中 每个节点自带代价，可以减少路径的冲突。
选择拍卖者的过程，可以每次选择自身估计的路径最小的智能体，作为对比实验。


core paper:
Competitive on-line coverage of grid environments
by a mobile robot

![Image text](https://raw.githubusercontent.com/Tesla2fox/ComplexSystemIntelligentControl/master/png/0-0.jpg)