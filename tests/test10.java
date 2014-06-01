/*Impressão de variáveis globais não inicializadas*/
class test10
{
	static int a, b, c;
	static boolean e, f, g;
	
	public static void main(String[] args)
	{
		int a, b;
		boolean e;
		
		a = -1; b = 07; e = true;
		
		System.out.println(a);
		System.out.println(b);
		System.out.println(c);
		System.out.println(e);
		System.out.println(f);
		System.out.println(g);
	}
}
