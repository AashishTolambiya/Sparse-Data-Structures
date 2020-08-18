#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>
#include<unistd.h>

void sparse_mat_gen_coo(int m, int n, int nonzero_percent, int negative_percent);
void convert_coo_to_csr(int values[], int row_indx[], int col_indx[]);
void convert_coo_to_csc(int values[], int row_indx[], int col_indx[]);
void csc_row_transformation(int r1, int k1, int r2, int k2);
void print_csc_matrix();
int csc_no_of_rows, csc_no_of_cols, csc_val[], csc_row[], csc_col[];
int coo_val[15000], coo_row[15000], coo_col[151];
int csr_val[15000], csr_row[101], csr_col[15000], csr_row_int[101];
int csc_val[15000], csc_row[15000], csc_col[151], csc_col_int[151];
int no_of_rows, no_of_cols, nonzero_count, negative_count;
char b;
int line=14;

void gotoxy(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

int main()
{
int i, j, sp_pr, ng_pr, r1, r2, k1, k2;
clear;

//Taking input to generate random sparse matrix

printf("\nEnter details of Matrix\n");
printf("------------------------\n");
printf("Enter number of rows in matrix     : ");
scanf("%d", &no_of_rows);
printf("Enter number of columns in matrix  : ");
scanf("%d", &no_of_cols);
printf("Enter percentage of non-zero values: ");
scanf("%d", &sp_pr);

neg_per:

printf("Enter percentage of -ve elements   : ");
scanf("%d", &ng_pr);

	if(ng_pr > sp_pr)
	{
	printf("Percentage of -ve elements cannot exceed percentage of nonzero values");
	goto neg_per;
	}

sparse_mat_gen_coo(no_of_rows, no_of_cols, sp_pr, ng_pr);

convert_coo_to_csc(coo_val, coo_row, coo_col);

//Printing matrix in generic form from CSC format 

	if(no_of_cols <= 20 && no_of_rows <= 20)
	{
	print_csc_matrix();
	}

repeat:

//Taking input for row transformation operation

printf("\n\n\nEnter details of Row Transformation\n");
printf("-----------------------------------\n");
printf("Enter Row 1              : ");
scanf("%d", &r1);
printf("Enter Scaling Factor k1  : ");
scanf("%d", &k1);
printf("Enter Row 2              : ");
scanf("%d", &r2);
printf("Enter Scaling Factor k2  : ");
scanf("%d", &k2);

csc_row_transformation(r1, k1, r2, k2);

inp_again:

scanf("%c",&b);
printf("\nWant to continue with further row transformations? (Y/N) : ");
scanf("%c",&b);
	if(b == 'Y' || b == 'y')
	{
	goto repeat;
	}
	else
	{
		if(b != 'N' && b != 'n')
		{
		printf("Invalid input");
		goto inp_again;
		}
	}

return 0;
}

void csc_row_transformation(int r1, int k1, int r2, int k2)
{
int i,j,k,l,m,n,done,new_count;

	for(i=1;i<=nonzero_count;i++)
	{
		if(csc_row[i] == r2)
		{
		csc_val[i] = k2 * csc_val[i];
		}
	}

	new_count = nonzero_count;
	for(i=1;i<=nonzero_count;i++)
	{
		if(csc_row[i] == r1)
		{
			for(j=1;j<=no_of_cols+1;j++)
			{
				if(csc_col[j]>i)
				{
				break;
				}
			}

			done=0;
			for(k=1;k<=nonzero_count;k++)
			{
				if(csc_row[k] == r2)
				{
					for(l=1;l<=no_of_cols+1;l++)
					{
						if(csc_col[l]>k)
						{
						break;
						}
					}

					if(j == l)
					{
					csc_val[k] = csc_val[k] + k1 * csc_val[i];
					done=1;
					break;
					}

					else
					{
					continue;
					}
				}
			}

			if(!done && (k1 * csc_val[i]!=0))
			{
			new_count = new_count + 1;

				for(m=new_count;m>=i+2;m--)
				{
				csc_val[m] = csc_val[m-1];
				csc_row[m] = csc_row[m-1];
				}
			
			csc_val[m] = k1 * csc_val[i];
			csc_row[m] = r2;

				for(n=1;n<=no_of_cols+1;n++)
				{
					if(csc_col[n]>=m)
					{
					break;
					}
				}

				for(m=n;m<=no_of_cols+1;m++)
				{
				csc_col[m]=csc_col[m]+1;
				}

			nonzero_count++;
			i++;
			}

			else
			{
			continue;
			}
		}

	}

	nonzero_count = new_count;

//Following piece of code removes 0s which have might have occured after performing row transformation

	for(i=1;i<=nonzero_count;i++)
	{
		if(csc_val[i] == 0)
		{
			for(j=i+1;j<=nonzero_count;j++)
			{
			csc_val[j-1]=csc_val[j];
			csc_row[j-1]=csc_row[j];
			}

			for(j=1;j<=no_of_cols+1;j++)
			{
				if(csc_col[j]>=i)
				{
				break;
				}
			}

			for(m=j;m<=no_of_cols+1;m++)
			{
			csc_col[m]=csc_col[m]-1;
			}	
		
		nonzero_count--;
		}		
	}

//Printing matrix in CSC format

	printf("\n\n\nCSC Format after Row Transformation\n");
	printf("-----------------------------------\n");

	printf("\nValue : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csc_val[i]);
	}

	printf("\nRow   : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csc_row[i]);
	}

	printf("\nColumn: ");
	for(i=1; i<=no_of_cols+1; i++)
	{
	printf("%d\t",csc_col[i]);
	}

//Printing matrix in generic form from CSC format

	if(no_of_cols <= 20 && no_of_rows <= 20)
	{
	print_csc_matrix();
	}

}

//Below function prints the matrix in generoc form from CSC format

void print_csc_matrix()
{
int i, j, k, a[101][151];

printf("\n\n\nMatrix Representaion from CSC Format");
printf("\n------------------------------------\n\n");

	for(j=1;j<=no_of_cols;j++)
	{
		for(i=1;i<=no_of_rows;i++)
		{
		a[i][j]=0;
		}

		for(i=csc_col[j];i<=csc_col[j+1]-1;i++)
		{
		a[csc_row[i]][j] = csc_val[i];
		}
	}
	
	for(i=1;i<=no_of_rows;i++)
	{
		for(k=1;k<=no_of_cols;k++)
		{
		printf("%d\t", a[i][k]);
		}
	printf("\n");
	}
printf("\n");
}

void convert_coo_to_csc(int values[], int row_indx[], int col_indx[])
{
int count[100],i,sorted_pos,pointer;

	for(i=1;i<=100;i++)
	{
	count[i]=0;
	}

	for(i=1;i<=nonzero_count;i++)
	{
	count[col_indx[i]]+=1;
	}

	for(i=2;i<=100;i++)
	{
	count[i]=count[i-1]+count[i];
	}

	for(i=1;i<=nonzero_count;i++)
	{
	sorted_pos=count[col_indx[i]];
	csc_val[sorted_pos]=values[i];
	csc_row[sorted_pos]=row_indx[i];
	csc_col_int[sorted_pos]=col_indx[i];
	count[col_indx[i]]-=1;
	}

	pointer=1;

	for(i=1;i<=no_of_cols;i++)
	{

	check2:

		if(pointer>nonzero_count)
		{
		csc_col[i]=0;
		}
		else
		{

			if(csc_col_int[pointer]==i)
			{
			csc_col[i]=pointer;
			pointer++;
			}
			else
			{
				if(csc_col_int[pointer]<i)
				{
				pointer++;
				goto check2;
				}
				else
				{
				csc_col[i]=0;
				}
			}
		}
	}

	csc_col[i]=nonzero_count+1;

	for(i=no_of_cols; i>=1;i--)
	{
		if(csc_col[i]==0)
		{
		csc_col[i]=csc_col[i+1];
		}
	}

	printf("\n\n\nCSC Format\n");
	printf("----------\n");

	printf("\nValue : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csc_val[i]);
	}

	printf("\nRow   : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csc_row[i]);
	}

	printf("\nColumn: ");
	for(i=1; i<=no_of_cols+1; i++)
	{
	printf("%d\t",csc_col[i]);
	}
}

void sparse_mat_gen_coo(int m, int n, int nonzero_percent, int negative_percent)
{
int i,j;
int new_rand_row, new_rand_col, negate_pos;

nonzero_count  = ((m*n) * nonzero_percent)/100;
negative_count = ((m*n) * negative_percent)/100;

printf("\nTotal number of values in Matrix : %d",m*n);
printf("\nTotal nonzero values in Matrix   : %d",nonzero_count);
printf("\nTotal negative values in Matrix  : %d",negative_count);

	for(i=1; i<=nonzero_count; i++)
	{
		new_rand_row=rand()%m+1;
		new_rand_col=rand()%n+1;

		//Check if the new random position is occupied or not
		//If it is occupied then check availability of subsequent postion
		//The search for available position is followed by traversing rows

		pos_check:

		for(j=1;j<=i;j++)
		{
			if(coo_row[j]==new_rand_row)
			{
				if(coo_col[j]==new_rand_col)
				{
				new_rand_col=(new_rand_col+1)%(n+1);
					if(new_rand_col==0)
					{
					new_rand_col=1;
					new_rand_row=(new_rand_row+1)%(m+1);
						if(new_rand_row==0)
						{
						new_rand_row=1;
						}
					}
				goto pos_check;
				}
			}
		}

		coo_row[i]=new_rand_row;
		coo_col[i]=new_rand_col;
		coo_val[i]=rand()%10+1;
	}

//Values in the matrix is generated randomly by now. Need to negate the
//assigned values as per negative_count (negative_percent input parameter)

	for(i=1; i<=negative_count; i++)
	{
	negate_pos=rand()%nonzero_count+1;

	negate_pos_check:

		if(coo_val[negate_pos]<0)
		{
		negate_pos+=1;
			if(negate_pos>nonzero_count)
			{
			negate_pos=1;
			}
		goto negate_pos_check;
		}
	coo_val[negate_pos]*=-1;
	}

printf("\n");

}
