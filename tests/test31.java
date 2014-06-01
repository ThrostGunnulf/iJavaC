/*Atribuição a variáveis que são parâmetros da função*/
class test31
{
	public static void main(String[] args)
	{
		int a, e;
		int[] b;
		boolean c;
		boolean[] d;
		
		a = 1;
		b = new int[2];
		b[0] = -1;
		b[1] = 0x1;
		c = true;
		d = new boolean[2];
		d[0] = false;
		d[1] = true;
	
		e = test1(a, b, c, d);
	}
	
	public static int test1(int a, int[] b, boolean c, boolean[] d)
	{
		a = -a;
		b = new int[2];
		c = !c;
		d = new boolean[2];
		
		return 0;
	}
}
