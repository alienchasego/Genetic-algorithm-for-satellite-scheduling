#include"GA.h"
#include<iostream>


int main()
{
	int n = N;          //��������
	int optimal = 0;    //��¼���յó������Ž�
	int m = 0;          //��¼�ó����Ž�Ĵ���

	inputData();
	Init();

	ofstream out("out2.txt");
	for (int n = 0; n < N; n++)
	{
		CrossOver(chromosomes);
		Mutation(chromosomes);
		Roulette(chromosomes);


		//���ÿ�ε�����Ϣ
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
			out << "��" << n << "�����Ž�Ϊ��" << MaxValue(chromosomes) << "\n";			
		}
	}
	//������Ž�Ĵ���
	out << "��" << m << "��Ϊ���Ž⣺" << optimal << endl;
	out.close();

	return 0;
}