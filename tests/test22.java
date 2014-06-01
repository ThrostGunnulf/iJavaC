/*Atribuição de variáveis a outras variáveis (arrays globais)*/
class test22
{
	static int[] a1, a2;
	static boolean[] b1, b2;
	
	public static void main(String[] args)
	{
		a1 = a2;
		a2 = a1;
		
		b1 = b2;
		b2 = b1;
	}
}
