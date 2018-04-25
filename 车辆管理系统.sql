
////////////////////////////////////////////*�������*////////////////////////////////////////////
////////*�ɳ���¼*//////////////////
create table outcar(id int primary key identity(1,1),
	     coding varchar(16) unique,
	     number varchar(16) foreign key references cardata(number), /*���ƺ���*/
	     driver varchar(24), /* ��ʻԱ */
	     driverphone varchar(16), /* ��ʻԱ�绰 */
	     [user] varchar(16), /* �ó��� */
	     userphone varchar(16), /* �ó��˵绰 */
	     dept varchar(32), /* �ó����� */
             usetime datetime, /* �ó�ʱ�� */
 	     backtime datetime, /* ���Ʒ���ʱ�� */
	     startplace varchar(64), /* �����ص� */
	     viaplace varchar(64), /* ;���ص� */
	     destination varchar(64), /*Ŀ�ĵص�*/
	     usereason varchar(64), /* �ó�ԭ�� */
	     whopast varchar(16), /* ������ */
	     comment varchar(64) /* ��ע */
	    );

alter table outcar alter column 
alter table outcar add whoback varchar(16)
alter table outcar drop column dept
alter table outcar add dept int foreign key references dept(id)
drop table outcar;
select * from outcar
////////////////////////////////////////////*��������*////////////////////////////////////////////
///////////*�������ϱ�*//////////////
create table cardata(id int primary key identity(1,1),  /* id ���� */
		     coding varchar(16), /* �������� */
		     number varchar(16), /* ���� */
		     carbrand varchar(64), /* Ʒ���ͺ� */
		     cartype varchar(64), /*�������� */
		     belongdept varchar(16), /* �������� */
		     carstatus tinyint, /* ����״̬(���� 0������ 1��ά�� 2��ͣ�� 3�� ���� 4������ 5 ...?) */
		     driver varchar(16), /* ��ʻԱ */
		     buytime datetime, /* �������� */
		     entertime datetime, /* �Ǽ����� */
		     terminalnum varchar(16) /* �ն˺��� */
		    );
alter table cardata add constraint cons_02 unique (number) 
alter table cardata alter column carstatus tinyint
alter table cardata alter column carstatus bit
alter table cardata add terminalnum varchar(16);
alter table cardata add constraint PK_T_TARGET foreign key(terminalnum) REFERENCES target(coding)
add constraint ����� foreign key(����) references ��������ñ���(�����Ӧ����)

alter table cardata add constraint �ҵ�Լ�� unique(coding)
alter table cardata add constraint FK__cardata__termina__6501FCD8;
alter table target drop constraint �ҵ�Լ��

select coding,number,carbrand,cartype,belongdept,status=(case when carstatus=0 then '����'
	else case when carstatus=1 then '����' else case when carstatus=2 then 'ά��' else
	case when carstatus=3 then 'ͣ��' else case when carstatus=4 then '����' else
	case when carstatus=5 then '����' else '' end end end end end end),
	driver,buytime,entertime,terminalnum 
	from cardata
result=case   when   @j=0   then   null   else   @i/@j   end 
///////////*��ʻԱ���ϱ�*///////////
create table driver(id int primary key identity(1,1), /* id ���� */
	coding varchar(16) unique, /* ���� */
	name varchar(16), /* ���� */
	sex tinyint, /* �Ա� */
	status tinyint, /* ״̬ */
	dept varchar(16), /* �������� */
	type varchar(32), /* �������� */
	phone varchar(16), /* ��ϵ���� */
	identitycard varchar(24), /* ���֤ */
	intime datetime, /* ��ְ���� */	
	adress varchar(64), /* ��ͥסַ */
	comment varchar(64) /* ��ע */
);

///////////*���ű�*/////////////////////
create table dept(id int primary key identity(1,1), /* id ���� */
	coding varchar(16) unique, /* ���� */
	name varchar(16), /* �������� */
);
sp_rename 'dept', 'depts ' /* ���ı��� */

/////////////*�������ͱ�*/////////////////
create table cartype(id int primary key identity(1,1), /* id ���� */
	typename varchar(16));

////////////*�����̱��ͺű�*//////////////
create table carbrand(id int primary key identity(1,1),
	brandname varchar(16));

drop table driver
select * from driver(
	
DName char(10), DNum char(10), DIdentification char(18),
		    DRecordNum char(8), DGetDate datetime, DDriverType char(20), 
		    DMobilePhoneNum char(11), DAddress ntext);

//////////*��ǰ����λ����Ϣ��*////////
create table CarCurLoc(CNum char(8) primary key foreign key references CarsData(CNum),
			LocX numeric(10, 5), LocY numeric(10, 5));

//////////*��ǰ����λ����Ϣ��*////////
create table CarDynLoc(CNum char(8) primary key foreign key references CarsData(CNum),
			LTime datetime,
			LocX numeric(10, 5), LocY numeric(10, 5));

///////////*�ն˱�*//////////////////
create table Terminal(TNum char(10), TCallNum char(11), TCNum char(8));


/////////////////////////////////////*Ȩ�޹���*/////////////////////////////////////////////////////
//////////*Ȩ�ޱ�*///////////////////
create table action(id char(20) primary key,
		    name char(20) unique,
		    parentId char(20));

//////////*��ɫ��*////////////////////
create table role(id int primary key identity(1,1),
		  name text,
		  mid int foreign key references master(id),
		  createtime datetime);

//////////*����Ա��*////////////////////
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
		 

//////////*��ɫ-Ȩ��ӳ���*//////////////
create table role_action(id int primary key identity(1,1),
			rid int foreign key references role(id) on delete cascade,
			aid char(20) foreign key references action(id) on delete cascade,
			aname char(20),
			mid int foreign key references master(id),
			createtime datetime);

//////////*��Ա-��ɫӳ���*//////////////
create table master_role(id int primary key identity(1,1),
			mid int foreign key references master(id) on delete cascade,
			rid int foreign key references role(id) on delete cascade,
			createby int foreign key references master(id),
			createtime datetime);

//////////*�����������ݱ�*////////////////
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

ALTER table target add constraint �ҵ�Լ�� unique(coding)
ALTER table target add coding varchar(16) unique

////////////////*������Ϣ��*///////////////////
create table team(id int primary key identity(1,1),
		  coding varchar(20) unique,
		  name varchar(64),
		  phoneno varchar(20),
		  comment ntext);

//////////*������̬���ݱ�*////////////////
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

/*�������(���ڱ��Ѿ�������ɣ������Ǵ��������*/
alter table CarsData
add constraint CDriver_FK
foreign key(CDriver) references Driver(DNum)
on delete cascade;
alter table CarsData drop constraint CDriver_FK /* ɾ����� */
/*�����ֶΣ������Ѿ�������ɣ�Ҫ�����ֶΣ�*/
alter table CarsData
add CSNum char(20);
/*ɾ���ֶ�*/
ALTER TABLE CarsData
DROP COLUMN CSNum;

drop table Terminal

////////////////////////////////////////////*����*////////////////////////////////////////////////
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
				status=(case when carstatus=0 then '����' else
				case when carstatus=1 then '����' else 
				case when carstatus=2 then 'ά��' else 
				case when carstatus=3 then 'ͣ��' else 
				case when carstatus=4 then '����' else 
				case when carstatus=5 then '����' end end end end end end),
				driver,buytime,entertime,terminalnum  
				from cardata

select number,target.name targetName,target.coding targetCoding,carbrand,target.coding,driver,driverphone=(select phone from driver where name=driver),
	adress,team.name cargroup,speed,longitude,latitude
	from cardata,target,team where cardata.terminalnum=target.coding and team.coding=target.cargroup