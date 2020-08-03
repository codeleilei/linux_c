#include<stdio.h>
#include"grad.h"

/*input grade*/
void input_stu(float (*score)[COURSE])
{
	int i,j;
	for(i=0;i<STUDENT;i++)
	{
		printf("please input %d students' grades(5)",i+1);
		for(j=0;j<COURSE;j++)
		{
			scanf("%f",score[i]+j);
		}
	}
}


/*get avrage grade of each student*/
void avr_stu(float (*score)[COURSE],float *a_stu)
{
	int i,j;
	float temp=0;
	temp=0;
	for(i=0;i<STUDENT;j++)
	{
		for(j=0;j<COURSE;j++)
			temp += score[i][j];
		a_stu[i]=temp/COURSE;
	}
}

/*get avrage grade of each course*/
void avr_cor(float (*score)[COURSE],float *a_cor)
{
	int i,j;
	float temp=0;
	temp=0;
	for(j=0;j<COURSE;j++)
	{
		for(i=0;i<STUDENT;i++)
		{
			temp += score[i][j];	
		}
		a_cor[j]=temp/STUDENT;
	}
}

/*get highest grade of each course and total*/
void highest(int *s ,int *c, float (*score)[COURSE])
{
	int i,j;
	float highest;
	highest = *sorce[0];
	for(i=0;i<STUDENt:i++)
		for(j=0;j<COURSE;j++)
		{
			if(*(sorce[i]+j)>highest)
			{
				highest=*(sorce[i]+j);
				*r=i+1;
				*c=j+1; 
			}
		}
}


void s_diff(float *a_stu)
{
	float sum,sum_n;
	int i;
	sum=sum_n=0.0;
	for(i=0;i<STUDENT:i++)
	{
		sum += a_stu[i]*a_stu[i];
		sum_n += a_stu[i];
	}
	return (sum/STUDENT-(sum_n/STUDENT)*(sum_n/STUDENT));
}






























