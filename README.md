# MaterialManager-物料管理软件

#### 介绍
简单的物料管理软件，对物料进行管理，`本人主要用于管理设备配件`

### 软件使用
1. 先在 'database.json' 里填写你的数据库链接信息
2. 再在 'user.json' 里填写物料员姓名 密码用于登录校验
3. 默认新建用户密码为 `123456`，也可以点击开始 进行登录和修改密码 

#### 功能介绍
1. 物料出库，入库。
2. 物料挂账人转变。
3. 可按时间、物料，查询 出库、入库、转账记录。
4. 每个人管理自己名下的物料。

### 操作定义
#### 入库：
物料采购后，管理员将物料，数量直接挂到负责人，备注说明。

![入库](./images/入库.PNG)

- 挂账人
- 备注
- 入库

| 物料编号 | 物料名称 | 数量 |
| ------- | -------- | --- |
|  TB0001 |  摄像头  |  10  |

#### 出库
物料使用者将自己名下的物料 数量 使用的小区 备注说明登记，名下库存自动减少。

![出库](./images/出库.PNG)

- 省-市-区-街道-社区-小区
- 备注
- 出库

| 物料编号 | 物料名称 | 数量 |
| ------- | -------- | --- |
| TB0001  |  摄像头 | 10 |

#### 转挂
物料对应的负责人进行交换，用于将物料转给物料使用者，以便其出库。

![转挂](./images/转挂.PNG)

- 转出人
- 转入人
- 备注
- 转挂

| 物料编号 | 物料名称 | 数量 |
| ------- | -------- | --- |
| TB0001  |  摄像头  |  10  |

#### 库存查询
查询仓库内的物料库存 及其 挂账人 情况。

![库存查询](./images/库存查询.PNG)

- 物料编号
- 物料名称
- 挂账人
- 查询

| 物料编号 | 物料名称 | 数量 |
| ------- | -------- | --- |
|  TB0001 |  摄像头  |  10  |

- 上一页
- 下一页

#### 操作记录查询
查村 出库 入库 转挂 记录

![操作记录查询](./images/操作记录查询.PNG)

- 操作类型
- 操作人
- 起始时间
- 截止时间

|   时间   | 操作类型 | 物料编号 | 物料名称 | 数量 | 备注 | 操作人 | 转出人 | 转入人 |
| -------- | ------- |-------- | -------- | --  | ---- | ----- | ------ | ------ |
| 20211809 |   入库   | TB0001 |  摄像头  |  10  | 说明 |  szj  |        |        |

|   时间   | 操作类型 | 物料编号 | 物料名称 | 数量 | 备注 | 操作人 | 转出人 | 转入人 |
| -------- | ------- | ------- | -------- | --- | ---- | ----- | ------ | ------ |
| 20211809 |   出库   | TB0001 |  摄像头   | 10  | 说明 |  szj  |        |        |

|   时间   | 操作类型 | 物料编号 | 物料名称 | 数量 | 备注 | 操作人 |转出人 | 转入人 |
| -------- | ------- | ------- | -------- | --- | ---- | ----- | ----- | ------ |
| 20211809 |   转挂   | TB0001 |   摄像头   | 10 | 说明 |  szj   |  szj  |  zjl  |

- 上一页
- 下一页

6. 物料信息查询
查询已登记的物料信息

![物料信息查询](./images/物料信息查询.PNG)

- 物料编号
- 物料名称

| 物料编号 | 供应商 | 物料分类 | 物料名称 | 型号规格 | 单价/元 |   备注   |
| ------- | ------ |-------- | ------- | -------- | ------ | -------- |
| TB0001 | 思远创智能 | RFID | USB RFID |   R20C   |   45   | USB RFID |

- 上一页
- 下一页

7. 新增物料编号
登记新的物料类别，通过 xlsx 导入

![新增物料编号](./images/新增物料.PNG)

- 新增物料编号

| 物料编号 | 供应商 | 物料分类 | 物料名称 | 型号规格 | 单价/元 |   备注   |
| ------- | ------ |-------- | ------- | -------- | ------ | -------- |
| TB0001 | 思远创智能 | RFID | USB RFID |   R20C   |   45   | USB RFID |

### 数据库结构
- 表 MaterialList

  | 物料编号 | 供应商 | 物料分类 | 物料名称 | 型号规格 | 单价/元 | 备注 |

- 表 MaterialManagers

  | 名字 |

- 表 OperationRecords-2021

  | 时间 | 操作类型 | 物料编号 | 物料名称 | 数量 | 备注 | 操作人 |转出人 | 转入人 |

- 表 Stock
  
  | 物料编号 | 物料名称 | 数量 | 挂账人 |

![数据库表](./images/数据库表.PNG)

![CityStatistiscCode](./images/CityStatistiscCode.PNG)

![MaterialManagers](./images/MaterialManagers.PNG)

![MaterialStock](./images/MaterialStock.PNG)

![MaterialList](./images/MaterialList.PNG)

![OperationRecord](./images/OperationRecord.PNG)


### TODO
- 添加账户登录（已完成）
- 行政区域划分（已完成）
- 操作记录查询（已完成，日期时间选择待完善）
- 远程部署数据库（已完成）
- 修改 出入库 list 校验后再统一插入
- 修改提示
- 行政区域划分数据库更新，部分数据有误缺少

### 说明
1. 行政区域划数据分请参照我的另一个仓库 [StatisticsZoneDatabase](https://gitee.com/One2INF/StatisticsZoneDatabase)
2. 推荐使用 [Dbeaver](https://dbeaver.io/) 数据库可视化管理软件，有对应免费社区版，功能够用。

### 结语
1. 由于对物料管理认知有限，软件欠缺通用行，适合实际；
2. 提前确认好软件需求，进行功能划分，是一个良好的开端，为之后程序设计提供方便；
3. 通过改软件 学习了 QT 多个 UI 设计，数据库 CRUD，数据库远程部署；
4. 适当分解任务，避免长期无进展而陷入挫折感；
