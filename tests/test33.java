/*Cortesia de Ribeiro Lourenço e Joca Leitão*/
class test33
{
	static int a, b, c;
	static boolean d, e;
	static int[] f, g;
	static boolean[] h, i, j;
	
	public static void main(String[] args)
	{
		int a;

		a = 0;

		h = new boolean[9];
		g = new int[3];

		while (a < args.length)
		{
			System.out.println(Integer.parseInt(args[a]));
			a = a + 1;
		}

		g[g[g[2]]] = 2;

		f = new int[4];

		f[3] = 3;

		a = f[1];

		g = banana(3);

		f[0] = g[2];
		
		return;
	}

	public static int[] banana(int a){
		int[] f2;
		int i;

		f2 = new int[a];

		if (a > 7)
		{
			h[8] = true;
			f2[3] = 3;
		}

		else
		{
			h[0] = false;
			i = 0;
			while (i < 9)
			{
				if ( a%2 == 0)
					f2[i] = a*2 + i*7;
				h[i] = true;
				i = i + 1;
			}
		}

		return f2;
	}
}

