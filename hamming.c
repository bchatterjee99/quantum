#include <stdio.h>
#include <stdlib.h>

#define R 3

// get ith bit of n bit number
int get_bit(unsigned long a, int i)
{
    unsigned long x = ((unsigned long)1)<<i;
    return !!(a & x);
}


/*
        _                         _  
       |              |            |
       |              |            |
  H =  |      A       |    I       |
       |       rxk    |     rxr    |
       |_             |           _| 

 */
void parity_check_matrix(int r, unsigned char** M)
{
    unsigned long n = (1 << r) - 1;
    unsigned long k = n - r;
    unsigned long num_row = r;
    unsigned long num_col = n;

    for(unsigned long x=1; x<((unsigned long)1<<r); x++)
    {
	for(int i=0; i < r; i++)
	    M[i][x - 1] = get_bit(x, i);
    }

    printf("H (parity check matrix):\n");
    printf("------------------------\n");
    for(unsigned long i=0; i<num_row; i++)
    {
	for(unsigned long j=0; j<num_col; j++)
	    printf("%d ", M[i][j]);
	printf("\n");
    }
    printf("\n\n");
}


// get weight of n bit number
int wt(unsigned long a, int n)
{
    unsigned long y = ((unsigned long)1<<(n-1));
    int ans = 0;
    while(y > 0)
    {
	ans = ans + !!( a & y);
	y = y >> 1;
    }
    return ans;
}



/*
        _                         _  
       |              |            |
       |              |     tr     |
  G =  |      I       |    A       |
       |       kxk    |     kxr    |
       |_             |           _| 

 */
void generator_matrix(int r, unsigned char** M)
{
    unsigned long n = (1 << r) - 1;
    unsigned long k = n - r;
    unsigned long num_row = k;
    unsigned long num_col = n;

    unsigned long curr_row = 0;
    for(unsigned long x=3; x<((unsigned long)1<<r); x++)
    {
	// printf("x = %x   wt = %d\n", x, wt(x, r));
	if(wt(x, r) < 2) continue;


	for(unsigned long j=0; j<k; j++)
	{
	    M[curr_row][j] = 0;
	    if(j == curr_row) M[curr_row][j] = 1;
	}

	for(unsigned long j=k; j<n; j++)
	{
	    unsigned long y = j - k;
	    M[curr_row][j] = get_bit(x, y);
	}

	curr_row++;
    }


    printf("G (generator matrix):\n");
    printf("---------------------\n");
    for(unsigned long i=0; i<num_row; i++)
    {
	for(unsigned long j=0; j<num_col; j++)
	    printf("%d ", M[i][j]);
	printf("\n");
    }
    printf("\n\n");
}


unsigned long encode(unsigned long m, unsigned char** G, int r)
{
    unsigned long n = (1 << r) - 1;
    unsigned long k = n - r;
    unsigned long num_row = k;
    unsigned long num_col = n;
    

    printf("message:\n");
    for(int i=0; i<k; i++)
    {
	int bit = get_bit(m, k - 1 - i);
	printf("%d", bit);
    }
    printf("\n\n");

    printf("codeword:\n");
    unsigned long codeword = 0;
    for(int j=0; j<n; j++)
    {
	unsigned long ans = 0;
	for(int i=0; i<k; i++)
	{
	    int x = get_bit(m, k - 1 - i);
	    ans = ans + x * G[i][j];
	    ans = ans & 1; // % 2
	}
	codeword = codeword | ((unsigned long)ans<<(n - 1 - j));
	printf("%d", ans);
    }
    printf("\n\n");
}

void size(int r)
{
    printf("r = %d,  ", r);
    unsigned long n = (1 << r) - 1;
    unsigned long k = n - r;
    printf("(n = %ld, k = %ld, d = 3) hamming code:\n", n, k);
    printf("---------------------------------------\n");
    printf("\n\n");
}

int main()
{
    int r = R;
    unsigned long n = (1 << r) - 1;
    unsigned long k = n - r;
    printf("\n\n");
    size(r);


    unsigned long h_num_row = r;
    unsigned long h_num_col = n;
    unsigned char **H = malloc(h_num_row * sizeof(unsigned char*));
    for(unsigned long i=0; i<h_num_row; i++)
	H[i] = malloc(h_num_col * sizeof(unsigned char));
    parity_check_matrix(r, H);

    unsigned long g_num_row = k;
    unsigned long g_num_col = n;
    unsigned char **G = malloc(g_num_row * sizeof(unsigned char*));
    for(unsigned long i=0; i<g_num_row; i++)
	G[i] = malloc(g_num_col * sizeof(unsigned char));
    generator_matrix(r, G);



    unsigned long m = 4;
    unsigned long c = encode(m, G, r);


    for(unsigned long i=0; i<h_num_row; i++)
	free(H[i]);
    free(H);

    return 0;
}
