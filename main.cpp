#include"GA.h"
#include<iostream>


int main()
{
	int n = N;          //迭代次数
	int optimal = 0;    //记录最终得出的最优解
	int m = 0;          //记录得出最优解的代数

	inputData();
	Init();

	ofstream out("out2.txt");
	for (int n = 0; n < N; n++)
	{
		CrossOver(chromosomes);
		Mutation(chromosomes);
		Roulette(chromosomes);


		//输出每次迭代信息
		if ((int)MaxValue(chromosomes) > optimal)
		{
			optimal = (int)MaxValue(chromosomes);
			m = n;
		}
		if (out.is_open())
		{
			for (int i = 0; i < Population; i++)
			{
				out << "#" << i << "  " << chromosomes[i].GetChro() << "\n";
			}
			out << "第" << n << "代最优解为：" << MaxValue(chromosomes) << "\n";			
		}
	}
	//输出最优解的代数
	out << "第" << m << "代为最优解：" << optimal << endl;
	out.close();

	return 0;
}