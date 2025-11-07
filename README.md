# CardGame
一个简单的纸牌游戏程序设计，实现一个简单普通点击匹配以及回退功能。
基于 Cocos2d-x 3.17 / C++ 的轻量纸牌小游戏示例。项目采用清晰的 MVC 分层与可撤销（Undo）操作，支持关卡 JSON 配置与皮肤资源解耦。

目录结构为：
Classes/
├─ configs/
│  ├─ models/
│  │  ├─ CardResConfig.h        # 全局布局/比例/动画时长/规则开关等
│  │  ├─ CardSkinConfig.h       # 卡牌皮肤资源
│  │  └─ LevelConfig.h          # 静态关卡数据结构
│  └─ loaders/
│     ├─ LevelConfigLoader.h    # 读 JSON → LevelConfig
│     └─ LevelConfigLoader.cpp
│
├─ models/
│  ├─ CardModel.h               # 一张牌的数据
│  ├─ GameModel.h               # 运行时数据（table/hand/top）
│  └─ UndoModel.h               # 撤销记录
│
├─ views/
│  ├─ CardView.h/.cpp           # 单牌视图：渲染与点击上抛
│  ├─ GameView.h/.cpp           # 根视图：上下分区背景/UI与子视图挂载
│  ├─ PlayFieldView.h/.cpp      # 桌面区容器：命中检测、跨父搬运动画
│  └─ StackView.h/.cpp          # 托盘/手牌区容器
│
├─ controllers/
│  ├─ GameController.h/.cpp     # 场景级总控：载关卡→建模型→渲染→撤销
│  ├─ PlayFieldController.h/.cpp# 桌面交互：判规/入栈/搬到顶部
│  └─ StackController.h/.cpp    # 托盘交互：手牌顶牌→顶部替换
│
├─ managers/
│  ├─ LayoutManager.h           # 位置提供者（handPos/topPos）
│  └─ UndoManager.h             # 撤销栈容器
│
└─ services/
   ├─ GameModelFromLevelGenerator.h/.cpp # LevelConfig → GameModel
   ├─ RuleService.h/.cpp        # 相邻规则
   ├─ TweenService.h            # 统一MoveTo动画
   └─ UndoService.h             # 对撤销栈的静态操作



资源放置
├─ Classes/
└─ Resources/
   ├─ card_general.png
   ├─ number/
   │  ├─ big_black_A.png … big_black_K.png
   │  ├─ big_red_A.png   … big_red_K.png
   │  ├─ small_black_A.png … small_black_K.png
   │  └─ small_red_A.png   … small_red_K.png
   ├─ suits/
   │  ├─ club.png  diamond.png  heart.png  spade.png
   └─ config/
      └─ level1.json


项目架构（MVC）
Model：GameModel（cards/table/hand/top）、CardModel、UndoModel
View：GameView / PlayFieldView / StackView / CardView
Controller：GameController（装配/撤销）、PlayFieldController（桌面判规）、StackController（手牌替换）
Services/Managers：
RuleService（判相邻）
└─TweenService（统一动画时长CardResConfig::kMoveDuration）
└─LayoutManager（handPos/topPos；参数来自CardResConfig）
└─UndoService/UndoManager（撤销入栈/出栈）
点击 → 控制器判规/入栈 → 动画 → 更新GameModel。
撤销 → 从栈取记录 → 视图与GameModel一起回滚。
