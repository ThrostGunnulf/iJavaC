/*Operadores unários aplicados a variáveis*/
class test14
{
	public static void main(String[] args)
	{
		boolean b1, b2;
		int a, b, c, d, e, f, g, h, i, j, k, l;
		
		b1 = false;
		b2 = true;
	
		a = -1; b = 1; c = 0;
		d = -01; e = 01; f = 00;
		g = -0x1; h = 0x1; i = 0x0;
	
		System.out.println(-a);
		System.out.println(-b);
		System.out.println(-c);
		System.out.println(+a);
		System.out.println(+b);
		System.out.println(+c);
		
		System.out.println(-d);
		System.out.println(-e);
		System.out.println(-f);
		System.out.println(+d);
		System.out.println(+e);
		System.out.println(+f);
		
		System.out.println(-g);
		System.out.println(-h);
		System.out.println(-i);
		System.out.println(+g);
		System.out.println(+h);
		System.out.println(+i);
		
		System.out.println(!b1);
		System.out.println(!b2);
	}
}
