# NotAGame

#### Description
基于The Stanley Parable的一个室内场景建模小游戏~

#### Phsics Engine
- 添加了**物理引擎**，玩家在场景中会受到**重力**的影响
- 基本实现了**碰撞检测**，但是可能有迷之bug
- 按*空格*键起跳
- 实现了一个跳跃demo，场景中有一个不可通过的玻璃立方体，玩家可以跳上去，**just enjoy it!**

#### Crystal System
- 水晶个数、类型、与初始生成点设置
- **水晶碰撞检测**，水晶不会穿过墙体和玩家，但是水晶之间暂可以相互穿过
- 水晶**移动路径规划**，水晶在离玩家较远时会做规则圆周运动，离玩家较近时朝玩家移动
- 红色水晶攻击玩家，玩家离红色水晶较近时，**屏幕闪动血色**
- 玩家**吃掉蓝色水晶**，增加玩家蓝条。。
