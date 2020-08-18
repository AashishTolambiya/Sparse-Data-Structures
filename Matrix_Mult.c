#include<stdio.h>
#include<ncurses.h>
#include<stdlib.h>
#include<unistd.h>

void sparse_mat_gen_coo(int m, int n, int nonzero_percent, int negative_percent);
void convert_coo_to_csr(int values[], int row_indx[], int col_indx[]);
void convert_coo_to_csc(int values[], int row_indx[], int col_indx[]);
void csr_multiply_csc(int csr_no_rows, int csr_no_of_cols, int csr_val[], int csr_row[], int csr_col[], 
int csc_no_of_rows, int csc_no_of_cols, int csc_val[], int csc_row[], int csc_col[]);
void print_coo_matrix();
int coo_val[15000], coo_row[15000], coo_col[15000];
int csr_val[15000], csr_row[101], csr_col[15000], csr_row_int[101];
int csc_val[15000], csc_row[15000], csc_col[151], csc_col_int[151];
int res_mat_val[15000], res_mat_row[15000], res_mat_col[15000], no_of_rows_rm, no_of_cols_rm;
int no_of_rows_a, no_of_cols_a, no_of_rows_b, no_of_cols_b, nonzero_count, negative_count;
char a;
int m=0;

void gotoxy(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

int main()
{
int i, j, sp_pr_a, ng_pr_a, sp_pr_b, ng_pr_b;
clear;

//Taking input to generate random sparse matrix

//textcolor(WHITE);
printf("\nEnter details of Matrix A\n");
printf("--------------------------\n");
printf("Enter number of rows in matrix     : ");
scanf("%d", &no_of_rows_a);
printf("Enter number of columns in matrix  : ");
scanf("%d", &no_of_cols_a);
printf("Enter percentage of non-zero values: ");
scanf("%d", &sp_pr_a);

neg_per_a:

printf("Enter percentage of -ve elements   : ");
scanf("%d", &ng_pr_a);

	if(ng_pr_a > sp_pr_a)
	{
	printf("Percentage of -ve elements cannot exceed percentage of nonzero values");
	goto neg_per_a;
	}


//clrscr();

sparse_mat_gen_coo(no_of_rows_a, no_of_cols_a, sp_pr_a, ng_pr_a);

convert_coo_to_csr(coo_val, coo_row, coo_col);

printf("\n\n\nEnter details of Matrix B\n");
printf("--------------------------\n");
printf("Enter number of rows in matrix     : ");
scanf("%d", &no_of_rows_b);
printf("Enter number of columns in matrix  : ");
scanf("%d", &no_of_cols_b);
printf("Enter percentage of non-zero values: ");
scanf("%d", &sp_pr_b);

neg_per_b:

printf("Enter percentage of -ve elements   : ");
scanf("%d", &ng_pr_b);

	if(ng_pr_b > sp_pr_b)
	{
	printf("Percentage of -ve elements cannot exceed percentage of nonzero values");
	goto neg_per_b;
	}

//clrscr();

sparse_mat_gen_coo(no_of_rows_b, no_of_cols_b, sp_pr_b, ng_pr_b);

convert_coo_to_csc(coo_val, coo_row, coo_col);

csr_multiply_csc(no_of_rows_a, no_of_cols_a, csr_val, csr_row, csr_col, no_of_rows_b, no_of_cols_b, csc_val, csc_row, csc_col);

return 0;

//getch();
}



void csr_multiply_csc(int csr_no_of_rows, int csr_no_of_cols, int csr_val[], int csr_row[], int csr_col[], int csc_no_of_rows, int csc_no_of_cols, int csc_val[], int csc_row[], int csc_col[])
{
//sleep(3);
int i,j,k,l,sum;

	if(csr_no_of_cols != csc_no_of_rows)
	{
	printf("\n\n\nMatrix multiplication not possible!");
	printf("\n-----------------------------------\n");
	}
	else
	{
		for(i=1;i<=csr_no_of_rows;i++)
		{
			for(j=1;j<=csc_no_of_cols;j++)
			{
			sum=0;
				for(k=csr_row[i];k<=csr_row[i+1]-1;k++)
				{
					for(l=csc_col[j];l<=csc_col[j+1]-1;l++)
					{
						if(csr_col[k] == csc_row[l])
						{
						sum = sum + csr_val[k] * csc_val[l];
						}
					}
				}

				if(sum!=0)
				{
				m++;
				res_mat_val[m]=sum;
				res_mat_row[m]=i;
				res_mat_col[m]=j;
				}
			}
		}

//Following piece of code removes 0s which have might have occured after matrix multiplication

		for(i=1;i<=m;i++)
		{
			if(res_mat_val[i] == 0)
			{
				for(j=i+1;j<=m;j++)
				{
				res_mat_val[j-1]=res_mat_val[j];
				res_mat_row[j-1]=res_mat_row[j];
				res_mat_col[j-1]=res_mat_col[j];		
				}
			
			m--;
			}	
		}

		no_of_rows_rm = csr_no_of_rows;
		no_of_cols_rm = csc_no_of_cols;

		printf("\n\n\nCOO Format of Resultant Matrix after Multiplication\n");
		printf("---------------------------------------------------\n");
	
		printf("\nValue : ");
		for(i=1; i<=m; i++)
		{
		printf("%d\t",res_mat_val[i]);
		}

		printf("\nRow   : ");
		for(i=1; i<=m; i++)
		{
		printf("%d\t",res_mat_row[i]);
		}

		printf("\nColumn: ");
		for(i=1; i<=m; i++)
		{
		printf("%d\t",res_mat_col[i]);
		}

//Printing matrix in generic form from COO format
	
		if(no_of_cols_rm <= 20 && no_of_rows_rm <= 20)
		{
		print_coo_matrix();
		}
	}

printf("\n\n\n");
}

void print_coo_matrix()
{
int i, j, k, a[101][151];

printf("\n\n\nMatrix Representaion from COO Format");
printf("\n------------------------------------\n\n");

	for(i=1;i<=no_of_rows_rm;i++)
	{
		for(j=1;j<=no_of_cols_rm;j++)
		{
		a[i][j]=0;
		}
	}
	
//In below loop, value 'm' is the number of nonzero elements in the resultant matrix after multiplication

	for(i=1;i<=m;i++)
	{
	a[res_mat_row[i]][res_mat_col[i]] = res_mat_val[i];	
	}

	for(i=1;i<=no_of_rows_rm;i++)
	{
		for(j=1;j<=no_of_cols_rm;j++)
		{
		printf("%d\t", a[i][j]);
		}
	printf("\n");
	}
printf("\n");
}



void convert_coo_to_csr(int values[], int row_indx[], int col_indx[])
{
//sleep(3);
int count[100],i,sorted_pos,pointer;

	for(i=1;i<=100;i++)
	{
	count[i]=0;
	}

	for(i=1;i<=nonzero_count;i++)
	{
	count[row_indx[i]]+=1;
	}

	for(i=2;i<=100;i++)
	{
	count[i]=count[i-1]+count[i];
	}

	for(i=1;i<=nonzero_count;i++)
	{
	sorted_pos=count[row_indx[i]];
	csr_val[sorted_pos]=values[i];
	csr_row_int[sorted_pos]=row_indx[i];
	csr_col[sorted_pos]=col_indx[i];
	count[row_indx[i]]-=1;
	}

	pointer=1;

	for(i=1;i<=no_of_rows_a;i++)
	{

	check1:

		if(pointer>nonzero_count)
		{
		csr_row[i]=0;
		}
		else
		{

			if(csr_row_int[pointer]==i)
			{
			csr_row[i]=pointer;
			pointer++;
			}
			else
			{
				if(csr_row_int[pointer]<i)
				{
				pointer++;
				goto check1;
				}
				else
				{
				csr_row[i]=0;
				}
			}
		}
	}

	csr_row[i]=nonzero_count+1;

	for(i=no_of_rows_a; i>=1;i--)
	{
		if(csr_row[i]==0)
		{
		csr_row[i]=csr_row[i+1];
		}
	}

	printf("\n\n\nCSR Format\n");
	printf("----------\n");

	printf("\nValue : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csr_val[i]);
	}

	printf("\nRow   : ");
	for(i=1; i<=no_of_rows_a+1; i++)
	{
	printf("%d\t",csr_row[i]);
	}

	printf("\nColumn: ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",csr_col[i]);
	}

//getch();

}

void convert_coo_to_csc(int values[], int row_indx[], int col_indx[])
{
//sleep(3);
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

	for(i=1;i<=no_of_cols_b;i++)
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

	for(i=no_of_cols_b; i>=1;i--)
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
	for(i=1; i<=no_of_cols_b+1; i++)
	{
	printf("%d\t",csc_col[i]);
	}

//getch();

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
	
/*  	printf("\n\n\nCOO Format\n");
	printf("----------\n");

	printf("\nValue : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",coo_val[i]);
	}

	printf("\nRow   : ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",coo_row[i]);
	}

	printf("\nColumn: ");
	for(i=1; i<=nonzero_count; i++)
	{
	printf("%d\t",coo_col[i]);
	}
*/

printf("\n");
//getch();

}
