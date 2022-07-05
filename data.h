#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;


constexpr auto Mission_size = 45;                  //�۲���������
constexpr auto DayPerSecond = 1.0 / 86400;       //��/��ĵ�λת��
constexpr auto PrepareTime = 30 * DayPerSecond;  //��������ת��ʱ��30�룬ת�����ԡ��족Ϊ��λ

vector<int>indexArray(Mission_size);                       //��Ȩ�ؽ��м�¼�����±������
void sortIndex(vector<int> beforeSort, vector<int>& indexArr);


class AccessResult                        //�ɼ�ʱ�䴰��
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

class ObsScheme                           //ÿ������Ĺ۲ⷽ��
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



class Mission                           //�۲����񣨵�Ŀ�ꡢ���У�
{
private:
	string cityName;
	//float longitude;
	//float latitude;
	double covTime;
	int weight;
	vector<AccessResult> timeWindows;   //�ɼ�ʱ�䴰�ڼ���
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

//��Ȩ�ؽ��м�¼�����±������
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
/// ��ȡ��Ӧ����
/// </summary>
void inputData()
{
	//��ȡ����۲������
	string temp;
	ifstream inCityName("cityName.txt");
	if (!inCityName.is_open())
	{
		cout << "��cityName�ļ�����"; exit(1);
	}
	int num = 0;
	while (!inCityName.eof())
	{
		getline(inCityName, cityName[num++]);
	}

	//��ȡ���񸲸�ʱ��
	ifstream inCovTime("covTime.txt");
	if (!inCovTime.is_open())
	{
		cout << "��covTime�ļ�����"; exit(1);
	}
	num = 0;
	while (!inCovTime.eof())
	{
		getline(inCovTime, temp);
		covTime[num++] = stoi(temp);
	}

	//��ȡ����Ȩ��
	ifstream inWeight("weight.txt");
	if (!inWeight.is_open())
	{
		cout << "��weight�ļ�����"; exit(1);
	}
	num = 0;
	while (!inWeight.eof())
	{
		getline(inWeight, temp);
		weight[num++] = stoi(temp);
	}
	sortIndex(weight, indexArray);                       //��ȡȨ����Ϣ��Ͱ�Ȩ�ش�С��ȡ�±�

	//��ȡÿ�������ʱ�䴰�ڵ��ļ�·��
	ifstream inTimeWindowFileName("timeWindowFileName.txt");
	if (!inTimeWindowFileName.is_open())
	{
		cout << "��timeWindowFileName�ļ�����"; exit(1);
	}
	num = 0;
	while (!inTimeWindowFileName.eof())
	{
		getline(inTimeWindowFileName, timeWindowFileName[num++]);
	}

	//��ȡÿ�������ʱ�䴰��
	string tw;
	string sa, se, st_s, et_s;
	double st = 0, et = 0;
	for (int i = 0; i < Mission_size; i++)
	{
		ifstream inTimeWindow(timeWindowFileName[i]);
		if (!inTimeWindow.is_open())
		{
			cout << "�򿪵�" << i + 1 << "�����е�timeWindow�ļ�����"; exit(1);
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