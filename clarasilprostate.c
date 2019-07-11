#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<math.h>
#include<malloc.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

int k,row,col,classlable;
int *currentmedoid,*bestmedoid,*cluster,*cluster2,*newmedoid;

float **p,**currentdata;

float dist[50],count[50];





void input()
{
	printf("Enter the no. of medoids(k):\n");
	scanf("%d",&k);
	
	currentmedoid=(int *)malloc(sizeof(int)*k);
	newmedoid=(int *)malloc(sizeof(int)*k);
	bestmedoid=(int *)malloc(sizeof(int)*k);
	cluster2=(int *)malloc(sizeof(int)*(40+2*k));

}



void fileread()//reads from file initializes dataset , sample dataset matrix and cluster matrix are   
{
	FILE *fp1=fopen("prostate.txt","r");
	
	float test;
	int i,j,s;
	
	if(fp1==NULL)
	{
		printf("error");
		exit(0);
	}
	fscanf(fp1,"%d",&row);
	fscanf(fp1,"%d",&col);
	fscanf(fp1,"%d",&classlable);
	col-=1;
//	row=6303;
//	col=10;
	printf("\n %d %d",row,col);
	s=(k*2)+40;
	p=(float**)malloc(sizeof(float*)*row);
	currentdata=(float**)malloc(sizeof(float*)*s);
	printf("\n s=%d",s);
	for(i=0;i<row;i++)
	{
		p[i]=(float *)malloc(sizeof(float)*col);
	}
	
	for(i=0;i<s;i++)
	{
		currentdata[i]=(float *)malloc(sizeof(float)*col);
	}
	printf("\n%d\n",i);
	
	for(i=0;i<row;i++)
	{
		for(j=0;j<col;j++)
		{
			fscanf(fp1,"%f",&test);
			p[i][j]=test;
		}
	}
	
	cluster=(int *)malloc(sizeof(int)*row);
	
	fclose(fp1);
	printf("\n file read \n");
}




float distance(int x,int y)
{
	
	int i;
	float d=0.0;
	printf("Finding distance");
	for(i=0;i<col;i++)
	{
		d+=abs(p[x][i]-p[y][i]);
	}
	return d;
}

void clusterassign()//assigns cluster to each row of dataset according to minimum distance
{
	int pos,in,j;
	float l=0.0;
	printf("clusterassign()");
	 for(in=0;in<row;in++)
	 {
	 	l=distance(in,bestmedoid[0]);
		pos=0;
	//	printf("%d",in);
		for(j=1;j<k;j++)
		{
		//	printf("checking");
			if((distance(in,bestmedoid[j]))<l)
			{
				l=distance(in,bestmedoid[j]);
				pos=j;
			
				printf("%d\n",pos);
			}
		//	printf("%d\n",pos);
			
		}
//	printf("finishline");
		cluster[in]=pos;
		
	
 	}	
 //	printf("finishassign");
}

float totaldiff()
{
	float cost=0.0,temp,l=0.0;
	int i,j,m;
	for(i=0;i<row;i++)
	{
		l=distance(i,currentmedoid[0]);
		
		for(j=1;j<k;j++)
		{
			if(distance(i,currentmedoid[j])<l)
				l=distance(i,currentmedoid[j]);
		}
		cost+=l;
	}
	return cost;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////pam



float distancesample(int x,int y)//x=object row no.  ,  y=medoid i.e row no. in dataset that is selected
{
	float d=0.0;
	int i,j;
	for(i=0;i<col;i++)
	{
		d+=abs(currentdata[x][i]-currentdata[y][i]);	
	}
	return d;
}

void cluster_assign()//assigns cluster to each row of dataset according to minimum distance
{
	int pos,in,j;
	float l=0.0;
	printf("cluster_assign()");
	 for(in=0;in<(40+2*k);in++)
	 {
	 	l=distancesample(in,currentmedoid[0]);
		pos=0;
		printf("%d",in);
		for(j=1;j<k;j++)
		{
			printf("checking");
			if((distancesample(in,currentmedoid[j]))<l)
			{
				l=distancesample(in,currentmedoid[j]);
				pos=j;
			
			//	printf("%d\n",pos);
			}
			printf("\n %d \n",pos);
			
		}
//	printf("finishline");
		cluster2[in]=pos;
		
	
 	}	
 	printf("finishassign");
}

float cal_cost()//finds total differences afeter all rows are assigned one cluster
{
	float cost=0.0;
	int i,j;
	for(i=0;i<(40+2*k);i++)
	{
		cost+=distancesample(i,cluster2[i]);
	}
	return cost;
}

int notselected(int x)//finds out whether an object already selected
{
	
	int i;
	for(i=0;i<k;i++)
	{
		if(currentmedoid[i]==x)
			return 0;
			
	}
	return 1;
}

float swapfindcost()//finds net differece sum if one of the medoid is replaced by a nonselected object
{
	float cost=0.0,l=0.0;
	int i,j,m;
	for(i=0;i<(40+2*k);i++)
	{
		l=distancesample(i,newmedoid[0]);
		
		for(j=1;j<k;j++)
		{
			if(distancesample(i,newmedoid[j])<l)
				l=distancesample(i,newmedoid[j]);
		}
		cost+=l;
	}
	return cost;
}

void pam()
{
		int i,j,l,m,n;
		float newcost=0.0;
		float cost1=cal_cost();
		for(i=0;i<k;i++)
		{
			printf("pam loop 1\n");
		
			for(j=0;j<(40+2*k);j++)
			{
			//	printf("k medoid loop 2\n");
				if(notselected(j))
				{
				   for(m=0;m<k;m++)
				   {
				   		if(m==i)
				   			newmedoid[m]=j;
				   		else
				   			newmedoid[m]=currentmedoid[m];
				   }
				   newcost=swapfindcost();
				   if(newcost<cost1)
				   {
						currentmedoid[i]=j;
						cost1=newcost;
						//cluster_assign();
				   }
				}
			}
		}
	//	cluster_assign();
	printf("\n total difference= %f",cost1);
}


////////////////////////////////////////////////////////////////////////////////////////pam










void kmedoid()
{
	
	int i;
	srand(time(NULL));
	for(i=0;i<k;i++)
	{
		
		currentmedoid[i]=rand()%((40+2*k)-0)+0;
	//	printf("%d   ",currentmedoid[i]);
	}
	cluster_assign();
	pam();
}


void clara()
{
	int i,j,h,temp,f=0,s, aa;
	float diff=0.0;
	
	int *t;

	printf("\n CLARA() \n");

	srand(time(NULL));
	s=40+(2*k);
	t = (int*)malloc(sizeof(int)*s);
	printf("\n s = %d",s);

	for(i=1;i<=5;i++)
	{
		//printf("clara loop 1\n");
		
		for(j=0;j<s;j++)
		{
			 //printf("clara loop2\n");
			 temp=rand()%(row);
			 t[j]=temp;
			 printf(" \n  temp = %d  ",temp);
			 //currentdataset[j]=temp;
			for(h=0;h<col;h++)
			{
				currentdata[j][h]=p[temp][h];
//				printf("%d",h);
			}
//			printf("\n current data set");
			printf("j = %d s = %d",j,s);
		}
		f++;

	 
//		scanf("%d", &aa);		

		printf("\ncalling k medoid\n");	
		kmedoid();//set centriods and set 1D array currentmedoid with centroids of each medoid in a sample 
		
		printf("\n end kmedoid");
		
		if(f==1 || diff>totaldiff())
		{
			diff=totaldiff();
			for(j=0;j<k;j++)
			{
				bestmedoid[j]=t[currentmedoid[j]];
				printf("\ninside for\n");
			}
		}
		
	}
	clusterassign();
	
printf("clara completed");	
}





float intra(int r)
{
	int i,j,clusterno=cluster[r],c=0,in=0;
	float s=0.0,sum=0.0;
	for(i=0;i<row;i++)
	{
		if(cluster[i]==clusterno)
		{
			c++;
			s=0;
			for(j=0;j<col;j++)
			{
				s+=(p[r][j]-p[i][j]);
			}
			
			sum+=s;
		}
	}
	return fabsf(sum/c);

}

double inter(int r)
{
	int i,j;
	double s=0.0;
	for(i=0;i<k;i++)
	{
		dist[i]=0;
		count[i]=0;
	}
	count[cluster[r]]=1;
	for(i=0;i<row;i++)
	{
		if(cluster[i]==cluster[r])
			continue;
		for(j=0;j<col;j++)
			dist[cluster[i]]+=fabs(p[i][j]-p[r][j]);
		count[cluster[i]]++;
	}
	double min;
	min=HUGE_VAL;
	
	
	int index=0;
	for(i=0;i<k;i++)
	{
		if(cluster[r]==i)
			continue;
		dist[i]=dist[i]/count[i];
	
		if(min>dist[i])
		{
			min=dist[i];
			index=i;
		}
	}
	
	return min;

}

void silhoutte()
{
	int i;
	double s=0.0,sh,ai=0,bi=0,m,d;
	for(i=0;i<row;i++)
	{
		ai=inter(i);  
		bi=intra(i);
		d=ai-bi;
		printf("d=%lf ",d);
		m=MAX(ai,bi);
		printf("max=%lf ",m);
		sh=d/m;
		printf("sh=%lf \n",sh);
		s+=sh;
	}
	s/=row;
	printf("%lf",s);
}


int main()
{
	
	int i;

	input();
	fileread();
	
	clara();

	for(i=0;i<row;i++)
	{
			printf("ROW:%d CLUSTER:%d\n",i,cluster[i]);	
	}
	
	silhoutte();
	free(p);
	free(currentmedoid);
	free(newmedoid);
	free(bestmedoid);
	free(cluster);
	free(cluster2);
	
	
	return 40;
}
