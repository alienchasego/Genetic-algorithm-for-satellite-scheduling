#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;


constexpr auto Mission_size = 45;                  //观测任务数量
constexpr auto DayPerSecond = 1.0 / 86400;       //天/秒的单位转换
constexpr auto PrepareTime = 30 * DayPerSecond;  //卫星任务转换时长30秒，转换成以“天”为单位

vector<int>indexArray(Mission_size);                       //按权重进行记录数组下标的排列
void sortIndex(vector<int> beforeSort, vector<int>& indexArr);


class AccessResult                        //可见时间窗口
{
private:
	string sat;
	string sensor;
	double startTime;
	double endTime;
public:
	AccessResult() :sat("null"), sensor("null"), startTime(0), endTime(0) {}
	AccessResult(string sa, string se, double st, double et) :sat(sa), sensor(se), startTime(st), endTime(et) {}
	string GetSat() { return this->sat; }
	double GetStartTime() { return this->startTime; }
	double GetEndTime() { return this->endTime; }
};

class ObsScheme                           //每个任务的观测方案
{
private:
	string sat;
	double startTime;
	double endTime;
public:
	ObsScheme() :sat("null"), startTime(0), endTime(0) {}
	ObsScheme(double st, double et) :startTime(st), endTime(et) {}
	ObsScheme(vector<AccessResult> tw, int num) :sat(tw[num].GetSat()), startTime(tw[num].GetStartTime()), endTime(tw[num].GetEndTime()) {}
	string GetSat() { return this->sat; }
	double GetStartTime() { return this->startTime; }
	double GetEndTime() { return this->endTime; }
	void SetStartTime(double t) { this->startTime = t; }
	void SetEndTime(double t) { this->endTime = t; }
};



class Mission                           //观测任务（点目标、城市）
{
private:
	string cityName;
	//float longitude;
	//float latitude;
	double covTime;
	int weight;
	vector<AccessResult> timeWindows;   //可见时间窗口集合
	ObsScheme obsScheme;
public:
	Mission() :covTime(0), weight(0), timeWindows(1) {}
	Mission(string name, int t, int w, vector<AccessResult>tw) :cityName(name), covTime(t* DayPerSecond), weight(w), timeWindows(tw) {}
	string GetName() { return this->cityName; }
	double GetCovTime() { return this->covTime; }
	int GetWeight() { return this->weight; }
	ObsScheme GetObsScheme() { return this->obsScheme; }
	void SetObsScheme(ObsScheme obs) { this->obsScheme = obs; }
	vector<AccessResult> GetTimeWindows() { return this->timeWindows; }
	int GetTWsize() { return this->timeWindows.size(); }
};

//按权重进行记录数组下标的排列
void sortIndex(vector<int> originalArray, vector<int>& indexArr)
{
	vector<int> beforeSort(originalArray);
	sort(originalArray.begin(), originalArray.end());
	reverse(originalArray.begin(), originalArray.end());
	vector<int> afterSort(originalArray);
	vector<bool> isRecord(beforeSort.size(), false);
	for (int i = 0; i < afterSort.size(); i++)
	{
		for (int j = 0; j < beforeSort.size(); j++)
		{
			if (afterSort[i] == beforeSort[j] && !isRecord[j])
			{
				indexArr[i] = j;
				isRecord[j] = true;
				break;
			}
		}
	}
}


vector<string> cityName(Mission_size);
vector<int> covTime(Mission_size);
vector<int> weight(Mission_size);
vector<string> timeWindowFileName(Mission_size);
vector<AccessResult> timeWindow;
vector<Mission> missions(Mission_size);
/// <summary>
/// 读取相应数据
/// </summary>
void inputData()
{
	//读取任务观测城市名
	string temp;
	ifstream inCityName("cityName.txt");
	if (!inCityName.is_open())
	{
		cout << "打开cityName文件错误"; exit(1);
	}
	int num = 0;
	while (!inCityName.eof())
	{
		getline(inCityName, cityName[num++]);
	}

	//读取任务覆盖时长
	ifstream inCovTime("covTime.txt");
	if (!inCovTime.is_open())
	{
		cout << "打开covTime文件错误"; exit(1);
	}
	num = 0;
	while (!inCovTime.eof())
	{
		getline(inCovTime, temp);
		covTime[num++] = stoi(temp);
	}

	//读取任务权重
	ifstream inWeight("weight.txt");
	if (!inWeight.is_open())
	{
		cout << "打开weight文件错误"; exit(1);
	}
	num = 0;
	while (!inWeight.eof())
	{
		getline(inWeight, temp);
		weight[num++] = stoi(temp);
	}
	sortIndex(weight, indexArray);                       //读取权重信息后就按权重大小获取下标

	//读取每个任务的时间窗口的文件路径
	ifstream inTimeWindowFileName("timeWindowFileName.txt");
	if (!inTimeWindowFileName.is_open())
	{
		cout << "打开timeWindowFileName文件错误"; exit(1);
	}
	num = 0;
	while (!inTimeWindowFileName.eof())
	{
		getline(inTimeWindowFileName, timeWindowFileName[num++]);
	}

	//读取每个任务的时间窗口
	string tw;
	string sa, se, st_s, et_s;
	double st = 0, et = 0;
	for (int i = 0; i < Mission_size; i++)
	{
		ifstream inTimeWindow(timeWindowFileName[i]);
		if (!inTimeWindow.is_open())
		{
			cout << "打开第" << i + 1 << "个城市的timeWindow文件错误"; exit(1);
		}
		while (inTimeWindow >> sa >> se >> st_s >> et_s)
		{
			/*cout << sa << "\t" << se << "\t" << st_s << "\t" << et_s << endl;*/
			st = stod(st_s);
			et = stod(et_s);
			timeWindow.push_back( AccessResult(sa, se, st, et));
		}
		if (!inTimeWindow.eof()) throw std::runtime_error("Invalid data from file");
		missions[i] = Mission(cityName[i], covTime[i], weight[i], timeWindow);
		timeWindow.clear();
	}
}