#pragma once

#include"data.h"
#include<ctime>

constexpr auto N = 100;                  //迭代代数
constexpr auto Population = 20;          //种群大小
constexpr auto Chro_len = 90;            //染色体长度，需要观测的任务*2，因其时间窗口序号为两位数
constexpr auto Gene_num = 45;            //基因数量即染色体数量
constexpr auto Population_m = 10;        //初始种群的染色体个数;
constexpr auto TimeWindow_maxSize = 29;  //所有任务中可见时间窗口的最大数量，用于种群染色体随机初始化
constexpr auto Crossover_rate = 0.9;     //交叉率;
constexpr auto Mutation_rate = 0.1;      //变异率;



//染色体
class Chromosome
{
private:
	vector<Mission> mission;
	vector<int>order;                                        //基因长度即观测任务数量
	string chro;
	double fitness;
public:
	Chromosome() :order(Mission_size, 0), fitness(0) {}
	Chromosome(vector<Mission>& m) :mission(m), order(Mission_size, 0), fitness(0) {}
	string GetChro() { return this->chro; }
	void SetChro(string s) { this->chro = s; }
	void Encode();
	void Decode();
	bool IsConflict(ObsScheme os1, ObsScheme os2, int m1, int m2);                 //计算两个Mission的时间窗口是否冲突
	double Eval();
	double GetFitness() { return this->fitness; }
	void SetFitness(double f) { this->fitness = f; }
	int GetOrder(int i) { return this->order[i]; }
	void ChangeOrder(int pos, int val) { this->order[pos] = val; }
	vector<Mission> GetMission() { return this->mission; }
};


//种群
vector<Chromosome>chromosomes(Population);


//编码
void Chromosome::Encode()
{
	for (int i = 0; i < Mission_size; i++)
	{
		if (order[i] < 10)
		{
			chro += '0' + to_string(order[i]);
		}
		else
		{
			chro += to_string(order[i]);
		}
	}
}


//解码
void Chromosome::Decode()
{
	for (int i = 0; i < Chro_len; i += 2)
	{
		string temp = chro.substr(i, 2);
		order[i / 2] = stoi(temp);
	}
}

int testData[20][45] = {
5,0,8,9,0,9,3,0,4,10,7,1,6,4,4,3,12,17,8,10,1,11,0,6,1,28,0,10,14,2,0,11,4,4,8,9,5,0,2,12,3,3,4,2,8,
7,9,6,2,1,4,7,7,7,10,0,4,0,7,15,21,6,8,11,5,6,0,3,4,1,4,5,1,12,13,8,10,13,7,19,1,14,13,11,12,4,11,12,5,9,
8,7,16,7,13,2,7,15,2,6,0,2,2,4,8,8,8,1,5,9,8,0,5,8,0,21,0,10,17,6,10,8,8,0,5,8,9,10,3,14,8,0,2,6,12,
8,2,2,7,11,9,8,7,7,8,11,2,0,5,9,5,12,2,11,7,7,5,6,3,5,0,0,10,4,0,1,2,11,3,1,14,12,6,10,3,17,4,6,4,7,
8,0,16,7,14,5,11,17,6,0,3,10,12,0,7,5,9,4,7,8,9,3,9,5,6,2,3,0,0,6,8,5,15,6,7,6,9,9,6,14,2,9,9,1,12,
13,7,7,13,4,2,5,0,0,12,2,2,0,12,14,7,13,2,7,2,3,12,1,2,2,15,11,13,11,16,9,11,10,11,6,3,3,1,1,12,2,2,5,7,9,
1,1,6,15,8,10,0,9,5,7,17,6,12,6,7,7,13,11,1,3,4,1,2,10,5,25,6,2,3,8,11,3,1,9,2,10,6,7,0,5,17,6,2,5,12,
1,7,1,3,0,0,1,14,10,6,5,5,9,11,0,18,10,17,5,2,6,7,1,5,8,7,11,13,7,3,2,4,0,4,5,10,11,15,0,1,8,4,0,5,13,
6,0,10,6,8,4,1,18,2,8,1,0,7,6,0,2,3,1,8,0,11,6,4,2,0,14,12,13,13,13,0,6,5,14,12,9,13,9,5,1,3,10,7,6,5,
11,0,10,9,9,11,6,12,10,8,12,10,12,8,3,18,1,17,10,0,0,10,0,6,1,12,12,0,5,5,4,8,11,10,17,7,3,9,4,9,2,2,11,1,3,
18,7,14,17,10,0,2,15,1,1,1,0,1,9,17,6,7,5,5,4,9,3,9,5,5,7,3,3,10,9,2,1,11,13,5,13,5,12,3,13,7,1,2,7,2,
6,4,1,11,1,6,3,17,1,1,1,2,12,6,8,7,4,6,0,9,12,3,12,3,5,8,5,7,6,8,2,8,7,0,5,7,8,13,2,1,16,10,2,3,7,
4,0,8,15,12,15,10,12,2,9,3,4,13,2,4,14,4,3,4,2,9,9,12,6,0,14,13,7,8,3,6,3,4,15,8,14,0,8,3,12,12,11,0,12,0,
0,0,5,17,14,0,1,5,4,6,0,10,3,6,4,3,9,7,7,0,10,6,2,5,4,23,8,12,3,10,0,8,1,6,13,7,1,11,8,1,5,2,4,5,5,
4,11,14,2,10,0,0,13,10,2,2,4,2,11,7,21,8,14,9,9,13,10,1,1,10,15,5,8,5,4,2,0,4,12,9,13,13,12,3,13,20,8,2,1,1,
6,8,0,15,7,5,6,13,0,1,19,1,8,9,2,19,10,6,10,5,2,11,6,11,0,16,4,5,9,6,0,15,4,9,8,4,11,9,1,7,6,3,0,1,0,
0,7,1,6,4,9,7,2,8,0,8,0,5,1,9,0,3,6,10,10,12,10,12,5,1,0,2,0,8,11,6,9,4,1,3,8,3,6,3,7,0,2,1,4,4,
7,6,13,10,0,7,7,7,5,3,7,10,8,6,4,1,3,10,1,11,1,8,10,3,0,22,4,0,18,1,7,5,10,6,15,11,6,7,4,9,4,12,4,10,1,
2,4,3,4,10,16,10,2,0,5,22,2,3,6,12,5,0,13,11,9,9,1,1,3,2,18,1,8,3,0,7,1,4,12,8,11,12,0,10,5,17,9,3,7,11,
9,8,8,14,12,2,6,7,2,7,6,2,1,1,4,6,8,16,1,4,6,10,11,2,7,27,5,11,3,14,10,1,11,12,2,13,14,6,2,2,11,13,3,8,2,
};

//种群初始化
void Init()
{
	string ccc;
	Chromosome ch(missions);
	for (int i = 0; i < Population; i++)
	{
		chromosomes[i] = ch;
	}
	srand((unsigned)time(NULL));
	for (int popu = 0; popu < Population; popu++)
	{
		for (int ord = 0; ord < Mission_size; ord++)
		{
			int val = (rand() % (TimeWindow_maxSize + 1)) % (missions[ord].GetTWsize());
			//int val = testData[popu][ord];
			chromosomes[popu].ChangeOrder(ord, val);
		}
		ccc.clear();
		chromosomes[popu].Encode();
	}
}


//计算两个Mission的观测方案是否冲突(前提是二者占用同一颗卫星)
//(即在前一个任务观测完成后是否能空余出大于转换时长的时间来完成后一个任务)
bool Chromosome::IsConflict(ObsScheme os1, ObsScheme os2, int m1, int m2)
{
	double smallerStartTime = min(os1.GetStartTime(), os2.GetStartTime());
	double largerEndTime = max(os1.GetEndTime(), os2.GetEndTime());
	if ((largerEndTime - smallerStartTime) < (mission[m1].GetCovTime() + mission[m2].GetCovTime() + PrepareTime))
	{
		return false;
	}
	return true;
}


//计算适应度值（处理冲突后形成可行解的收益总和）
double Chromosome::Eval()
{
	Decode();
	vector<bool> isValid(Mission_size, true);                                  //isValid数组记录该任务是否被列入调度方案中
	int index_i, index_j, validValue = 0;
	for (int i = 0; i < Mission_size - 1; i++)                                //置index_i为当前任务，将其与比它权值更小的任务进行冲突处理
	{                                                                         //indexArray数组按权重从大到小依次记录任务的下标
		index_i = indexArray[i];                                              //按照任务权重从大到小处理任务间的冲突
		if (!isValid[index_i]) continue;                                      //如果该任务已被排除出调度方案，则不用处理冲突
		ObsScheme os_i;
		if (mission[index_i].GetObsScheme().GetSat() == "null")                //获取当前任务的观测方案，若不存在，则设为染色体中对应的可见时间窗口
		{                                                                     //若存在，则用os_i保存其观测方案
			os_i = ObsScheme(mission[index_i].GetTimeWindows(), order[index_i]);
		}
		else
		{
			os_i = mission[index_i].GetObsScheme();
		}
		for (int j = i + 1; j < Mission_size; j++)
		{
			index_j = indexArray[j];
			if (!isValid[index_j]) continue;                                  //如果对比任务已被排除出调度方案，则不冲突
			ObsScheme os_j;
			if (mission[index_j].GetObsScheme().GetSat() == "null")           //获取对比任务的观测方案，若不存在，则设为染色体中对应的可见时间窗口
			{                                                                 //若存在，则用os_j保存其观测方案
				os_j = ObsScheme(mission[index_j].GetTimeWindows(), order[index_j]);
			}
			else
			{
				os_j = mission[index_j].GetObsScheme();
			}
			if (os_j.GetSat() != os_i.GetSat()) continue;                    //如果两个任务观测方案的卫星不同，则不冲突
			if (IsConflict(os_i, os_j, index_i, index_j))
			{
				isValid[index_j] = false;                                     //如果冲突，则将权值较小的任务排除出调度方案
			}
			else                                                              //若不冲突，分两种情况
			{                                                                 //一是两者占用同一颗卫星的时间存在交集，则将观测方案os_i中去除必要的os_j观测时间和准备时间,
																			  //二是两者占用同一颗卫星的时间没有交集，则直接令该可见时间窗口为其观测方案，
																			  //以此缩小当前任务观测方案的时间窗口，保证与后续观测窗口是否冲突的正确判断
				if (os_j.GetStartTime() > os_i.GetStartTime() && os_j.GetStartTime() < os_i.GetEndTime())
				{
					double newEndTime = min(os_i.GetEndTime(), (os_j.GetEndTime() - PrepareTime - mission[index_j].GetCovTime()));
					os_i.SetEndTime(newEndTime);
				}
				if (os_j.GetEndTime() > os_i.GetStartTime() && os_j.GetEndTime() < os_i.GetEndTime())
				{
					double newStartTime = max(os_i.GetStartTime(), (os_j.GetStartTime() + PrepareTime + mission[index_j].GetCovTime()));
					os_i.SetStartTime(newStartTime);
				}
				mission[index_i].SetObsScheme(os_i);
			}
		}
	}
	for (int i = 0; i < Mission_size; i++)
	{
		if (isValid[i])
		{
			validValue += mission[i].GetWeight();
		}
	}
	return validValue;
}


//计算当代最优解的收益值
double MaxValue(vector<Chromosome>& c)
{
	double maxfit = 0;
	for (int i = 0; i < Population; i++)
	{
		if (chromosomes[i].GetFitness() > maxfit)
		{
			maxfit = chromosomes[i].GetFitness();
		}
	}
	return maxfit;
}


//交叉
void CrossOver(vector<Chromosome>& c)
{
	//通过产生随机数选出要进行交叉的染色体
	vector<Chromosome> crossChro;
	for (int i = 0; i < Population; i++)
	{
		double p = (double)(rand() % (1000)) / 1000.0;
		if (p < Crossover_rate)
		{
			crossChro.push_back(c[i]);
		}
	}
	//子代存储
	int crossNum = 0;
	for (int i = 0; i < (crossChro.size() / 2); i++, crossNum += 2)
	{
		int crossoverPoint = rand() % (Chro_len + 1);            //产生随机交叉点
		if (crossoverPoint % 2) { crossoverPoint++; }            //保证交叉点位为偶数位，从偶数位置交叉，以免破坏染色体基因结构
		string ch_1 = crossChro[crossNum].GetChro();
		string ch_2 = crossChro[crossNum + 1].GetChro();
		string gene_1 = ch_1.substr(crossoverPoint, Chro_len - crossoverPoint);
		string gene_2 = ch_2.substr(crossoverPoint, Chro_len - crossoverPoint);
		crossChro[crossNum].SetChro(ch_1.replace(crossoverPoint, Chro_len - crossoverPoint, gene_2));
		crossChro[crossNum + 1].SetChro(ch_2.replace(crossoverPoint, Chro_len - crossoverPoint, gene_1));
	}
	//将子代染色体融入种群
	for (int i = 0; i < crossChro.size(); i++)
	{
		c.push_back(crossChro[i]);
	}
	crossChro.clear();
}


//变异
void Mutation(vector<Chromosome>& c)
{
	vector<double> geneMuta;
	//给种群每个基因产生随机数（一条染色体有Gene_num个基因）
	for (int i = 0; i < Gene_num * c.size(); i++)
	{
		geneMuta.push_back((rand() % 1000) / 1000.0);
	}
	//遍历基因，检测变异基因
	for (int i = 0; i < Gene_num * c.size(); i++)
	{
		if (geneMuta[i] < Mutation_rate)                //记录基因位置
		{
			int chroNum = i / Gene_num;                 //位于哪一条染色体
			int geneNum = i - chroNum * Gene_num;       //该染色体上的基因位置
			string temp = c[chroNum].GetChro();
			int randNum = (rand() % (TimeWindow_maxSize + 1)) % (c[chroNum].GetMission()[geneNum].GetTWsize());
			string tempGene;
			if (randNum < 10)
			{
				tempGene = '0' + to_string(randNum);
			}
			else
			{
				tempGene = to_string(randNum);

			}
			c[chroNum].SetChro(temp.replace(geneNum * 2, 2, tempGene));
		}
	}
	geneMuta.clear();
}


//轮盘赌
void Roulette(vector<Chromosome>& c)
{
	//计算种群个体适应度和总适应度
	vector<double> eval(c.size());
	double evalSum = 0;
	for (int i = 0; i < c.size(); i++)
	{
		eval[i] = c[i].Eval();
		c[i].SetFitness(eval[i]);
		evalSum += eval[i];
	}
	//计算累计概率
	vector<double> pr(c.size());
	double p = 0;
	for (int i = 0; i < c.size(); i++)
	{
		p += eval[i] / evalSum;
		pr[i] = p;
	}
	//产生新种群
	vector<Chromosome>newC = c;
	for (int i = 0; i < Population; i++)               //轮盘指针次数等于种群大小
	{
		p = rand() % 1000 / 1000.0;
		if (p < pr[0])
		{
			newC[i] = c[0];
		}
		else
		{
			for (int j = 0; j < Population - 1; j++)
			{
				if (p >= pr[j] && p < pr[j + 1])
				{
					newC[i] = c[j + 1];
				}
			}
		}
	}
	c = newC;
	pr.clear();
	newC.clear();
}