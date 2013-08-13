#ifndef _classify_data_defs_h
#define _classify_data_defs_h
#include<iostream>
using namespace std;

const int NURSERY = 0;
const int PRIMARY = 1;
const int MIDDLE  = 2;
const int HIGH    = 3;

/*
const int CSCHOOLNO = 10;
string school_number_c[CSCHOOLNO] = {"一","二","三","四","五","六","七","八","九","十"};									
const int ASCHOOLNO = 10;
string school_number_a[ASCHOOLNO] = {"0","1","2","3","4","5","6","7","8","9"};
*/

const int NURSERYNUM = 4;
string nursery_school[NURSERYNUM] = {"托儿所","幼儿园","幼稚园","附幼"};

const int PRIMARYNUM = 16;
string primary_school[PRIMARYNUM] = {"小学","附小","完小","联小","高小","铁小","矿小","实小", "乡小","城小","职小","初小","寄小","子弟校","小校","完全小"};

const int PRIMARYSPECIALNUM = 0;
string primary_special_case[PRIMARYSPECIALNUM];

const int MIDDLENUM = 13;
string middle_school[MIDDLENUM] = {"中学","附中","高中","初中","农中","回中","联中","职中","附属外国语学校","附设外语学校","铁中","矿中","中學"};

const int OCCUPNUM = 21;
string occup_school[OCCUPNUM] = {"职高","中专","中等专科学校","卫校","卫生学校","农校","农业学校","体校","体育学校","艺校","艺术学校","警校","警察学校","技校","职校","职业技术学校","中等专业学校","中等职业学校","保安学校","技工学校","职业学校"};

const int MIDDLESPECIALNUM = 4;
string middle_special_case[MIDDLESPECIALNUM] = {"中学院","卫校区","中学习中心","高中等教育"};

const int TRAINNUM = 1;
string train_school[TRAINNUM] = {"驾校"};

const int TRAINNUM2 = 2;
string train_school2[TRAINNUM2] = {"培训中心","培训学校"};

const int SPECIALSCHOOLNUM = 4;
string special_school[SPECIALSCHOOLNUM] = {"党校","团校","干校","干部学校"};

const int ADULTNUM = 3;
string adult_school[ADULTNUM] = {"进修","成人","夜校",};

const int NOTSCHOOLNUM = 1;
string not_school[NOTSCHOOLNUM] = {"公司"};

const int NOTSCHOOLSPECIALNUM = 6;
string not_school_special[NOTSCHOOLSPECIALNUM] = {"大学","中学","幼儿园","小学","校","驾驶员培训"};

const int OTHERNUM = 3;
string other_school[OTHERNUM] = {"校","继续教育","远程教育"};

const int UNUM = 3;
const int UABBNUM = 18;
const int HIGHNUM = 53;
string high_school[HIGHNUM] = {"大学","学院","专科学校","体院","职大","师院","电大","师范","师专","职院","航院","船院","旅院","医专","美院","轻院","建院","曲师","北大","矿大","科大","财大","东大","天大","吉大","工大","联大","师大","农大","交大","医大","浙大","西大","山大","中大","南大","北外","北航","南大","上大","川大","贵大","云大","苏大","青大","宁大","农函大","济大","林大","电力大","上海交大","大學","师大"};

const int SECONDNUM = 10;
string second_school[SECONDNUM] = {"学院","体院","师院","职院""航院","船院","旅院","美院","轻院","建院"};


const int SCHOOL_KIND_NUM = 44;
string school_key[SCHOOL_KIND_NUM] = {"托儿所","幼儿园","幼稚园","小学","附小","中学","附中","高中","附设外语学校","外国语实验学校","外语教学实验基地学校","私立学校","大学","学院","专科学校","北大","人大","传大","法大","矿大","石大","地大","电大","海大","科大","财大","东大","天大","吉大","工大","联大","师大","农大","交大","医大","大专","浙大","理工","西大","山大","体院","职大","中大","师院"};

/*
const int UKINDNUM = 30;
string univer[UKINDNUM] = {"大学","学院","专科学校","北大","人大","传大","法大","矿大","石大","地大","电大","海大","科大",  "财大","东大","天大","吉大","工大","联大","师大","农大","交大","医大","大专","浙大","理工","西大","山大","体院","职大",     "中大","师院"};
*/

const int SUNUM = 9;
string suniver[SUNUM] = {"老年大学","科普大学","社区大学","社区学院","老年学校","老年人学校","市民学校","社区学校","科普学校"};

const int CAMPUSNUM = 19;
string campus[CAMPUSNUM] = {"分校","分院","校区","分部","分教点","教学区","教学点","办学点","教学站","函授站","函授总站",                               "教育站","分教处","函授点","本部","总部","校部","办学部","分教部"};
const int DOORNUM = 15;
string door[DOORNUM] = {"门","区","东院","南院","北院","西院","苑","南部","东部","北部","西部","东校","西校","南校","北校"};
const int ADMINCENTERNUM = 49;
string admincenter[ADMINCENTERNUM] = {"管理中心","服务中心","供应中心","结算中心","就业中心","接待中心","援助中心",                                               "指导中心","推广中心","助学中心","办公中心","处理中心","场管中心","传理中心",                                               "档案中心","发行中心","工作中心","保育中心","核算中心","后勤中心","环保中心",												  "家教中心","驾运中心","交流中心","交通中心","教材中心","考级中心","考试中心",												  "扩展中心","留学中心","旅游中心","绿化中心","认证中心","事务中心","收费中心",												  "网管中心","网络中心","维护中心","维修中心","文印中心","物管中心","行政中心",												  "修建中心","印刷中心","指挥中心","注册中心","咨询中心","资助中心","水电中心"};
const int PLACECENTERNUM = 5;
string placecenter[PLACECENTERNUM] = {"活动中心","会议中心","康乐中心","拓展中心","学生中心"};

const int TECHDEPARTNUM = 30;
string techdepart[TECHDEPARTNUM] = {"产业教学部","大学教学部","大专教学部","动画教学部","管理教学部","理工教学部",												"管校教学部","护理教学部","基础教学部","技工教学部","交通教学部","教育教学部",												"临床教学部","师范教学部","体育教学部","外语教学部","艺术教学部","英语教学部",												"自考教学部","政治理论课教学部","基础课教学部","数控教学部","政治理论教学部",												"研究生教学部","硕士教学部","医学教学部","体育产业教学部","文化素质教学部",													"继续教育中心教学部","师范教学部"};
const int TECHCENTERNUM = 10;
string techcenter[TECHCENTERNUM] = {"汉语教学中心","技术教学中心","实践教学中心","验教学中心","硕士教学中心",													"计算机教学中心","外语教学中心","英语教学中心","艺术教学中心","语音教学中心"};

const int EDUCATECENTERNUM = 27;
string educatecenter[EDUCATECENTERNUM] = {"创业教育中心","艺术教育中心","医药教育中心","医学教育中心","咨询与教育中心",												  "文化教育中心","网络教育中心","外语教育中心","体育教育中心","现代教育中心",												  "硕士教育中心","多媒体教育中心","健康教育中心","继续教育中心","技术教育中心",
										  "电化教育中心","可持续发展教育中心","韩国语教育中心","ＭＢＡ教育中心",                                                      "ＭＰＡ教育中心","MBA教育中心","MPA教育中心","SSD教育中心","海外教育中心",                                                  "留学教育中心","留学生教育中心","学位教育中心"};

const int STUDYCENTERNUM = 2;
string studycenter[STUDYCENTERNUM] = {"语言学习中心","英语学习中心"};

#endif
