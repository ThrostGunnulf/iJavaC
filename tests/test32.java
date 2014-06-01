/*Cortesia de Joca Leitão*/
class test32
{
	static int a, b, c;
	static boolean d, e;
	static int[] f, g;
	static boolean[] h, i, j;
	
	public static void main(String[] args)
	{
		int a;

		a = 0;
		while (a < args.length)
		{
			System.out.println(Integer.parseInt(args[a]));
			a = a + 1;
		}	

		f = new int[4];

		f[3] = 3;

		g = banana();

		f[0] = g[2];
	}

	public static int[] banana(){
		int[] f2;

		f2 = new int[4];

		f2[3] = 3;

		return f2;
	}
}

