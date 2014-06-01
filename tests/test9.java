/*Distinção entre variáveis locais e globais*/
class test9
{
	static int a, b, c;
	static boolean e, f, g;
	
	public static void main(String[] args)
	{
		int a, b;
		boolean e;
		
		a = -1; b = 07; c = 0x10;
		e = true; f = false; g = true;
		
		System.out.println(a);
		System.out.println(b);
		System.out.println(c);
		System.out.println(e);
		System.out.println(f);
		System.out.println(g);
	}
}
