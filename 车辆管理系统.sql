
////////////////////////////////////////////*车务管理*////////////////////////////////////////////
////////*派车记录*//////////////////
create table outcar(id int primary key identity(1,1),
	     coding varchar(16) unique,
	     number varchar(16) foreign key references cardata(number), /*车牌号码*/
	     driver varchar(24), /* 驾驶员 */
	     driverphone varchar(16), /* 驾驶员电话 */
	     [user] varchar(16), /* 用车人 */
	     userphone varchar(16), /* 用车人电话 */
	     dept varchar(32), /* 用车部门 */
             usetime datetime, /* 用车时间 */
 	     backtime datetime, /* 估计返程时间 */
	     startplace varchar(64), /* 出发地点 */
	     viaplace varchar(64), /* 途经地点 */
	     destination varchar(64), /*目的地点*/
	     usereason varchar(64), /* 用车原因 */
	     whopast varchar(16), /* 审批人 */
	     comment varchar(64) /* 备注 */
	    );

alter table outcar alter column 
alter table outcar add whoback varchar(16)
alter table outcar drop column dept
alter table outcar add dept int foreign key references dept(id)
drop table outcar;
select * from outcar
////////////////////////////////////////////*基础数据*////////////////////////////////////////////
///////////*车辆资料表*//////////////
create table cardata(id int primary key identity(1,1),  /* id 主键 */
		     coding varchar(16), /* 车辆编码 */
		     number varchar(16), /* 车牌 */
		     carbrand varchar(64), /* 品牌型号 */
		     cartype varchar(64), /*车辆类型 */
		     belongdept varchar(16), /* 所属部门 */
		     carstatus tinyint, /* 车辆状态(可用 0、出车 1、维修 2、停用 3、 报废 4、其它 5 ...?) */
		     driver varchar(16), /* 驾驶员 */
		     buytime datetime, /* 购买日期 */
		     entertime datetime, /* 登记日期 */
		     terminalnum varchar(16) /* 终端号码 */
		    );
alter table cardata add constraint cons_02 unique (number) 
alter table cardata alter column carstatus tinyint
alter table cardata alter column carstatus bit
alter table cardata add terminalnum varchar(16);
alter table cardata add constraint PK_T_TARGET foreign key(terminalnum) REFERENCES target(coding)
add constraint 外键名 foreign key(列名) references 外键所引用表名(外键对应的列)

alter table cardata add constraint 我的约束 unique(coding)
alter table cardata add constraint FK__cardata__termina__6501FCD8;
alter table target drop constraint 我的约束

select coding,number,carbrand,cartype,belongdept,status=(case when carstatus=0 then '可用'
	else case when carstatus=1 then '出车' else case when carstatus=2 then '维修' else
	case when carstatus=3 then '停用' else case when carstatus=4 then '报废' else
	case when carstatus=5 then '其它' else '' end end end end end end),
	driver,buytime,entertime,terminalnum 
	from cardata
result=case   when   @j=0   then   null   else   @i/@j   end 
///////////*驾驶员资料表*///////////
create table driver(id int primary key identity(1,1), /* id 主键 */
	coding varchar(16) unique, /* 编码 */
	name varchar(16), /* 名字 */
	sex tinyint, /* 性别 */
	status tinyint, /* 状态 */
	dept varchar(16), /* 所属部门 */
	type varchar(32), /* 驾照类型 */
	phone varchar(16), /* 联系号码 */
	identitycard varchar(24), /* 身份证 */
	intime datetime, /* 入职日期 */	
	adress varchar(64), /* 家庭住址 */
	comment varchar(64) /* 备注 */
);

///////////*部门表*/////////////////////
create table dept(id int primary key identity(1,1), /* id 主键 */
	coding varchar(16) unique, /* 编码 */
	name varchar(16), /* 部门名字 */
);
sp_rename 'dept', 'depts ' /* 更改表名 */

/////////////*车辆类型表*/////////////////
create table cartype(id int primary key identity(1,1), /* id 主键 */
	typename varchar(16));

////////////*车辆商标型号表*//////////////
create table carbrand(id int primary key identity(1,1),
	brandname varchar(16));

drop table driver
select * from driver(
	
DName char(10), DNum char(10), DIdentification char(18),
		    DRecordNum char(8), DGetDate datetime, DDriverType char(20), 
		    DMobilePhoneNum char(11), DAddress ntext);

//////////*当前车辆位置信息表*////////
create table CarCurLoc(CNum char(8) primary key foreign key references CarsData(CNum),
			LocX numeric(10, 5), LocY numeric(10, 5));

//////////*当前车辆位置信息表*////////
create table CarDynLoc(CNum char(8) primary key foreign key references CarsData(CNum),
			LTime datetime,
			LocX numeric(10, 5), LocY numeric(10, 5));

///////////*终端表*//////////////////
create table Terminal(TNum char(10), TCallNum char(11), TCNum char(8));


/////////////////////////////////////*权限管理*/////////////////////////////////////////////////////
//////////*权限表*///////////////////
create table action(id char(20) primary key,
		    name char(20) unique,
		    parentId char(20));

//////////*角色表*////////////////////
create table role(id int primary key identity(1,1),
		  name text,
		  mid int foreign key references master(id),
		  createtime datetime);

//////////*管理员表*////////////////////
create table master(id int primary key identity(1,1),
		  name char(20) unique,
		  password char(16),
		  truename text,
		  sex char(2),
		  dept text,
		  adress text,
		  officephone char(12),
		  mobile char(11),
		  homephone char(12),
		  email text,
		  createtime datetime,
		  createby int)
		 

//////////*角色-权限映射表*//////////////
create table role_action(id int primary key identity(1,1),
			rid int foreign key references role(id) on delete cascade,
			aid char(20) foreign key references action(id) on delete cascade,
			aname char(20),
			mid int foreign key references master(id),
			createtime datetime);

//////////*人员-角色映射表*//////////////
create table master_role(id int primary key identity(1,1),
			mid int foreign key references master(id) on delete cascade,
			rid int foreign key references role(id) on delete cascade,
			createby int foreign key references master(id),
			createtime datetime);

//////////*车辆最新数据表*////////////////
create table target(id int primary key identity(1,1),
		    type int,
		    coding varchar(20),
		    name varchar(64),
		    adress varchar(64),
		    contract varchar(20),
		    phoneno varchar(20),
		    [deny] tinyint,
		    online bigint,
		    longitude float,
		    latitude float,
		    speed float,
		    degrees float,
		    waterlevel float,
		    time datetime,
		    comment ntext,
		    password varchar(20),
		    cargroup varchar(20) foreign key references team(coding),
		    usergroup varchar(20));

ALTER table target add constraint 我的约束 unique(coding)
ALTER table target add coding varchar(16) unique

////////////////*车队信息表*///////////////////
create table team(id int primary key identity(1,1),
		  coding varchar(20) unique,
		  name varchar(64),
		  phoneno varchar(20),
		  comment ntext);

//////////*车辆动态数据表*////////////////
create table track(id int primary key identity(1,1),
		   tid int foreign key references target(id),
		   longitude float,
		   latitude float,
		   speed float,
		   degrees float,
		   waterlevel float,
		   time datetime);

drop table Action;
drop table Role;
drop table Master;
drop table RoleAction;
drop table MasterRole;
drop table target;
drop table track;

/*设置外键(用于表已经创建完成，但忘记创建外键）*/
alter table CarsData
add constraint CDriver_FK
foreign key(CDriver) references Driver(DNum)
on delete cascade;
alter table CarsData drop constraint CDriver_FK /* 删除外键 */
/*增加字段（用于已经创建完成，要新增字段）*/
alter table CarsData
add CSNum char(20);
/*删除字段*/
ALTER TABLE CarsData
DROP COLUMN CSNum;

drop table Terminal

////////////////////////////////////////////*测试*////////////////////////////////////////////////
select * from Master

select AID from RoleAction
	where RID=1

select RName from Role 
where RID=(select RID from MasterRole
	where MID=(select MID from Master
		where MName='123'))

select * from MasterRole

delete from MasterRole where RID=10

insert into MasterRole(MID,MID2,MRCreateDate,RID) values(1,1,'2011/12/12',1)
delete from MasterRole
  where MID=1 AND RID=1
select AID from RoleAction where RID=1

select distinct team.coding from team,target
where team.coding=target.cargroup

select id,name from target
where cargroup='66566'

select id,name from target where cargroup='55655'
select name,longitude,latitude from target where id=2

select team.name from target,team where target.id=1 and target.cargroup=team.coding

select team.name xxx,target.name,track.time,track.speed,track.longitude,track.latitude from team,target,track
where target.id=2 and target.cargroup=team.coding and track.tid=target.id and track.time > '2011-10-10' and track.time < '2011-12-19'

select target.name targetName,track.time,track.speed,track.longitude,track.latitude from team,target,track 
where target.id=2 and target.cargroup=team.coding and track.tid=target.id and track.time>'2011-12-15 15:52:28' and track.time<'2011-12-31'
order by track.time

select Action.AID from Action,RoleAction
where Action.AID=RoleAction.AID and RoleAction.MID=1 and Action.parentID=-1

select distinct  role_action.aid from role_action,action
where role_action.aid=action.id and action.parentID=-1 and rid in(
select rid from master_role
where id=1)

select  count(coding) res from cardata where entertime='2011-10-11'

select  
case  
when hidName=1 then strUser
else ''
select id,coding,number,carbrand,cartype,belongdept,
				status=(case when carstatus=0 then '可用' else
				case when carstatus=1 then '出车' else 
				case when carstatus=2 then '维修' else 
				case when carstatus=3 then '停用' else 
				case when carstatus=4 then '报废' else 
				case when carstatus=5 then '其它' end end end end end end),
				driver,buytime,entertime,terminalnum  
				from cardata

select number,target.name targetName,target.coding targetCoding,carbrand,target.coding,driver,driverphone=(select phone from driver where name=driver),
	adress,team.name cargroup,speed,longitude,latitude
	from cardata,target,team where cardata.terminalnum=target.coding and team.coding=target.cargroup