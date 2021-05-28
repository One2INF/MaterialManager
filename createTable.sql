create table MaterialManagers (id primary key, 姓名 varchar(20))
insert into MaterialManagers values(0, '请选择')
insert into MaterialManagers values(1, '施郑杰')
insert into MaterialManagers values(2, '沈聪')
insert into MaterialManagers values(3, '苏骥轩')
insert into MaterialManagers values(4, '杨俊')
insert into MaterialManagers values(5, '陈深达')

create table MaterialList (物料编号 varchar(6) primary key, 供应商 varchar(20), 物料类型 varchar(20), 物料名称 varchar(20), 型号规格 varchar(20), '单价/元' int, 备注 varchar(20))
create table MaterialStock (物料编号 varchar(6), 物料名称 varchar(20), 数量 int, 挂账人 varchar(20))
create table OperationRecords (时间 DATETIME, 操作类型 varchar(20), 物料编号 varchar(20), 物料名称 varchar(20), 数量 int, 备注 varchar(20), 操作人 varchar(20), 转出人 varchar(20), 转入人 varchar(20))
